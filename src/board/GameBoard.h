#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <string>
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
    
public:
    GameBoard();
    ~GameBoard();

    void init();
    void togglePlayer();
    std::string Player() const;
    bool validMove(int start_row, int start_col, int dest_row, int dest_col) const;
    bool movePiece(int start_row, int start_col, int dest_row, int dest_col);
    std::pair<int, int> kingPos() const;
    bool isCheck() const;
    void undo();
    void addMove(int start_row, int start_col, int dest_row, int dest_col, ChessPiece* captured = nullptr);
    bool checkMate();
    bool isValidPieceSelection(int row, int col) const;
    void promotePawn(int row, int col, char promoteTo);

    const std::vector<std::vector<ChessPiece*>>& getBoard() const { return ChessBoard; }

    void displayBoard() const;
};

void displayChessboard(const std::vector<std::vector<ChessPiece*>> &chessboard);

#endif