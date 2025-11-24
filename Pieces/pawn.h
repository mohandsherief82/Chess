#include "../chessTypes.h"
#include <stdbool.h>

// Prevents compiling this file more than once in the main file
#ifndef PAWN_H
#define PAWN_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;
    bool promoted;
    const char  promotion;
} Pawn;

Pawn createPawn(int color, int row, char col);

#endif
