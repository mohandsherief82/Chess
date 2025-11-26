#include "./include/player.h"

#include <stdio.h>
#include <stdlib.h>

int main ()
{
    Player player = createPlayer(COLOR_WHITE);

    for (int i = 0; i < NUM_PAWNS; i++) printf("%c ", player.pawns[i].symbol);

    printf("\n");

    for (int i = 0; i < NUM_PIECES; i++) printf("%c ", player.rocks[i].symbol);

    printf("\n");

    for (int i = 0; i < NUM_PIECES; i++) printf("%c ", player.bishops[i].symbol);

    printf("\n");

    for (int i = 0; i < NUM_PIECES; i++) printf("%c ", player.knights[i].symbol);

    printf("\n");

    printf("%c %c", player.king.symbol, player.queen.symbol);

    printf("\n");

    return 0;
}
