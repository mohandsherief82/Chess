#include "./include/player.h"

#include <stdio.h>
#include <stdlib.h>

int main ()
{
    Player player = createPlayer(COLOR_WHITE);

    printf("Player %d's move: \n", (player.color == COLOR_WHITE) ? 1: 2);
    Move move = getMove();


    free(player.pawns);
    free(player.rooks);
    free(player.bishops);
    free(player.knights);

    return 0;
}
