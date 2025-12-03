#include <stdio.h>

#include "./include/pawnMoves.h"
#include "./include/rockMoves.h"
#include "./include/knightMoves.h"
#include "./include/bishopMoves.h"
#include "./include/queenMoves.h"
#include "./include/captures.h"
#include "./include/saveGame.h"
#include "../Board/include/board.h"

// Important for Game Logic
bool playerTurn(char** board, Player player, Captured* capture)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 's')
        {
            return true;
        }
        else if (move.symbol == 'p')
        {
            if (movePawn(board, player, move, capture)) break;
        }
        else if (move.symbol == 'r')
        {
            if (moveRock(board, player, move)) break;
        }
        else if (move.symbol == 'n')
        {
            if (moveKnight(board, player, move)) break;
        }
        else if (move.symbol == 'b')
        {
            if (moveBishop(board, player, move, capture)) break;
        }
        else if (move.symbol == 'q')
        {
            if (moveQueen(board, player, move, capture)) break;
        }
        // else if (move.symbol == 'k')
        // {
        //     if (moveKing(board, player, move, capture)) break;
        // }
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
    char** board = initializeBoard();
    bool saveGame = false;

    printf("%p\n", &ply1.queen);

    displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

    while (true)
    {
        printf("Player 1's turn: ");

        saveGame = playerTurn(board, ply1, &whiteCaptures);
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
        
        saveGame = playerTurn(board, ply2, &blackCaptures);
        if (blackCaptures.newCapture == true) capturePiece(ply1, &blackCaptures);

        clearScreen();
        displayBoard(board, ply1, ply2, whiteCaptures, blackCaptures);

        if (saveGame)
        {
            clearScreen();
            printf("Done, Game Saved!!!\n");
            break;
        }
    }

    freeBoard(board, ply1, ply2);

    return 0;
}
