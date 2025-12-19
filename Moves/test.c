#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../Moves/include/pawnMoves.h"
#include "../Moves/include/rockMoves.h"
#include "../Moves/include/knightMoves.h"
#include "../Moves/include/bishopMoves.h"
#include "../Moves/include/queenMoves.h"
#include "../Moves/include/kingMoves.h"
#include "../Moves/include/captures.h"
#include "../Game-End/include/saveGame.h"
#include "../Board/include/board.h"
#include "../Pieces/include/player.h"

extern char *path;

bool playerTurn(char** board, Player* player, Captured* capture, int* plyEnPassantCol, int* opponentEnPassantCol)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 's') return true;
        // else if (move.symbol == 'u') return false;

        bool pieceMoveValid = false;
        char moveSymbol = tolower(move.symbol);
        
        if (moveSymbol == 'p') pieceMoveValid = movePawn(board, player, move, capture, plyEnPassantCol, opponentEnPassantCol);
        else if (moveSymbol == 'r') pieceMoveValid = moveRock(board, player, move, capture);
        else if (moveSymbol == 'n') pieceMoveValid = moveKnight(board, player, move, capture);
        else if (moveSymbol == 'b') pieceMoveValid = moveBishop(board, player, move, capture);
        else if (moveSymbol == 'q') pieceMoveValid = moveQueen(board, player, move, capture);
        else if (moveSymbol == 'k') pieceMoveValid = moveKing(board, player, move, capture);
        
        if (!pieceMoveValid) continue;
        
        if (isChecked(board, player))
        {
            printf("Illegal move: King remains in check, Try Again!!!\n");
            
            // undoMove(board, player, move, capture); 
            continue; 
        }

        break; 
    }
    
    saveMove(move);

    return false;
}

int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(COLOR_WHITE)
            , blackCaptures = initializeCapture(COLOR_BLACK);
    char** board = initializeBoard(), gameInit = '\0';
    bool saveGame = false;
    int whiteEnPassantCol = -1, blackEnPassantCol = -1;
    int c, currentPlayerTurn = 1;

    
    printf("|-------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("\t\t\t\t\t\t\t\t Welcome To Terminal Chess\n");
    printf("- In our game, we represent white pieces with lowercase letters and black pieces with uppercase letters.\n\
        - Each piece has a different letter, where: \n \t- p: white pawn\n\t- r: white rock.\n\t- N: black knight.\n\t- b: white bishop\n\
        - Q: black queen.\n\t- K: black king.\n");
        
    while (gameInit == '\0')
    {
        printf("Do you want to load a game or play game(p, l): ");
        if (scanf(" %c", &gameInit) != 1) 
        {
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Invalid input. Try again.\n");
            continue;
        }

        if (tolower(gameInit) != 'p' && tolower(gameInit) != 'l')
        {
            printf("Invalid Symbol, Try Again!!!\n");
            gameInit = '\0';
            while ((c = getchar()) != '\n' && c != EOF);

            continue;
        }
        else if (tolower(gameInit) == 'l')
        {
            currentPlayerTurn = loadGame(board, &ply1, &ply2, &whiteCaptures, &blackCaptures, &whiteEnPassantCol, &blackEnPassantCol);
            break;
        }
        else
        {
            remove(path);
            FILE *fptr = fopen(path, "wb");
            if (fptr != NULL) fclose(fptr);
            currentPlayerTurn = 1;
            break;
        }
    }
    
    while ((c = getchar()) != '\n' && c != EOF);

    updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

    while (true && !saveGame)
    {
        // Player 1's turn
        if (currentPlayerTurn == 1)
        {
            printf("Player 1's turn: \n");

            isChecked(board, &ply1);
            saveGame = playerTurn(board, &ply1, &whiteCaptures, &whiteEnPassantCol, &blackEnPassantCol);

            if (whiteCaptures.newCapture == true) capturePiece(ply2, &whiteCaptures);
            
            if (saveGame) break;

            clearScreen();
            updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures);
            currentPlayerTurn = 2;

            if (blackEnPassantCol != -1) blackEnPassantCol = -1; 
        }

        // Player 2's turn
        if (currentPlayerTurn == 2)
        {
            printf("Player 2's turn: \n");
            
            isChecked(board, &ply2);
            saveGame = playerTurn(board, &ply2, &blackCaptures, &blackEnPassantCol, &whiteEnPassantCol);
            if (blackCaptures.newCapture == true) capturePiece(ply1, &blackCaptures);
            
            if (saveGame) break;
            
            clearScreen();
            updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures);
            currentPlayerTurn = 1;

            if (whiteEnPassantCol != -1) whiteEnPassantCol = -1; 
        }
    }

    freeBoard(board, ply1, ply2);

    if (saveGame)
    {
        clearScreen();
        printf("Done, Game Saved!!!\n");
    }
    else remove(path);

    return 0;
}
