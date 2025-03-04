#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <string>
#include <vector>

enum class PieceType { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, NONE };

class ChessPiece {
protected:
    std::string colour;
    int row;
    int col;
    PieceType type;
    bool hasMoved;

public:
    ChessPiece(std::string clr, int x, int y, PieceType t, bool moved = false);
    
    // Getters
    PieceType getPieceType() const;
    std::string getColour() const;
    int getRow() const;
    int getCol() const;
    bool getHasMoved() const;
    char getSymbol() const;

    // Set
    void setPosition(int x, int y);

    // Movement
    bool checkLinearPath(int destRow, int destCol, 
        const std::vector<std::vector<ChessPiece*>>& board) const;
    virtual bool isValidMove(int destRow, int destCol,
                        const std::vector<std::vector<ChessPiece*>>& board,
                        bool checkAttack = false) const;
    std::vector<std::pair<int, int>> getLegalMoves(const std::vector<std::vector<ChessPiece*>>& board) const;
};

#endif