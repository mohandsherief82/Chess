#include "../chessTypes.h"

#ifndef KING_H
#define KING_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition;
    char colPosition;
} King;

King createKing(int color, int row);

#endif
