#ifndef KING_MOVES_H
#define KING_MOVES_H

#include "player.h"
#include "captures.h"

MoveValidation moveKing(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
