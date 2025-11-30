#include "../../chessTypes.h"
#include <stdbool.h>

#ifndef BISHOP_H
#define BISHOP_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
} Bishop;

Bishop createBishop(PieceColor color, int row, int col);

#endif
