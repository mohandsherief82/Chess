#include "pawnMoves.h"
#include "rookMoves.h"
#include "knightMoves.h"
#include "bishopMoves.h"
#include "queenMoves.h"
#include "kingMoves.h"
#include "captures.h"
#include "board.h"
#include "chessTypes.h"
#include "player.h"
#include "check.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


char** copyBoard(char** board)
{
    char** cpyBoard = malloc(sizeof(char*) * BOARD_SIZE);
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cpyBoard[i] = malloc(sizeof(char) * BOARD_SIZE);
        memcpy(cpyBoard[i], board[i], BOARD_SIZE * sizeof(char)); 
    }
    return cpyBoard;
}


Player copyPlayer(Player* player)
{
    Player cpy;
    memcpy(&cpy, player, sizeof(Player));

    cpy.bishops = malloc(NUM_PIECES * sizeof(Bishop));
    memcpy(cpy.bishops, player->bishops, NUM_PIECES * sizeof(Bishop)); 

    cpy.knights = malloc(NUM_PIECES * sizeof(Knight));
    memcpy(cpy.knights, player->knights, NUM_PIECES * sizeof(Knight));

    cpy.rooks = malloc(NUM_PIECES * sizeof(Rook));
    memcpy(cpy.rooks, player->rooks, NUM_PIECES * sizeof(Rook));

    cpy.pawns = malloc(NUM_PAWNS * sizeof(Pawn));
    memcpy(cpy.pawns, player->pawns, NUM_PAWNS * sizeof(Pawn));

    cpy.queen = malloc(sizeof(Queen));
    memcpy(cpy.queen, player->queen, sizeof(Queen));

    cpy.king = malloc(sizeof(King));
    memcpy(cpy.king, player->king, sizeof(King));

    return cpy;
}


void freeCopy(Player cpyPlayer, char** cpyBoard)
{
    free(cpyPlayer.pawns);
    free(cpyPlayer.rooks);
    free(cpyPlayer.knights);
    free(cpyPlayer.bishops);
    free(cpyPlayer.queen);
    free(cpyPlayer.king);
    
    for (int i = 0; i < BOARD_SIZE; i++) free(cpyBoard[i]);
    free(cpyBoard);
}


bool legalMove(char** board, Player* player)
{
    bool legalCheck = true;
    Move testMove;
    Captured tempCapture = {0};
    int plyEpCol = -1, oppEpCol = -1;
    
    for (int rPrev = 0; rPrev < BOARD_SIZE; rPrev++)
    {
        for (int cPrev = 0; cPrev < BOARD_SIZE; cPrev++)
        {
            if (isEmpty(board, rPrev, cPrev) || pieceColorAt(board, rPrev, cPrev) != player->color) continue;

            testMove.rowPrev = rPrev;
            testMove.colPrev = cPrev;

            for (int rNext = 0; rNext < BOARD_SIZE; rNext++)
            {
                for (int cNext = 0; cNext < BOARD_SIZE; cNext++)
                {
                    if (rNext == rPrev && cNext == cPrev) continue;

                    char** cpyB = copyBoard(board);
                    Player cpyP = copyPlayer(player);
                    
                    testMove.rowNext = rNext;
                    testMove.colNext = cNext;

                    bool moveSuccessful = false;
                    char pieceType = board[rPrev][cPrev];
                    tolower(pieceType);
                    
                    if (pieceType == 'p') moveSuccessful = movePawn(cpyB, &cpyP, testMove, &tempCapture, &plyEpCol, &oppEpCol, legalCheck, false);
                    else if (pieceType == 'r') moveSuccessful = moveRook(cpyB, &cpyP, testMove, &tempCapture, legalCheck);
                    else if (pieceType == 'n') moveSuccessful = moveKnight(cpyB, &cpyP, testMove, &tempCapture, legalCheck);
                    else if (pieceType == 'b') moveSuccessful = moveBishop(cpyB, &cpyP, testMove, &tempCapture, legalCheck);
                    else if (pieceType == 'q') moveSuccessful = moveQueen(cpyB, &cpyP, testMove, &tempCapture, legalCheck);
                    else if (pieceType == 'k') moveSuccessful = moveKing(cpyB, &cpyP, testMove, &tempCapture, legalCheck);

                    if (moveSuccessful && !isChecked(cpyB, &cpyP, true))
                    {
                        freeCopy(cpyP, cpyB);
                        return true;
                    } 
                    
                    freeCopy(cpyP, cpyB);
                    plyEpCol = -1;
                    oppEpCol = -1;
                }
            }
        }
    }
    return false;
}


bool inSufficientMaterial(Player player1, Player player2)
{
    int p1Minor = 0, p2Minor = 0;

    if (player1.queen->isActive || player2.queen->isActive) return false;
    
    for (int i = 0; i < 2; i++) 
    {
        if (player1.rooks[i].isActive || player2.rooks[i].isActive) return false;
    }

    for (int i = 0; i < 8; i++) 
    {
        if (player1.pawns[i].isActive || player2.pawns[i].isActive) return false;
    }

    for (int i = 0; i < 2; i++) {
        if (player1.bishops[i].isActive) p1Minor++;
        if (player1.knights[i].isActive) p1Minor++;
        
        if (player2.bishops[i].isActive) p2Minor++;
        if (player2.knights[i].isActive) p2Minor++;
    }

    if (p1Minor == 0 && p2Minor == 0) return true;
    if ((p1Minor == 1 && p2Minor == 0) || (p1Minor == 0 && p2Minor == 1)) return true;
    if (p1Minor == 1 && p2Minor == 1) return true; 

    return false;
}


bool checkStalemate(char** board, Player* player)
{
    if(!legalMove(board, player) && !isChecked(board, player, true)) return true;
    else return false;
}
