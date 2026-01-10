#include "player.h"
#include "board.h"
#include "pawnMoves.h"
#include "captures.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

MoveValidation moveRook(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    int diffrow, diffcol, rowstep = 0, colstep = 0, r, c;
    Rook* rock = (Rook*)checkPromotedPawn(player, move);

    if (rock == NULL)
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->rooks[i].colPosition == move.colPrev && player->rooks[i].rowPosition == move.rowPrev && player->rooks[i].isActive)
            {
                rock = &player->rooks[i];
                break;
            }
        }
    }

    diffrow = move.rowNext - move.rowPrev;
    diffcol = move.colNext - move.colPrev;

    if (diffrow != 0 && diffcol != 0) return INVALID_MOVE;

    if (diffrow != 0) rowstep = (diffrow > 0) ? 1 : -1;
    else colstep = (diffcol > 0) ? 1 : -1;

    r = move.rowPrev + rowstep;
    c = move.colPrev + colstep;

    while (r != move.rowNext || c != move.colNext)
    {
        if (!isEmpty(board, r, c)) return INVALID_MOVE;

        r += rowstep;
        c += colstep;
    }

    if (!isEmpty(board, move.rowNext, move.colNext))
    {
        if (pieceColorAt(board, move.rowNext, move.colNext) == rock->color) return FRIENDLY_CAPTURE;

        if (!legalCheck)
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK : COLOR_WHITE;
            playerCaptures->capturedPiece.colPosition = move.colNext;

            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = rock->symbol;
            rock->rowPosition = move.rowNext;
            rock->colPosition = move.colNext;
            rock->firstMove = false;

            return ENEMY_CAPTURE;
        }
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    board[move.rowNext][move.colNext] = rock->symbol;
    rock->rowPosition = move.rowNext;
    rock->colPosition = move.colNext;
    rock->firstMove = false;

    return VALID_MOVE;
}
