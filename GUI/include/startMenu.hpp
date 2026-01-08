#pragma once

#include <vector>

#include <QMainWindow>
#include <QLabel>

extern "C"
{
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
}

struct PlayerData
{
        Player player;
        Captured ply_captures;
};

void display_start_window (QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label);
std::vector<PlayerData> start_game(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label);
