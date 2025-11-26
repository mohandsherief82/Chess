#include "../chessTypes.h"

#ifndef QUEEN_H
#define QUEEN_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;

} Queen;

Queen createQueen(PieceColor color, int row);

#endif
