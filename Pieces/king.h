#include "../chessTypes.h"

#ifndef KING_H
#define KING_H

typedef struct 
{
    const char symbol;
    const PieceColor color;
    int rowPosition;
    char colPosition;

} King;

King createKing(int color, int row, char col);

#endif
