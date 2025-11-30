#include <stdio.h>

#include "./include/pawnMoves.h"
#include "./include/rockMoves.h"
#include "../Board/include/board.h"
#include "./include/captures.h"

// Important for Game Logic
void playerTurn(char** board, Player player, Captured captures)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 'p')
        {
            if (movePawn(board, player, move)) break;
        }
    }
}

// Prototype for game logic
int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(ply1.color)
            , blackCaptures = initializeCapture(ply2.color);
    char** board = initializeBoard();

    displayBoard(board, ply1, ply2);

    while (true)
    {
        printf("Player 1's turn: ");

        playerTurn(board, ply1, whiteCaptures);

        clearScreen();
        displayBoard(board, ply1, ply2);

        printf("Player 2's turn: ");
        
        playerTurn(board, ply2, blackCaptures);

        clearScreen();
        displayBoard(board, ply1, ply2);
    }

    freeBoard(board, ply1, ply2);

    return 0;
}
