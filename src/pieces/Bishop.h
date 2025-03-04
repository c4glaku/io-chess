#ifndef BISHOP_PIECE_H
#define BISHOP_PIECE_H

#include <ChessPiece.h>
#include <string>
#include <vector>

class BishopPiece : public ChessPiece {
private:
    std::string pieceColour;
    int curr_row;
    int curr_col;
    std::string pieceType;

public:
    BishopPiece(std::string colour, int x, int y, std::string type);
    BishopPiece(const ChessPiece* other);
    virtual ~BishopPiece();

    std::string getType() const override;
    std::string getColour() const override;
    int getRow() const override;
    int getCol() const override;
    int SetCoordinate(int x, int y) override;

    bool getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;
    std::vector<int> getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;
};

#endif