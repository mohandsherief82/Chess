#include "../include/player.h"
#include "../../chessTypes.h"

#include <stdlib.h>
#include <stdio.h>


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


bool isValidPiece(char symbol)
{
    if (symbol == 'p') return true;
    else if (symbol == 'r') return true;
    else if (symbol == 'n') return true;
    else if (symbol == 'b') return true;
    else if (symbol == 'q') return true;
    else if (symbol == 'k') return true;
    else return false;
}


bool isValidMove(int coordinate)
{
    if (coordinate >= 0 && coordinate <= 7) return true;

    printf("Invalid Move, Try Again!!!!\n");

    return false;
}


void toLower(char *letter)
{
    if (*letter >= 'A' && *letter <= 'Z') *letter += 32;
}


Move getMove()
{
    char tempColPrev, tempColNext, pieceSymbol;
    int rowPrev, rowNext, colPrev, colNext;

    while (true)
    {
        printf("\nEnter piece symbol(p, r, n, b, q, k): ");
        scanf(" %c", &pieceSymbol);

        toLower(&pieceSymbol);

        if (isValidPiece(pieceSymbol)) break;
        printf("Invalid Piece Symbol, Try Again!!!!\n");
    }

    while (true)
    {
        printf("\nEnter move: ");
        scanf(" %c%d%c%d", &tempColPrev, &rowPrev, &tempColNext, &rowNext);

        toLower(&tempColPrev);
        toLower(&tempColNext);

        colPrev = (int)tempColPrev - 97;
        colNext = (int)tempColNext - 97;


        if (isValidMove(colPrev) && isValidMove(--rowPrev) 
                && isValidMove(colNext) && isValidMove(--rowNext)) break;
    }
    
    Move move = {
        .pieceSymbol = pieceSymbol,
        .colPrev = colPrev,
        .rowPrev = rowPrev,
        .colNext = colNext,
        .rowNext = rowNext,
        .isValid = true
    };

    return move;
}


void freePlayer(Player player)
{
    free(player.pawns);
    free(player.bishops);
    free(player.knights);
    free(player.rocks);

    return;
}
