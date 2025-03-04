#ifndef PAWN_PIECE_H
#define PAWN_PIECE_H

#include <ChessPiece.h>
#include <string>
#include <vector>

class PawnPiece : public ChessPiece {
private:
    std::string pieceColour;
    int curr_row;
    int curr_col;
    std::string pieceType;

public:
    PawnPiece(std::string colour, int x, int y, std::string type);
    PawnPiece(const ChessPiece* other);
    virtual ~PawnPiece();

    std::string getType() const override;
    std::string getColour() const override;
    int getRow() const override;
    int getCol() const override;
    int SetCoordinate(int x, int y) override;

    bool getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;
    std::vector<int> getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const override;
};

#endif