#include "ChessGame.h"

int main() {
    ChessGame game;
    game.display_board();

    int start_x, start_y, end_x, end_y;
    PieceColor current_player = WHITE;

    while (true) {
        std::cout << (current_player == WHITE ? "White" : "Black") << "'s turn. Enter the start and end coordinates (x y x y): ";
        std::cin >> start_x >> start_y >> end_x >> end_y;

        if (game.move_piece(start_x, start_y, end_x, end_y)) {
            game.display_board();

            if (game.is_check(current_player == WHITE ? BLACK : WHITE)) {
                if (game.is_checkmate(current_player == WHITE ? BLACK : WHITE)) {
                    std::cout << (current_player == WHITE ? "White" : "Black") << " wins by checkmate!" << std::endl;
                    break;
                } else {
                    std::cout << (current_player == WHITE ? "Black" : "White") << " is in check!" << std::endl;
                }
            }

            current_player = (current_player == WHITE ? BLACK : WHITE);
        } else {
            std::cout << "Invalid move. Try again." << std::endl;
        }
    }

    return 0;
}
