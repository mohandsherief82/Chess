// #include "../Pieces/bishop.h"
#include "../Pieces/pawn.h"
// #include "../Pieces/king.h"
// #include "../Pieces/knight.h"
// #include "../Pieces/queen.h"
// #include "../Pieces/rock.h"
#include "Board.h"

#include <stdio.h>

int main()
{
    char Board[BOARD_SIZE][BOARD_SIZE];
    initialize_board(Board);
    display_board(Board);


    Pawn whitePawn = {.color = COLOR_WHITE, .symbol = 'p', .promoted=true};
    Pawn blackPawn = {.color = COLOR_WHITE, .symbol = 'P'};

    if (whitePawn.promoted || blackPawn.colPosition == 'a') printf("\nPromoted!!!\n");

    return 0;
}
