#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../Moves/include/pawnMoves.h"
#include "../../Moves/include/rockMoves.h"
#include "../../Moves/include/knightMoves.h"
#include "../../Moves/include/bishopMoves.h"
#include "../../Moves/include/queenMoves.h"
#include "../../Moves/include/kingMoves.h"
#include "../../Moves/include/captures.h"
#include "../../chessTypes.h"

#define MOVE_SIZE 7

const char* path = "./Game-End/testing/game.bin";

void loadPlayerTurn(char** board, Player* player, Move move, Captured* capture)
{
    
    if (move.symbol == 'p') movePawn(board, player, move, capture);
    else if (move.symbol == 'r') moveRock(board, player, move, capture);
    else if (move.symbol == 'n') moveKnight(board, player, move, capture);
    else if (move.symbol == 'b') moveBishop(board, player, move, capture);
    else if (move.symbol == 'q') moveQueen(board, player, move, capture);
    else if (move.symbol == 'k') moveKing(board, player, move, capture);
}


void undoLastMove()
{

}


bool loadGame(char** board, Player* player1, Player* player2, 
                        Captured* ply1Captures, Captured* ply2Captures)
{
    FILE* fptr = fopen(path, "rb");
    Move move[2];
    int readData;

    while ((readData = fread(move, sizeof(Move), 2, fptr)) >= 1)
    {
        loadPlayerTurn(board, player1, move[0], ply1Captures);
        if (ply1Captures->newCapture == true) capturePiece(*player2, &ply1Captures);
                
        if (readData == 2) 
        {
            loadPlayerTurn(board, player2, move[1], ply2Captures);
            if (ply2Captures->newCapture == true) capturePiece(*player1, &ply2Captures);
        }
    }    

    isChecked(board, player1);
    isChecked(board, player2);

    fclose(fptr);

    if (readData == 1) return true;
    else return false;
}


void saveMove(Move move)
{
    FILE* fptr = fopen(path, "ab");

    fwrite(&move, sizeof(Move), 1, fptr);

    fclose(fptr);
    return;
}
