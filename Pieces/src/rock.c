#include "../include/rock.h"
#include "../../chessTypes.h"

Rock createRock(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'R' : 'r';

    Rock r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 0 + col * 7
    };

    return r;
}
