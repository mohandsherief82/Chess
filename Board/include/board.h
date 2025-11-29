#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define BLACK_SQUARE '.'
#define WHITE_SQUARE '-'
#define EMPTY_SQUARE ' '

#include "../../Pieces/include/player.h"

char** initializeBoard();

void displayBoard(char** board, Player player1, Player player2);
void clearScreen();
void freeBoard(char** board, Player player1, Player player2);
bool isEmpty(char** board, int r, int c);

#endif