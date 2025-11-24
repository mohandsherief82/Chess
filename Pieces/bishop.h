#include "../chessTypes.h"

#ifndef BISHOP_H
#define BISHOP_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;

} Bishop;

Bishop createBishop(int color, int row, char col);

#endif
