#ifndef PAWN_MOVES_H
#define PAWN_MOVES_H

#include "../../Pieces/include/player.h"
#include "./captures.h"

Piece* checkPromotedPawn(Player* player, Move move);
bool movePawn(char** board, Player* player, Move move
        , Captured* playerCaptures, int *plyEnPassantCol, int *oppEnPassantCol);

#endif
