#ifndef UTILITY_H
#define UTILITY_H

#include "../../chessTypes.h"

typedef struct
{
    const PieceColor color;
    Piece capturedPiece;
    int capturedPawns;
    int capturedRocks;    
    int capturedKnights;    
    int capturedBishops;    
    int capturedQueens;    
}Captured;

Captured initializeCapture(PieceColor color);
PieceColor pieceColorAt(char** board, int row, int col);

#endif
