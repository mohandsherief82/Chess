#ifndef KNIGHT_MOVES_H
#define KNIGHT_MOVES_H

#include "../../Pieces/include/player.h"

bool moveKnight(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck);

#endif
