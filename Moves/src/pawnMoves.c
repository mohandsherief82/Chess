#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

bool movePawn(char** board, Move move, Player player)
{
    Pawn* pawn = NULL;
    int moveDirection; // 1 for Black (0->7), -1 for White (7->0)

    // Locate the correct Pawn object
    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move.colPrev == player.pawns[i].colPosition && move.rowPrev == player.pawns[i].rowPosition) 
        {
            pawn = &player.pawns[i];
            break;
        }
    }    
    
    if (pawn == NULL) return false; 

    // Determine direction
    moveDirection = (pawn->color == COLOR_WHITE) ? -1 : 1; 

    int rowDiff = move.rowNext - move.rowPrev;
    int colDiff = move.colNext - move.colPrev;

    // Single Square Advance
    if (colDiff == 0 && rowDiff == moveDirection)
    {
        if (isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = pawn->symbol;
            
            pawn->rowPosition += rowDiff;
            pawn->firstMove = false;
            return true;
        }
    }

    // Two Square Advance
    if (colDiff == 0 && rowDiff == (moveDirection * 2) && pawn->firstMove)
    {
        int midRow = move.rowPrev + moveDirection;
        
        if (isEmpty(board, midRow, move.colNext) && isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = pawn->symbol;
            
            pawn->rowPosition += rowDiff;
            pawn->firstMove = false;
            
            // En Passant flag setup would happen here
            return true;
        }
    }

    // Pawn Captures
    if (abs(colDiff) == 1 && rowDiff == moveDirection)
    {
        if (!isEmpty(board, move.rowNext, move.colNext)) 
        { 
            // NOTE: Full validation requires checking if the piece is an opponent's piece
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = pawn->symbol;
            
            pawn->rowPosition = rowDiff;
            pawn->colPosition = colDiff;
            pawn->firstMove = false;
            return true;
        }
    }
    
    // En Passant and Promotion logic would be added here
    return false;
}
