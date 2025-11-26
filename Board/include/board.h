#ifndef BOARD_H
#define BOARD_H

#define BOARD_SIZE 8
#define BLACK_SQUARE '.'
#define WHITE_SQUARE '-'
#define EMPTY_SQUARE ' '

void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]);

void display_board(char board[BOARD_SIZE][BOARD_SIZE]);

#endif