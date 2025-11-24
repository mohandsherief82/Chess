#include "../chessTypes.h"

#ifndef QUEEN_H
#define QUEEN_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;

} Queen;

Queen createQueen(int color, int row, char col);

#endif
