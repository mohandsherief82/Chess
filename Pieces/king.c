#include "king.h"

King createKing(int color, int row)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    King k = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 'E'
    };

    return k;
}
