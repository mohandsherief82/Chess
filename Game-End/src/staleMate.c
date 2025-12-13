#include  "../include/staleMate.h"
#include "../../Moves/include/pawnMoves.h"
#include "../../Moves/include/rockMoves.h"
#include "../../Moves/include/knightMoves.h"
#include "../../Moves/include/bishopMoves.h"
#include "../../Moves/include/queenMoves.h"
#include "../../Moves/include/kingMoves.h"
#include "../../Moves/include/captures.h"
#include  "../../Board/include/board.h"
#include  "../../chessTypes.h"
#include  "../../Pieces/include/player.h"

#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


char** copyBoard(char** board)
{
    char** copyBoard = malloc(sizeof(char*) * BOARD_SIZE);
    for(int i =0; i < BOARD_SIZE; i++)
    {
        copyBoard[i] = malloc(sizeof(char) * BOARD_SIZE);
        memcpy(copyBoard[i], board[i], BOARD_SIZE * sizeof(char)); 
    }
    return copyBoard;
}

Player copyPlayer(Player* player)
{
    Player copyPlayer;

    copyPlayer.bishops = (Bishop*)malloc(NUM_PIECES * sizeof(Bishop));
    memcpy(copyPlayer.bishops, player->bishops, NUM_PIECES * sizeof(Bishop)); 

    copyPlayer.knights = (Knight*)malloc(NUM_PIECES * sizeof(Knight));
    memcpy(copyPlayer.knights, player->knights, NUM_PIECES * sizeof(Knight));

    copyPlayer.rocks = (Rock*)malloc(NUM_PIECES * sizeof(Rock));
    memcpy(copyPlayer.rocks, player->rocks, NUM_PIECES * sizeof(Rock));

    copyPlayer.pawns = (Pawn*)malloc(NUM_PAWNS * sizeof(Pawn));
    memcpy(copyPlayer.pawns, player->pawns, NUM_PAWNS * sizeof(Pawn));

    copyPlayer.queen = (Queen*)malloc(sizeof(Queen));
    memcpy(copyPlayer.queen, player->queen, sizeof(Queen));

    copyPlayer.king = (King*)malloc(sizeof(King));
    memcpy(copyPlayer.king, player->king, sizeof(King));

    return copyPlayer;
}

void freeCopy(Player copyPlayer, char** copyBoard)
{
    free(copyPlayer.pawns);
    free(copyPlayer.rocks);
    free(copyPlayer.knights);
    free(copyPlayer.bishops);
    free(copyPlayer.queen);
    free(copyPlayer.king);
    
    for(int i = 0; i < BOARD_SIZE; i++) free(copyBoard[i]);

    free(copyBoard);
}


bool legalMove(char** board, Player* player)
{
    Move testMove;
    Captured tempCapture = {0};
    
    int plyEpCol = -1; 
    int oppEpCol = -1;
    
    for (int rPrev = 0; rPrev < BOARD_SIZE; rPrev++)
    {
        for (int cPrev = 0; cPrev < BOARD_SIZE; cPrev++)
        {
            if (isEmpty(board, rPrev, cPrev) || pieceColorAt(board, rPrev, cPrev) != player->color)
                    continue;

            testMove.symbol = board[rPrev][cPrev];
            testMove.rowPrev = rPrev;
            testMove.colPrev = cPrev;

            for (int rNext = 0; rNext < BOARD_SIZE; rNext++)
            {
                for (int cNext = 0; cNext < BOARD_SIZE; cNext++)
                {
                    char** cpyBoard = copyBoard(board);
                    Player cpyPlayer = copyPlayer(player);
                    
                    testMove.rowNext = rNext;
                    testMove.colNext = cNext;

                    bool moveSuccessful = false;
                    char pieceType = tolower(testMove.symbol);

                    if (pieceType == 'p') moveSuccessful = movePawn(cpyBoard , &cpyPlayer, testMove, &tempCapture, &plyEpCol, &oppEpCol);
                    else if (pieceType == 'r') moveSuccessful = moveRock(cpyBoard , &cpyPlayer, testMove, &tempCapture);
                    else if (pieceType == 'n') moveSuccessful = moveKnight(cpyBoard , &cpyPlayer, testMove, &tempCapture);
                    else if (pieceType == 'b') moveSuccessful = moveBishop(cpyBoard , &cpyPlayer, testMove, &tempCapture);
                    else if (pieceType == 'q') moveSuccessful = moveQueen(cpyBoard , &cpyPlayer, testMove, &tempCapture);
                    else if (pieceType == 'k') moveSuccessful = moveKing(cpyBoard , &cpyPlayer, testMove, &tempCapture);

                    if (moveSuccessful && !isChecked(cpyBoard, &cpyPlayer))
                    {
                        freeCopy(cpyPlayer, cpyBoard);
                        return true;
                    } 
                    
                    freeCopy(cpyPlayer, cpyBoard);

                    cpyPlayer.pawns = NULL;
                    cpyPlayer.rocks = NULL;
                    cpyPlayer.knights = NULL;
                    cpyPlayer.bishops = NULL;
                    cpyPlayer.queen = NULL;
                    cpyPlayer.king = NULL;
                    cpyBoard = NULL;
                    
                    plyEpCol = -1;
                    oppEpCol = -1;
                }
            }
        }
    }

    return false;
}


bool checkStalemate(char** board, Player* player)
{
    if(!legalMove(board, player) && !isChecked(board, player)) return true;
    else return false;
}