#include "../../Pieces/include/player.h"
#include "../include/check.h"
#include "../include/staleMate.h"

bool checkMate(char** board, Player* player)
{
    if (isChecked(board, player) && !legalMove(board, player)) return true;
    else return false;   
}
