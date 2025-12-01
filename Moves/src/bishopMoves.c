#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>

bool moveBishop(char** board ,Player player, Move move, Captured* capture)
{
    int dispX, dispY, midRow, midCol, rowStep, colStep;
    Bishop* bishop = (Bishop*)checkPromotedPawn(player, move);

    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (player.bishops[i].colPosition == move.colPrev 
            && player.bishops[i].rowPosition == move.rowPrev && player.bishops[i].isActive)
        {
            bishop = &player.bishops[i];
            break;
        }
    }

    if (bishop == NULL) 
    {
        printf("No Bishop At This Position, Try Again!!!\n");
        return false;
    }

    int currentrow = bishop->rowPosition;
    int currentcol = bishop->colPosition;

    // Bishop move logic: abs(difference) of rows = abs(difference) of columns
    dispX = move.rowNext - currentrow;
    dispY = move.colNext - currentcol;

    if(abs(dispX) != abs(dispY)) 
    {
        printf("Invalid Bishop Move, Try Again!!!\n");
        return false;
    }

    rowStep = (dispX > 0) ? 1 : -1;
    colStep = (dispY > 0) ? 1 : -1;

    midRow = currentrow + rowStep;
    midCol = currentcol + colStep;

    while (midRow != move.rowNext)
    {
        if(!isEmpty(board, midRow, midCol))
        {
            printf("Invalid Bishop Move, Try Again!!!\n");
            return false;
        }

        midRow += rowStep;
        midCol += colStep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == bishop->color)
        {
            printf("Can't Capture Friendly piece, Try Again!!!\n");
            return false;
        }

        // Capture Logic
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    bishop->rowPosition = move.rowNext;
    bishop->colPosition = move.colNext;
    return true;
}
