#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/utility.h"

#include <stdlib.h>

bool moveRock(char** board ,Player player, Move move)
{
    int diffrow, diffcol, rowstep = 0, colstep = 0, r, c;
    Rock* rock = NULL;

    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (player.rocks[i].colPosition == move.colPrev && player.rocks[i].rowPosition == move.rowPrev)
        {
            rock = &player.rocks[i];
            break;
        }
    }

    if (rock == NULL) return false; // Invalid rock postion.

    diffrow = move.rowNext - move.rowPrev;
    diffcol = move.colNext - move.colPrev;

    if ((diffrow == 0) && (diffcol == 0)) return false;
    if((diffrow != 0) && (diffcol !=0)) return false;

    if(diffrow != 0) rowstep = (diffrow > 0) ? 1 : -1;
    else colstep = (diffcol > 0) ? 1 : -1;

    r = move.rowPrev + rowstep;
    c = move.colPrev + colstep;

    while ((r != move.rowNext) || (c != move.colNext))
    {
        if(!isEmpty(board, r, c)) return false; // Blocked path.
        r += rowstep;
        c += colstep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext))
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == rock->color)
            return false; // Invalid: Allied piece at destination

        // Capture Logic
    }
    
    return true;
}
