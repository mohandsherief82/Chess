#pragma once

#include <QGuiApplication>
#include <QGridLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QDockWidget>
#include <QScrollArea>
#include <QScreen>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QIcon>

#include "interfaces.hpp"
#include "piecesIcon.hpp"
#include "boardCell.hpp"
#include "helpers.hpp"
#include "dialog.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cctype>
#include <memory>
#include <array>

extern "C"
{
	#include "player.h"
	#include "captures.h"
    #include "saveGame.h"

    extern char *loadPath;
    extern char *redoPath;
}

namespace fs = std::filesystem;

namespace Chess
{
    class Board;

	class GInterface: public Concrete::Observer, public QMainWindow
    {
        private:
            std::shared_ptr<Board> game_board = nullptr;

            void add_captures(QVBoxLayout *ply_data, QLabel *ply_msg, Captured *ply_captures, bool redo_flag);
            void add_redo_undo(QHBoxLayout *box);
            void add_left_menu(QWidget *container);
            void add_moves_view();
            void save_game_as();
            void delete_files();
            void resign();
        public:
            GInterface(std::shared_ptr<Board> game_board);
            void update() override;
            void load_game(const std::string file_path);
            void start_game();
        protected:
            void keyPressEvent(QKeyEvent *event) override;
    };
}
