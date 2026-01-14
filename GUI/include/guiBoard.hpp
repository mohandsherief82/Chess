#pragma once

#include <QGuiApplication>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QScreen>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QIcon>

#include <memory>

#include "interfaces.hpp"

#include <cctype>

// GUI Constants
#define CELL_SIZE 70
#define PIECE_ICON_SIZE 60
#define CAPTURE_CELL_SIZE 30

extern "C"
{
	#include "player.h"
	#include "captures.h"
}

namespace Chess
{
    class Board;

	class GInterface: public Concrete::Observer, public QMainWindow
    {
        private:
            QGridLayout *gl = nullptr;
            QLabel *player2_msg = nullptr, *player1_msg = nullptr;
            QWidget *master_container = nullptr, *container_central = nullptr;
            QVBoxLayout *master_layout = nullptr, *ply1_data = nullptr, *ply2_data = nullptr;

            void add_captures(int ply_num, Captured *ply_captures);
        public:
            GInterface(QString label_style);
            void display_board(std::shared_ptr<Board> &game_board);
            void update(Concrete::Subject *subject) override;
    };
}

