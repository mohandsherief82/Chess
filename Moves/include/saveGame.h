#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "../../chessTypes.h"

#include <stdio.h>

void saveMove(Move move);
void loadGame(char** board, Player player1, Player player2);
void undoLastMove();

#endif
