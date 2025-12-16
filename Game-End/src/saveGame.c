#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../../Moves/include/pawnMoves.h"
#include "../../Moves/include/rockMoves.h"
#include "../../Moves/include/knightMoves.h"
#include "../../Moves/include/bishopMoves.h"
#include "../../Moves/include/queenMoves.h"
#include "../../Moves/include/kingMoves.h"
#include "../../Moves/include/captures.h"
#include "../../Board/include/board.h"
#include "../../Pieces/include/player.h"
#include "../../chessTypes.h"

const char* path = "./Game-End/testing/game.bin";

void loadPlayerTurn(char** board, Player* player, Move move, Captured* capture
                         , int *whiteEnPassantCol, int *blackEnPassantCol)
{
    if (tolower(move.symbol) == 'p') movePawn(board, player, move, capture, whiteEnPassantCol, blackEnPassantCol, false);
    else if (tolower(move.symbol) == 'r') moveRock(board, player, move, capture, false);
    else if (tolower(move.symbol) == 'n') moveKnight(board, player, move, capture, false);
    else if (tolower(move.symbol) == 'b') moveBishop(board, player, move, capture, false);
    else if (tolower(move.symbol) == 'q') moveQueen(board, player, move, capture, false);
    else if (tolower(move.symbol) == 'k') moveKing(board, player, move, capture, false);

}


int loadGame(char** board, Player* player1, Player* player2, Captured* ply1Captures
    , Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol)
{
    FILE* fptr = fopen(path, "rb");
    if (fptr == NULL)
    {
        printf("No Game to Load, Starting a New Game!!!\n");
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
        if (ply1Captures->newCapture == true) capturePiece(*player2, ply1Captures);
        totalMovesRead++; 
        
        if (readData == 2) 
        {
            loadPlayerTurn(board, player2, move[1], ply2Captures, blackEnPassantCol, whiteEnPassantCol);
            if (ply2Captures->newCapture == true) capturePiece(*player1, ply2Captures);
            totalMovesRead++;
        } 
        else break; 
    }    
    
    fclose(fptr);
    
    isChecked(board, player1);
    isChecked(board, player2);
    
    if (totalMovesRead == 0 || totalMovesRead % 2 == 0) return 1;
    else return 2;
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
    
    fwrite(&move, sizeof(Move), 1, fptr);
    
    fclose(fptr);
    return;
}
