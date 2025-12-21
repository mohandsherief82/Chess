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
        if (move.colPrev == player->pawns[i].colPosition && move.rowPrev == player->pawns[i].rowPosition
            && player->pawns[i].promoted == true && tolower(player->pawns[i].symbol) == move.symbol) 
        {
            promotedPawn = (Piece*)&player->pawns[i];
            break;
        }
    }

    return promotedPawn;
}

void promotePawn(Pawn* pawn, Move* move, bool load)
{
    if ((pawn->rowPosition == 0 && pawn->color == COLOR_WHITE) || 
        (pawn->rowPosition == 7 && pawn->color == COLOR_BLACK))
    {
        char newSymbol;
        pawn->promoted = true;

        if (isValidPiece(tolower(move->promotedPawn)) && load) newSymbol = tolower(move->promotedPawn);
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

        if (!load) move->promotedPawn = newSymbol;
        pawn->symbol = (pawn->color == COLOR_BLACK) ? toupper(newSymbol) : newSymbol;
    }
}



bool movePawn(char** board, Player* player, Move* move, Captured* playerCaptures
    , int *plyEnPassantCol, int *oppEnPassantCol, bool legalCheck, bool load)
{
    Pawn* pawn = NULL;
    int moveDirection;

    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move->colPrev == player->pawns[i].colPosition && move->rowPrev == player->pawns[i].rowPosition 
                    && player->pawns[i].isActive && !player->pawns[i].promoted) 
        {
            pawn = &player->pawns[i];
            break;
        }
    }    
    
    if (pawn == NULL || !pawn->isActive)
    {
        if (!legalCheck) printf("No Pawn At This Position, Try Again!!!\n");
        return false;
    }

    moveDirection = (pawn->color == COLOR_WHITE) ? -1 : 1; 
    int rowDiff = move->rowNext - move->rowPrev;
    int colDiff = move->colNext - move->colPrev;

    // One Square Advance
    if (colDiff == 0 && rowDiff == moveDirection)
    {
        if (isEmpty(board, move->rowNext, move->colNext)) 
        {
            board[move->rowPrev][move->colPrev] = EMPTY_SQUARE;            
            pawn->rowPosition = move->rowNext;
            board[move->rowNext][move->colNext] = pawn->symbol;

            // if (pawn->isPinned)
            // {
            //     if (!legalCheck) printf("This Pawn is pinned, Try Again!!!\n");
            //     return false;
            // }

            if (!legalCheck)
            {
                if (pawn->firstMove) pawn->firstMove = false;
                promotePawn(pawn, move, load);
                board[move->rowNext][move->colNext] = pawn->symbol;
            }
            
            return true;
        }
    }

    // Two Square Advance
    if (colDiff == 0 && rowDiff == (moveDirection * 2) && pawn->firstMove)
    {
        int midRow = move->rowPrev + moveDirection;
        
        if (isEmpty(board, midRow, move->colNext) && isEmpty(board, move->rowNext, move->colNext)) 
        {
            board[move->rowPrev][move->colPrev] = EMPTY_SQUARE;
            pawn->rowPosition = move->rowNext;
            board[move->rowNext][move->colNext] = pawn->symbol;

            if (!legalCheck)
            {
                pawn->firstMove = false;
                *plyEnPassantCol = move->colNext;
            }

            // if (pawn->isPinned)
            // {
            //     if (!legalCheck) printf("This Pawn is pinned, Try Again!!!\n");
            //     return false;
            // }
            
            return true;
        }
    }

    if (abs(colDiff) == 1 && rowDiff == moveDirection)
    {
        if (!isEmpty(board, move->rowNext, move->colNext)) 
        { 
            if (pieceColorAt(board, move->rowNext, move->colNext) == pawn->color)
            {
                if (!legalCheck) printf("Can't Capture Friendly Piece, Try Again!!!\n");
                return false;
            }

            char capturedSymbol = board[move->rowNext][move->colNext];
            board[move->rowPrev][move->colPrev] = EMPTY_SQUARE;
            
            pawn->rowPosition = move->rowNext;
            pawn->colPosition = move->colNext;
            board[move->rowNext][move->colNext] = pawn->symbol;

            if (!legalCheck)
            {
                playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK : COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move->colNext;
                playerCaptures->capturedPiece.rowPosition = move->rowNext;
                playerCaptures->capturedPiece.symbol = capturedSymbol;
                playerCaptures->capturedPiece.isActive = false;
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;

                if (pawn->firstMove) pawn->firstMove = false;
                promotePawn(pawn, move, load);
                board[move->rowNext][move->colNext] = pawn->symbol;
            }
            
            return true;
        }

        

        if (tolower(board[move->rowPrev][move->colNext]) == 'p' && (move->rowPrev == 4 || move->rowPrev == 3) && *oppEnPassantCol == move->colNext)
        {
            char capturedSymbol = board[move->rowPrev][move->colNext];

            board[move->rowPrev][move->colPrev] = EMPTY_SQUARE;
            board[move->rowPrev][move->colNext] = EMPTY_SQUARE;
            
            pawn->rowPosition = move->rowNext;
            pawn->colPosition = move->colNext;
            board[move->rowNext][move->colNext] = pawn->symbol;

            if (!legalCheck)
            {
                playerCaptures->capturedPiece.color = (isupper(capturedSymbol)) ? COLOR_BLACK : COLOR_WHITE;
                playerCaptures->capturedPiece.colPosition = move->colNext;
                playerCaptures->capturedPiece.rowPosition = move->rowPrev;
                playerCaptures->capturedPiece.symbol = capturedSymbol;
                
                playerCaptures->capturedPiece.isActive = false;
                playerCaptures->captureCount++;
                playerCaptures->newCapture = true;

                if (pawn->firstMove) pawn->firstMove = false;
            }

            return true;
        }
    }
    
    if (!legalCheck) printf("Invalid Pawn Move, Try Again!!!\n");
    return false;
}
