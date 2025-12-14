#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/captures.h"
#include "../../Game-End/include/check.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static const int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
static const int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};


bool performCastling(char** board, Player* player, Move move)
{
    if (isChecked(board, player)) 
    {
        printf("Invalid Castling: King is currently in check.\n");
        return false;
    }
    
    if (abs(move.colNext - move.colPrev) == 2 && player->king->firstMove 
            && move.rowNext == move.rowPrev && move.colPrev == 4)
    {
        if (move.colNext == 2 && player->rocks[0].firstMove == true)
        {
            if (isEmpty(board, move.rowPrev, 1) && isEmpty(board, move.rowPrev, 2) && isEmpty(board, move.rowPrev, 3))
            {
                board[move.rowPrev][3] = player->king->symbol;
                board[move.rowPrev][4] = EMPTY_SQUARE; 
                player->king->colPosition = 3;
                if (isChecked(board, player)) 
                {
                    printf("Invalid Castling: King passes through an attacked square (D-file)!\n");

                    board[move.rowPrev][4] = board[move.rowPrev][3];
                    board[move.rowPrev][3] = EMPTY_SQUARE;
                    player->king->colPosition = 4;
                    return false;
                }
                
                board[move.rowPrev][2] = player->king->symbol;
                board[move.rowPrev][3] = EMPTY_SQUARE;
                player->king->colPosition = 2;
                if (isChecked(board, player)) 
                {
                    printf("Invalid Castling: King lands on an attacked square (C-file)!\n");

                    board[move.rowPrev][4] = board[move.rowPrev][2];
                    board[move.rowPrev][2] = EMPTY_SQUARE;
                    player->king->colPosition = 4;
                    return false;
                }
                
                board[move.rowPrev][3] = board[move.rowPrev][0];
                board[move.rowPrev][0] = EMPTY_SQUARE;
                
                player->rocks[0].colPosition = 3;
                player->rocks[0].firstMove = false;
                player->king->firstMove = false;

                return true;
            }
        }

        else if (move.colNext == 6 && player->rocks[1].firstMove == true)
        {
            if (isEmpty(board, move.rowPrev, 5) && isEmpty(board, move.rowPrev, 6))
            {    
                board[move.rowPrev][5] = player->king->symbol;
                board[move.rowPrev][4] = EMPTY_SQUARE; 
                player->king->colPosition = 5;
                
                if (isChecked(board, player))
                {
                    printf("Invalid Castling: King passes through an attacked square (F-file)!\n");
                    
                    board[move.rowPrev][4] = board[move.rowPrev][5];
                    board[move.rowPrev][5] = EMPTY_SQUARE;
                    player->king->colPosition = 4;
                    return false;
                }
                
                board[move.rowPrev][6] = player->king->symbol;
                board[move.rowPrev][5] = EMPTY_SQUARE;
                player->king->colPosition = 6;
                if (isChecked(board, player)) 
                {
                    printf("Invalid Castling: King lands on an attacked square (G-file)!\n");
                    
                    board[move.rowPrev][4] = board[move.rowPrev][6];
                    board[move.rowPrev][6] = EMPTY_SQUARE;
                    player->king->colPosition = 4;
                    return false;
                }

                board[move.rowPrev][5] = board[move.rowPrev][7];
                board[move.rowPrev][7] = EMPTY_SQUARE;
                player->rocks[1].colPosition = 5;
                player->rocks[1].firstMove = false;
                player->king->firstMove = false;
                
                return true;
            }
        }
    }

    return false;
}


bool moveKing(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck)
{
    King *king = player->king; 

    if (king->rowPosition != move.rowPrev || king->colPosition != move.colPrev)
    {
        if (!legalCheck) printf("No King at This Position, Try Again!!!\n");
        return false;
    }
    
    if (performCastling(board, player, move) && !legalCheck) return true;

    for (int i = 0; i < 8; i++)
    {
        if (king->rowPosition + dy[i] == move.rowNext && king->colPosition + dx[i] == move.colNext)
        {
            char capturedSymbol = EMPTY_SQUARE;

            if (!isEmpty(board, move.rowNext, move.colNext))
            {
                if(pieceColorAt(board, move.rowNext, move.colNext) == player->color)
                {
                    if (!legalCheck) printf("Can't Capture Friendly piece, Try Again!!!\n");
                    return false;
                }

                // Store the captured piece symbol for potential rollback
                capturedSymbol = board[move.rowNext][move.colNext];
            }

            
            int oldRow = king->rowPosition;
            int oldCol = king->colPosition;
            
            if (!legalCheck) 
            {
                board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
                board[move.rowNext][move.colNext] = king->symbol;
                king->rowPosition = move.rowNext;
                king->colPosition = move.colNext;
            }

            if (isChecked(board, player))
            {
                if (!legalCheck) 
                {
                    printf("Invalid King Move: Move leaves King in check, Try Again!!!\n");

                    king->rowPosition = oldRow;
                    king->colPosition = oldCol;
                    board[move.rowPrev][move.colPrev] = king->symbol;
                    board[move.rowNext][move.colNext] = capturedSymbol;
                }

                return false;
            }

            if (capturedSymbol != EMPTY_SQUARE && !legalCheck)
            {
                playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK: COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowNext;
                playerCaptures->capturedPiece.symbol = capturedSymbol;
                playerCaptures->capturedPiece.isActive = false;
                
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;
            }

            if (!legalCheck) king->firstMove = false;
            return true;
        }
    }

    if (!legalCheck) printf("Invalid King Move, Try Again!!!\n");
    return false;
}
