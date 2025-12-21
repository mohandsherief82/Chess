#ifndef ROCK_MOVES_H
#define ROCK_MOVES_H

#include "../../Pieces/include/player.h"
#include "./captures.h"

bool moveRook(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
