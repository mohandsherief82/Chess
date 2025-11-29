#include "./include/player.h"

#include <stdio.h>
#include <stdlib.h>

int main ()
{
    Player player = createPlayer(COLOR_WHITE);

    printf("Player %d's move: \n", (player.color == COLOR_WHITE) ? 1: 2);
    Move move = getMove();

    printf("%d %d %c %d %d %d\n", move.colPrev, move.rowPrev, move.symbol, move.colNext, move.rowNext, move.isValid);

    free(player.pawns);
    free(player.rocks);
    free(player.bishops);
    free(player.knights);

    return 0;
}
