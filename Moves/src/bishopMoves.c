#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/utility.h"

#include <stdlib.h>

bool moveBishop(char** board ,Player player, Move move)
{
    int dispX, dispY, midRow, midCol, rowStep, colStep;
    Bishop* bishop = NULL;

    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (player.bishops[i].colPosition == move.colPrev && player.bishops[i].rowPosition == move.rowPrev)
        {
            bishop = &player.bishops[i];
            break;
        }
    }

    if (bishop == NULL) return false;

    int currentrow = bishop->rowPosition;
    int currentcol = bishop->colPosition;

    // Bishop move logic: abs(difference) of rows = abs(difference) of columns
    dispX = move.rowNext - currentrow;
    dispY = move.colNext - currentcol;

    if(abs(dispX) != abs(dispY)) return false;

    rowStep = (dispX > 0) ? 1 : -1;
    colStep = (dispY > 0) ? 1 : -1;

    midRow = currentrow + rowStep;
    midCol = currentcol + colStep;

    while (midRow != move.rowNext)
    {
        if(!isEmpty(board, midRow, midCol)) return false;

        midRow += rowStep;
        midCol += colStep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == bishop->color) return false;
        
        // Capture logic
    }

    return true;
}
