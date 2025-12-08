#include <stdbool.h>

#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

typedef enum {
    COLOR_WHITE = 0,
    COLOR_BLACK,
} PieceColor;

// Placeholder struct for adding pieces
typedef struct
{
    char symbol;
    PieceColor color;
    int rowPosition; // Stores the row value 0 --> 7
    int colPosition; // Stores the col value 0 --> 7
    bool isActive;
    bool isPinned;
} Piece;

typedef struct
{
    char symbol;
    int colPrev;
    int colNext;
    int rowPrev;
    int rowNext;
} Move;

#endif