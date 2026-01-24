#include <stdbool.h>

#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#define BOARD_SIZE 8
#define BLACK_SQUARE '.'
#define WHITE_SQUARE '-'
#define EMPTY_SQUARE ' '


typedef enum
{
    INVALID_MOVE,
    VALID_MOVE,
    PROMOTION
} MoveValidation;


typedef enum
{
    PAWN, 
    ROOK, 
    QUEEN, 
    BISHOP, 
    KING, 
    KNIGHT
} PieceType;


typedef enum 
{
    COLOR_WHITE = 0,
    COLOR_BLACK
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
    int colPrev, rowPrev;
    int colNext, rowNext;
    char promotedPawn;
} Move;

#endif