#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool moveQueen(char** board, Player player, Move move)
{
    int diffrow, diffcol, rowstep = 0, colstep = 0, r, c;
    Queen* queen = (Queen*)checkPromotedPawn(player, move);
    
    if (queen == NULL && player.queen.isActive) 
    {
        if (player.queen.colPosition == move.colPrev && player.queen.rowPosition == move.rowPrev) 
        {
            queen = &player.queen;
        }
    }
    
    if (queen == NULL)
    {
        printf("No Queen At This Position, Try Again!!!\n");
        return false;
    }

    diffrow = move.rowNext - move.rowPrev;
    diffcol = move.colNext - move.colPrev;

    // Queen's Path Check:
    // If both row and column change, they must change by the same absolute amount (diagonal)
    // or one of them is zero.
    if (diffrow != 0 && diffcol != 0 && abs(diffrow) != abs(diffcol)) 
    {
        printf("Invalid Queen Move, Try Again!!!\n");
        return false; 
    }

    if (diffrow != 0) rowstep = (diffrow > 0) ? 1 : -1;
    if (diffcol != 0) colstep = (diffcol > 0) ? 1 : -1;

    r = move.rowPrev + rowstep;
    c = move.colPrev + colstep;

    while ((r != move.rowNext) || (c != move.colNext))
    {
        if(!isEmpty(board, r, c)) 
        {
            printf("Invalid Queen Move, Try Again!!!\n");
            return false;
        }

        r += rowstep;
        c += colstep;
    }

    // --- 4. Destination Check and Execution ---

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == queen->color)
        {
            printf("Can't Capture Friendly piece, Try Again!!!\n");
            return false;
        }
        
        // Capture Logic
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    queen->rowPosition = move.rowNext;
    queen->colPosition = move.colNext;
    return true;
}