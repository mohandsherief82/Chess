#include <stdio.h>

#include "./include/pawnMoves.h"
#include "./include/rockMoves.h"
#include "./include/knightMoves.h"
#include "./include/bishopMoves.h"
#include "./include/queenMoves.h"
#include "./include/captures.h"
#include "../Board/include/board.h"

extern CaptureArray capture;

// Important for Game Logic
void playerTurn(char** board, Player player)
{
    Move move;
    while (true)
    {
        move = getMove();

        if (move.symbol == 'p')
        {
            if (movePawn(board, player, move)) break;
        }
        if (move.symbol == 'r')
        {
            if (moveRock(board, player, move)) break;
        }
        if (move.symbol == 'n')
        {
            if (moveKnight(board, player, move)) break;
        }
        if (move.symbol == 'b')
        {
            if (moveBishop(board, player, move, &capture)) break;
        }
        if (move.symbol == 'q')
        {
            if (moveQueen(board, player, move)) break;
        }
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
    char** board = initializeBoard();

    displayBoard(board, ply1, ply2);

    while (true)
    {
        printf("Player 1's turn: ");

        playerTurn(board, ply1);

        clearScreen();
        displayBoard(board, ply1, ply2);

        printf("Player 2's turn: ");
        
        playerTurn(board, ply2);

        clearScreen();
        displayBoard(board, ply1, ply2);
    }

    freeBoard(board, ply1, ply2);

    return 0;
}
