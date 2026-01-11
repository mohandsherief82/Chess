#include "player.h"
#include "board.h"
#include "pawnMoves.h"
#include "captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

static const int movesRBRow[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int movesRBCol[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

static const int knightRow[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
static const int knightCol[8] = {-1, 1, -2, 2, -2, 2, -1, 1};


Piece* findPiece(Player* player, Move move, char symbol)
{
    tolower(symbol);

    if (symbol == 'p')
    {
        for (int i = 0; i < NUM_PAWNS; i++)
        {
            if (move.colPrev == player->pawns[i].colPosition && move.rowPrev == player->pawns[i].rowPosition && player->pawns[i].isActive) 
                return (Piece*)&player->pawns[i];
        } 
    }
    else if (symbol == 'r')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->rooks[i].colPosition == move.colPrev && player->rooks[i].rowPosition == move.rowPrev && player->rooks[i].isActive)
                return (Piece*)&player->rooks[i];
        }
    }
    else if (symbol == 'n')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->knights[i].colPosition == move.colPrev && player->knights[i].rowPosition == move.rowPrev && player->knights[i].isActive)
                return (Piece*)&player->knights[i];
        }
    }
    else if (symbol == 'b')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (player->bishops[i].colPosition == move.colPrev && player->bishops[i].rowPosition == move.rowPrev && player->bishops[i].isActive)
                return (Piece*)&player->bishops[i];
        }
    }
    else if (symbol == 'q')
    {
        if (player->queen != NULL && player->queen->colPosition == move.colPrev && player->queen->rowPosition == move.rowPrev && player->queen->isActive)
            return (Piece*)player->queen;
    }

    return checkPromotedPawn(player, move);
}


MoveValidation isChecked(char** board, Player* player, bool legalCheck)
{
    King* king = player->king;
    
    if (!legalCheck)
    {
        for (int i = 0; i < NUM_PAWNS; i++) player->pawns[i].isPinned = false;
        for (int i = 0; i < NUM_PIECES; i++) player->rooks[i].isPinned = false;
        for (int i = 0; i < NUM_PIECES; i++) player->knights[i].isPinned = false;
        for (int i = 0; i < NUM_PIECES; i++) player->bishops[i].isPinned = false;
        
        if (player->queen != NULL) player->queen->isPinned = false; 

        king->isChecked = false; 
    }
    
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
                    if (!legalCheck) 
                    {
                        king->isChecked = true;
                        printf("King is checked by a Knight at (%c%d)!!!\n", c + 'A', 8 - r);
                    }

                    return KING_CHECKED;
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
            r = king->rowPosition + (movesRBRow[d] * step);
            c = king->colPosition + (movesRBCol[d] * step);

            if (r < 0 || r >= BOARD_SIZE || c < 0 || c >= BOARD_SIZE) break;
            if (isEmpty(board, r, c)) continue;
                       
            if (pieceColorAt(board, r, c) != player->color)
            {           
                char pType = tolower(board[r][c]);

                if (pinCandidate == NULL)
                {
                    if ((isOrthogonal && (pType == 'r' || pType == 'q')) || (isDiagonal && (pType == 'b' || pType == 'q')))
                    {
                        if (!legalCheck) 
                        {
                            king->isChecked = true;
                            printf("King is checked by a %s at (%c%d)!!!\n", (pType == 'q' ? "Queen" : (pType == 'r' ? "Rook" : "Bishop")), c + 'A', 8 - r);
                        }

                        return KING_CHECKED;
                    }
                    
                    if (step == 1 && isDiagonal && pType == 'p')
                    {
                        if ((islower(king->symbol) && movesRBRow[d] == -1) || (isupper(king->symbol) && movesRBRow[d] == 1))
                        {
                            if (!legalCheck) 
                            {
                                king->isChecked = true;
                                printf("King checked by Pawn at (%c%d)!\n", c + 'A', 8 - r);
                            }

                            return KING_CHECKED;
                        }
                    }

                    if (step == 1 && pType == 'k')
                    {
                        if (!legalCheck) king->isChecked = true;
                        return KING_CHECKED;
                    }

                    break;
                } 
                else
                {
                    if (!legalCheck)
                    {
                        if ((isOrthogonal && (pType == 'r' || pType == 'q')) || (isDiagonal && (pType == 'b' || pType == 'q')))
                            pinCandidate->isPinned = true;
                    }

                    break;
                }
            }
            else
            {
                if (pinCandidate == NULL)
                {
                    Move m = { .colPrev = c, .rowPrev = r };
                    pinCandidate = findPiece(player, m, board[r][c]);
                    if (pinCandidate == NULL) break; 
                }

                else break;
            }
        }
    }

    return VALID_MOVE;
}
