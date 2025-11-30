#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>

bool moveBishop(char** board ,Player player, Move move,captureArray* capture)
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
        printf("Invalid Move, Try Again!!!\n");
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
            printf("Invalid Move, Try Again!!!\n");
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

        if(pieceColorAt(board, move.rowNext, move.colNext) != bishop->color)
        {
            char symbolAt= board[move.rowNext][move.colNext];

            if(pieceColorAt(board, move.rowNext, move.colNext) == COLOR_BLACK)
            {
                capture->captBlack[capture->countBlack] = symbolAt;
                capture ->countBlack++;
            }
            else
            {
                capture->captWhite[capture->countWhite] = symbolAt;
                capture ->countWhite++;
            }
        }
    }

    return true;
}
