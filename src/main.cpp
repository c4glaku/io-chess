#include <SFML/Graphics.hpp>
#include "board/GameBoard.h"
#include <iostream>

int main() {
    // Initialize SFML window
    sf::RenderWindow window(sf::VideoMode(512, 512), "Chess"); // 8x64 pixels
    window.setFramerateLimit(60);

    GameBoard chess;
    chess.init();

    bool isGameOver = false;
    bool isCheck = false;
    bool isCheckMate = false;
    int selectedRow = -1, selectedCol = -1;
    const float squareSize = 64.0f;

    sf::Font font;
    if (!font.loadFromFile("../resources/Roboto-VariableFont_wdth,wght.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }
    sf::Text statusText("", font, 20);
    statusText.setFillColor(sf::Color::Black);
    statusText.setPosition(10, 10);

    // Highlight for selected piece
    sf::RectangleShape highlightSquare(sf::Vector2f(squareSize, squareSize));
    highlightSquare.setFillColor(sf::Color(255, 255, 0, 100)); // Transparent yellow

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (!isGameOver) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // Precise coordinate calculation
                        int mouseY = event.mouseButton.y;
                        int mouseX = event.mouseButton.x;

                        // Ensure coordinates are within window
                        if (mouseX >= 0 && mouseX < 512 && mouseY >= 0 && mouseY < 512) {
                            // Flipped coordinate calculation
                            int col = mouseX / squareSize;
                            int row = 7 - (mouseY / squareSize);

                            std::cout << "Raw Click - MouseY: " << mouseY 
                                    << ", Calculated Row: " << row 
                                    << ", Column: " << col << std::endl;

                            // First click: Select a piece
                            if (selectedRow == -1) {
                                if (chess.isValidPieceSelection(row, col)) {
                                    selectedRow = row;
                                    selectedCol = col;
                                    std::cout << "Piece selected at row " << row 
                                            << ", col " << col << std::endl;
                                } else {
                                    std::cout << "Invalid piece selection" << std::endl;
                                }
                            } 
                            // Second click: Attempt to move the piece
                            else {
                                std::cout << "Attempting to move from (" 
                                        << selectedRow << "," << selectedCol 
                                        << ") to (" << row << "," << col << ")" << std::endl;

                                if (chess.movePiece(selectedRow, selectedCol, row, col)) {
                                    chess.togglePlayer();
                                    isCheck = chess.isCheck();
                                    
                                    // Check for checkmate
                                    if (isCheck) {
                                        isCheckMate = chess.checkMate();
                                        if (isCheckMate) {
                                            isGameOver = true;
                                        }
                                    }

                                    // Basic pawn promotion (always to Queen)
                                    if ((chess.getBoard()[row][col]->getSymbol() == 'P' && row == 7) ||
                                        (chess.getBoard()[row][col]->getSymbol() == 'p' && row == 0)) {
                                        chess.promotePawn(row, col, 'Q');
                                    }
                                } else {
                                    std::cout << "Move failed" << std::endl;
                                }
                                
                                // Reset selection
                                selectedRow = -1;
                                selectedCol = -1;
                            }
                        }
                    }
                }

                // Undo move
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
                    chess.undo();
                    chess.togglePlayer();
                    selectedRow = -1;
                    selectedCol = -1;
                }
            }
        }

        // Update status text
        if (isCheckMate) {
            statusText.setString(std::string("Checkmate! ") + (chess.Player() == "White" ? "Black" : "White") + " wins!");
        } else if (isCheck) {
            statusText.setString(chess.Player() + " is in check!");
        } else {
            statusText.setString(chess.Player() + "'s turn");
        }

        // Render
        window.clear();
        chess.draw(window);

        // Draw highlight for selected piece
        if (selectedRow != -1 && selectedCol != -1) {
            highlightSquare.setPosition(selectedCol * squareSize, (7 - selectedRow) * squareSize);
            window.draw(highlightSquare);
        }

        window.draw(statusText);
        window.display();
    }

    return 0;
}

/* I/O implementation
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



*/


