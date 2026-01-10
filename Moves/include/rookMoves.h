#ifndef ROCK_MOVES_H
#define ROCK_MOVES_H

#include "player.h"
#include "captures.h"

MoveValidation moveRook(char** board ,Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
