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
    FRIENDLY_CAPTURE,
    ENEMY_CAPTURE,
    PROMOTION,
    CASTLING,
    KING_CHECKED
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
    int colPrev;
    int rowPrev;
    int colNext;
    int rowNext;
    char promotedPawn;
} Move;

#endif