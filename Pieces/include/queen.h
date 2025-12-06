#include "../../chessTypes.h"

#ifndef QUEEN_H
#define QUEEN_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
    bool isActive;
    bool isPinned;
} Queen;

Queen createQueen(PieceColor color, int row);

#endif
