#include "ChessGame.h"

// Constructor to initialize the board with pieces in their starting positions
ChessGame::ChessGame() : en_passant_target_x(-1), en_passant_target_y(-1) {
    board.resize(8, std::vector<Piece>(8));

    // Initialize white pieces
    board[0] = {Piece(ROOK, WHITE), Piece(KNIGHT, WHITE), Piece(BISHOP, WHITE), Piece(QUEEN, WHITE),
                Piece(KING, WHITE), Piece(BISHOP, WHITE), Piece(KNIGHT, WHITE), Piece(ROOK, WHITE)};
    board[1] = {Piece(PAWN, WHITE), Piece(PAWN, WHITE), Piece(PAWN, WHITE), Piece(PAWN, WHITE),
                Piece(PAWN, WHITE), Piece(PAWN, WHITE), Piece(PAWN, WHITE), Piece(PAWN, WHITE)};
    
    // Initialize black pieces
    board[6] = {Piece(PAWN, BLACK), Piece(PAWN, BLACK), Piece(PAWN, BLACK), Piece(PAWN, BLACK),
                Piece(PAWN, BLACK), Piece(PAWN, BLACK), Piece(PAWN, BLACK), Piece(PAWN, BLACK)};
    board[7] = {Piece(ROOK, BLACK), Piece(KNIGHT, BLACK), Piece(BISHOP, BLACK), Piece(QUEEN, BLACK),
                Piece(KING, BLACK), Piece(BISHOP, BLACK), Piece(KNIGHT, BLACK), Piece(ROOK, BLACK)};
}

// Display the current state of the board
void ChessGame::display_board() const {
    for (const auto& row : board) {
        for (const auto& piece : row) {
            std::cout << piece.get_symbol() << ' ';
        }
        std::cout << std::endl;
    }
}

// Move a piece from (start_x, start_y) to (end_x, end_y)
bool ChessGame::move_piece(int start_x, int start_y, int end_x, int end_y) {
    if (!is_legal_move(start_x, start_y, end_x, end_y)) return false;

    Piece& piece = board[start_y][start_x];
    Piece& target = board[end_y][end_x];

    // Handle en passant capture
    if (piece.type == PAWN && end_x == en_passant_target_x && end_y == en_passant_target_y) {
        board[start_y][end_x] = Piece();
    }

    std::swap(board[start_y][start_x], board[end_y][end_x]);
    board[start_y][start_x] = Piece();
    piece.has_moved = true;

    // Handle pawn promotion
    if (piece.type == PAWN && (end_y == 0 || end_y == 7)) {
        handle_pawn_promotion(end_x, end_y);
    }

    // Update en passant target
    en_passant_target_x = -1;
    en_passant_target_y = -1;
    if (piece.type == PAWN && std::abs(start_y - end_y) == 2) {
        en_passant_target_x = end_x;
        en_passant_target_y = (start_y + end_y) / 2;
    }

    return true;
}

// Check if the move from (start_x, start_y) to (end_x, end_y) is legal
bool ChessGame::is_legal_move(int start_x, int start_y, int end_x, int end_y) const {
    const Piece& piece = board[start_y][start_x];
    if (piece.color == NONE || (start_x == end_x && start_y == end_y)) return false;

    const Piece& target = board[end_y][end_x];
    if (target.color == piece.color) return false;

    switch (piece.type) {
        case PAWN:
            return can_pawn_move(start_x, start_y, end_x, end_y);
        case KNIGHT:
            return can_knight_move(start_x, start_y, end_x, end_y);
        case BISHOP:
            return can_bishop_move(start_x, start_y, end_x, end_y);
        case ROOK:
            return can_rook_move(start_x, start_y, end_x, end_y);
        case QUEEN:
            return can_queen_move(start_x, start_y, end_x, end_y);
        case KING:
            return can_king_move(start_x, start_y, end_x, end_y);
        default:
            return false;
    }
}

// Check if a pawn move is legal
bool ChessGame::can_pawn_move(int start_x, int start_y, int end_x, int end_y) const {
    const Piece& piece = board[start_y][start_x];
    int direction = piece.color == WHITE ? -1 : 1;
    if (start_x == end_x) {
        if (board[end_y][end_x].type == EMPTY) {
            if (end_y == start_y + direction) return true;
            if (!piece.has_moved && end_y == start_y + 2 * direction && board[start_y + direction][start_x].type == EMPTY)
                return true;
        }
    } else if (std::abs(start_x - end_x) == 1 && end_y == start_y + direction && (board[end_y][end_x].color == (piece.color == WHITE ? BLACK : WHITE) || (end_x == en_passant_target_x && end_y == en_passant_target_y))) {
        return true;
    }
    return false;
}

