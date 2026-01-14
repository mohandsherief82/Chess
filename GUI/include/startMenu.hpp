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

void display_start_window(Chess::GInterface *&main_window, Chess::Board *&boar);
