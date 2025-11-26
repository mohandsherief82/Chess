#include "../include/rock.h"
#include "../../chessTypes.h"

Rock createRock(PieceColor color, int row, char col)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    Rock r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 'A' + 6 * col + 1
    };

    return r;
}
