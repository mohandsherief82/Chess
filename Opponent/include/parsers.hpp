
#pragma once

#include <string>

extern "C"
{
    #include "player.h"
    #include "captures.h"
    #include "board.h"
}

char **board_parser(std::string board_string);
Player player_parser(char **board, PieceColor color);
Captured captures_parser(char **board, Player ply);
Move move_parser(std::string move_string);
