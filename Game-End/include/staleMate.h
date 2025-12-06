#ifndef STALE_MATE_H
#define STALE_MATE_H

#include "../../Pieces/include/player.h"

#include <stdbool.h>

bool checkStalemate(char** board, Player* player);
bool legalMove(char** board, Player* player);

#endif