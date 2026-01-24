#ifndef PAWN_MOVES_H
#define PAWN_MOVES_H

#include "player.h"
#include "captures.h"

Piece* checkPromotedPawn(Player* player, Move move);
void promotePawn(Move move, Player* player, char promoSymbol);
MoveValidation movePawn(char** board, Player* player, Move move
        , Captured* playerCaptures, int *plyEnPassantCol, int *oppEnPassantCol
                , bool legalCheck, bool load);

#endif
