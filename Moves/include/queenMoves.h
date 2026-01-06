#ifndef QUEEN_MOVES_H
#define QUEEN_MOVES_H

#include "player.h"

bool moveQueen(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
