#include "../chessTypes.h"

#ifndef ROCK_H
#define ROCK_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;

} Rock;

Rock createRock(int color, int row, char col);

#endif
