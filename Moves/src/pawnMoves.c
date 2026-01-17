#include "player.h"
#include "board.h"
#include "captures.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


Piece* checkPromotedPawn(Player* player, Move move)
{
    Piece* promotedPawn = NULL;

    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move.colPrev == player->pawns[i].colPosition && move.rowPrev == player->pawns[i].rowPosition
            && player->pawns[i].promoted == true) 
        {
            promotedPawn = (Piece*)&player->pawns[i];
            break;
        }
    }

    return promotedPawn;
}


void promotePawn(Pawn* pawn, Move move, bool load)
{
    if ((pawn->rowPosition == 0 && pawn->color == COLOR_WHITE) || 
        (pawn->rowPosition == 7 && pawn->color == COLOR_BLACK))
    {
        char newSymbol;
        pawn->promoted = true;

        if (isValidSymbol(tolower(move.promotedPawn)) && load) newSymbol = tolower(move.promotedPawn);
        else if (!load)
        {
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
                
                if (newSymbol == 'r' || newSymbol == 'n' || newSymbol == 'b' || newSymbol == 'q') break; 
                
                printf("Invalid Piece Symbol, Try Again! (Must be r, n, b, or q)\n");
            }
        }

        if (!load) move.promotedPawn = newSymbol;
        pawn->symbol = (pawn->color == COLOR_BLACK) ? toupper(newSymbol) : newSymbol;
    }
}


MoveValidation movePawn(char** board, Player* player, Move move, Captured* playerCaptures
    , int *plyEnPassantCol, int *oppEnPassantCol, bool legalCheck, bool load)
{
    Pawn* pawn = NULL;

    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move.colPrev == player->pawns[i].colPosition && move.rowPrev == player->pawns[i].rowPosition 
                    && player->pawns[i].isActive && !player->pawns[i].promoted) 
        {
            pawn = &player->pawns[i];
            break;
        }
    }    

    if (pawn == NULL) return INVALID_MOVE;
    
    int moveDirection = (pawn->color == COLOR_WHITE) ? -1 : 1; 
    int rowDiff = move.rowNext - move.rowPrev;
    int colDiff = move.colNext - move.colPrev;

    if (colDiff == 0 && rowDiff == moveDirection)
    {
        if (isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;            
            board[move.rowNext][move.colNext] = pawn->symbol;

            if (!legalCheck)
            {
                pawn->rowPosition = move.rowNext;
                pawn->colPosition = move.colNext;

                if (pawn->firstMove) pawn->firstMove = false;

                promotePawn(pawn, move, load);
                      
                board[move.rowNext][move.colNext] = pawn->symbol;
                
                if (pawn->promoted) return PROMOTION;
            }
            
            return VALID_MOVE;
        }
    }

    if (colDiff == 0 && rowDiff == (moveDirection * 2) && pawn->firstMove)
    {
        int midRow = move.rowPrev + moveDirection;
        
        if (isEmpty(board, midRow, move.colNext) && isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = pawn->symbol;

            if (!legalCheck)
            {
                pawn->rowPosition = move.rowNext;
                pawn->colPosition = move.colNext;

                pawn->firstMove = false;
                
                *plyEnPassantCol = move.colNext;
            }
            
            return VALID_MOVE;
        }
    }

    if (abs(colDiff) == 1 && rowDiff == moveDirection)
    {
        if (!isEmpty(board, move.rowNext, move.colNext)) 
        { 
            if (pieceColorAt(board, move.rowNext, move.colNext) == pawn->color) return INVALID_MOVE;

            char capturedSymbol = board[move.rowNext][move.colNext];
            
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            board[move.rowNext][move.colNext] = pawn->symbol;

            if (!legalCheck)
            {
                playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK : COLOR_WHITE;

                playerCaptures->capturedPiece.colPosition = move.colNext;
                playerCaptures->capturedPiece.rowPosition = move.rowNext;

                playerCaptures->capturedPiece.symbol = capturedSymbol;
                playerCaptures->capturedPiece.isActive = false;

                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;

                pawn->rowPosition = move.rowNext;
                pawn->colPosition = move.colNext;
                
                if (pawn->firstMove) pawn->firstMove = false;

                promotePawn(pawn, move, load);
                
                board[move.rowNext][move.colNext] = pawn->symbol;

                if (pawn->promoted) return PROMOTION;
            }
            
            return ENEMY_CAPTURE;
        }

        if (move.colNext == *oppEnPassantCol)
        {
            int epRank = (pawn->color == COLOR_WHITE) ? 3 : 4;

            if (move.rowPrev == epRank && isEmpty(board, move.rowNext, move.colNext))
            {
                char capturedSymbol = board[move.rowPrev][move.colNext];

                board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
                board[move.rowPrev][move.colNext] = EMPTY_SQUARE;
                board[move.rowNext][move.colNext] = pawn->symbol;

                if (!legalCheck)
                {
                    playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK : COLOR_WHITE;
                    playerCaptures->capturedPiece.colPosition = move.colNext;
                    playerCaptures->capturedPiece.rowPosition = move.rowPrev;
                    playerCaptures->capturedPiece.symbol = capturedSymbol;
                    playerCaptures->capturedPiece.isActive = false;
                    playerCaptures->captureCount++;
                    playerCaptures->newCapture = true;

                    pawn->rowPosition = move.rowNext;
                    pawn->colPosition = move.colNext;
                    if (pawn->firstMove) pawn->firstMove = false;
                }

                return ENEMY_CAPTURE;
            }
        }
    }
    
    return INVALID_MOVE;
}
