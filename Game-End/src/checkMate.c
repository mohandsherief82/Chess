#include "../../Moves/include/kingMoves.h"
#include "../include/saveGame.h"
#include "../../Board/include/board.h"
#include "../../Pieces/include/player.h"
#include "../include/check.h"
#include "../include/staleMate.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

bool checkMate(char** board, Player* currentPlayer) 
{
    if (!isChecked(board, currentPlayer, true)) return false;
    if (!legalMove(board, currentPlayer)) return true;

    return false;
}
