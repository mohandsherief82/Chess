#include "../include/player.h"
#include "../../chessTypes.h"

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


Move getMove()
{
    Move move;

    char symbol, moveInput[BUFFER]; 
    char tempColPrev, tempColNext;
    int rowPrev, rowNext,
            colPrev, colNext;
    int c; // For input buffer clearing
    bool moveFlag = false;

    while (true)
    {
        printf("\nEnter piece symbol(p, r, n, b, q, k), (u for undo, d for redo, s for save, e for resign): ");
        if (scanf(" %c", &symbol) != 1) 
        {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n");
            continue;
        }

        symbol = tolower(symbol);

        if (isValidSymbol(symbol)) break;
        printf("Invalid Piece Symbol, Try Again!!!!\n");
        while ((c = getchar()) != '\n' && c != EOF);
    }

    while ((c = getchar()) != '\n' && c != EOF);

    move.symbol = symbol;
    if (move.symbol == 's' || move.symbol == 'u' || move.symbol == 'e'|| move.symbol == 'd') return move;


    while (!moveFlag)
    {
        char inputLine[20];

        printf("\nEnter move (e.g., e2e4): ");
        if (fgets(inputLine, sizeof(inputLine), stdin) == NULL) 
        {
            printf("Error reading input, Try Again!!!\n");
            continue;
        }

        // Remove newline
        inputLine[strcspn(inputLine, "\n")] = 0;

        // Validate length
        if (strlen(inputLine) != 4) 
        {
            printf("Move must be 4 characters, Try Again!\n");
            continue;
        }

        strcpy(moveInput, inputLine);

        // Extract and preprocess characters
        tempColPrev = moveInput[0];
        tempColNext = moveInput[2];
        
        tempColPrev = tolower(tempColPrev);
        tempColNext = tolower(tempColNext);

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

    move.promotedPawn = '_';

    return move;
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