// Check if a knight move is legal
bool ChessGame::can_knight_move(int start_x, int start_y, int end_x, int end_y) const {
    int dx = std::abs(end_x - start_x);
    int dy = std::abs(end_y - start_y);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

// Check if a bishop move is legal
bool ChessGame::can_bishop_move(int start_x, int start_y, int end_x, int end_y) const {
    if (std::abs(start_x - end_x) == std::abs(start_y - end_y))
        return is_path_clear(start_x, start_y, end_x, end_y);
    return false;
}

// Check if a rook move is legal
bool ChessGame::can_rook_move(int start_x, int start_y, int end_x, int end_y) const {
    if (start_x == end_x || start_y == end_y)
        return is_path_clear(start_x, start_y, end_x, end_y);
    return false;
}

// Check if a queen move is legal
bool ChessGame::can_queen_move(int start_x, int start_y, int end_x, int end_y) const {
    return can_rook_move(start_x, start_y, end_x, end_y) || can_bishop_move(start_x, start_y, end_x, end_y);
}

// Check if a king move is legal
bool ChessGame::can_king_move(int start_x, int start_y, int end_x, int end_y) const {
    int dx = std::abs(end_x - start_x);
    int dy = std::abs(end_y - start_y);
    if (dx <= 1 && dy <= 1) return true;

    // Castling logic
    if (!board[start_y][start_x].has_moved && dx == 2 && dy == 0) {
        int rook_x = (end_x == 6) ? 7 : 0;
        int rook_y = start_y;
        if (!board[rook_y][rook_x].has_moved && is_path_clear(start_x, start_y, rook_x, rook_y)) {
            return true;
        }
    }

    return false;
}

// Check if the path between (start_x, start_y) and (end_x, end_y) is clear
bool ChessGame::is_path_clear(int start_x, int start_y, int end_x, int end_y) const {
    if (start_x == end_x) {
        int step = (end_y - start_y) / std::abs(end_y - start_y);
        for (int y = start_y + step; y != end_y; y += step) {
            if (board[y][start_x].type != EMPTY) return false;
        }
    } else if (start_y == end_y) {
        int step = (end_x - start_x) / std::abs(end_x - start_x);
        for (int x = start_x + step; x != end_x; x += step) {
            if (board[start_y][x].type != EMPTY) return false;
        }
    } else {
        int step_x = (end_x - start_x) / std::abs(end_x - start_x);
        int step_y = (end_y - start_y) / std::abs(end_y - start_y);
        int x = start_x + step_x;
        int y = start_y + step_y;
        while (x != end_x && y != end_y) {
            if (board[y][x].type != EMPTY) return false;
            x += step_x;
            y += step_y;
        }
    }
    return true;
}

// Check if the position (x, y) is under attack by the opponent's pieces
bool ChessGame::is_under_attack(int x, int y, PieceColor color) const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            const Piece& piece = board[i][j];
            if (piece.color != NONE && piece.color != color) {
                if (is_legal_move(j, i, x, y)) {
                    return true;
                }
            }
        }
    }
    return false;
}

// Check if the given color is in check
bool ChessGame::is_check(PieceColor color) {
    int king_x = -1, king_y = -1;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (board[y][x].type == KING && board[y][x].color == color) {
                king_x = x;
                king_y = y;
                break;
            }
        }
    }
    return is_under_attack(king_x, king_y, color);
}

// Check if the given color is in checkmate
bool ChessGame::is_checkmate(PieceColor color) {
    if (!is_check(color)) return false;

    for (int start_y = 0; start_y < 8; ++start_y) {
        for (int start_x = 0; start_x < 8; ++start_x) {
            if (board[start_y][start_x].color == color) {
                for (int end_y = 0; end_y < 8; ++end_y) {
                    for (int end_x = 0; end_x < 8; ++end_x) {
                        ChessGame copy = *this;
                        if (copy.move_piece(start_x, start_y, end_x, end_y) && !copy.is_check(color)) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

// Handle pawn promotion
void ChessGame::handle_pawn_promotion(int x, int y) {
    char choice;
    std::cout << "Promote pawn to (Q/R/B/N): ";
    std::cin >> choice;
    PieceType new_type;
    switch (choice) {
        case 'Q':
            new_type = QUEEN;
            break;
        case 'R':
            new_type = ROOK;
            break;
        case 'B':
            new_type = BISHOP;
            break;
        case 'N':
            new_type = KNIGHT;
            break;
        default:
            new_type = QUEEN;
            break;
    }
    board[y][x].type = new_type;
}
