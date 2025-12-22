#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include "./Board/include/board.h"
#include "./Pieces/include/player.h"
#include "./Moves/include/pawnMoves.h"
#include "./Moves/include/rookMoves.h"
#include "./Moves/include/knightMoves.h"
#include "./Moves/include/bishopMoves.h"
#include "./Moves/include/queenMoves.h"
#include "./Moves/include/kingMoves.h"
#include "./Moves/include/captures.h"
#include "./Game-End/include/saveGame.h"
#include "./Game-End/include/staleMate.h"
#include "./Game-End/include/checkMate.h"
#include "./Game-End/include/check.h"

extern char *path;

char playerTurn(char** board, Player* player, Captured* capture, int* plyEnPassantCol, int* opponentEnPassantCol, int* counter)
{
    if (*counter >= 100)
    {
        printf("Draw available by the 50-move rule!\n");
        printf("Would you like to claim the draw? (y/n): ");
        char choice;
        int c;
        
        if (scanf(" %c", &choice) == 1) 
        {
            while ((c = getchar()) != '\n' && c != EOF);
            
            if (tolower(choice) == 'y') 
            {
                printf("Game drawn by the 50-move rule!\n");
                return 't'; 
            }
            else printf("Continuing game...\n");

        }
    }

    if (checkStalemate(board, player))
    {
        printf("No Valid Moves for this player.\nGame Ended with a Stalemate!!!\n");
        exit(0);
    }

    bool legalCheck = false;
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 's') return 's';
        else if (move.symbol == 'u') return 'u';
        else if (move.symbol == 'e') return 'e';

        bool pieceMoveValid = false;
        char moveSymbol = tolower(move.symbol);
        
        if (moveSymbol == 'p') pieceMoveValid = movePawn(board, player, &move, capture, plyEnPassantCol, opponentEnPassantCol, legalCheck, false);
        else if (moveSymbol == 'r') pieceMoveValid = moveRook(board, player, move, capture, legalCheck);
        else if (moveSymbol == 'n') pieceMoveValid = moveKnight(board, player, move, capture, legalCheck);
        else if (moveSymbol == 'b') pieceMoveValid = moveBishop(board, player, move, capture, legalCheck);
        else if (moveSymbol == 'q') pieceMoveValid = moveQueen(board, player, move, capture, legalCheck);
        else if (moveSymbol == 'k') pieceMoveValid = moveKing(board, player, move, capture, legalCheck);
        
        if (!pieceMoveValid) continue;
        
        if (isChecked(board, player, !legalCheck)) return 'i';

        if (capture->newCapture == true || moveSymbol == 'p') *counter = 0;
        else (*counter)++;

        break; 
    }
    
    saveMove(move);

    return '_';
}

