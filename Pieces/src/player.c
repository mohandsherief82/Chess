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

    Queen *queen = (Queen*)malloc(sizeof(Queen));
    *queen = createQueen(color, backRank);

    King *king = (King*)malloc(sizeof(King));
    *king = createKing(color, backRank);

    Player player = {
        .color = color,
        .bishops = bishops,
        .knights = knights,
        .rocks = rocks,
        .pawns = pawns,
        .queen = queen,
        .king = king
    };

    return player;
}


bool isValidPiece(char symbol)
{
    return (symbol == 'p' || symbol == 'r' || symbol == 'n' || symbol == 'u' || symbol == 'r'
            ||  symbol == 'b' || symbol == 'q' || symbol == 'k' || symbol == 's');
}


bool isValidMove(int coordinate)
{
    return (coordinate >= 1 && coordinate <= 8);
}


void toLower(char *letter)
{
    if (*letter >= 'A' && *letter <= 'Z') *letter += 32;
}


Move getMove()
{
    Move move;

    char symbol, moveInput[5]; 
    char tempColPrev, tempColNext;
    int rowPrev, rowNext,
            colPrev, colNext;
    int c; // For input buffer clearing
    bool moveFlag = false;

    while (true)
    {
        printf("\nEnter piece symbol(p, r, n, b, q, k), (u for undo, s for save, r for resign): ");
        if (scanf(" %c", &symbol) != 1) 
        {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n");
            continue;
        }

        toLower(&symbol);

        if (isValidPiece(symbol)) break;
        printf("Invalid Piece Symbol, Try Again!!!!\n");
        while ((c = getchar()) != '\n' && c != EOF);
    }

    while ((c = getchar()) != '\n' && c != EOF);

    move.symbol = symbol;
    if (move.symbol == 's' || move.symbol == 'u' || move.symbol == 'r') return move;


    while (!moveFlag)
    {
        printf("\nEnter move (e.g., e2e4): ");
        if (scanf(" %4s", moveInput) != 1) 
        {
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

        // Extract and preprocess characters
        tempColPrev = moveInput[0];
        tempColNext = moveInput[2];
        
        toLower(&tempColPrev);
        toLower(&tempColNext);

        // Convert row characters ('1'-'8') to integers (1-8)
        rowPrev = moveInput[1] - '0'; 
        rowNext = moveInput[3] - '0';
        
        bool colsValid = (tempColPrev >= 'a' && tempColPrev <= 'h' &&
                          tempColNext >= 'a' && tempColNext <= 'h');
                          
        bool rowsValid = (isValidMove(rowPrev) && isValidMove(rowNext));

        if (colsValid && rowsValid) 
        {
            // Conversion to 0-7 indices happens here for storage
            colPrev = (int)tempColPrev - 97;
            colNext = (int)tempColNext - 97;
            
            if (rowPrev != rowNext || colPrev != colNext) moveFlag = true;
            else printf("Invalid Move, Try Again!!!\n");
        }
        
        else printf("Invalid coordinates: columns must be a-h and rows must be 1-8. Try Again!!!!\n");
    }
    
    rowPrev--;
    rowNext--;

    rowPrev = 7 - rowPrev;
    rowNext = 7 - rowNext;

    move.colPrev = colPrev;
    move.rowPrev = rowPrev;
    move.colNext = colNext;
    move.rowNext = rowNext;

    return move;
}


void freePlayer(Player player)
{
    free(player.pawns);
    free(player.bishops);
    free(player.knights);
    free(player.rocks);
    free(player.queen);
    free(player.king);

    return;
}
