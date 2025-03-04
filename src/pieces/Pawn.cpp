#include "Pawn.h"

PawnPiece::PawnPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type) {}

PawnPiece::PawnPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()) {}

std::string PawnPiece::getType() const {
    return pieceType;
}

std::string PawnPiece::getColour() const {
    return pieceColour;
}

int PawnPiece::getCol() const {
    return curr_col;
}

int PawnPiece::getRow() const {
    return curr_row;
}

int PawnPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    return 0;
}

bool PawnPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }

    bool emptyDest = (ChessBoard[x][y] == nullptr);
    
    if (pieceColour == "White") {
        // Forward move
        if (y == curr_col && x == curr_row + 1) {
            return emptyDest;
        }

        // First move can be 2 squares forward
        if (curr_row == 1 && y == curr_col && x == (curr_row + 2)) {
            return emptyDest && (ChessBoard[x - 1][y] == nullptr);
        }

        // Diagonal capture
        if ((x == curr_row + 1) && (y == (curr_col + 1) || y == (curr_col - 1))) {
            return !emptyDest && ChessBoard[x][y]->getColour() != pieceColour;
        }
    }

    if (pieceColour == "Black") {
        // Forward move
        if (y == curr_col && x == curr_row - 1) {
            return emptyDest;
        }

        // First move can be 2 squares forward
        if (curr_row == 6 && y == curr_col && x == (curr_row - 2)) {
            return emptyDest && (ChessBoard[x + 1][y] == nullptr);
        }

        // Diagonal capture
        if ((x == curr_row - 1) && (y == (curr_col + 1) || y == (curr_col - 1))) {
            return !emptyDest && ChessBoard[x][y]->getColour() != pieceColour;
        }
    }

    return false;
}

std::vector<int> PawnPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
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