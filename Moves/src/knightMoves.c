#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


bool moveKnight(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck)
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

    if (knight == NULL) 
    {
        if (!legalCheck) printf("No Knight At This Position, Try Again!!!\n");
        return false;
    }

    if (knight->isPinned)
    {
        if (!legalCheck) printf("This knight is Pinned, Try Again!!!\n");
        return false;
    }

    dispX = abs(move.rowNext - move.rowPrev);
    dispY = abs(move.colNext - move.colPrev);

    if (!((dispX == 2 && dispY == 1) || (dispX == 1 && dispY == 2))) 
    {
        if (!legalCheck) printf("Invalid Knight Move, Try Again!!!\n");
        return false;
    }

    if (!isEmpty(board, move.rowNext, move.colNext))
    {      
        if (pieceColorAt(board, move.rowNext, move.colNext) == knight->color)
        {
            if (!legalCheck) printf("Invalid Knight Move, Try Again!!!\n");
            return false;
        }
        
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

    return true;
}
