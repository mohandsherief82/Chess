#include "chessTypes.h"
#include <stdbool.h>

// Prevents compiling this file more than once in the main file
#ifndef PAWN_H
#define PAWN_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
    bool isActive;
    bool isPinned;
    bool promoted; // Promotion flag
    bool firstMove; // First Move Flag
} Pawn;

Pawn createPawn(PieceColor color, int row, int col);

#endif
