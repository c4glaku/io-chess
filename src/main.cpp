#include <iostream>
#include <sstream>
#include <string>
#include <cctype>
#include "board/GameBoard.h"

// Parse and validate chess notation
bool parseChessNotation(const std::string& input, int& startRow, int& startCol, int& endRow, int& endCol) {
    if (input.length() != 4) {
        return false;
    }

    startCol = std::tolower(input[0]) - 'a';
    startRow = input[1] - '1';
    endCol = std::tolower(input[2]) - 'a';
    endRow = input[3] - '1';

    if (startRow < 0 || startRow > 7 || startCol < 0 || startCol > 7 ||
        endRow < 0 || endRow > 7 || endCol < 0 || endCol > 7) {
            return false;
    }

    return true;
}

// Print current game status
void printGameStatus(const GameBoard& game, bool isCheck, bool isCheckMate) {
    if (isCheckMate) {
        std::cout << "Checkmate!" << (game.Player() == "White" ? "Black" : "White") << "wins!\n";
    } else if (isCheck) {
        std::cout <<game.Player() << " is in check!\n";
    } else {
        std::cout << game.Player() << "'s turn.\n";
    }
}

// Pawn promotion handler
char getPawnPromotionPiece() {
    char promotionPiece;
    std::cout << "Promote to (Q=Queen, R=Rook, B=Bishop, N=Knight): ";
    std::cin >> promotionPiece;

    while (std::tolower(promotionPiece) != 'q' && std::tolower(promotionPiece) != 'r' &&
        std::tolower(promotionPiece) != 'b' && std::tolower(promotionPiece) != 'n') {
            std::cout <<"Invalid choice. Please enter Q, R, B or N: ";
            std::cin >> promotionPiece;
    }

    return promotionPiece;
}

int main() {
    GameBoard chess;
    chess.init();
    chess.displayBoard();

    bool isGameOver = false;
    bool isCheck = false;
    bool isCheckMate = false;

    std::cout << "Welcome to Chess!\n";
    std::cout << "Enter moves in the format 'e2e4' (source square to destination dquare)\n";
    std::cout << "Type 'quit' to exit, 'undo' to take back a move\n\n";

    std::cout <<chess.Player() << "'s turn: ";

    std::string input;
    while (!isGameOver && std::getline(std::cin, input)) {
        if (input == "quit") break;
        if (input == "undo") {
            chess.undo();
            chess.togglePlayer(); // toggle back to the previous player
            chess.displayBoard();
            continue;
        }

        int startRow, startCol, endRow, endCol;

        if (parseChessNotation(input, startRow, startCol, endRow, endCol)) {
            // Validate and make move
            if (chess.isValidPieceSelection(startRow, startCol)) {
                if (chess.movePiece(startRow, startCol, endRow, endCol)) {
                    // Check for pawn promotion
                    if ((chess.getBoard()[endRow][endCol]->getSymbol() == 'P' && endRow == 7) ||
                        (chess.getBoard()[endRow][endCol]->getSymbol() == 'p' && endRow == 0)) {
                            char promotionPiece = getPawnPromotionPiece();
                            chess.promotePawn(endRow, endCol, promotionPiece);
                    }

                    // Toggle player
                    chess.togglePlayer();

                    // Check game status
                    isCheck = chess.isCheck();
                    if (isCheck) {
                        isCheckMate = chess.checkMate();
                        if (isCheckMate) {
                            isGameOver = true;
                        }
                    }
                } else {
                    std::cout << "Invalid move. Try again.\n";
                } 
            } else {
                std::cout << "Invalid piece selection. It's " << chess.Player() << "'s turn.\n";
            }
        } else {
            std::cout << "Invalid input format. Use format'e2e4' (from square to square).\n";
        }

        // display updated board
        chess.displayBoard();

        // print game status
        printGameStatus(chess, isCheck, isCheckMate);

        if (!isGameOver) {
            std::cout << chess.Player() << "'s turn: ";
        }
    }

    std::cout << "Thanks for playing!\n";
    return 0;
}
