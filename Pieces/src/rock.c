#include "../include/rock.h"
#include "../../chessTypes.h"
#include "../../Board/include/board.h"

#include <stdbool.h>
#include<stdlib.h>

Rock createRock(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'R' : 'r';

    Rock r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 0 + col * 7,
        .isActive = true,
        .firstMove = true
    };

    return r;
}
