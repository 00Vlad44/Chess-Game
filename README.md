# Advanced Chess Game in C++

## Overview
- Advanced implementation of a chess game in C++
- Supports standard and complex chess rules
- Command-line interface for player interaction

## Features
- **Standard Chess Rules**: 
  - Piece movement
  - Captures
  - Turn-based play
- **Complex Moves**: 
  - Castling (kingside and queenside)
  - En Passant
  - Pawn Promotion (to queen, rook, bishop, or knight)
- **Game Integrity**: 
  - Check detection
  - Checkmate detection
- **User Interface**: 
  - Simple command-line interface for moves and board display

## How to Play
- **Clone the Repository**:
  ```sh
  git clone https://github.com/your-username/chess-game-cpp.git
  cd chess-game-cpp
  ```
- **Build the Project**:
  ```sh
  g++ -o chess main.cpp ChessGame.cpp
  ```
- **Run the Game**:
  ```sh
  ./chess
  ```
- **Enter Moves**:
  - Format: `start_x start_y end_x end_y`
  - Example: To move a piece from (1, 1) to (2, 2), enter: `1 1 2 2`
