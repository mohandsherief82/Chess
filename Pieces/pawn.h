#include "../chessTypes.h"
#include <stdbool.h>

// Prevents compiling this file more than once in the main file
#ifndef PAWN_H
#define PAWN_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;
    bool promoted;
} Pawn;

Pawn createPawn(int color, int row, char col);

#endif
