#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool moveQueen(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    int dispX, dispY, rowStep = 0, colStep = 0, r, c;
    Queen* queen = (Queen*)checkPromotedPawn(player, move);
    
    if (queen == NULL && player->queen[0].isActive) 
    {
        if (player->queen[0].colPosition == move.colPrev && player->queen[0].rowPosition == move.rowPrev) 
        {
            queen = player->queen;
        }
    }
    
    if (queen == NULL)
    {
        if (!legalCheck) printf("No Queen At This Position, Try Again!!!\n");
        return false;
    }
    if (queen->isPinned) 
    {
        if (!legalCheck) printf("This queen is pinned, Try Again!!!\n");
        return false;
    }

    dispX = move.rowNext - move.rowPrev;
    dispY = move.colNext - move.colPrev;

    if ((dispX != 0 && dispY != 0) && abs(dispX) != abs(dispY)) 
    {
        if (!legalCheck) printf("Invalid Queen Move, Try Again!!!\n");
        return false; 
    }

    if (dispX != 0) rowStep = (dispX > 0) ? 1 : -1;
    if (dispY != 0) colStep = (dispY > 0) ? 1 : -1;

    r = move.rowPrev + rowStep;
    c = move.colPrev + colStep;

    if (dispX == 0 || dispY == 0) // Orthogonal (Rook-like) Movement
    {   
        while ((r != move.rowNext) || (c != move.colNext))
        {
            if(!isEmpty(board, r, c)) 
            {
                if (!legalCheck) printf("Invalid Queen Move, Try Again!!!\n");
                return false;
            }

            r += rowStep;
            c += colStep;
        }
    }
    else // Diagonal (Bishop-like) Movement
    {
        while ((r != move.rowNext) || (c != move.colNext))
        {
            if(!isEmpty(board, r, c))
            {
                if (!legalCheck) printf("Invalid Queen Move, Try Again!!!\n");
                return false;
            }

            r += rowStep;
            c += colStep;
        }
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == queen->color)
        {
            if (!legalCheck) printf("Can't Capture Friendly piece, Try Again!!!\n");
            return false;
        }
        
        if (!legalCheck)
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->capturedPiece.isActive = false;
            
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;
        }
    }

    if (!legalCheck)
    {
        board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
        queen->rowPosition = move.rowNext;
        queen->colPosition = move.colNext;
    }
    
    return true;
}
