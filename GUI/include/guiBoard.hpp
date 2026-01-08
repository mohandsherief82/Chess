#pragma once

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>

void display_board(QGridLayout *gl, const char (&board)[8][8]
        , QLabel *player1_label, QLabel *player2_label);
