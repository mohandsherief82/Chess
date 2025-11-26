#include "../include/rock.h"
#include "../../chessTypes.h"

Rock createRock(PieceColor color, int row, char col)
{
    char sym = (color == COLOR_BLACK) ? 'R' : 'r';

    Rock r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 'A'
    };

    return r;
}
