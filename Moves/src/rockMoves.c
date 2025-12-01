#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>

bool moveRock(char** board ,Player player, Move move, Captured* playerCaptures)
{
    int diffrow, diffcol, rowstep = 0, colstep = 0, r, c;
    Rock* rock = (Rock*)checkPromotedPawn(player, move);

    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (player.rocks[i].colPosition == move.colPrev 
            && player.rocks[i].rowPosition == move.rowPrev && player.rocks[i].isActive)
        {
            rock = &player.rocks[i];
            break;
        }
    }

    if (rock == NULL)
    {
        printf("No Rock At This Position, Try Again!!!\n");
        return false;
    }

    diffrow = move.rowNext - move.rowPrev;
    diffcol = move.colNext - move.colPrev;

    if((diffrow != 0) && (diffcol !=0)) 
    {
        printf("Invalid Rock Move, Try Agian!!!\n");
        return false;
    }

    if(diffrow != 0) rowstep = (diffrow > 0) ? 1 : -1;
    else colstep = (diffcol > 0) ? 1 : -1;

    r = move.rowPrev + rowstep;
    c = move.colPrev + colstep;

    while ((r != move.rowNext) || (c != move.colNext))
    {
        if(!isEmpty(board, r, c)) // Blocked path.
        {
            printf("Invalid Rock Move, Try Agian!!!\n");
            return false;
        }
        r += rowstep;
        c += colstep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == rock->color)
        {   
            printf("Can't Capture Friendly Piece, Try Again!!!\n"); 
            return false;
        }

        // Capture Logic
        playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            
        playerCaptures->capturedPiece.colPosition = move.colNext;
        playerCaptures->capturedPiece.rowPosition = move.rowNext;
        playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            
        playerCaptures->captureCount++;
        playerCaptures->newCapture = true;
    }
    
    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    rock->rowPosition = move.rowNext;
    rock->colPosition = move.colNext;
    return true;
}
