#include "../../chessTypes.h"
#include <stdbool.h>

#ifndef KNIGHT_H
#define KNIGHT_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
} Knight;

Knight createKnight(PieceColor color, int row, int col);

#endif
