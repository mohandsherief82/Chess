#include "../include/rock.h"
#include "../../chessTypes.h"
#include "../../board/include/board.h"

#include <stdbool.h>
#include<stdlib.h>

Rock createRock(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'R' : 'r';

    Rock r = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 0 + col * 7
    };

    return r;
}


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

bool Valid_rockMove(const Rock* rock, int newrow, int newcol, char** board){

    if(newrow < 0 || newrow >= BOARD_SIZE || newcol < 0 || newcol >= BOARD_SIZE){
        return false;
    }

    int currentrow = rock->rowPosition;
    int currentcol = rock->colPosition;

    // rock move logic: horizontally (diffrow=0, diffcol!=0) or vertically (diffrow!=0, diffcol=0)
    int diffrow = newrow - currentrow;
    int diffcol = newcol - currentcol;

    if ((diffrow == 0) && (diffcol == 0)){
        return false;
    } 

    if((diffrow != 0) && (diffcol !=0)){
        return false;
    }

    int rowstep =0,colstep=0;

    if(diffrow != 0){
        rowstep = (diffrow > 0) ? 1 : -1;
    }
    else{
        colstep = (diffcol > 0) ? 1 : -1;
    }

    int r = currentrow + rowstep;
    int c = currentcol + colstep;

    while ((r != newrow ) || (c != newcol ))
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

        if(color_check == rock->color){
            return false;
        }
    }

    return true;
}