#ifndef CHECK_H
#define CHECK_H

#include <stdbool.h>

#include "player.h"

MoveValidation isChecked(char** board, Player* player, bool legalCheck);

#endif
