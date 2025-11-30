#include "../include/captures.h"
#include "../../Board/include/board.h"

Captured initializeCapture(PieceColor color)
{
    Captured captures = {
        .color = color,
        .capturedPawns = 0,
        .capturedRocks = 0,
        .capturedKnights = 0,
        .capturedBishops = 0,
        .capturedQueens = 0
    };

    return captures;
}

PieceColor pieceColorAt(char** board, int row, int col)
{    
    if (isEmpty(board, row, col)) return -1; 

    if (board[row][col] >= 'A' && board[row][col] <= 'Z') return COLOR_BLACK;
    
    return COLOR_WHITE;
}


captureArray capt ={
    .countBlack =0,
    .countWhite =0,
    .captBlack = {'\0'},
    .captBlack = {'\0'}
};
