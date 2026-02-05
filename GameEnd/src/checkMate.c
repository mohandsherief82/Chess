#include "player.h"
#include "check.h"
#include "staleMate.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/**
 * @brief Determines if the current state is Checkmate.
 */
bool checkMate(char** board, Player* player) 
{
    return !legalMove(board, player) && isChecked(board, player, true);
}
