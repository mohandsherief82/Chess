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


bool movePawn(char** board, Player* player, Move move, Captured* playerCaptures)
{
    Pawn* pawn = NULL;
    int moveDirection; // 1 for Black (0->7), -1 for White (7->0)

    // Locate the correct Pawn object
    for (int i = 0; i < NUM_PAWNS; i++)
    {
        if (move.colPrev == player->pawns[i].colPosition && move.rowPrev == player->pawns[i].rowPosition) 
        {
            pawn = &player->pawns[i];
            break;
        }
    }    
    
    if (pawn == NULL || pawn->isActive == false)
    {
        printf("No Pawn At This Position, Try Again!!!\n");
        return false;
    }
    else if (pawn->isPinned)
    {
        printf("This Pawn is pinned, Try Again!!!\n");
    }

    // Determine direction
    moveDirection = (pawn->color == COLOR_WHITE) ? -1 : 1; 

    int rowDiff = move.rowNext - move.rowPrev;
    int colDiff = move.colNext - move.colPrev;

    // Single Square Advance
    if (colDiff == 0 && rowDiff == moveDirection)
    {
        if (isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;            
            pawn->rowPosition = move.rowNext;
            if (pawn->firstMove) pawn->firstMove = false;
            promotePawn(pawn);

            return true;
        }
    }

    // Two Square Advance
    if (colDiff == 0 && rowDiff == (moveDirection * 2) && pawn->firstMove)
    {
        int midRow = move.rowPrev + moveDirection;

        if (isEmpty(board, midRow, move.colNext) && isEmpty(board, move.rowNext, move.colNext)) 
        {
            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            pawn->rowPosition = move.rowNext;
            pawn->firstMove = false;
            
            // En Passant flag setup would happen here
            return true;
        }
    }

    // Pawn Captures
    if (abs(colDiff) == 1 && rowDiff == moveDirection)
    {
        if (!isEmpty(board, move.rowNext, move.colNext)) 
        { 
            if (pieceColorAt(board, move.rowNext, move.colNext) == pawn->color)
            {
                printf("Can't Capture Friendly Piece, Try Again!!!\n");
                return false;
            }

            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->capturedPiece.isActive = false;
            
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            pawn->rowPosition = move.rowNext;
            pawn->colPosition = move.colNext;
            
            if (!pawn->firstMove) promotePawn(pawn);
            else pawn->firstMove = false;

            return true;
        }
        // For En Passant Capturing
        if (tolower(board[move.rowPrev][move.colNext]) == 'p' && (move.rowPrev == 4 || move.rowPrev == 3)) // && enPassantFlag)
        {
            playerCaptures->capturedPiece.color = (isupper(board[move.rowNext][move.colNext])) ? COLOR_BLACK: COLOR_WHITE;
            
            playerCaptures->capturedPiece.colPosition = move.colNext;
            playerCaptures->capturedPiece.rowPosition = move.rowNext;
            playerCaptures->capturedPiece.symbol = board[move.rowNext][move.colNext];
            playerCaptures->capturedPiece.isActive = false;
            
            playerCaptures->captureCount++;
            playerCaptures->newCapture = true;

            board[move.rowPrev][move.colPrev] = EMPTY_SQUARE;
            pawn->rowPosition = move.rowNext;
            pawn->colPosition = move.colNext;
            
            if (pawn->firstMove) pawn->firstMove = false;
            return true;
        }
    }
    
    printf("Invalid Pawn Move, Try Again!!!\n");
    return false;
}
