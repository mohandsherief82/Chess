#include "../include/bishop.h"
#include "../../chessTypes.h"

Bishop createBishop(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'B' : 'b';

    Bishop b = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 2 + 3 * col
    };

    return b;
}
