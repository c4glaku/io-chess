#ifndef KING_PIECE_H
#define KING_PIECE_H

#include <ChessPiece.h>
#include <string>
#include <vector>

class KingPiece : public ChessPiece {
private:
    std::string pieceColour;
    int curr_row;
    int curr_col;
    std::string pieceType;
    bool hasMoved;

    bool isSquareAttacked(int row, int col, const std::vector<std::vector<ChessPiece*>>& ChessBoard) const;

public:
    KingPiece(std::string colour, int x, int y, std::string type);
    KingPiece(const ChessPiece* other);
    virtual ~KingPiece();

    std::string getType() const override;
    std::string getColour() const override;
    int getRow() const override;
    int getCol() const override;
    int SetCoordinate(int x, int y) override;
    
    bool getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;
    std::vector<int> getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;

    bool getHasMoved() const;
    void setHadMoved(bool moved);

    bool performCastling(std::vector<std::vector<ChessPiece*>>& ChessBoard, int dest_row, int dest_col);
};

#endif