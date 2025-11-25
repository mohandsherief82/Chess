#include "../chessTypes.h"

#ifndef KNIGHT_H
#define KNIGHT_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;

} Knight;

Knight createKnight(int color, int row, char col);

#endif
