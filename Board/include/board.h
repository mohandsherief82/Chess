#ifndef BOARD_H
#define BOARD_H

#include "player.h"
#include "captures.h"

#include <stdio.h>

char** initializeBoard();

void updateBoard(char** board, Player player1, Player player2);
void freeBoard(char** board, Player player1, Player player2);
long getFileSize(FILE *fp);

#endif