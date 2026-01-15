#pragma once

#include <vector>

#include <QMainWindow>
#include <QLabel>

extern "C"
{
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
}


void display_start_window(std::unique_ptr<Chess::GInterface> &main_window, std::shared_ptr<Chess::Board> &game_board);
