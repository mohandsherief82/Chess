#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../include/captures.h"
#include "../../chessTypes.h"

#define MOVE_SIZE 7

void saveMove(Move move)
{
    FILE* fptr = fopen("./Moves/testing/game.bin", "ab");

    char moveData[MOVE_SIZE];

    moveData[0] = move.symbol;
    moveData[1] = move.colPrev + 'a';
    moveData[2] = move.rowPrev + '0';
    moveData[3] = move.colNext + 'a';
    moveData[4] = move.rowNext + '0';
    moveData[5] = '\n';
    moveData[6] = '\0';

    fwrite(moveData, sizeof(char), MOVE_SIZE, fptr);

    fclose(fptr);
    return;
}
