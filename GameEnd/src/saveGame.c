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

const char* path = "./GameBin/Load/game.bin";
const char* redoPath = "./GameBin/Redo/redo.bin";


bool loadPlayerTurn(char** board, Player* player, Move move, Captured* capture, int *whiteEnPassantCol, int *blackEnPassantCol) 
{
    bool state = false;
    char sym = tolower(move.symbol);

    if (sym == 'p') state = movePawn(board, player, &move, capture, whiteEnPassantCol,
                                         blackEnPassantCol, false, true);
    else if (sym == 'r') state = moveRook(board, player, move, capture, false);
    else if (sym == 'n') state = moveKnight(board, player, move, capture, false);
    else if (sym == 'b') state = moveBishop(board, player, move, capture, false);
    else if (sym == 'q') state = moveQueen(board, player, move, capture, false);
    else if (sym == 'k') state = moveKing(board, player, move, capture, false);
    
    return state;
}


int loadGame(char** board, Player* player1, Player* player2, Captured* ply1Captures, 
    Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
    if (board == NULL) return -1;

    for (int i = 0; i < BOARD_SIZE; i++) 
        for (int j = 0; j < BOARD_SIZE; j++) board[i][j] = EMPTY_SQUARE;

    if (player1 != NULL) freePlayer(*player1);
    if (player2 != NULL) freePlayer(*player2);

    *player1 = createPlayer(COLOR_WHITE);
    *player2 = createPlayer(COLOR_BLACK);
    *whiteEnPassantCol = -1;
    *blackEnPassantCol = -1;
    *ply1Captures = initializeCapture(COLOR_WHITE);
    *ply2Captures = initializeCapture(COLOR_BLACK);

    FILE* fptr = fopen(path, "rb");
    if (fptr == NULL) 
    {
        printf("No Game to Load, Starting a New Game!!!\n");
        return 1; 
    }

    Move move[2];
    int readData;
    int totalMovesRead = 0;
    
    while ((readData = fread(move, sizeof(Move), 2, fptr)) > 0) 
    {
        loadPlayerTurn(board, player1, move[0], ply1Captures, whiteEnPassantCol, blackEnPassantCol);
        if (ply1Captures->newCapture) capturePiece(player2, ply1Captures);

        totalMovesRead++;
        updateBoard(board, *player1, *player2);
        
        if (readData == 2) 
        {
            loadPlayerTurn(board, player2, move[1], ply2Captures, blackEnPassantCol, whiteEnPassantCol);
            if (ply2Captures->newCapture) capturePiece(player1, ply2Captures);

            totalMovesRead++;
            updateBoard(board, *player1, *player2);
        }
    }    
    
    fclose(fptr);
    isChecked(board, player1, true);
    isChecked(board, player2, true);
    
    return (totalMovesRead % 2 == 0) ? 1 : 2;
}


bool undoLastMove(char** board, Player* player1, Player* player2, Captured* ply1Captures,
     Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
    FILE* fptr = fopen(path, "rb");
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
    
    FILE* redoFile = fopen(redoPath, "ab");
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
    
    fptr = fopen(path, "wb");
    if (fptr) 
    {
        if (backMove) 
        {
            fwrite(backMove, sizeof(Move), totalMoves - 1, fptr);
            free(backMove);
        }

        fclose(fptr);
    }

    loadGame(board, player1, player2, ply1Captures, ply2Captures, whiteEnPassantCol, blackEnPassantCol);

    return true;
}


void saveMove(Move move)
{
    FILE* fptr = fopen(path, "ab");

    if (fptr) 
    {
        fwrite(&move, sizeof(Move), 1, fptr);
        fclose(fptr);
    }
}


bool redoLastMove(char** board, Player* player1, Player* player2, Captured* ply1Captures, Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
    FILE* fptr = fopen(redoPath, "rb");

    if (fptr == NULL) 
    {
        return false;
    }
    
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
    
    fptr = fopen(redoPath, "wb");
    if (fptr) 
    {
        if (remainingMoves) 
        {
            fwrite(remainingMoves, sizeof(Move), totalMoves - 1, fptr);
            free(remainingMoves);
        }

        fclose(fptr);
    }
    
    saveMove(redoMove);
    loadGame(board, player1, player2, ply1Captures, ply2Captures, whiteEnPassantCol, blackEnPassantCol);
    
    return true;
}


void clearRedo()
{
    FILE* fptr = fopen(redoPath, "wb");
    if (fptr != NULL) fclose(fptr);
}
