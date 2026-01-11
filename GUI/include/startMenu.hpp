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

void display_start_window(QMainWindow *main_window, Player *ply1, Player *ply2
                          , Captured *ply1_captures, Captured *ply2_captures,
                            char**& board, int *whiteEP, int *blackEP);
