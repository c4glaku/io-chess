#include "GameBoard.h"
#include "../pieces/ChessPiece.h"
#include <iostream>
#include <cctype>
#include <algorithm>

GameBoard::GameBoard() : ChessBoard(8, std::vector<ChessPiece*>(8, nullptr)),
                        currentPlayer("White"),
                        isCapture(false) {}

GameBoard::~GameBoard() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            delete ChessBoard[i][j];
            ChessBoard[i][j] = nullptr;
        }
    }

    for (auto piece : CapturedPieces) {
        delete piece;
    }
    CapturedPieces.clear();
}

void GameBoard::init() {
    // Initialize white pieces
    ChessBoard[0][0] = new ChessPiece("White", 0, 0, PieceType::ROOK);
    ChessBoard[0][1] = new ChessPiece("White", 0, 1, PieceType::KNIGHT);
    ChessBoard[0][2] = new ChessPiece("White", 0, 2, PieceType::BISHOP);
    ChessBoard[0][3] = new ChessPiece("White", 0, 3, PieceType::QUEEN);
    ChessBoard[0][4] = new ChessPiece("White", 0, 4, PieceType::KING);
    ChessBoard[0][5] = new ChessPiece("White", 0, 5, PieceType::BISHOP);
    ChessBoard[0][6] = new ChessPiece("White", 0, 6, PieceType::KNIGHT);
    ChessBoard[0][7] = new ChessPiece("White", 0, 7, PieceType::ROOK);

    // Initialize black pieces
    ChessBoard[7][0] = new ChessPiece("Black", 7, 0, PieceType::ROOK);
    ChessBoard[7][1] = new ChessPiece("Black", 7, 1, PieceType::KNIGHT);
    ChessBoard[7][2] = new ChessPiece("Black", 7, 2, PieceType::BISHOP);
    ChessBoard[7][3] = new ChessPiece("Black", 7, 3, PieceType::QUEEN);
    ChessBoard[7][4] = new ChessPiece("Black", 7, 4, PieceType::KING);
    ChessBoard[7][5] = new ChessPiece("Black", 7, 5, PieceType::BISHOP);
    ChessBoard[7][6] = new ChessPiece("Black", 7, 6, PieceType::KNIGHT);
    ChessBoard[7][7] = new ChessPiece("Black", 7, 7, PieceType::ROOK);

    // Initialize pawns
    for (int i = 0; i < 8; ++i) {
        ChessBoard[1][i] = new ChessPiece("White", 1, i, PieceType::PAWN);
        ChessBoard[6][i] = new ChessPiece("Black", 6, i, PieceType::PAWN);
    }

    loadTextures();
}

void GameBoard::loadTextures() {
    std::vector<std::pair<std::string, std::string>> pieceFiles = {
        {"w_king", "../resources/w_king.png"},
        {"b_king", "../resources/b_king.png"},
        {"w_queen", "../resources/w_queen.png"},
        {"b_queen", "../resources/b_queen.png"},
        {"w_rook", "../resources/w_rook.png"},
        {"b_rook", "../resources/b_rook.png"},
        {"w_bishop", "../resources/w_bishop.png"},
        {"b_bishop", "../resources/b_bishop.png"},
        {"w_knight", "../resources/w_knight.png"},
        {"b_knight", "../resources/b_knight.png"},
        {"w_pawn", "../resources/w_pawn.png"},
        {"b_pawn", "../resources/b_pawn.png"}
    };

    for (const auto& [key, path] : pieceFiles) {
        sf::Texture texture;
        if (!texture.loadFromFile(path)) {
            std::cerr << "Failed to load " << path << std::endl;
            continue;
        }
        pieceTextures[key] = texture;
    }
}

void GameBoard::draw(sf::RenderWindow& window, float squareSize, float boardStartX, float boardStartY) {
    sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));

    // Draw the board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            // position squares with offset for centering
            square.setPosition(boardStartX + j * squareSize, boardStartY + (7 - i) * squareSize); 
            square.setFillColor((i + j) % 2 == 0 ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99));
            window.draw(square);

            // Draw Piece if present
            if (ChessBoard[i][j]) {
                std::string key = (ChessBoard[i][j]->getColour() == "White" ? "w_" : "b_");
                switch (ChessBoard[i][j]->getPieceType()) {
                    case PieceType::KING: key += "king"; break;
                    case PieceType::QUEEN: key += "queen"; break;
                    case PieceType::ROOK: key += "rook"; break;
                    case PieceType::BISHOP: key += "bishop"; break;
                    case PieceType::KNIGHT: key += "knight"; break;
                    case PieceType::PAWN: key += "pawn"; break;
                    default: continue;
                }
                sf::Sprite pieceSprite;
                pieceSprite.setTexture(pieceTextures[key]);

                // scale sprite to fit the square size
                float spriteScale = squareSize / 64.0f;
                pieceSprite.setScale(spriteScale, spriteScale);

                // position the sprite with board offset
                pieceSprite.setPosition(boardStartX + j * squareSize, boardStartY + (7 - i) * squareSize);
                window.draw(pieceSprite);
            }
        }
    }
}

