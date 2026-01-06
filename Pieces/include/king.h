#include "chessTypes.h"

#ifndef KING_H
#define KING_H

typedef struct 
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
    bool isActive;
    bool isChecked;
    bool firstMove;
} King;

King createKing(PieceColor color, int row);

#endif
