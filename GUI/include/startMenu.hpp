#pragma once

#include <filesystem>
#include <vector>
#include <string>

#include <QListWidgetItem>
#include <QListWidget>
#include <QMainWindow>
#include <QDialog>
#include <QStyle>
#include <QLabel>

extern "C"
{
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
}


void display_start_window(std::unique_ptr<Chess::GInterface> &main_window, std::shared_ptr<Chess::Board> &game_board);
void load_game(Chess::GInterface *&main_window, std::shared_ptr<Chess::Board> &game_board);
