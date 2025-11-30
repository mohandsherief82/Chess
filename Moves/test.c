#include <stdio.h>

#include "./include/pawnMoves.h"
#include "./include/rockMoves.h"
#include "../Board/include/board.h"

int main ()
{
    Player ply1 = createPlayer(COLOR_WHITE)
            , ply2 = createPlayer(COLOR_BLACK);
    Move move;
    char** board = initializeBoard();

    displayBoard(board, ply1, ply2);

    while (true)
    {
        printf("Player 1's turn: ");

        while (true)
        {
            move = getMove();

            if (move.symbol == 'p')
            {
                if (movePawn(board, ply1, move)) break;
                else printf("Invalid Pawn Move, Try Again!!!\n");
            }
        }

        clearScreen();
        displayBoard(board, ply1, ply2);

        printf("Player 2's turn: ");
        while (true)
        {
            move = getMove();

            if (move.symbol == 'p')
            {
                if (movePawn(board, ply2, move)) break;
                else printf("Invalid Pawn Move, Try Again!!!\n");
            }
        }

        clearScreen();
        displayBoard(board, ply1, ply2);
    }

    freeBoard(board, ply1, ply2);

    return 0;
}
