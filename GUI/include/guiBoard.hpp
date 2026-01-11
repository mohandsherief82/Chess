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

void display_board(QMainWindow *main_window, char **board,
	 Captured *ply1_captures = nullptr, Captured *ply2_captures = nullptr, int player_turn = 1);
