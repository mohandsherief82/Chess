#ifndef KNIGHT_MOVES_H
#define KNIGHT_MOVES_H

#include "player.h"
#include "captures.h"

MoveValidation moveKnight(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
