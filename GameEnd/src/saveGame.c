#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "pawnMoves.h"
#include "rookMoves.h"
#include "knightMoves.h"
#include "bishopMoves.h"
#include "queenMoves.h"
#include "kingMoves.h"
#include "captures.h"
#include "board.h"
#include "player.h"
#include "chessTypes.h"
#include "check.h"

const char* loadPath = "./GameBin/Load/";
const char* redoPath = "./GameBin/Redo/";


bool loadPlayerTurn(char** board, Player* player, Move move, Captured* capture, int *whiteEnPassantCol, int *blackEnPassantCol) 
{
    bool state = false;
    char sym = tolower(move.symbol);

    if (sym == 'p') state = movePawn(board, player, move, capture, whiteEnPassantCol,
                                         blackEnPassantCol, false, true);
    else if (sym == 'r') state = moveRook(board, player, move, capture, false);
    else if (sym == 'n') state = moveKnight(board, player, move, capture, false);
    else if (sym == 'b') state = moveBishop(board, player, move, capture, false);
    else if (sym == 'q') state = moveQueen(board, player, move, capture, false);
    else if (sym == 'k') state = moveKing(board, player, move, capture, false);
    
    return state;
}


int loadGame(char*** boardPtr, Player* player1, Player* player2, Captured* ply1Captures, 
    Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol, const char *game_path)
{
    if (boardPtr == NULL) return -1;
    else if (*boardPtr == NULL) *boardPtr = initializeBoard();

    if (*boardPtr == NULL) exit(1);

    FILE* fptr = fopen(game_path, "rb");
    if (fptr == NULL) return -1;

    for (int i = 0; i < BOARD_SIZE; i++) 
        for (int j = 0; j < BOARD_SIZE; j++) (*boardPtr)[i][j] = EMPTY_SQUARE;
    
    resetPlayer(player1, COLOR_WHITE);
    resetPlayer(player2, COLOR_BLACK);

    *whiteEnPassantCol = -1;
    *blackEnPassantCol = -1;

    *ply1Captures = initializeCapture(COLOR_WHITE);
    *ply2Captures = initializeCapture(COLOR_BLACK);

    Move move[2];
    int readData;
    int totalMovesRead = 0;
    
    while ((readData = fread(move, sizeof(Move), 2, fptr)) > 0) 
    {
        loadPlayerTurn(*boardPtr, player1, move[0], ply1Captures, whiteEnPassantCol, blackEnPassantCol);
        if (ply1Captures->newCapture) 
        {
            capturePiece(player2, ply1Captures);
            ply1Captures->newCapture = false;
        }

        totalMovesRead++;
        updateBoard(*boardPtr, player1, player2);
        
        if (readData == 2) 
        {
            loadPlayerTurn(*boardPtr, player2, move[1], ply2Captures, blackEnPassantCol, whiteEnPassantCol);
            if (ply2Captures->newCapture) 
            {
                capturePiece(player1, ply2Captures);
                ply2Captures->newCapture = false;
            }

            totalMovesRead++;
            updateBoard(*boardPtr, player1, player2);
        }
    }    
    
    fclose(fptr);

    isChecked(*boardPtr, player1, true);
    isChecked(*boardPtr, player2, true);
    
    return (totalMovesRead % 2 == 0) ? 2 : 1;
}


bool undoLastMove(char*** boardPtr, Player* player1, Player* player2, Captured* ply1Captures,
     Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol
     , const char *game_path, const char *redo_path)
{
    char **board;

    if (boardPtr == NULL)
    {
        board = initializeBoard();
        *boardPtr = board;
    }
    else board = *boardPtr;

    FILE* fptr = fopen(game_path, "rb");
    if (!fptr) return false;

    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    int totalMoves = (int)(fileSize / sizeof(Move));
    
    if (totalMoves == 0) 
    {
        fclose(fptr);
        return false;
    }

    fseek(fptr, -(long)sizeof(Move), SEEK_END);
    Move lastMove;
    fread(&lastMove, sizeof(Move), 1, fptr);
    
    FILE* redoFile = fopen(redo_path, "ab");
    if (redoFile) 
    {
        fwrite(&lastMove, sizeof(Move), 1, redoFile);
        fclose(redoFile);
    }

    Move* backMove = NULL;
    if (totalMoves > 1) 
    {
        backMove = malloc(sizeof(Move) * (totalMoves - 1));
        if (backMove) 
        {
            fseek(fptr, 0, SEEK_SET);
            fread(backMove, sizeof(Move), totalMoves - 1, fptr);
        }
    }
    fclose(fptr); 
    
    fptr = fopen(game_path, "wb");
    if (fptr) 
    {
        if (backMove) 
        {
            fwrite(backMove, sizeof(Move), totalMoves - 1, fptr);
            free(backMove);
        }

        fclose(fptr);
    }

    loadGame(boardPtr, player1, player2, ply1Captures, ply2Captures, whiteEnPassantCol, blackEnPassantCol, game_path);

    updateBoard(*boardPtr, player1, player2);

    return true;
}


void saveMove(Move move, const char* game_path)
{
    FILE* fptr = fopen(game_path, "ab");

    if (fptr) 
    {
        fwrite(&move, sizeof(Move), 1, fptr);
        fclose(fptr);
    }
}


bool redoLastMove(char*** boardPtr, Player* player1, Player* player2, Captured* ply1Captures
    , Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol, const char *game_path, const char *redo_path)
{
    char **board;

    if (boardPtr == NULL)
    {
        board = initializeBoard();
        *boardPtr = board;
    }
    else board = *boardPtr;

    FILE* fptr = fopen(redo_path, "rb");

    if (fptr == NULL) return false;
    
    fseek(fptr, 0, SEEK_END);
    long fileSize = ftell(fptr);
    int totalMoves = (int)(fileSize / sizeof(Move));
    
    if (totalMoves == 0) 
    {
        fclose(fptr);
        return false;
    }

    fseek(fptr, -(long)sizeof(Move), SEEK_END);
    Move redoMove;
    fread(&redoMove, sizeof(Move), 1, fptr);
    
    Move* remainingMoves = NULL;
    if (totalMoves > 1) 
    {
        remainingMoves = malloc(sizeof(Move) * (totalMoves - 1));
        if (remainingMoves) 
        {
            fseek(fptr, 0, SEEK_SET);
            fread(remainingMoves, sizeof(Move), totalMoves - 1, fptr);
        }
    }

    fclose(fptr);
    
    fptr = fopen(redo_path, "wb");
    if (fptr) 
    {
        if (remainingMoves) 
        {
            fwrite(remainingMoves, sizeof(Move), totalMoves - 1, fptr);
            free(remainingMoves);
        }

        fclose(fptr);
    }
    
    saveMove(redoMove, game_path);
    loadGame(boardPtr, player1, player2, ply1Captures, ply2Captures
                , whiteEnPassantCol, blackEnPassantCol, game_path);
    
    updateBoard(*boardPtr, player1, player2);

    return true;
}


void clearRedo(const char *redo_path)
{
    FILE* fptr = fopen(redo_path, "wb");
    if (fptr != NULL) fclose(fptr);
}
