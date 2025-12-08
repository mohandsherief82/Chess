#include <stdio.h>
#include <ctype.h>

#include "../Moves/include/pawnMoves.h"
#include "../Moves/include/rockMoves.h"
#include "../Moves/include/knightMoves.h"
#include "../Moves/include/bishopMoves.h"
#include "../Moves/include/queenMoves.h"
#include "../Moves/include/kingMoves.h"
#include "../Moves/include/captures.h"
#include "./include/saveGame.h"
#include "../Board/include/board.h"

extern char *path;


bool playerTurn(char** board, Player* player, Captured* capture)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 's') return true;

        bool pieceMoveValid = false;
        
        if (move.symbol == 'p') pieceMoveValid = movePawn(board, player, move, capture);
        else if (move.symbol == 'r') pieceMoveValid = moveRock(board, player, move, capture);
        else if (move.symbol == 'n') pieceMoveValid = moveKnight(board, player, move, capture);
        else if (move.symbol == 'b') pieceMoveValid = moveBishop(board, player, move, capture);
        else if (move.symbol == 'q') pieceMoveValid = moveQueen(board, player, move, capture);
        else if (move.symbol == 'k') pieceMoveValid = moveKing(board, player, move, capture);
        
        if (!pieceMoveValid) continue;
        
        if (isChecked(board, player))
        {
            printf("Illegal move: King remains in check or moved into check. Reverting move.\n");
            
            // undoLastMove(board, player, move, capture); 
            continue; 
        }

        break; 
    }
    
    saveMove(move);

    return false;
}

// Prototype for game logic
int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(COLOR_WHITE)
            , blackCaptures = initializeCapture(COLOR_BLACK);
    char** board = initializeBoard(), gameInit = '\0';
    bool saveGame = false;
    int c;


    printf("|-------------------------------------------------------------------------------------------------------------------"
            "----------------------------------------------------------|\n");
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
        else break;
    }

    while ((c = getchar()) != '\n' && c != EOF);

    if (gameInit == 'l')
    {
        bool turnFlag = loadGame(board, &ply1, &ply2, &whiteCaptures, &blackCaptures);

        if (turnFlag)
        {
            displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

            printf("Player 2's turn: ");
            
            isChecked(board, &ply2);
            saveGame = playerTurn(board, &ply2, &blackCaptures);
            if (blackCaptures.newCapture == true) capturePiece(ply1, &blackCaptures);
            if (saveGame)
            {
                clearScreen();
                printf("Done, Game Saved!!!\n");
                return 0;
            }

            clearScreen();
        }
    }

    displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

    while (true && !saveGame)
    {
        printf("Player 1's turn: ");

        isChecked(board, &ply1);
        saveGame = playerTurn(board, &ply1, &whiteCaptures);
        if (whiteCaptures.newCapture == true) capturePiece(ply2, &whiteCaptures);
        if (saveGame)
        {
            clearScreen();
            printf("Done, Game Saved!!!\n");
            break;
        }

        clearScreen();
        displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

        printf("Player 2's turn: ");
        
        isChecked(board, &ply2);
        saveGame = playerTurn(board, &ply2, &blackCaptures);
        if (blackCaptures.newCapture == true) capturePiece(ply1, &blackCaptures);
        if (saveGame)
        {
            clearScreen();
            printf("Done, Game Saved!!!\n");
            break;
        }

        clearScreen();
        displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);
    }

    freeBoard(board, ply1, ply2);

    if (!saveGame) remove(path);

    return 0;
}
