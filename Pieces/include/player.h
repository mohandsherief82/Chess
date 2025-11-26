#include "pawn.h"
#include "king.h"
#include "knight.h"
#include "queen.h"
#include "bishop.h"
#include "rock.h"
#include "../../chessTypes.h"

#ifndef PLAYER_H
#define PLAYER_H

#define NUM_PAWNS 8
#define NUM_PIECES 2

typedef struct
{
    const PieceColor color;
    Pawn *pawns;
    Rock *rocks;
    Knight *knights;
    Bishop *bishops;
    Queen queen;
    King king;
} Player;

// Placeholder struct for adding pieces
typedef struct
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
} Piece;

Player createPlayer(PieceColor color);

#endif
