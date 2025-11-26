#include "../chessTypes.h"

#ifndef BISHOP_H
#define BISHOP_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;

} Bishop;

Bishop createBishop(int color, int row, char col);

#endif
