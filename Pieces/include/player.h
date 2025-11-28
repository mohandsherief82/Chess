#include "pawn.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "bishop.h"
#include "rock.h"
#include "../../chessTypes.h"

#ifndef PLAYER_H
#define PLAYER_H

#define NUM_PAWNS 8
#define NUM_PIECES 2

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
void freePlayer(Player player);
Move getMove();

#endif
