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
	#include "captures.h"
}

void display_board(QMainWindow *main_window, char **board
        , QLabel *player1_label, QLabel *player2_label, Captured *ply1_captures = nullptr, 
			Captured *ply2_captures = nullptr, int player_turn = 1);
