#ifndef CAPTURES_H
#define CAPTURES_H

#define MAXCAPTNUM 16

#include "../../chessTypes.h"
#include "../../Pieces/include/player.h"

#include <stdbool.h>

typedef struct 
{
    Piece capturedPiece;
    int captureCount;
    char capturedSymbol[MAXCAPTNUM];
    bool newCapture;
} Captured;

Captured initializeCapture(PieceColor color);
PieceColor pieceColorAt(char** board, int row, int col);
void capturePiece(Player player, Captured* playerCaptures);

#endif
