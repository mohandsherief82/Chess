#include "player.h"
#include "check.h"
#include "staleMate.h"

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
