#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "chessTypes.h"
#include "player.h" 
#include "captures.h"

#include <stdio.h>

void saveMove(Move move);
int loadGame(char*** board, Player* player1, Player* player2, Captured* ply1Captures, 
    Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol);
bool undoLastMove(char*** boardPtr, Player* player1, Player* player2, Captured* ply1Captures,
     Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol);
bool redoLastMove(char*** boardPtr, Player* player1, Player* player2,
     Captured* ply1Captures, Captured* ply2Captures, int *whiteEnPassantCol, int *blackEnPassantCol);
void clearRedo();

#endif
