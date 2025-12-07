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
#include <stdlib.h>
#include <string.h>


char** copyBoard(char** board)
{
    char** copyBoard = malloc(sizeof(char*) * BOARD_SIZE);
    for(int i =0; i < BOARD_SIZE; i++)
    {
        copyBoard[i] = malloc(sizeof(char) * BOARD_SIZE);
        memcpy(copyBoard[i], board[i], sizeof(board[i]));
    }
    return copyBoard;
}

Player copyPlayer(Player* player)
{
    Player copyPlayer;

    copyPlayer.bishops = (Bishop*)malloc(NUM_PIECES * sizeof(Bishop));
    memcpy(copyPlayer.bishops, player -> bishops, sizeof(player -> bishops));

    copyPlayer.knights = (Knight*)malloc(NUM_PIECES * sizeof(Knight));
    memcpy(copyPlayer.knights, player -> knights, sizeof(player -> knights));

    copyPlayer.rocks = (Rock*)malloc(NUM_PIECES * sizeof(Rock));
    memcpy(copyPlayer.rocks, player -> rocks, sizeof(player -> rocks));

    copyPlayer.pawns = (Pawn*)malloc(NUM_PAWNS * sizeof(Pawn));
    memcpy(copyPlayer.pawns, player -> pawns, sizeof(player -> pawns));

    copyPlayer.queen = (Queen*)malloc(sizeof(Queen));
    memcpy(copyPlayer.queen, player -> queen, sizeof(player -> queen));

    copyPlayer.king = (King*)malloc(sizeof(King));
    memcpy(copyPlayer.king, player -> king, sizeof(player -> king));

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
    free(copyBoard);
}

bool legalMove(char** board, Player* player)
{
    Move testMove;
    testMove.isValid = true;

    char** cpyBoard = copyBoard(board);
    Player cpyPlayer = copyPlayer(player);
    
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
                    
                    if (testMove.symbol == 'p') moveSuccessful = movePawn(cpyBoard , &cpyPlayer, testMove, NULL);
                    else if (testMove.symbol == 'r') moveSuccessful = moveRock(cpyBoard , &cpyPlayer, testMove, NULL);
                    else if (testMove.symbol == 'n') moveSuccessful = moveKnight(cpyBoard , &cpyPlayer, testMove, NULL);
                    else if (testMove.symbol == 'b') moveSuccessful = moveBishop(cpyBoard , &cpyPlayer, testMove, NULL);
                    else if (testMove.symbol == 'q') moveSuccessful = moveQueen(cpyBoard , &cpyPlayer, testMove, NULL);
                    else if (testMove.symbol == 'k') moveSuccessful = moveKing(cpyBoard , &cpyPlayer, testMove, NULL);

                    if (moveSuccessful)
                    {
                        freeCopy(cpyPlayer,cpyBoard);
                        return true;
                    } 
                }
            }
        }
    }

    freeCopy(cpyPlayer,cpyBoard);
    return false;
}

bool checkStalemate(char** board, Player* player)
{
    if(!legalMove(board, player)) return true;
    if(isChecked(board, player)) return false;
    return false;
}