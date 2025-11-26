#include "../chessTypes.h"

#ifndef ROCK_H
#define ROCK_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;
} Rock;

Rock createRock(int color, int row, char col);

#endif
