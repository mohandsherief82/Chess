#include "../include/utility.h"
#include "../../Board/include/board.h"

PieceColor pieceColorAt(char** board, int row, int col)
{    
    if (isEmpty(board, row, col)) return -1; 

    if (board[row][col] >= 'A' && board[row][col] <= 'Z') return COLOR_BLACK;
    
    return COLOR_WHITE;
}