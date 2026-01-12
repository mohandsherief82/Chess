#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QIcon>

#include <cctype>

extern "C"
{
	#include "player.h"
	#include "captures.h"
}

void display_board(QMainWindow *main_window, char **board, Player *ply1
    , Captured *ply1_captures, Captured *ply2_captures, int player_turn = 1);
