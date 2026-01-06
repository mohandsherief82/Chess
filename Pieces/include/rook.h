#include "chessTypes.h"
#include <stdbool.h>

#ifndef ROCK_H
#define ROCK_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
    bool isActive;
    bool isPinned;
    bool firstMove;
} Rook;

Rook createRook(PieceColor color, int row, int col);

#endif
