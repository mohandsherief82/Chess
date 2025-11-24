#include "../chessTypes.h"

#ifndef KNIGHT_H
#define KNIGHT_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;

} Knight;

Knight createKnight(int color, int row, char col);

#endif
