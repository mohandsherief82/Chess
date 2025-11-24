#include "bishop.h"

Bishop createBishop(int color, int row, char col)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    Bishop b = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = col
    };

    return b;
}