void GameBoard::togglePlayer() {
    if (currentPlayer == "Black") {
        currentPlayer = "White";
    } else {
        currentPlayer = "Black";
    }
}

std::string GameBoard::Player() const {
    return currentPlayer;
}

bool GameBoard::validMove(int start_row, int start_col, int dest_row, int dest_col) const {
    // Check coordinates' validity
    if (start_row < 0 || start_row > 7 || start_col < 0 || start_col > 7 ||
        dest_row < 0 || dest_row > 7 || dest_col < 0 || dest_col > 7) {
            return false;
    }

    // Check if a piece is at the starting position
    if (!ChessBoard[start_row][start_col]) {
        return false;
    }

    // Check if the piece belongs to the current player
    if (ChessBoard[start_row][start_col]->getColour() != currentPlayer) {
        return false;
    }

    // Check move legality
    return ChessBoard[start_row][start_col]->isValidMove(dest_row, dest_col, ChessBoard);
}

bool GameBoard::movePiece(int start_row, int start_col, int dest_row, int dest_col) {
    if (!validMove(start_row, start_col, dest_row, dest_col)) return false;
    

    ChessPiece* piece = ChessBoard[start_row][start_col];
    if (piece->getPieceType() == PieceType::KING && abs(dest_col - start_col) == 2) {
        if (!validateCastling(start_row, start_col, dest_col)) return false;

        performCastling(start_row, start_col, dest_col);
        return true;
    }

    ChessPiece* capturedPiece = nullptr;
    isCapture = false;

    if (ChessBoard[dest_row][dest_col]) {
        isCapture = true;
        capturedPiece = ChessBoard[dest_row][dest_col];
    }


    ChessPiece* movingPiece = ChessBoard[start_row][start_col];
    ChessBoard[start_row][start_col] = nullptr;
    ChessBoard[dest_row][dest_col] = movingPiece;
    movingPiece->setPosition(dest_row, dest_col);

    bool inCheck = isCheck();

    // undo the stored move if the current player is in check
    if (inCheck) {
        ChessBoard[start_row][start_col] = movingPiece;
        movingPiece->setPosition(start_row, start_col);
        ChessBoard[dest_row][dest_col] = capturedPiece;

        return false;
    }

    if (isCapture) {
        CapturedPieces.push_back(capturedPiece);
    }

    addMove(start_row, start_col, dest_row, dest_col, capturedPiece);

    // Promotion
    if ((movingPiece->getPieceType() == PieceType::PAWN && 
        (movingPiece->getColour() == "White" && dest_row == 7) ||
        (movingPiece->getColour() == "Black" && dest_row == 0))) {
            char promotionPiece = (movingPiece->getColour() == "White") ? 'Q' : 'q';
            promotePawn(dest_row, dest_col, promotionPiece);
    }

    return true;
}

bool GameBoard::validateCastling(int row, int col, int destCol) {
    if (isCheck()) return false;
    return true;

    int rookCol = (destCol > col) ? 7 : 0;
    int direction = (destCol > col) ? 1 : -1;

    ChessPiece* rook = ChessBoard[row][rookCol];
    if (!rook || rook->getPieceType() != PieceType::ROOK || rook->getHasMoved()) return false;

    // Check the path between the king and the rook
    for (int c = col + direction; c != rookCol; c+= direction) {
        if (ChessBoard[row][c]) return false;
    }

    // Check the king's path not being under attack
    for (int c = col; c != destCol; c += direction) {
        if (isSquareAttacked(row, c)) return false;
    }

    return true;
}

void GameBoard::performCastling(int row, int col, int destCol) {
    int rookCol = (destCol > col) ? 7 : 0;
    int newRookCol = (destCol > col) ? col + 1 : col - 1;

    // Move king
    ChessPiece* king = ChessBoard[row][col];
    ChessBoard[row][destCol] = king;
    ChessBoard[row][col] = nullptr;
    king->setPosition(row, destCol);

    // Move rook
    ChessPiece* rook = ChessBoard[row][rookCol];
    ChessBoard[row][newRookCol] = rook;
    ChessBoard[row][rookCol] = nullptr;
    rook->setPosition(row, newRookCol);
}

std::pair<int, int> GameBoard::kingPos() const {
    std::string King = (currentPlayer == "White") ? "K" : "k";

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (ChessBoard[x][y] && ChessBoard[x][y]->getPieceType() == PieceType::KING) {
                return {x, y};
            }
        }
    }

    // should not happen
    return {-1, -1};
}

