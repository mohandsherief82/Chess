#pragma once

#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QIcon>

#include <cctype>

void display_board(QMainWindow *main_window, const char (&board)[8][8]
        , QLabel *player1_label, QLabel *player2_label, int player_turn);
