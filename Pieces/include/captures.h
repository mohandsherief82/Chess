#ifndef CAPTURES_H
#define CAPTURES_H

#define MAXCAPTNUM 15

#include "chessTypes.h"
#include "player.h"

#include <stdbool.h>

typedef struct 
{
    PieceColor color;
    Piece capturedPiece;
    int captureCount;
    char capturedSymbols[MAXCAPTNUM];
    bool newCapture;
} Captured;

Captured initializeCapture(PieceColor color);
PieceColor pieceColorAt(char** board, int row, int col);
void capturePiece(Player* player, Captured* playerCaptures);
bool isEmpty(char** board, int r, int c);

#endif
