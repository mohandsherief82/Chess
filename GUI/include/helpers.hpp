#pragma once

#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QString>
#include <QLayout>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QIcon>
#include <QPixmap>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <chrono>
#include <string>
#include <ctime>
#include <array>

#include "boardCell.hpp"
#include "piecesIcon.hpp"

namespace fs = std::filesystem;

// GUI Constants
#define CELL_SIZE 70
#define PIECE_ICON_SIZE 60
#define CAPTURE_CELL_SIZE 30
#define MOVES_READ 32
#define REDO_BUTTON_SIZE 32
#define SAVE_BUTTON_SIZE 30

// External variables used for file reading
extern "C" 
{
    #include "saveGame.h"
    extern char *loadPath;
}

namespace helpers
{
    QString getIconPath(char piece);
    std::array<Move, MOVES_READ> read_moves(std::string path);

    void clear_items(QLayout *gl);
    void add_piece_to_cell(QWidget *cell, char pieceChar);

    void add_piece_to_cell(BoardCell *cell, char pieceChar, int row, int col);
    std::string get_formatted_time();

    std::string load_menu(QWidget* parent, const std::string& folderPath);
}