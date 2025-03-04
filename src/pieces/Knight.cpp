#include "Knight.h"

KnightPiece::KnightPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type) {}

KnightPiece::KnightPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()) {}

std::string KnightPiece::getType() const {
    return pieceType;
}

std::string KnightPiece::getColour() const {
    return pieceColour;
}

int KnightPiece::getCol() const {
    return curr_col;
}

int KnightPiece::getRow() const {
    return curr_row;
}

int KnightPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    return 0;
}

bool KnightPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }
    
    int xDiff = abs(curr_row - x);
    int yDiff = abs(curr_col - y);

    if (!((xDiff == 1 && yDiff == 2) || (xDiff == 2 && yDiff == 1))) return false;

    // Check if a piece is on the target square, and if it can be captured
    if (ChessBoard[x][y] && ChessBoard[x][y]->getColour() == pieceColour) return false;

    // Knight can jump over pieces, no need to check for path
    return true;
}

std::vector<int> KnightPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    std::vector<int> allMoves;

    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            if (this->getLegalMove(x, y, ChessBoard)) {
                int move = (y * 8 + x);
                allMoves.push_back(move);
            }
        }
    }

    return allMoves;
}