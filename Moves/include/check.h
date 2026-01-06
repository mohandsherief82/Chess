#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>

#include "player.h"

bool isChecked(char** board, Player* player, bool legalCheck);

#endif
