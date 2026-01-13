#include "bishop.h"
#include "chessTypes.h"

#include <stdbool.h>
#include <stdlib.h>

Bishop createBishop(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'B' : 'b';

    Bishop b = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        // 2 for the C col and 3 * col "0/1" to assign the other bishop as the difference between initial positions is 3
        .colPosition = 2 + 3 * col,
        .isActive = true
    };

    return b;
}
