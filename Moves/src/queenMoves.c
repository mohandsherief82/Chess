#include "player.h"
#include "board.h"
#include "pawnMoves.h"
#include "captures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


MoveValidation moveQueen(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    int dispX, dispY, rowStep = 0, colStep = 0, r, c;
    Queen* queen = (Queen*)checkPromotedPawn(player, move);
    
    if (queen == NULL && player->queen->isActive) 
    {
        if (player->queen->colPosition == move.colPrev && player->queen->rowPosition == move.rowPrev) 
        {
            queen = player->queen;
        }
    }

    dispX = move.rowNext - move.rowPrev;
    dispY = move.colNext - move.colPrev;

    // Validate queen movement "Diagonal or on axes"
    if (!((abs(dispX) == abs(dispY)) || (dispX == 0 || dispY == 0)) || (dispX == 0 && dispY == 0)) 
        return INVALID_MOVE;

    if (dispX != 0) rowStep = (dispX > 0) ? 1 : -1;
    if (dispY != 0) colStep = (dispY > 0) ? 1 : -1;

    r = move.rowPrev + rowStep;
    c = move.colPrev + colStep;

    // Path collision check
    while (r != move.rowNext || c != move.colNext)
    {
        if (!isEmpty(board, r, c)) return INVALID_MOVE;

        r += rowStep;
        c += colStep;
    }

    if (!isEmpty(board, move.rowNext, move.colNext))
    {
        if (pieceColorAt(board, move.rowNext, move.colNext) == queen->color) return FRIENDLY_CAPTURE;
        
        if (!legalCheck)
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->capturedPiece.isActive = false;
            
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = queen->symbol;
            queen->rowPosition = move.rowNext;
            queen->colPosition = move.colNext;

            return ENEMY_CAPTURE;
        }
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    board[move.rowNext][move.colNext] = queen->symbol;
    queen->rowPosition = move.rowNext;
    queen->colPosition = move.colNext;
    
    return true;
}
