#pragma once

#include <string>
#include <vector>

#include <functional>

extern "C" 
{
    #include "player.h"
    #include "captures.h"

    #include "board.h"
}

Player player_parser(char** board, PieceColor color);
char** copy_board(char** original_board);

Player copy_player(const Player* player);
void free_engine_state(char*** board_ptr, Player* ply1, Player* ply2) ;
