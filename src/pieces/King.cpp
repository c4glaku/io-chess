#include "King.h"
#include "Rook.h"
#include <cmath>
#include <cstdlib>

KingPiece::KingPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type), hasMoved(false) {}

KingPiece::KingPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()),
    hasMoved(false) {}

KingPiece::~KingPiece() {}

std::string KingPiece::getType() const {
    return pieceType;
}

std::string KingPiece::getColour() const {
    return pieceColour;
}

int KingPiece::getCol() const {
    return curr_col;
}

int KingPiece::getRow() const {
    return curr_row;
}

int KingPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    hasMoved = true;
    return 0;
}

bool KingPiece::isSquareAttacked(int row, int col, const std::vector<std::vector<ChessPiece*>>& ChessBoard) const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            if (ChessBoard[i][j] && ChessBoard[i][j]->getColour() != pieceColour) {
                if (ChessBoard[i][j]->getLegalMove(row, col, ChessBoard)) {
                    return true;
                }
            }
        }
    }
    return false;
}

bool KingPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }
    
    int xDiff = abs(curr_row - x);
    int yDiff = abs(curr_col - y);

    if (xDiff == 0 && yDiff == 2) {
        if (hasMoved) return false;

        int dir = (y > curr_col) ? 1 : -1;
        int rookCol = (dir == 1) ? 7 : 0;
        
        if (dir == 1) {
            if (ChessBoard[curr_row][curr_col + 1] || ChessBoard[curr_row][curr_col + 2]) return false;
        } else {
            if (ChessBoard[curr_row][curr_col - 1] || 
                ChessBoard[curr_row][curr_col - 2] || 
                ChessBoard[curr_row][curr_col - 3])
                return false;
        }

        // Check that the rook exists and has not moved.
        ChessPiece* rookPiece = ChessBoard[curr_row][rookCol];
        if (!rookPiece || (rookPiece->getType() != (pieceColour == "White" ? "R" : "r"))) return false;
        RookPiece* rook = dynamic_cast<RookPiece*>(rookPiece);
        if (!rook || rook->getHasMoved()) return false;

        if (isSquareAttacked(curr_row, curr_col, ChessBoard) ||
                isSquareAttacked(curr_row, curr_col + direction, ChessBoard) ||
                isSquareAttacked(curr_row, y, ChessBoard))
                return false;

        return true;
    }


}

std::vector<int> KingPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
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