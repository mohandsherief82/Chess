#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/pawnMoves.h"
#include "../include/captures.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

bool moveBishop(char** board, Player* player, Move move, Captured* playerCaptures, bool legalCheck) 
{
    int dispX, dispY, r, c, rowStep, colStep;
    Bishop* bishop = (Bishop*)checkPromotedPawn(player, move);

    for (int i = 0; i < NUM_PIECES; i++) 
    {
        if (player->bishops[i].colPosition == move.colPrev 
            && player->bishops[i].rowPosition == move.rowPrev && player->bishops[i].isActive) {
            bishop = &player->bishops[i];
            break;
        }
    }

    if (bishop == NULL) 
    {
        if (!legalCheck) printf("No Bishop At This Position, Try Again!!!\n");
        return false;
    }
    if (bishop->isPinned) 
    {
        if (!legalCheck) printf("This bishop is pinned, Try Again!!!\n");
        return false;
    }

    dispX = move.rowNext - bishop->rowPosition;
    dispY = move.colNext - bishop->colPosition;

    if(abs(dispX) != abs(dispY)) 
    {
        if (!legalCheck) printf("Invalid Bishop Move, Try Again!!!\n");
        return false;
    }

    rowStep = (dispX > 0) ? 1 : -1;
    colStep = (dispY > 0) ? 1 : -1;
    r = bishop->rowPosition + rowStep;
    c = bishop->colPosition + colStep;

    while (r != move.rowNext) 
    {
        if(!isEmpty(board, r, c)) 
        {
            if (!legalCheck) printf("Invalid Bishop Move, Try Again!!!\n");
            return false;
        }
        r += rowStep; c += colStep;
    }

    if(!isEmpty(board, move.rowNext, move.colNext)) 
    {
        if(pieceColorAt(board, move.rowNext, move.colNext) == bishop->color) 
        {
            if (!legalCheck) printf("Can't Capture Friendly piece, Try Again!!!\n");
            return false;
        }
        if (!legalCheck) 
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;
        }
    }

    board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
    board[move.rowNext][move.colNext] = move.symbol;
    bishop->rowPosition = move.rowNext;
    bishop->colPosition = move.colNext;

    return true;
}
