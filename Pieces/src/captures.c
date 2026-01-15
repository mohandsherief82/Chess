#include "captures.h"

#include <stdlib.h>
#include <ctype.h>

Captured initializeCapture(PieceColor color)
{
    Captured captures;
    captures.newCapture = false;

    for (int i = 0; i < MAXCAPTNUM; i++) captures.capturedSymbols[i] = ' ';

    captures.captureCount = 0;
    captures.color = color;

    return captures;
}


bool isEmpty(char** board, int r, int c)
{
    return board[r][c] == WHITE_SQUARE || board[r][c] == BLACK_SQUARE || board[r][c] == EMPTY_SQUARE;
}


PieceColor pieceColorAt(char** board, int row, int col)
{    
    if (isEmpty(board, row, col)) return -1; 

    if (isupper(board[row][col])) return COLOR_BLACK;
    
    return COLOR_WHITE;
}

void capturePiece(Player* player, Captured* captureData)
{
    if (!captureData->newCapture) return;

    
    char sym = tolower(captureData->capturedPiece.symbol);
    bool found = false;

    if (sym == 'r')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player->rooks[i].colPosition 
                && captureData->capturedPiece.rowPosition == player->rooks[i].rowPosition 
                && player->rooks[i].isActive)
            {
                player->rooks[i].isActive = false;
                found = true;
                break;
            }
        }
    }
    else if (sym == 'n')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player->knights[i].colPosition 
                && captureData->capturedPiece.rowPosition == player->knights[i].rowPosition 
                && player->knights[i].isActive)
            {
                player->knights[i].isActive = false;
                found = true;
                break;
            }
        }
    }
    else if (sym == 'b')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player->bishops[i].colPosition 
                && captureData->capturedPiece.rowPosition == player->bishops[i].rowPosition 
                && player->bishops[i].isActive)
            {
                player->bishops[i].isActive = false;
                found = true;
                break;
            }
        }
    }
    else if (sym == 'q')
    {
        if (captureData->capturedPiece.colPosition == player->queen->colPosition 
            && captureData->capturedPiece.rowPosition == player->queen->rowPosition 
            && player->queen->isActive)
        {
            player->queen->isActive = false;
            found = true;
        }
    }

    // Handles Pawns and Promoted Pawns
    if (!found)
    {
        for (int i = 0; i < NUM_PAWNS; i++)
        {
            if (captureData->capturedPiece.colPosition == player->pawns[i].colPosition 
                && captureData->capturedPiece.rowPosition == player->pawns[i].rowPosition 
                && player->pawns[i].isActive)
            {
                player->pawns[i].isActive = false;
                found = true;
                break;
            }
        }
    }

    if (captureData->color == COLOR_BLACK) sym = tolower(captureData->capturedPiece.symbol);
    else if (captureData->color == COLOR_WHITE) sym = toupper(captureData->capturedPiece.symbol);

    captureData->capturedSymbols[captureData->captureCount - 1] = sym;
    captureData->newCapture = false;
}
