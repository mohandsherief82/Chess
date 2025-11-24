#include <stdio.h>
#include "Board.h"

void initialize_board(char board[BOARD_SIZE][BOARD_SIZE]){

    // initialize board with empty squares
    for(int i =0; i<BOARD_SIZE; i++){
        for (int j = 0; i < BOARD_SIZE; j++)
        {
            board[i][j]= EMPTY_SQUARE;
        }
    }

    // Setup black pieces at the top of the board
    board[0][0] = 'R'; board[0][1] = 'N'; board[0][2] = 'B'; board[0][3] = 'Q';
    board[0][4] = 'K'; board[0][5] = 'B'; board[0][6] = 'N'; board[0][7] = 'R';
    for(int i =0; i < BOARD_SIZE; i++){
        board[1][i]='P';
    }

    // Setup white pieces at the bottom of the board
    board[7][0] = 'r'; board[7][1] = 'n'; board[7][2] = 'b'; board[7][3] = 'q';
    board[7][4] = 'k'; board[7][5] = 'b'; board[7][6] = 'n'; board[7][7] = 'r';
    for(int i =0; i < BOARD_SIZE; i++){
        board[6][i]='p';
    }

}