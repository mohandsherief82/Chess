#include "player.h"
#include "../chessTypes.h"

#include <stdlib.h>

#define NUM_PAWNS 8
#define NUM_PIECES 2


Player createPlayer(PieceColor color)
{
    int backRank, pawnRank;

    if (color = COLOR_BLACK)
    {
        backRank = 0;
        pawnRank = 1;
    }
    else if (color == COLOR_WHITE)
    {
        backRank = 7;
        pawnRank = 6;
    }
    else return;

    Bishop bishops[NUM_PIECES];
    for (int i = 0; i < NUM_PIECES; i++) bishops[i] = createBishop(color, backRank, i);

    Knight knights[NUM_PIECES];
    for (int i = 0; i < NUM_PIECES; i++) knights[i] = createKnight(color, backRank, i);

    Rock rocks[NUM_PIECES];
    for (int i = 0; i < NUM_PIECES; i++) rocks[i] = createRock(color, backRank, i);

    Pawn pawns[NUM_PAWNS];
    for (int i = 0; i < NUM_PAWNS; i++) pawns[i] = createPawn(color, pawnRank, i);

    Player player = {
        .color = color,
        .bishops = bishops,
        .knights = knights,
        .rocks = rocks,
        .pawns = pawns,
        .queen = createQueen(color, backRank),
        .king = createKing(color, backRank)
    };

    return player;
}
