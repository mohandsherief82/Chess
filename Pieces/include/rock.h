#include "../../chessTypes.h"

#ifndef ROCK_H
#define ROCK_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
} Rock;

Rock createRock(PieceColor color, int row, int col);

#endif
