#include <stdio.h>

#include "./include/pawnMoves.h"
#include "../Board/include/board.h"

int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE);
    Player ply2 = createPlayer(COLOR_BLACK);
    Move move;
    char** board = initializeBoard();

    displayBoard(board, ply1, ply2);
    printf("Player 1's turn: ");
    move = getMove();

    movePawn(board, move, ply1);
    clearScreen();
    displayBoard(board, ply1, ply2);

    move = getMove();
    movePawn(board, move, ply2);
    clearScreen();
    displayBoard(board, ply1, ply2);

    freeBoard(board, ply1, ply2);

    return 0;
}