int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(COLOR_WHITE)
            , blackCaptures = initializeCapture(COLOR_BLACK);
    char** board = initializeBoard(), gameInit = '\0';
    char gameState = '_';
    int whiteEnPassantCol = -1, blackEnPassantCol = -1;
    int c, currentPlayerTurn = 1, moveCounter = 0;

    
    clearScreen();
    printf("|----------------------------------------------------------------------------------------------------------------------"
            "-------------------------------------------------------|\n");
    printf("\t\t\t\t\t\t\t\t Welcome To Terminal Chess\n");
    printf("- In our game, we represent white pieces with lowercase letters and black pieces with uppercase letters.\n\
        - Each piece has a different letter, where: \n \t- p: white pawn\n\t- r: white rook.\n\t- N: black knight.\n\t- b: white bishop\n\
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

    updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);

    while (true && gameState != 's')
    {
        // Player 1's turn
        if (currentPlayerTurn == 1)
        {
            printf("Player 1's turn: \n");

            isChecked(board, &ply1, false);
            gameState = playerTurn(board, &ply1, &whiteCaptures, &whiteEnPassantCol, &blackEnPassantCol, &moveCounter);

            if (whiteCaptures.newCapture == true) capturePiece(&ply2, &whiteCaptures);
            
            if (gameState == 's') break;
            else if (gameState == 'u')
            {
                if(undoLastMove(board, &ply1, &ply2, &whiteCaptures, &blackCaptures
                            , &whiteEnPassantCol, &blackEnPassantCol))
                {
                    printf("Move undone.\nReturning turn to player 2!!!\n");
                    currentPlayerTurn = 2;
                    clearScreen();
                    updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                }
                else printf("No moves to undo, Try Again!!!\n");

                continue;
            }
            else if (gameState == 'i')
            {
                clearScreen();
                loadGame(board, &ply1, &ply2, 
                            &whiteCaptures, &blackCaptures, 
                                &whiteEnPassantCol, &blackEnPassantCol);
                updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                printf("Illegal move: King is in check, Try Again!!!\n\n");
                continue;
            }
            else if (gameState == 'e')
            {
                printf("Game Ends!!!\nPlayer 2 wins by Resignation!!!\n");
                break;
            }
            else if (gameState == 't') break;

            clearScreen();
            updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
            currentPlayerTurn = 2;

            if (checkMate(board, &ply2))
            {
                printf("Game Ended By Checkmate.\nPlayer 1 wins!!!\n");
                printf("Do you want to undo last move(u, press any key to end game)? ");
                if (scanf("%c", &gameState) == 1)
                {
                    if (gameState == 'u')
                    {    
                        undoLastMove(board, &ply1, &ply2, &whiteCaptures, &blackCaptures, &whiteEnPassantCol, &blackEnPassantCol);
                        currentPlayerTurn = 1;
                        clearScreen();
                        updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                    
                        continue;
                    }
                }

                printf("Game Ended, Good Game!!!\n");
                remove(path);
                break;
            }

            if (blackEnPassantCol != -1) blackEnPassantCol = -1; 
        }

        // Player 2's turn
        if (currentPlayerTurn == 2)
        {
            printf("Player 2's turn: \n");
            
            isChecked(board, &ply2, false);
            gameState = playerTurn(board, &ply2, &blackCaptures, &blackEnPassantCol, &whiteEnPassantCol, &moveCounter);
            if (blackCaptures.newCapture == true) capturePiece(&ply1, &blackCaptures);
            
            if (gameState == 's') break;
            else if (gameState == 'u')
            {
                if(undoLastMove(board, &ply1, &ply2, &whiteCaptures, &blackCaptures
                            , &whiteEnPassantCol, &blackEnPassantCol))
                {
                    printf("Move undone.\nReturning turn to player 1!!!\n");
                    currentPlayerTurn = 1;
                    clearScreen();
                    updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                }
                else printf("No moves to undo, Try Again!!!\n");
                
                continue;
            }
            else if (gameState == 'i')
            {
                clearScreen();
                loadGame(board, &ply1, &ply2, 
                            &whiteCaptures, &blackCaptures, 
                                &whiteEnPassantCol, &blackEnPassantCol);
                updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                printf("Illegal move: King is in check, Try Again!!!\n\n");
                continue;
            }
            else if (gameState == 'e')
            {
                printf("Game Ends!!!\nPlayer 1 wins by Resignation!!!\n");
                break;
            }
            else if (gameState == 't') break;
            
            clearScreen();
            updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
            currentPlayerTurn = 1;

            if (checkMate(board, &ply1))
            {
                printf("Game Ended By Checkmate.\nPlayer 2 wins!!!\n");
                printf("Do you want to undo last move(u, press any key to end game)? ");
                if (scanf("%c", &gameState) == 1)
                {
                    if (gameState == 'u')
                    {    
                        undoLastMove(board, &ply1, &ply2, &whiteCaptures, &blackCaptures, &whiteEnPassantCol, &blackEnPassantCol);
                        currentPlayerTurn = 2;
                        clearScreen();
                        updateBoard(board, ply1, ply2, whiteCaptures, blackCaptures, true);
                    
                        continue;
                    }
                }

                printf("Game Ended, Good Game!!!\n");
                remove(path);
                break;
            }

            if (whiteEnPassantCol != -1) whiteEnPassantCol = -1; 
        }
    }

    freeBoard(board, ply1, ply2);

    if (gameState == 's')
    {
        clearScreen();
        printf("Done, Game Saved!!!\n");
    }
    else remove(path);

    printf("-----------------------------------------------------------------------------------------------------------------------"
            "--------------------------------------------------------\n");

    return 0;
}
