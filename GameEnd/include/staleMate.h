#ifndef STALE_MATE_H
#define STALE_MATE_H

#include "player.h"

#include <stdbool.h>

char** copyBoard(char** board);
Player copyPlayer(Player* player);
void freeCopy(Player copyPlayer, char** copyBoard);
bool checkStalemate(char** board, Player* player);
bool legalMove(char** board, Player* player);
bool inSufficientMaterial(Player player1, Player player2);

#endif