#include "player.h"
#include "board.h"
#include "pawnMoves.h"
#include "captures.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


MoveValidation moveBishop(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    int dispX, dispY, r, c, rowStep, colStep;
    Bishop* bishop = (Bishop*)checkPromotedPawn(player, move);

    if (bishop == NULL)
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->bishops[i].colPosition == move.colPrev && 
                player->bishops[i].rowPosition == move.rowPrev && 
                player->bishops[i].isActive)
            {
                bishop = &player->bishops[i];
                break;
            }
        }
    }

    // Bishop move logic: abs(difference) of rows must equal abs(difference) of columns
    dispX = move.rowNext - move.rowPrev;
    dispY = move.colNext - move.colPrev;

    if (abs(dispX) != abs(dispY) || dispX == 0) return INVALID_MOVE;

    rowStep = (dispX > 0) ? 1 : -1;
    colStep = (dispY > 0) ? 1 : -1;

    r = move.rowPrev + rowStep;
    c = move.colPrev + colStep;

    // Check for blocking pieces along the loadPath
    while (r != move.rowNext)
    {
        if (!isEmpty(board, r, c)) return INVALID_MOVE;

        r += rowStep;
        c += colStep;
    }

    // Handle Capture logic
    if (!isEmpty(board, move.rowNext, move.colNext))
    {
        if (pieceColorAt(board, move.rowNext, move.colNext) == bishop->color) return INVALID_MOVE;

        if (!legalCheck)
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK : COLOR_WHITE;
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;
        }
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    board[move.rowNext][move.colNext] = bishop->symbol;
    bishop->rowPosition = move.rowNext;
    bishop->colPosition = move.colNext;

    return VALID_MOVE;
}
