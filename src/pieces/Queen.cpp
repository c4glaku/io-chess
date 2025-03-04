#include "Queen.h"

QueenPiece::QueenPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type) {}

QueenPiece::QueenPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()) {}

std::string QueenPiece::getType() const {
    return pieceType;
}

std::string QueenPiece::getColour() const {
    return pieceColour;
}

int QueenPiece::getCol() const {
    return curr_col;
}

int QueenPiece::getRow() const {
    return curr_row;
}

int QueenPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    return 0;
}

bool QueenPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) return false;

    if (ChessBoard[x][y] && ChessBoard[x][y]->getColour() == pieceColour) return false;

    bool rookMove = false;

    if (x == curr_row) {
        rookMove = true;
        int step = (y > curr_col) ? 1 : -1;
        for (int j = curr_col + step; j != y; j += step) {
            if (ChessBoard[x][j] != nullptr) return false;
        }
    }

    if (y == curr_col) {
        rookMove = true;
        int step = (x > curr_row) ? 1 : -1;
        for (int i = curr_row + step; i != y; i += step) {
            if (ChessBoard[i][y] != nullptr) return false;
        }
    }

    bool bishopMove = false;
    if (abs(x - curr_row) == abs(y - curr_col)) {
        bishopMove = true;
        int rowDir = (x > curr_row) ? 1 : -1;
        int colDir = (y > curr_col) ? 1 : -1;

        int i = curr_row + rowDir;
        int j = curr_col + colDir;

        while (i != x && j != y) {
            if (ChessBoard[i][j] != nullptr) return false;
            i += rowDir;
            j += colDir;
        }
    }
    return rookMove || bishopMove;
}

std::vector<int> QueenPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
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