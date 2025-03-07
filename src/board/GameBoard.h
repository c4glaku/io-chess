#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "../pieces/ChessPiece.h"

struct StoreMove {
    int sourceRow;
    int sourceCol;
    int destRow;
    int destCol;
    ChessPiece* CapturedPiece;
};

class GameBoard {
private:
    std::vector<std::vector<ChessPiece*>> ChessBoard;
    std::vector<StoreMove> ChessHistory;
    std::vector<ChessPiece*> CapturedPieces;
    std::string currentPlayer;
    bool isCapture;
    bool isSquareAttacked(int row, int col) const;
    bool validateCastling(int row, int col, int destCol);
    void performCastling(int row, int col, int destCol);
    std::map<std::string, sf::Texture> pieceTextures;
    
public:
    GameBoard();
    ~GameBoard();

    void init();
    void loadTextures();
    void togglePlayer();
    std::string Player() const;
    bool validMove(int startRow, int startCol, int destRow, int destCol) const;
    bool movePiece(int startRow, int startCol, int destRow, int destCol);
    std::pair<int, int> kingPos() const;
    bool isCheck() const;
    void undo();
    void addMove(int startRow, int startCol, int destRow, int destCol, ChessPiece* captured = nullptr);
    bool checkMate();
    bool isValidPieceSelection(int row, int col) const;
    void promotePawn(int row, int col, char promoteTo);

    const std::vector<std::vector<ChessPiece*>>& getBoard() const { return ChessBoard; }

    void draw(sf::RenderWindow& window, float squareSize = 64.0f, float boardStartX = 0.0f, float boardStaryY = 0.0f);
    // void displayBoard() const;
};

// void displayChessboard(const std::vector<std::vector<ChessPiece*>> &chessboard);

#endif