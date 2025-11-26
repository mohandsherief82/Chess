#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define BLACK_SQUARE '.'
#define WHITE_SQUARE '-'
#define EMPTY_SQUARE ' '

#include "../../Pieces/include/player.h"

char** initialize_board();

void display_board(char** board, Player player1, Player player2);

#endif