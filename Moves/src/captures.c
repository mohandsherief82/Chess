#include "../include/captures.h"
#include "../../Board/include/board.h"

#include <stdlib.h>
#include <ctype.h>

Captured initializeCapture(PieceColor color)
{
    Captured captures = {
        .captureCount = 0,
        .capturedSymbols = {},
        .capturedSymbols[MAXCAPTNUM - 1] = 'X',
        .newCapture = false
    };

    for (int i = 0; i < MAXCAPTNUM - 1; i++)
    {
        captures.capturedSymbols[i] = ' ';
    }

    return captures;
}

PieceColor pieceColorAt(char** board, int row, int col)
{    
    if (isEmpty(board, row, col)) return -1; 

    if (board[row][col] >= 'A' && board[row][col] <= 'Z') return COLOR_BLACK;
    
    return COLOR_WHITE;
}

void capturePiece(Player player, Captured* captureData)
{
    if (tolower(captureData->capturedPiece.symbol) == 'p')
    {
        for (int i = 0; i < NUM_PAWNS; i++)
        {
            if (captureData->capturedPiece.colPosition == player.pawns[i].colPosition 
                    && captureData->capturedPiece.rowPosition == player.pawns[i].rowPosition 
                        && captureData->newCapture == true)
            {
                player.pawns[i].isActive = false;
                captureData->newCapture = false;
                captureData->capturedSymbols[captureData->captureCount - 1] = player.pawns[i].symbol;
                return;
            }
        }
    }
    else if (tolower(captureData->capturedPiece.symbol) == 'r')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player.rocks[i].colPosition 
                    && captureData->capturedPiece.rowPosition == player.rocks[i].rowPosition 
                        && captureData->newCapture == true)
            {
                player.rocks[i].isActive = false;
                captureData->newCapture = false;
                break;
            }
        }
    }
    else if (tolower(captureData->capturedPiece.symbol) == 'n')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player.knights[i].colPosition 
                    && captureData->capturedPiece.rowPosition == player.knights[i].rowPosition 
                        && captureData->newCapture == true)
            {
                player.knights[i].isActive = false;
                captureData->newCapture = false;
                break;
            }
        }
    }
    else if (tolower(captureData->capturedPiece.symbol) == 'b')
    {
        for (int i = 0; i < NUM_PIECES; i++)
        {
            if (captureData->capturedPiece.colPosition == player.bishops[i].colPosition 
                    && captureData->capturedPiece.rowPosition == player.bishops[i].rowPosition 
                        && captureData->newCapture == true)
            {
                player.bishops[i].isActive = false;
                captureData->newCapture = false;
                break;
            }
        }
    }
    else if (tolower(captureData->capturedPiece.symbol) == 'q')
    {
        if (captureData->capturedPiece.colPosition == player.queen.colPosition 
                    && captureData->capturedPiece.rowPosition == player.queen.rowPosition 
                        && captureData->newCapture == true)
        {
            player.queen.isActive = false;
            captureData->newCapture = false;
        }
    }

    // Check if the capture is a promoted Pawn
    else
    {
        for (int i = 0; i < NUM_PAWNS; i++)
        {
            if (captureData->capturedPiece.colPosition == player.pawns[i].colPosition 
                    && captureData->capturedPiece.rowPosition == player.pawns[i].rowPosition 
                        && captureData->capturedPiece.symbol == player.pawns[i].symbol
                            && captureData->newCapture == true )
            {
                player.pawns[i].isActive = false;
                captureData->newCapture = false;
                captureData->capturedSymbols[captureData->captureCount] = player.pawns[i].symbol;
                return;
            }
        }
    }
}
