#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "../../chessTypes.h"

#include <stdio.h>

void saveMove(Move move);
bool loadGame(char** board, Player* player1, Player* player2, 
                        Captured* ply1Captures, Captured* ply2Captures);
void undoLastMove();

#endif
