#include "../Pieces/include/player.h"
#include "../Moves/include/captures.h"
#include "./include/board.h"

#include <stdio.h>

int main()
{
    // Initialize Board and Players
    Player playerWhite = createPlayer(COLOR_WHITE),
        playerBlack = createPlayer(COLOR_BLACK);
    Captured whiteCaptures = initializeCapture(COLOR_WHITE)
            , blackCaptures = initializeCapture(COLOR_BLACK);
    char** Board = initializeBoard();
    
    // Display Board for testing
    updateBoard(Board, playerWhite, playerBlack, whiteCaptures, blackCaptures, true);

    freeBoard(Board, playerWhite, playerBlack);

    return 0;
}