bool GameBoard::isCheck() const {
    auto [kingRow, kingCol] = kingPos();
    if (kingRow == -1) return false; // should not happen

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (ChessBoard[x][y] && (ChessBoard[x][y]->getColour() != currentPlayer)) {
                if (ChessBoard[x][y]->isValidMove(kingRow, kingCol, ChessBoard)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void GameBoard::undo() {
    if (ChessHistory.empty()) return;

    StoreMove lastMove = ChessHistory.back();
    ChessHistory.pop_back();

    ChessPiece* piece = ChessBoard[lastMove.destRow][lastMove.destCol];
    ChessBoard[lastMove.sourceRow][lastMove.sourceCol] = piece;
    piece->setPosition(lastMove.sourceRow, lastMove.sourceCol);

    if (lastMove.CapturedPiece) {
        ChessBoard[lastMove.destRow][lastMove.destCol] = lastMove.CapturedPiece;

        auto it = std::find(CapturedPieces.begin(), CapturedPieces.end(), lastMove.CapturedPiece);
        if (it != CapturedPieces.end()) {
            CapturedPieces.erase(it);
        }
    } else {
        ChessBoard[lastMove.destRow][lastMove.destCol] = nullptr;
    }
}

void GameBoard::addMove(int start_row, int start_col, int dest_row, int dest_col, ChessPiece* captured) {
    StoreMove newMove = {start_row, start_col, dest_row, dest_col, captured};
    ChessHistory.push_back(newMove);
}

bool GameBoard::isSquareAttacked(int row, int col) const {
    std::string oppColour = (currentPlayer == "White") ? "Black" : "White";

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            ChessPiece* piece = ChessBoard[x][y];
            if (piece && piece->getColour() == oppColour) {
                if (piece->isValidMove(row, col, ChessBoard, /*checkAttack=*/true)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool GameBoard::checkMate() {
    if (!isCheck()) return false;

    // Exhaust all possible moves
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (ChessBoard[x][y] && ChessBoard[x][y]->getColour() == currentPlayer) {
                std::vector<std::pair<int, int>> moves = ChessBoard[x][y]->getLegalMoves(ChessBoard);

                for (std::pair<int, int> move : moves) {
                    int destRow = move.first;
                    int destCol = move.second;

                    // Save current state of the baord
                    ChessPiece* movingPiece = ChessBoard[x][y];
                    ChessPiece* destPiece = ChessBoard[destRow][destCol];

                    // Make the possible move
                    ChessBoard[x][y] = nullptr;
                    ChessBoard[destRow][destCol] = movingPiece;
                    movingPiece->setPosition(destRow, destCol);

                    // Update current check status after the possible move is played
                    bool stillInCheck = isCheck();

                    // Restore board status
                    ChessBoard[x][y] = movingPiece;
                    movingPiece->setPosition(x, y);
                    ChessBoard[destRow][destCol] = destPiece;

                    if (!stillInCheck) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool GameBoard::isValidPieceSelection(int row, int col) const {
    if (row < 0 || row > 7 || col < 0 || col > 7) {
        return false;
    }

    if (!ChessBoard[row][col]) {
        return false;
    }

    return ChessBoard[row][col]->getColour() == currentPlayer;
}

void GameBoard::promotePawn(int row, int col, char promoteTo) {
    if (!ChessBoard[row][col] ||
        (ChessBoard[row][col]->getPieceType() != PieceType::PAWN )) {
            return; // Not a pawn
    }

    std::string colour = ChessBoard[row][col]->getColour();
    ChessPiece* newPiece = nullptr;
    PieceType type;

    switch(toupper(promoteTo)) {
        case 'Q': type = PieceType::QUEEN; break;
        case 'R': type = PieceType::ROOK; break;
        case 'B': type = PieceType::BISHOP; break;
        case 'N': type = PieceType::KNIGHT; break;
        default: type = PieceType::QUEEN; // default to queen
    }

    // remove pawn from the board
    delete ChessBoard[row][col];

    // Create new piece
    newPiece = new ChessPiece(colour, row, col, type, true);

    ChessBoard[row][col] = newPiece;
}

/*
void GameBoard::displayBoard() const {
    displayChessboard(ChessBoard);
}

void displayChessboard(const std::vector<std::vector<ChessPiece*>> &chessboard) {
    for (int i = 7; i >= 0; --i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < 8; ++j) {
            std::string pieceSymbol;
            if (chessboard[i][j]) pieceSymbol =  chessboard[i][j]->getSymbol();
            else pieceSymbol = ((i + j) % 2 == 0) ? "_" : " ";
            std::cout << pieceSymbol;
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
    std::cout << "  abcdefgh" << std::endl;
}

*/