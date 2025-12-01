#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>

bool moveKnight(char** board ,Player player, Move move)
{
    int dispX, dispY;
    Knight* knight = (Knight*)checkPromotedPawn(player, move);

    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (player.knights[i].colPosition == move.colPrev && player.knights[i].rowPosition == move.rowPrev)
        {
            knight = &player.knights[i];
            break;
        }
    }

    if (knight == NULL) return false;

    dispX = abs(move.rowNext - move.rowPrev);
    dispY = abs(move.colNext - move.colPrev);

    // Fix: Corrected the logical expression to validate the L-shape movement.
    if (!((dispX == 2 && dispY == 1) || (dispX == 1 && dispY == 2))) 
    {
        printf("Invalid Knight Move, Try Again!!!\n");
        return false;
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {      
        if(pieceColorAt(board, move.rowNext, move.colNext) == knight->color)
        {
            printf("Invalid Knight Move, Try Again!!!\n");
            return false;
        }
        
        // Capture logic
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    knight->rowPosition = move.rowNext;
    knight->colPosition = move.colNext;
    return true;
}
