#include  "../include/staleMate.h"
#include  "../include/bishopMoves.h"
#include  "../include/kingMoves.h"
#include  "../include/knightMoves.h"
#include  "../include/pawnMoves.h"
#include  "../include/queenMoves.h"
#include  "../include/rockMoves.h"
#include  "../../Board/include/board.h"
#include  "../../chessTypes.h"
#include  "../../Pieces/include/player.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

bool legalMove(char** board, Player* player)
{
    Move testMove;
    testMove.isValid = true;
    
    for (int rPrev = 0; rPrev < BOARD_SIZE; rPrev++)
    {
        for (int cPrev = 0; cPrev < BOARD_SIZE; cPrev++)
        {
            if (isEmpty(board, rPrev, cPrev) || pieceColorAt(board, rPrev, cPrev) != player->color)
            {
                continue;
            }

            testMove.symbol = tolower(board[rPrev][cPrev]);
            testMove.rowPrev = rPrev;
            testMove.colPrev = cPrev;

            for (int rNext = 0; rNext < BOARD_SIZE; rNext++)
            {
                for (int cNext = 0; cNext < BOARD_SIZE; cNext++)
                {
                    testMove.rowNext = rNext;
                    testMove.colNext = cNext;

                    bool moveSuccessful = false;
                    
                    if (testMove.symbol == 'p') moveSuccessful = movePawn(board , player, testMove, NULL);
                    else if (testMove.symbol == 'r') moveSuccessful = moveRock(board , player, testMove, NULL);
                    else if (testMove.symbol == 'n') moveSuccessful = moveKnight(board , player, testMove, NULL);
                    else if (testMove.symbol == 'b') moveSuccessful = moveBishop(board , player, testMove, NULL);
                    else if (testMove.symbol == 'q') moveSuccessful = moveQueen(board , player, testMove, NULL);
                    else if (testMove.symbol == 'k') moveSuccessful = moveKing(board , player, testMove, NULL);

                    if (moveSuccessful) return true; 
                }
            }
        }
    }
    
    return false;
}

bool checkStalemate(char** board, Player* player)
{
    if(!legalMove(board, player)) return true;
    if(isChecked(board, player)) return false;
    return false;
}