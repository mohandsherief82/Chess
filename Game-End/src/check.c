#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../../Moves/include/pawnMoves.h"
#include "../../Moves/include/captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static const int movesRBRow[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int movesRBCol[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

static const int knightRow[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
static const int knightCol[8] = {-1, 1, -2, 2, -2, 2, -1, 1};


Piece* findPiece(Player* player, Move move)
{
    Piece* piece = NULL;

    if (move.symbol == 'p')
    {
        for (int i = 0; i < NUM_PAWNS; i++)
        {
            if (move.colPrev == player->pawns[i].colPosition && player->pawns[i].isActive
                    && move.rowPrev == player->pawns[i].rowPosition) 
            {
                piece = (Piece*)&player->pawns[i];
                break;
            }
        } 
    }
    else if (move.symbol == 'r')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->rocks[i].colPosition == move.colPrev 
                && player->rocks[i].rowPosition == move.rowPrev && player->rocks[i].isActive)
            {
                piece = (Piece*)&player->rocks[i];
                break;
            }
        }
    }
    else if (move.symbol == 'n')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->knights[i].colPosition == move.colPrev && player->knights[i].isActive
                    && player->knights[i].rowPosition == move.rowPrev)
            {
                piece = (Piece*)&player->knights[i];
                break;
            }
        }
    }
    else if (move.symbol == 'b')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->bishops[i].colPosition == move.colPrev 
                && player->bishops[i].rowPosition == move.rowPrev && player->bishops[i].isActive)
            {
                piece = (Piece*)&player->bishops[i];
                break;
            }
        }
    }
    else if (move.symbol == 'q' && player->queen->colPosition == move.colPrev && player->queen->isActive
                && player->queen->rowPosition == move.rowPrev) piece = (Piece*)player->queen;

    if (piece == NULL) piece = checkPromotedPawn(player, move);

    return piece;
}


bool isChecked(char** board, Player* player)
{
    King* king = player->king;
    
    for (int i = 0; i < NUM_PAWNS; i++) player->pawns[i].isPinned = false;
    for (int i = 0; i < NUM_PIECES; i++) player->rocks[i].isPinned = false;
    for (int i = 0; i < NUM_PIECES; i++) player->knights[i].isPinned = false;
    for (int i = 0; i < NUM_PIECES; i++) player->bishops[i].isPinned = false;
    if (player->queen != NULL) player->queen->isPinned = false; 
    
    king->isChecked = false; 
    int r, c;

    for (int i = 0; i < 8; i++)
    {
        r = king->rowPosition + knightRow[i];
        c = king->colPosition + knightCol[i];

        if (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE)
        {
            if (!isEmpty(board, r, c))
            {
                if (tolower(board[r][c]) == 'n' && (islower(king->symbol) != islower(board[r][c])))
                {
                    king->isChecked = true;
                    printf("King is checked by a Knight at (%c%d)!!!\n", c + 'A', r);
                    return true;
                }
            }
        }
    }

    for (int d = 0; d < 8; d++)
    {
        Piece* pinCandidate = NULL;
        
        bool isDiagonal = (movesRBRow[d] != 0 && movesRBCol[d] != 0);
        bool isOrthogonal = (movesRBRow[d] == 0 || movesRBCol[d] == 0) && (!isDiagonal);
        for (int step = 1; step < BOARD_SIZE; step++)
        {
            r = king->rowPosition + movesRBRow[d] * step;
            c = king->colPosition + movesRBCol[d] * step;

            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) break;

            if (isEmpty(board, r, c)) continue;
                       
            if (pieceColorAt(board, r, c) != player->color)
            {           
                // Case A: Opposing piece found
                if (pinCandidate == NULL)
                {
                    char piece = tolower(board[r][c]);

                    // Direct check by orthogonal attacker
                    if (isOrthogonal && (piece == 'r' || piece == 'q'))
                    {
                        king->isChecked = true;
                        if (piece == 'r') printf("King is checked by a Rook at (%c%d)!!!\n", c + 'A', r);
                        else printf("King is checked by a Queen at (%c%d)!!!\n", c + 'A', r);
                        return true;
                    }
                    
                    // Direct check by diagonal attacker
                    if (isDiagonal && (piece == 'b' || piece == 'q'))
                    {
                        king->isChecked = true;
                        if (piece == 'b') printf("King is checked by a Bishop at (%c%d)!!!\n", c + 'A', r);
                        else printf("King is checked by a Queen at (%c%d)!!!\n", c + 'A', r);
                        return true;
                    }
                    
                    // Direct check by Pawn (only 1 step away, diagonal)
                    if (step == 1 && isDiagonal && piece == 'p')
                    {
                        // White King checked by Black Pawn moves up (row -1)
                        if (islower(king->symbol) && movesRBRow[d] == -1)
                        {
                            king->isChecked = true;
                            printf("White King is checked by a Black Pawn at (%c%d)!\n", c + 'A', r);
                            return true;
                        }
                        // Black King checked by White Pawn moves down (row +1)
                        if (isupper(king->symbol) && movesRBRow[d] == 1)
                        {
                            king->isChecked = true;
                            printf("Black King is checked by a White Pawn at (%c%d)!\n", c + 'A', r);
                            return true;
                        }
                    }
                    if (step == 1 && piece == 'k')
                    {
                        king->isChecked = true;
                        printf("King is checked by the opposing King at (%c%d)!!!\n", c + 'A', r);
                        return true;
                    }
                    
                    break;
                } 
                else 
                {
                    char pieceType = tolower(board[r][c]);
                    
                    bool isPinningAttacker = false;
                    
                    if (isOrthogonal && (pieceType == 'r' || pieceType == 'q')) isPinningAttacker = true;
                    if (isDiagonal && (pieceType == 'b' || pieceType == 'q')) isPinningAttacker = true;

                    if (isPinningAttacker) pinCandidate->isPinned = true;
                    
                    break;
                }
            }
            else
            {
                if (pinCandidate == NULL)
                {
                    Move move = {
                        .symbol = tolower(board[r][c]),
                        .colPrev = c,
                        .rowPrev = r
                    };                 
                    
                    pinCandidate = findPiece(player, move);
                    
                    if (pinCandidate == NULL) break;
                }
                else break;
            }
        }
    }

    return false;
}
