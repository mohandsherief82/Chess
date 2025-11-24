// #include "../Pieces/bishop.h"
#include "../Pieces/pawn.h"
// #include "../Pieces/king.h"
// #include "../Pieces/knight.h"
// #include "../Pieces/queen.h"
// #include "../Pieces/rock.h"

#include <stdio.h>

int main()
{
    Pawn whitePawn = {.color = COLOR_WHITE, .symbol = 'p', .promoted=true};
    Pawn blackPawn = {.color = COLOR_WHITE, .symbol = 'P'};

    if (whitePawn.promoted || blackPawn.colPosition == 'a') printf("\nPromoted!!!\n");

    return 0;
}
