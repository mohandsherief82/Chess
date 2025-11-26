#include "../include/queen.h"
#include "../../chessTypes.h"

Queen createQueen(PieceColor color, int row)
{
    char sym = (color == COLOR_BLACK) ? 'Q' : 'q';

    Queen q = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 3
    };

    return q;
}
