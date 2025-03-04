#include "Bishop.h"

BishopPiece::BishopPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type) {}

BishopPiece::BishopPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()) {}

std::string BishopPiece::getType() const {
    return pieceType;
}

std::string BishopPiece::getColour() const {
    return pieceColour;
}

int BishopPiece::getCol() const {
    return curr_col;
}

int BishopPiece::getRow() const {
    return curr_row;
}

int BishopPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    return 0;
}

bool BishopPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }
    
    // Non-diagonal move, illegal for bishops
    if (abs(x - curr_row) != abs(y- curr_col)) return false;

    if ((ChessBoard[x][y] != nullptr) && ChessBoard[x][y]->getColour() == pieceColour) return false;

    int rowDir = (x > curr_row) ? 1 : -1;
    int colDir = (y > curr_col) ? 1 : -1;

    int i = curr_row + rowDir;
    int j = curr_col + colDir;

    while (i != x && j != y) {
        if (ChessBoard[i][j] != nullptr) {
            return false;
        }
        i += rowDir;
        j += colDir;
    }
    
    return true;
}

std::vector<int> BishopPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
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