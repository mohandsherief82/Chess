#include "../include/knight.h"
#include "../../chessTypes.h"
#include "../../board/include/board.h"

#include <stdbool.h>
#include<stdlib.h>


Knight createKnight(PieceColor color, int row, int col)
{
    char sym = (color == COLOR_BLACK) ? 'N' : 'n';

    Knight n = {
        .symbol = sym,
        .color = color,
        .rowPosition = row,
        .colPosition = 1 + 5 * col
    };

    return n;
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

bool Valid_knightMove(const Knight* knight, int newrow, int newcol, char** board){

    if(newrow < 0 || newrow >= BOARD_SIZE || newcol < 0 || newcol >= BOARD_SIZE){
        return false;
    }

    int currentrow = knight->rowPosition;
    int currentcol = knight->colPosition;

    // knight move logic: (diffrow=2, diffcol=1) or (diffrow=1, diffcol=2)
    int diffrow = abs(newrow - currentrow);
    int diffcol = abs(newcol - currentcol);

    if ((diffrow == 0) && (diffcol == 0)){
        return false;
    }

    bool check_logic = (diffrow == 2 && diffcol == 1) || (diffrow == 1 && diffcol == 2);

    if(!check_logic){
        return false;
    }

    char piece_check = board[newrow][newcol];

    if(piece_check != EMPTY_SQUARE){
        
        PieceColor color_check = piececolorat(newrow, newcol, board);

        if(color_check == knight->color){
            return false;
        }
    }

    return true;
}
