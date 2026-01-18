#pragma once

#include <QGuiApplication>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QDockWidget>
#include <QScreen>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QIcon>

#include <memory>

#include "interfaces.hpp"
#include "helpers.hpp"

#include <cctype>

extern "C"
{
	#include "player.h"
	#include "captures.h"
    #include "saveGame.h"

    extern char *loadPath;
    extern char *redoPath;
}

namespace Chess
{
    class Board;

	class GInterface: public Concrete::Observer, public QMainWindow
    {
        private:
            std::shared_ptr<Board> game_board = nullptr;

            void add_captures(QVBoxLayout *ply_data, QLabel *ply_msg, Captured *ply_captures, bool redo_flag);
            void add_redo_undo(QHBoxLayout *box);
            void add_moves_view();
        public:
            GInterface(std::shared_ptr<Board> game_board);
            void update() override;
        protected:
            void keyPressEvent(QKeyEvent *event) override;
    };
}
