#include "knight.h"

Knight createKnight(int color, int row, char col)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    Knight n = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 'B' + 4 * col + 1
    };

    return n;
}
