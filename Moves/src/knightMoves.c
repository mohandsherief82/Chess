#include "player.h"
#include "board.h"
#include "pawnMoves.h"
#include "captures.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


MoveValidation moveKnight(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    int dispX, dispY;
    Knight* knight = (Knight*)checkPromotedPawn(player, move);

    if (knight == NULL)
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->knights[i].colPosition == move.colPrev && player->knights[i].rowPosition == move.rowPrev && player->knights[i].isActive)
            {
                knight = &player->knights[i];
                break;
            }
        }
    }

    dispX = abs(move.rowNext - move.rowPrev);
    dispY = abs(move.colNext - move.colPrev);

    if (!((dispX == 2 && dispY == 1) || (dispX == 1 && dispY == 2))) return INVALID_MOVE;

    if (!isEmpty(board, move.rowNext, move.colNext))
    {      
        if (pieceColorAt(board, move.rowNext, move.colNext) == knight->color) return INVALID_MOVE;
        
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
    board[move.rowNext][move.colNext] = knight->symbol;
    knight->rowPosition = move.rowNext;
    knight->colPosition = move.colNext;

    return VALID_MOVE;
}
