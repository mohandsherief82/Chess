#include "rook.h"
#include "chessTypes.h"

#include <stdbool.h>
#include<stdlib.h>

Rook createRook(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'R' : 'r';

    Rook r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        // 0 for the A col and 7 * col to assign the other rook as the difference between initial positions is 7
        .colPosition = 0 + 7 * col,
        .isActive = true,
        .firstMove = true
    };

    return r;
}
