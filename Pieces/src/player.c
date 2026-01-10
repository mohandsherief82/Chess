#include "player.h"
#include "chessTypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define BUFFER 5

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
    if (bishops == NULL) exit(1);
    for (int i = 0; i < NUM_PIECES; i++) bishops[i] = createBishop(color, backRank, i);

    Knight *knights = (Knight*)malloc(NUM_PIECES * sizeof(Knight));
    if (knights == NULL) exit(1);
    for (int i = 0; i < NUM_PIECES; i++) knights[i] = createKnight(color, backRank, i);

    Rook *rooks = (Rook*)malloc(NUM_PIECES * sizeof(Rook));
    if (rooks == NULL) exit(1);
    for (int i = 0; i < NUM_PIECES; i++) rooks[i] = createRook(color, backRank, i);

    Pawn *pawns = (Pawn*)malloc(NUM_PAWNS * sizeof(Pawn));
    if (pawns == NULL) exit(1);
    for (int i = 0; i < NUM_PAWNS; i++) pawns[i] = createPawn(color, pawnRank, i);

    Queen *queen = (Queen*)malloc(sizeof(Queen));
    if (queen == NULL) exit(1);
    *queen = createQueen(color, backRank);

    King *king = (King*)malloc(sizeof(King));
    if (king == NULL) exit(1);
    *king = createKing(color, backRank);

    Player player = {
        .color = color,
        .bishops = bishops,
        .knights = knights,
        .rooks = rooks,
        .pawns = pawns,
        .queen = queen,
        .king = king
    };

    return player;
}


bool isValidSymbol(char symbol)
{
    return (symbol == 'p' || symbol == 'r' || symbol == 'n' || symbol == 'u' || symbol == 'e'
            ||  symbol == 'b' || symbol == 'q' || symbol == 'k' || symbol == 's' || symbol == 'd');
}


bool isValidMove(int coordinate)
{
    return (coordinate >= 1 && coordinate <= 8);
}


void freePlayer(Player player)
{
    free(player.pawns);
    player.pawns = NULL;
    
    free(player.bishops);
    player.bishops = NULL;
    
    free(player.knights);
    player.knights = NULL;
    
    free(player.rooks);
    player.rooks = NULL;
    
    free(player.queen);
    player.queen = NULL;
    
    free(player.king);
    player.king = NULL;

    return;
}
