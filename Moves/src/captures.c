#include "../include/captures.h"
#include "../../Board/include/board.h"

PieceColor pieceColorAt(char** board, int row, int col)
{    
    if (isEmpty(board, row, col)) return -1; 

    if (board[row][col] >= 'A' && board[row][col] <= 'Z') return COLOR_BLACK;
    
    return COLOR_WHITE;
}


CaptureArray capture ={
    .countBlack =2,
    .countWhite =0,
    .captBlack = {'p', 'p', 'q'},
    .captBlack = {'\0'}
};
