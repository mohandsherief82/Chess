#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
static int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};


bool performCastling(char** board, Player player, Move move)
{
    if (abs(move.colNext - move.colPrev) == 2 && player.king->firstMove 
            && move.rowNext == move.rowPrev && move.colPrev == 4)
    {
        if (move.colNext == 2 && player.rocks[0].firstMove == true)
        {
            // CHECK: Must also verify squares B, C, D are empty, and C, D, E are not attacked.
            if (isEmpty(board, move.rowPrev, 1) && isEmpty(board, move.rowPrev, 2) && isEmpty(board, move.rowPrev, 3))
            {
                board[move.rowPrev][0] = EMPTY_SQUARE;
                player.rocks[0].colPosition = 3;
                player.rocks[0].firstMove = false;
            }
        }
        else if (move.colNext == 6 && player.rocks[1].firstMove == true)
        {
            // CHECK: Must also verify squares F, G are empty, and E, F, G are not attacked.
            if (isEmpty(board, move.rowPrev, 5) && isEmpty(board, move.rowPrev, 6))
            {    
                board[move.rowPrev][7] = EMPTY_SQUARE;
                player.rocks[1].colPosition = 5;
                player.rocks[1].firstMove = false;
            }
        }

        board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
        player.king->rowPosition = move.rowNext;
        player.king->colPosition = move.colNext;
        player.king->firstMove = false;

        return true;
    }

    printf("Invalid Castling, Try Again!!!\n");
    return false;
}


bool moveKing(char** board, Player player, Move move, Captured* playerCaptures)
{
    King *king = player.king; 

    if (king->rowPosition != move.rowPrev && king->colPosition != move.colPrev)
    {
        printf("No King at This Position, Try Again!!!\n");
        return false;
    }
    
    if (performCastling(board, player, move)) return true;

    for (int i = 0; i < 8; i++)
    {
        if (king->rowPosition + dy[i] == move.rowNext && king->colPosition + dx[i] == move.colNext)
        {
            if (!isEmpty(board, move.rowNext, move.colNext))
            {
                if(pieceColorAt(board, move.rowNext, move.colNext) == player.color)
                {
                    printf("Can't Capture Friendly piece, Try Again!!!\n");
                    return false;
                }

                // Capture Logic
                playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowNext;
                playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
                playerCaptures->capturedPiece.isActive = false;
                
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;
            }

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            king->rowPosition = move.rowNext;
            king->colPosition = move.colNext;
            king->firstMove = false;

            return true;
        }
    }

    return false;
}