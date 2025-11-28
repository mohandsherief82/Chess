#include "../Pieces/include/player.h"
#include "./include/board.h"

#include <stdio.h>

int main()
{
    // Initialize Board and Players
    Player playerWhite = createPlayer(COLOR_WHITE),
        playerBlack = createPlayer(COLOR_BLACK);
    char** Board = initialize_board();
    
    // Display Board for testing
    display_board(Board, playerWhite, playerBlack);

    freeBoard(Board, playerWhite, playerBlack);

    return 0;
}
