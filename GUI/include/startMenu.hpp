
#pragma once

#include <QMainWindow>
#include <QLabel>

extern "C"
{
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
}

void display_start_window (QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label);
void start_game(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label);

struct PlayerData
{
        Player player;
        Captured ply_captures;
};
