#ifndef UTILITY_H
#define UTILITY_H
#define MAXCAPTNUM 15

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

typedef struct 
{
    char captWhite[MAXCAPTNUM];
    char captBlack[MAXCAPTNUM];
    int  countWhite;
    int  countBlack;
}captureArray;

extern captureArray capture;




#endif
