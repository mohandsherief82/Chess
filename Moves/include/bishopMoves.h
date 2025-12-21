#ifndef BISHOP_MOVES_H
#define BISHOP_MOVES_H

#include "../../Pieces/include/player.h"
#include "captures.h"

bool moveBishop(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
