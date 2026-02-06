#pragma once

#include <string>
#include <vector>

extern "C" 
{
    #include "player.h"
    #include "captures.h"

    #include "board.h"
}

char** board_parser(std::string board_string);
Player player_parser(char** board, PieceColor color);

char** copy_board(char** original_board);
Player copy_player(const Player* original_player);

void free_engine_state(char** board, Player* ply1, Player* ply2);
