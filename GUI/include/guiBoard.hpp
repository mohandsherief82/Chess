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
    #include "saveGame.h"

    extern char *loadPath;
}

namespace Chess
{
    class Board;

	class GInterface: public Concrete::Observer, public QMainWindow
    {
        private:
            std::shared_ptr<Board> game_board = nullptr;

            void add_captures(QVBoxLayout *ply_data, QLabel *ply_msg, Captured *ply_captures);
        public:
            GInterface(std::shared_ptr<Board> game_board);
            void update() override;
        protected:
            void keyPressEvent(QKeyEvent *event) override;
    };
}
