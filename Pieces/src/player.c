#include "../include/player.h"
#include "../../chessTypes.h"

#include <stdlib.h>


Player createPlayer(PieceColor color)
{
    int backRank, pawnRank;

    if (color == COLOR_BLACK)
    {
        backRank = 0;
        pawnRank = 1;
    }
    else if (color == COLOR_WHITE)
    {
        backRank = 7;
        pawnRank = 6;
    }

    Bishop *bishops = (Bishop*)malloc(NUM_PIECES * sizeof(Bishop));
    for (int i = 0; i < NUM_PIECES; i++) bishops[i] = createBishop(color, backRank, i);

    Knight *knights = (Knight*)malloc(NUM_PIECES * sizeof(Knight));
    for (int i = 0; i < NUM_PIECES; i++) knights[i] = createKnight(color, backRank, i);

    Rock *rocks = (Rock*)malloc(NUM_PIECES * sizeof(Rock));
    for (int i = 0; i < NUM_PIECES; i++) rocks[i] = createRock(color, backRank, i);

    Pawn *pawns = (Pawn*)malloc(NUM_PAWNS * sizeof(Pawn));
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

void freePlayer(Player player)
{
    free(player.pawns);
    free(player.bishops);
    free(player.knights);
    free(player.rocks);

    return;
}
