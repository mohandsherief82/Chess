#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define BLACK_SQUARE '.'
#define WHITE_SQUARE '-'
#define EMPTY_SQUARE ' '

#include "../../Pieces/include/player.h"
#include "../../Moves/include/captures.h"

char** initializeBoard();

void updateBoard(char** board, Player player1, Player player2, 
                        Captured ply1Captures, Captured ply2Captures, bool PRINT);
void clearScreen();
void freeBoard(char** board, Player player1, Player player2);
bool isEmpty(char** board, int r, int c);

#endif