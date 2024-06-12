#ifndef CHESSGAME_H
#define CHESSGAME_H

#include <iostream>
#include <vector>
#include <string>
#include <cmath>

// Define the types and colors of chess pieces
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };
enum PieceColor { WHITE, BLACK, NONE };

// Structure for a chess piece
struct Piece {
    PieceType type;
    PieceColor color;
    bool has_moved;

    // Constructor to initialize a piece
    Piece(PieceType t = EMPTY, PieceColor c = NONE, bool moved = false) 
        : type(t), color(c), has_moved(moved) {}

    // Get the symbol representing the piece
    char get_symbol() const {
        if (color == NONE) return '.';
        char symbols[2][6] = {
            {'K', 'Q', 'R', 'B', 'N', 'P'},
            {'k', 'q', 'r', 'b', 'n', 'p'}
        };
        return symbols[color][type];
    }
};

// Class for the chess game
class ChessGame {
public:
    ChessGame(); // Constructor to initialize the game
    void display_board() const; // Display the current state of the board
    bool move_piece(int start_x, int start_y, int end_x, int end_y); // Move a piece
    bool is_check(PieceColor color); // Check if a player is in check
    bool is_checkmate(PieceColor color); // Check if a player is in checkmate

private:
    std::vector<std::vector<Piece>> board; // The chess board
    int en_passant_target_x, en_passant_target_y; // En passant target square

    // Helper functions for move validation and other checks
    bool is_legal_move(int start_x, int start_y, int end_x, int end_y) const;
    bool is_path_clear(int start_x, int start_y, int end_x, int end_y) const;
    bool can_pawn_move(int start_x, int start_y, int end_x, int end_y) const;
    bool can_knight_move(int start_x, int start_y, int end_x, int end_y) const;
    bool can_bishop_move(int start_x, int start_y, int end_x, int end_y) const;
    bool can_rook_move(int start_x, int start_y, int end_x, int end_y) const;
    bool can_queen_move(int start_x, int start_y, int end_x, int end_y) const;
    bool can_king_move(int start_x, int start_y, int end_x, int end_y) const;
    bool is_under_attack(int x, int y, PieceColor color) const;
    void handle_pawn_promotion(int x, int y); // Handle pawn promotion
};

#endif // CHESSGAME_H
