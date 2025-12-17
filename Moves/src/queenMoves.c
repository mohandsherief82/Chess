#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool moveQueen(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck) {
    int dispX, dispY, rowStep = 0, colStep = 0, r, c;
    Queen* queen = (Queen*)checkPromotedPawn(player, move);
    
    if (queen == NULL && player->queen[0].isActive) 
    {
        if (player->queen->colPosition == move.colPrev 
                && player->queen->rowPosition == move.rowPrev && !player->queen->isPinned) 
        {
            queen = player->queen;
        }
    }
    
    if (queen == NULL || queen->isPinned) return false;

    dispX = move.rowNext - move.rowPrev;
    dispY = move.colNext - move.colPrev;

    if ((dispX != 0 && dispY != 0) && abs(dispX) != abs(dispY)) return false; 

    if (dispX != 0) rowStep = (dispX > 0) ? 1 : -1;
    if (dispY != 0) colStep = (dispY > 0) ? 1 : -1;

    r = move.rowPrev + rowStep;
    c = move.colPrev + colStep;

    while ((r != move.rowNext) || (c != move.colNext)) 
    {
        if(!isEmpty(board, r, c)) return false;
        r += rowStep; c += colStep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext)) 
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == queen->color) return false;
        if (!legalCheck) 
        {
            playerCaptures->newCapture = true;
            playerCaptures->captureCount++;
        }
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    board[move.rowNext][move.colNext] = move.symbol;
    queen->rowPosition = move.rowNext;
    queen->colPosition = move.colNext;
    
    return true;
}
