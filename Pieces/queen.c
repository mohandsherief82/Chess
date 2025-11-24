#include "queen.h"

Queen createQueen(int color, int row, char col)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    Queen q = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = col
    };

    return q;
}
