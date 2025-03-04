#include "Rook.h"

RookPiece::RookPiece(std::string colour, int x, int y, std::string type)
    : pieceColour(colour), curr_row(x), curr_col(y), pieceType(type) {}

RookPiece::RookPiece(const ChessPiece* other)
    : pieceColour(other->getColour()),
    curr_row(other->getRow()),
    curr_col(other->getCol()),
    pieceType(other->getType()) {}

std::string RookPiece::getType() const {
    return pieceType;
}

std::string RookPiece::getColour() const {
    return pieceColour;
}

int RookPiece::getCol() const {
    return curr_col;
}

int RookPiece::getRow() const {
    return curr_row;
}

int RookPiece::SetCoordinate(int x, int y) {
    curr_row = x;
    curr_col = y;
    return 0;
}

bool RookPiece::getLegalMove(int x, int y, const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return false;
    }
    
    // Diagonal Move -- Illegal for Rook
    if (y != curr_col && x != curr_row) return false;

    // Check if there is a piece at the destination and if it is capturable
    if (ChessBoard[x][y] && ChessBoard[x][y]->getColour() == pieceColour) return false; // same colour piece, cannot capture
    

    // Vertical Move
    if (y == curr_col) {
        // Moving up the board
        if (x > curr_row) {
            for (int i = curr_row + 1; i < x; ++i) {
                if (ChessBoard[i][y] != nullptr) return false;
            }
        } else {
            // Moving down the board
            for (int i = curr_row - 1; i > x; --i) {
                if (ChessBoard[i][y] != nullptr) return false;
            }
        }
        
        return true;
    }

    // Horizontal Move
    if (x == curr_row) {
        // Moving right
        if (y > curr_col) {
            for (int j = curr_col + 1; j < y; ++j) {
                if (ChessBoard[x][j] != nullptr) return false;
            } 
        } else {
            // Moving left
            for (int j = curr_col - 1; j > y; --j) {
                if (ChessBoard[x][j] != nullptr) return false;
            } 
        }      
        return true;
    }



    return false;
}

std::vector<int> RookPiece::getAllValidMoves(const std::vector<std::vector<ChessPiece*>> &ChessBoard) const {
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