#include "../include/player.h"
#include "../../chessTypes.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


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
    if (coordinate >= 1 && coordinate <= 8) return true;

    printf("Invalid Move, Try Again!!!!\n");

    return false;
}


void toLower(char *letter)
{
    if (*letter >= 'A' && *letter <= 'Z') *letter += 32;
}


Move getMove()
{
    char symbol, moveInput[5]; // Array to hold the 4-character move (e2e4) plus null terminator
    int rowPrev, rowNext,
            colPrev, colNext;
    int c; // For input buffer clearing
    bool moveFlag = false;

    while (true)
    {
        printf("\nEnter piece symbol(p, r, n, b, q, k): ");
        if (scanf(" %c", &symbol) != 1) 
        {
            // Handle scanf failures
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n");
            continue;
        }

        toLower(&symbol);

        if (isValidPiece(symbol)) break;
        printf("Invalid Piece Symbol, Try Again!!!!\n");
    }

    while (!moveFlag)
    {
        printf("\nEnter move (e.g., e2e4): ");
        // Read up to 4 characters of the move into moveInput, ignoring leading spaces
        if (scanf(" %4s", moveInput) != 1) 
        {
            // Handle scanf failure
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid move format, Try Again!!!!\n");
            continue;
        }

        // Clear the rest of the line
        while ((c = getchar()) != '\n' && c != EOF);

        if (strlen(moveInput) != 4) 
        {
            printf("Move must be 4 characters (colRowColRow), Try Again!!!!\n");
            continue;
        }

        // Extract and process characters
        char tempColPrev = moveInput[0];
        char tempColNext = moveInput[2];
        
        // Convert to lowercase (as before)
        toLower(&tempColPrev);
        toLower(&tempColNext);

        // Convert column characters to 0-7 indices
        colPrev = (int)tempColPrev - 97;
        colNext = (int)tempColNext - 97;

        // Convert row characters ('1'-'8') to integers
        rowPrev = moveInput[1] - '0'; 
        rowNext = moveInput[3] - '0';

        // 1. Validate the bounds (0-7 for columns, 1-8 for rows)
        // We check the 1-8 user input *before* converting to 0-7 index
        if (isValidMove(colPrev + 1) && isValidMove(colNext + 1)
            && isValidMove(rowPrev) && isValidMove(rowNext)) moveFlag = true;
        else printf("Invalid coordinates (must be a1-h8), Try Again!!!!\n");
    }
    
    rowPrev--;
    rowNext--;

    Move move = {
        .symbol = symbol,
        .colPrev = colPrev,
        .rowPrev = rowPrev,
        .colNext = colNext,
        .rowNext = rowNext,
        .isValid = true
    };

    return move;
}


void makeMove(Player player)
{
    Move move = getMove();

    printf("%c", move.symbol);
}


void freePlayer(Player player)
{
    free(player.pawns);
    free(player.bishops);
    free(player.knights);
    free(player.rocks);

    return;
}
