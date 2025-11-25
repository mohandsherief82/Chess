#include "pawn.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "bishop.h"
#include "rock.h"
#include "../chessTypes.h"

#ifndef PLAYER_H
#define PLAYER_H

typedef struct
{
    const PieceColor color;
    Pawn *pawns;
    Rock *rocks;
    Knight *knights;
    Bishop *bishops;
    Queen queen;
    King king;
} Player;

Player createPlayer(PieceColor color);

#endif
