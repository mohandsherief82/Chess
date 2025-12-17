#include "../../Pieces/include/player.h"
#include "../../Board/include/board.h"
#include "../include/captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

Piece* checkPromotedPawn(Player* player, Move move)
{
    Piece* promotedPawn = NULL;

    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move.colPrev == player->pawns[i].colPosition && player->pawns[i].isActive
                && move.rowPrev == player->pawns[i].rowPosition && !player->pawns[i].isPinned 
                    && player->pawns[i].promoted == true && tolower(player->pawns[i].symbol) == move.symbol) 
        {
            promotedPawn = (Piece*)&player->pawns[i];
            break;
        }
    }

    return promotedPawn;
}


void promotePawn(Pawn* pawn)
{
    if ((pawn->rowPosition == 0 && pawn->color == COLOR_WHITE) || 
        (pawn->rowPosition == 7 && pawn->color == COLOR_BLACK))
    {
        char newSymbol;
        pawn->promoted = true;

        while (true)
        {
            char c;
            printf("What do you want to promote the pawn to (r, n, b, q): ");
            
            if (scanf(" %c", &newSymbol) != 1)
            {
                while ((c = getchar()) != '\n' && c != EOF); 
                printf("Invalid input. Try again.\n");
                continue;
            }

            while ((c = getchar()) != '\n' && c != EOF);

            newSymbol = tolower(newSymbol); 
            
            if (newSymbol == 'r' || newSymbol == 'n' || 
                    newSymbol == 'b' || newSymbol == 'q') break; 
            
            printf("Invalid Piece Symbol, Try Again! (Must be r, n, b, or q)\n");
        }
        
        pawn->symbol = (pawn->color == COLOR_BLACK) ? toupper(newSymbol) : newSymbol;
    }
    
    return;
}


bool movePawn(char** board, Player* player, Move move, Captured* playerCaptures
                , int *plyEnPassantCol, int *oppEnPassantCol, bool legalCheck) 
{
    Pawn* pawn = NULL;
    for (int i = 0; i < NUM_PAWNS; i++) 
    {
        if (move.colPrev == player->pawns[i].colPosition 
                && player->pawns[i].isActive && move.rowPrev == player->pawns[i].rowPosition) 
        {
            pawn = &player->pawns[i];
            break;
        }
    }    
    
    if (pawn == NULL) return false;
    if (pawn->isPinned)
    {
        if (!legalCheck) printf("This pawn is pinned, Try Agin!!!\n");
        return false;
    }

    int moveDirection = (pawn->color == COLOR_WHITE) ? -1 : 1; 
    int rowDiff = move.rowNext - move.rowPrev;
    int colDiff = move.colNext - move.colPrev;

    bool isStep = (colDiff == 0 && rowDiff == moveDirection && isEmpty(board, move.rowNext, move.colNext));
    bool isJump = (colDiff == 0 && rowDiff == (moveDirection * 2) && pawn->firstMove && isEmpty(board, move.rowPrev + moveDirection, move.colNext) && isEmpty(board, move.rowNext, move.colNext));
    bool isCapture = (abs(colDiff) == 1 && rowDiff == moveDirection && !isEmpty(board, move.rowNext, move.colNext) && pieceColorAt(board, move.rowNext, move.colNext) != pawn->color);
    bool isEnPassant = (abs(colDiff) == 1 && rowDiff == moveDirection && isEmpty(board, move.rowNext, move.colNext) && move.colNext == *oppEnPassantCol);

    if (isStep || isJump || isCapture || isEnPassant) 
    {
        if (!legalCheck) 
        {
            if (isCapture) 
            {
                playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowNext;
                playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
                playerCaptures->capturedPiece.isActive = false;
                
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;
            }

            if (isEnPassant)
            {
                playerCaptures->capturedPiece.color = (isupper(board[move.rowPrev][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowPrev;
                playerCaptures->capturedPiece.symbol = board[move.rowPrev][move.colNext];
                playerCaptures->capturedPiece.isActive = false;
                
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;
            }

            if (isJump) *plyEnPassantCol = move.colNext;
            if (isEnPassant) board[move.rowPrev][move.colNext] = EMPTY_SQUARE;
            
            pawn->firstMove = false;
        }

        board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
        board[move.rowNext][move.colNext] = pawn->symbol;
        pawn->rowPosition = move.rowNext;
        pawn->colPosition = move.colNext;

        if (!legalCheck && (pawn->rowPosition == 0 || pawn->rowPosition == 7)) 
        {
            promotePawn(pawn);
            board[move.rowNext][move.colNext] = pawn->symbol;
        }
        return true;
    }

    return false;
}
