#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static const int dx[8] = {-1,  0,  1, -1, 1, -1, 0, 1};
static const int dy[8] = {-1, -1, -1,  0, 0,  1, 1, 1};


// Rock, Bishop and Queen Moves
static const int movesRBRow[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int movesRBCol[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

// Knight Moves
static const int knightRow[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
static const int knightCol[8] = {-1, 1, -2, 2, -2, 2, -1, 1};


bool isChecked(char** board, Player* player)
{
    King* king = player->king;
    king->isChecked = false;

    // Knight Attacks (8 L-shaped moves)
    for (int i = 0; i < 8; i++)
    {
        int r = king->rowPosition + knightRow[i];
        int c = king->colPosition + knightCol[i];

        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
        {
            if (!isEmpty(board, r, c))
            {
                if (tolower(board[r][c]) == 'n' && (islower(king->symbol) != islower(board[r][c])))
                {
                    king->isChecked = true;
                    printf("King is checked by a Knight at (%d, %d)!\n", r, c);
                    return true;
                }
            }
        }
    }

    // Linear Attacks (Rooks, Queens, Bishops)
    for (int d = 0; d < 8; d++)
    {
        for (int step = 1; ; step++)
        {
            int r = king->rowPosition + movesRBRow[d] * step;
            int c = king->colPosition + movesRBCol[d] * step;

            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) break;

            if (!isEmpty(board, r, c))
            {
                bool isEnemy = (islower(king->symbol) != islower(board[r][c]));

                if (isEnemy)
                {
                    if (d % 2 != 0) // d=1, 3, 4, 6 (Ranks/Files)
                    {
                        if (tolower(board[r][c]) == 'r' || tolower(board[r][c]) == 'q')
                        {
                            king->isChecked = true;
                            printf("King is checked by a Rook/Queen at (%d, %d)!\n", r, c);
                            return true;
                        }
                    }
                    else // d=0, 2, 5, 7 (Diagonals)
                    {
                        if (tolower(board[r][c]) == 'b' || tolower(board[r][c]) == 'q')
                        {
                            king->isChecked = true;
                            printf("King is checked by a Bishop/Queen at (%d, %d)!\n", r, c);
                            return true;
                        }
                    }
                    
                    // Check for King-to-King proximity (only 1 step)
                    if (step == 1 && tolower(board[r][c]) == 'k')
                    {
                        king->isChecked = true;
                        printf("King is checked by the opposing King at (%d, %d)!\n", r, c);
                        return true;
                    }
                    if (step == 1 && tolower(board[r][c]) == 'p')
                    {
                        // White King attacked by black pawn (moves down, so r < kRow)
                        if (islower(king->symbol) && movesRBRow[d] == -1 && abs(movesRBCol[d]) == 1)
                        {
                            king->isChecked = true;
                            printf("White King is checked by a Black Pawn at (%d, %d)!\n", r, c);
                            return true;
                        }
                        // Black King attacked by white pawn (moves up, so r > kRow)
                        if (isupper(king->symbol) && movesRBRow[d] == 1 && abs(movesRBCol[d]) == 1)
                        {
                            king->isChecked = true;
                            printf("Black King is checked by a White Pawn at (%d, %d)!\n", r, c);
                            return true;
                        }
                    }

                    break;
                }
                else
                {
                    // Pin Piece
                    break;
                }
            }
        }
    }

    return false;
}




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
                // Must verify King does not pass through check (D=3) or end in check (C=2).
                board[move.rowPrev][3] = player->king->symbol;
                board[move.rowPrev][4] = EMPTY_SQUARE; 
                player->king->colPosition = 3;
                if (isChecked(board, player)) 
                {
                    printf("Invalid Castling: King passes through an attacked square, Try Again!!!\n");

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
                    printf("Invalid Castling: King lands on an attacked square, Try Again!!!\n");

                    board[move.rowPrev][3] = board[move.rowPrev][2];
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
                // Must verify King does not pass through check (F=5) or end in check (G=6).
                board[move.rowPrev][5] = player->king->symbol;
                board[move.rowPrev][4] = EMPTY_SQUARE; 
                player->king->colPosition = 5;
                
                if (isChecked(board, player)) {
                    printf("Invalid Castling: King passes through an attacked square, Try Again!!!\n");
                    
                    board[move.rowPrev][4] = board[move.rowPrev][5];
                    board[move.rowPrev][5] = EMPTY_SQUARE;
                    player->king->colPosition = 4;
                    return false;
                }
                
                board[move.rowPrev][6] = player->king->symbol;
                board[move.rowPrev][5] = EMPTY_SQUARE;
                player->king->colPosition = 6;
                if (isChecked(board, player)) {
                    printf("Invalid Castling: King lands on an attacked square, Try Again!!!\n");
                    
                    board[move.rowPrev][5] = board[move.rowPrev][6];
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

    printf("Invalid Castling, Try Again!!!\n");
    return false;
}


bool moveKing(char** board, Player* player, Move move, Captured* playerCaptures)
{
    King *king = player->king; 

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
                if(pieceColorAt(board, move.rowNext, move.colNext) == player->color)
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

            king->rowPosition = move.rowNext;
            king->colPosition = move.colNext;

            isChecked(board, player);
            if (king->isChecked == true)
            {
                king->rowPosition = move.rowPrev;
                king->colPosition = move.colPrev;

                printf("Invalid King Move, Try Again!!!\n");
                return false;
            }

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            king->firstMove = false;

            return true;
        }
    }

    return false;
}
