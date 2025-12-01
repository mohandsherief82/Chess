#ifndef UTILITY_H
#define UTILITY_H
#define MAXCAPTNUM 15

#include "../../chessTypes.h"

typedef struct 
{
    char captWhite[MAXCAPTNUM];
    char captBlack[MAXCAPTNUM];
    int  countWhite;
    int  countBlack;
} CaptureArray;

PieceColor pieceColorAt(char** board, int row, int col);

extern CaptureArray capture;


#endif
