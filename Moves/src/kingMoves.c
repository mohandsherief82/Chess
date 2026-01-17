#include "player.h"
#include "board.h"
#include "captures.h"
#include "check.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

const int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
const int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};


bool performCastling(char** board, Player* player, Move move, bool legalCheck)
{    
    if (abs(move.colNext - move.colPrev) == 2 && player->king->firstMove 
            && move.rowNext == move.rowPrev && move.colPrev == 4)
    {
        if (isChecked(board, player, true)) return false;

        int row = move.rowPrev;

        if (move.colNext == 2 && player->rooks[0].firstMove == true)
        {
            if (isEmpty(board, row, 1) && isEmpty(board, row, 2) && isEmpty(board, row, 3))
            {
                board[row][4] = EMPTY_SQUARE;
                board[row][3] = player->king->symbol;
                player->king->colPosition = 3;

                if (isChecked(board, player, true)) 
                {
                    board[row][3] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return false;
                }
                
                board[row][3] = EMPTY_SQUARE;
                board[row][2] = player->king->symbol;
                player->king->colPosition = 2;
                
                if (isChecked(board, player, true)) 
                {
                    board[row][2] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return false;
                }
                
                if (legalCheck) 
                {
                    board[row][2] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return true; 
                }

                board[row][3] = board[row][0];
                board[row][0] = EMPTY_SQUARE;
               
                player->rooks[0].colPosition = 3;
                player->rooks[0].firstMove = false;
                player->king->firstMove = false;

                return true;
            }
        }

        else if (move.colNext == 6 && player->rooks[1].firstMove == true)
        {
            if (isEmpty(board, row, 5) && isEmpty(board, row, 6))
            {    
                board[row][4] = EMPTY_SQUARE;
                board[row][5] = player->king->symbol;
                player->king->colPosition = 5;

                if (isChecked(board, player, true))
                {
                    board[row][5] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return false;
                }
                
                board[row][5] = EMPTY_SQUARE;
                board[row][6] = player->king->symbol;
                player->king->colPosition = 6;

                if (isChecked(board, player, true)) 
                {
                    board[row][6] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return false;
                }

                if (legalCheck) 
                {
                    board[row][6] = EMPTY_SQUARE;
                    board[row][4] = player->king->symbol;
                    player->king->colPosition = 4;

                    return true;
                }

                board[row][5] = board[row][7];
                board[row][7] = EMPTY_SQUARE;
                
                player->rooks[1].colPosition = 5;
                player->rooks[1].firstMove = false;
                player->king->firstMove = false;

                return true;
            }
        }
    }

    return false;
}


MoveValidation moveKing(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    King *king = player->king; 

    if (king->rowPosition != move.rowPrev || king->colPosition != move.colPrev)
        return INVALID_MOVE;
    
    if (performCastling(board, player, move, legalCheck)) return VALID_MOVE;

    for (int i = 0; i < 8; i++)
    {
        if (king->rowPosition + dy[i] == move.rowNext && king->colPosition + dx[i] == move.colNext)
        {
            char capturedSymbol = EMPTY_SQUARE;

            if (!isEmpty(board, move.rowNext, move.colNext))
            {
                if (pieceColorAt(board, move.rowNext, move.colNext) == player->color)
                    return INVALID_MOVE;

                capturedSymbol = board[move.rowNext][move.colNext];
            }

            int oldRow = king->rowPosition;
            int oldCol = king->colPosition;

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = king->symbol;

            king->rowPosition = move.rowNext;
            king->colPosition = move.colNext;

            if (isChecked(board, player, true))
            {
                king->rowPosition = oldRow;
                king->colPosition = oldCol;
                board[move.rowPrev][move.colPrev] = king->symbol;
                board[move.rowNext][move.colNext] = capturedSymbol;

                return INVALID_MOVE;
            }

            if (legalCheck)
            {
                king->rowPosition = oldRow;
                king->colPosition = oldCol;

                board[move.rowPrev][move.colPrev] = king->symbol;
                board[move.rowNext][move.colNext] = capturedSymbol;
                
                return VALID_MOVE;
            }

            if (capturedSymbol != EMPTY_SQUARE)
            {
                playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK : COLOR_WHITE;
                
                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowNext;
                
                playerCaptures->capturedPiece.symbol = capturedSymbol;
                playerCaptures->capturedPiece.isActive = false;
                
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;
            }

            king->firstMove = false;
            return VALID_MOVE;
        }
    }

    return INVALID_MOVE;
}
