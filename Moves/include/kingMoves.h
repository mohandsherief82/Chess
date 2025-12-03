#ifndef KING_MOVES_H
#define KING_MOVES_H

#include "../../Pieces/include/player.h"
#include "./captures.h"

bool moveKing(char** board, Player player, Move move, Captured* playerCaptures);

#endif
