#include "../include/bishop.h"
#include "../../chessTypes.h"
#include "../../board/include/board.h"

#include <stdbool.h>
#include<stdlib.h>

Bishop createBishop(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'B' : 'b';

    Bishop b = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 2 + 3 * col
    };

    return b;
}

// to know the piece color at new position
PieceColor piececolorat(int row, int col, char** board){
    char symbol = board[row][col];
    
    if (symbol == EMPTY_SQUARE){
        return -1; 
    } 

    if (symbol >= 'A' && symbol <= 'Z'){
        return COLOR_BLACK;
    } 
    
    return COLOR_WHITE;
}

// check validation of move return true or false
bool Valid_BishopMove(const Bishop* bishop, int newrow, int newcol, char** board){

    if(newrow < 0 || newrow >= BOARD_SIZE || newcol < 0 || newcol >= BOARD_SIZE){
        return false;
    }

    int currentrow = bishop->rowPosition;
    int currentcol = bishop->colPosition;

    // bishop move logic: abs(difference) of rows = abs(difference) of columns
    int diffrow = newrow - currentrow;
    int diffcol = newcol - currentcol;

    if ((diffrow == 0) && (diffcol == 0)){
        return false;
    } 
    
    if(abs(diffrow) != abs(diffcol)){
        return false;
    }

    int rowstep = (diffrow > 0) ? 1 : -1;
    int colstep = (diffcol > 0) ? 1 : -1;

    int r = currentrow + rowstep;
    int c = currentcol + colstep;

    while (r != newrow)
    {
        if(board[r][c] != EMPTY_SQUARE){
            return false;
        }

        r += rowstep;
        c += colstep;
    }
    
    char piece_check = board[newrow][newcol];

    if(piece_check != EMPTY_SQUARE){
        PieceColor color_check = piececolorat(newrow, newcol, board);

        if(color_check == bishop->color){
            return false;
        }
    }

    return true;
}
