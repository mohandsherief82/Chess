#include "../include/knight.h"
#include "../../chessTypes.h"
#include "../../Board/include/board.h"

#include <stdbool.h>
#include<stdlib.h>


Knight createKnight(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'N' : 'n';

    Knight n = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 1 + 5 * col,
        .isActive = true
    };

    return n;
}
