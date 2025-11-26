#include "../include/king.h"
#include "../../chessTypes.h"

King createKing(PieceColor color, int row)
{
    char sym = (color == COLOR_BLACK) ? 'K' : 'k';

    King k = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 'E'
    };

    return k;
}
