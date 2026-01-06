#include "knight.h"
#include "chessTypes.h"

#include <stdbool.h>
#include <stdlib.h>


Knight createKnight(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'N' : 'n';

    Knight n = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        // 1 for the B col and 5 * col"0/1" to assign the other knight as the difference between initial positions is 5
        .colPosition = 1 + 5 * col, 
        .isActive = true
    };

    return n;
}
