#include <SFML/Graphics.hpp>
#include "board/GameBoard.h"
#include <iostream>

int main() {
    // Initialize SFML window with default size, while also allowing resizing
    unsigned int initialWidth = 512;
    unsigned int initialHeight = 512;
    sf::RenderWindow window(sf::VideoMode(initialWidth, initialHeight), "Chess", sf::Style::Resize); // 8x64 pixels
    window.setFramerateLimit(60);

    GameBoard chess;
    chess.init();

    bool isGameOver = false;
    bool isCheck = false;
    bool isCheckMate = false;
    int selectedRow = -1, selectedCol = -1;
    float squareSize = 64.0f; // initial square size
    float boardScale = 1.0f; // initial scale factor for the board

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

            // window resize handler
            if (event.type == sf::Event::Resized) {
                // calculate new dimensions
                float aspectRatio = 1.0f;
                unsigned int width = event.size.width;
                unsigned int height = event.size.height;
                float minDimension = std::min(width, height);

                // update viewposrt
                sf::FloatRect visibleArea(0, 0, width, height);
                window.setView(sf::View(visibleArea));

                // calculate new square size
                squareSize = minDimension / 8.0f;
                boardScale = minDimension / 512.0f;

                // update highlight square size
                highlightSquare.setSize(sf::Vector2f(squareSize, squareSize));

                // adjust status text position
                statusText.setPosition(10, 10);
            }

            if (!isGameOver) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        // get mouse coordinates
                        float mouseY = event.mouseButton.y;
                        float mouseX = event.mouseButton.x;

                        // debugging
                        std::cout << "Clicked on coordinate: (" << mouseX << ", " << mouseY << std::endl;

                        // calculate board dimensions based on current window size
                        float minDimension = std::min(window.getSize().x, window.getSize().y);
                        float currentSquareSize = minDimension / 8.0f;

                        // calculate board boundaries
                        float boardSize = currentSquareSize * 8.0f;
                        float boardStartX = (window.getSize().x - boardSize) / 2.0f;
                        float boardStartY = (window.getSize().y - boardSize) / 2.0f;

                        // check if the coordinates are within window
                        if (mouseX >= boardStartX && mouseX < boardStartX + boardSize &&
                            mouseY >= boardStartY && mouseY < boardStartY + boardSize) {
                            
                            // calculate coordinates
                            int col = (mouseX - boardStartX) / currentSquareSize;
                            int row = 7 - (int)((mouseY - boardStartY) / currentSquareSize); // flip y coordinate
                            
                            // debugging
                            std::cout << "Calculated board position: row " << row << ", col " << col << std::endl;


                            // first click: select a piece
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
                            // second click: attempt to move the piece
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

                                    // basic pawn promotion (always to Queen)
                                    if ((chess.getBoard()[row][col]->getSymbol() == 'P' && row == 7) ||
                                        (chess.getBoard()[row][col]->getSymbol() == 'p' && row == 0)) {
                                        chess.promotePawn(row, col, 'Q');
                                    }
                                } else {
                                    std::cout << "Move failed" << std::endl;
                                }
                                
                                // reset selection
                                selectedRow = -1;
                                selectedCol = -1;
                            }
                        }
                    }
                }

                // undo move
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::U) {
                    chess.undo();
                    chess.togglePlayer();
                    selectedRow = -1;
                    selectedCol = -1;
                }
            }
        }

        // update status text
        if (isCheckMate) {
            statusText.setString(std::string("Checkmate! ") + (chess.Player() == "White" ? "Black" : "White") + " wins!");
        } else if (isCheck) {
            statusText.setString(chess.Player() + " is in check!");
        } else {
            statusText.setString(chess.Player() + "'s turn");
        }

        // render
        window.clear(sf::Color(240, 240, 240));

        // calculate board position
        float boardSize = squareSize * 8.0f;
        float boardStartX = (window.getSize().x - boardSize) / 2.0f;
        float boardStartY = (window.getSize().y - boardSize) / 2.0f;

        // create a view for the board
        sf::View boardView = window.getDefaultView();
        boardView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        window.setView(boardView);

        // draw the board
        chess.draw(window, squareSize, boardStartX, boardStartY);

        // draw highlight for selected piece
        if (selectedRow != -1 && selectedCol != -1) {
            highlightSquare.setPosition(boardStartX + selectedCol * squareSize, 
                                        boardStartY + (7 - selectedRow) * squareSize);
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


