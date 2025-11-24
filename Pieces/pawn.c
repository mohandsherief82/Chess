#include <stdbool.h>
#include "pawn.h"
#include "../chessTypes.h"


// Store all Pawn piece data regardless of color
// Contains all moves that a pawn can perform
Pawn createPawn(int color, int row, char col)
{
    char sym = (color ==COLOR_BLACK) ? 'P' : 'p';

    Pawn p = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = col,
        .promoted = false,
        .promotion = '\0',
    };

    return p;
}
