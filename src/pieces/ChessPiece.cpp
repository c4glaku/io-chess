#include "ChessPiece.h"
#include <cmath> 
#include <vector>

ChessPiece::ChessPiece(std::string clr, int x, int y, PieceType t, bool moved)
    : colour(clr), row(x), col(y), type(t), hasMoved(moved) {}

PieceType ChessPiece::getPieceType() const { return type; }
std::string ChessPiece::getColour() const { return colour; }
int ChessPiece::getRow() const { return row; }
int ChessPiece::getCol() const { return col; }
bool ChessPiece::getHasMoved() const { return hasMoved; }

char ChessPiece::getSymbol() const {
    const std::string symbols = "KQBNRPkqbnrp";
    int offset = (colour == "White") ? 0 : 6;
    switch(type) {
        case PieceType::KING: return symbols[offset];
        case PieceType::QUEEN: return symbols[1+offset];
        case PieceType::BISHOP: return symbols[2+offset];
        case PieceType::KNIGHT: return symbols[3+offset];
        case PieceType::ROOK: return symbols[4+offset];
        case PieceType::PAWN: return symbols[5+offset];
        default: return ' ';
    }
}

void ChessPiece::setPosition(int x, int y) {
    row = x;
    col = y;
    hasMoved = true;
}

bool ChessPiece::checkLinearPath(int destRow, int destCol, 
    const std::vector<std::vector<ChessPiece*>>& board) const {
    int stepX = (destCol > col) ? 1 : (destCol < col) ? -1 : 0;
    int stepY = (destRow > row) ? 1 : (destRow < row) ? -1 : 0;

    int steps = std::max(abs(destCol - col), abs(destRow - row));

    for (int i = 1; i < steps; ++i) {
        int newCol = col + i*stepX;
        int newRow = row + i*stepY;
        if (board[newRow][newCol]) return false;
    }
    return true;
}


bool ChessPiece::isValidMove(int destRow, int destCol, 
                            const std::vector<std::vector<ChessPiece*>>& board,
                            bool checkAttack) const {
    if (destRow < 0 || destRow > 7 || destCol < 0 || destCol > 7) return false;
    if (row == destRow && col == destCol) return false;

    ChessPiece* target = board[destRow][destCol];
    if (!checkAttack && target && target->getColour() == colour) return false;

    int dx = abs(destCol - col);
    int dy = abs(destRow - row);

    switch(type) {
        case PieceType::KING:
            // basic king moves + castling flag
            return (dx <= 1 && dy <= 1) || (!hasMoved && dy == 0 && dx == 2);
            
        case PieceType::QUEEN:
            return (dx == dy || dx == 0 || dy == 0) && checkLinearPath(destRow, destCol, board);
            
        case PieceType::BISHOP:
            return (dx == dy) && checkLinearPath(destRow, destCol, board);
            
        case PieceType::KNIGHT:
            return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
            
        case PieceType::ROOK:
            return (dx == 0 || dy == 0) && checkLinearPath(destRow, destCol, board);
            
        case PieceType::PAWN: {
            int dir = (colour == "White") ? 1 : -1;
            bool initial = (colour == "White" && row == 1) || (colour == "Black" && row == 6);

            if (checkAttack) {
                // pawns attack diagonally
                return (dx == 1 && destRow == row + dir);
            } else {
                // Regular move
                if (col == destCol && !target) {
                    if (destRow == row + dir) return true;
                    if (initial && destRow == row + 2*dir && !board[row+dir][col]) return true;
                }
                // Capture
                return dx == 1 && destRow == row + dir && target;
            }
        }
        default: return false;
    }
}


std::vector<std::pair<int, int>> ChessPiece::getLegalMoves(const std::vector<std::vector<ChessPiece*>>& board) const {
    std::vector<std::pair<int, int>> moves;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if (isValidMove(y, x, board)) {
                moves.emplace_back(y, x);
            }
        }
    }
    return moves;
}