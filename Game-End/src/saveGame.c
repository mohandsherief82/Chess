#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../../Moves/include/pawnMoves.h"
#include "../../Moves/include/rookMoves.h"
#include "../../Moves/include/knightMoves.h"
#include "../../Moves/include/bishopMoves.h"
#include "../../Moves/include/queenMoves.h"
#include "../../Moves/include/kingMoves.h"
#include "../../Moves/include/captures.h"
#include "../../Board/include/board.h"
#include "../../Pieces/include/player.h"
#include "../../chessTypes.h"
#include "../include/check.h"

const char* path = "./Game-End/testing/game.bin";

bool loadPlayerTurn(char** board, Player* player, Move move, Captured* capture, int *whiteEnPassantCol, int *blackEnPassantCol) 
{
    bool state;
    char sym = tolower(move.symbol);

    if (sym == 'p') state = movePawn(board, player, &move, capture, whiteEnPassantCol, blackEnPassantCol, false, true);
    else if (sym == 'r') state = moveRook(board, player, move, capture, false);
    else if (sym == 'n') state = moveKnight(board, player, move, capture, false);
    else if (sym == 'b') state = moveBishop(board, player, move, capture, false);
    else if (sym == 'q') state = moveQueen(board, player, move, capture, false);
    else if (sym == 'k') state = moveKing(board, player, move, capture, false);
    
    return state;
}


int loadGame(char** board, Player* player1, Player* player2, Captured* ply1Captures
    , Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
     for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++) 
        {
            board[i][j] = EMPTY_SQUARE;
        }
    }

    freePlayer(*player1);
    freePlayer(*player2);

    board = initializeBoard();
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
        fptr = fopen(path, "wb");
        fclose(fptr);
        return 1;
    }

    Move move[2];
    int readData;
    int totalMovesRead = 0;
    
    while (true)
    {
        readData = fread(move, sizeof(Move), 2, fptr);
        
        if (readData == 0) break;

        loadPlayerTurn(board, player1, move[0], ply1Captures, whiteEnPassantCol, blackEnPassantCol);
        if (ply1Captures->newCapture == true) capturePiece(player2, ply1Captures);
        totalMovesRead++;
        updateBoard(board, *player1, *player2, *ply1Captures, *ply2Captures, false);
        
        if (readData == 2) 
        {
            loadPlayerTurn(board, player2, move[1], ply2Captures, blackEnPassantCol, whiteEnPassantCol);
            if (ply2Captures->newCapture == true) capturePiece(player1, ply2Captures);
            totalMovesRead++;
            updateBoard(board, *player1, *player2, *ply1Captures, *ply2Captures, false);
        }    
        else break; 
    }    
    
    fclose(fptr);
    isChecked(board, player1, true);
    isChecked(board, player2, true);
    
    return (totalMovesRead % 2 == 0) ? 1 : 2;
}


bool undoLastMove(char** board, Player* player1, Player* player2, Captured* ply1Captures
            , Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
    FILE* fptr = fopen(path, "rb");

    fseek(fptr, 0, SEEK_END);
    int totalMoves = ftell(fptr) / sizeof(Move);
    if(totalMoves ==0)
    {
        fclose(fptr);
        return false;
    }
    Move* backMove = malloc(sizeof(Move) * (totalMoves -1));
            fseek(fptr, 0, SEEK_SET);
    fread(backMove, sizeof(Move), totalMoves -1, fptr);
    fclose(fptr);
    fptr = fopen(path, "wb");
    fwrite(backMove, sizeof(Move), totalMoves -1, fptr);
        fclose(fptr);
    free(backMove);

    for(int i=0; i<BOARD_SIZE; i++)
    {
        for(int j =0; j <BOARD_SIZE;j++) board[i][j] = EMPTY_SQUARE;
    }

    freePlayer(*player1);
    freePlayer(*player2);
    *player1 = createPlayer(COLOR_WHITE);
    *player2 = createPlayer(COLOR_BLACK);
    *ply1Captures = initializeCapture(COLOR_WHITE);
    *ply2Captures = initializeCapture(COLOR_BLACK);

    loadGame(board, player1, player2, ply1Captures
            , ply2Captures, whiteEnPassantCol, blackEnPassantCol);
    return true;
}


void saveMove(Move move)
{
    FILE* fptr = fopen(path, "ab");
    if (fptr)
    {
        if (move.symbol == 'p') printf("%c\n", move.promotedPawn);
        fwrite(&move, sizeof(Move), 1, fptr);
        fclose(fptr);
    }
}
