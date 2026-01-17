#pragma once

#include <QString>
#include <QLayout>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QPixmap>

#include <array>
#include <fstream>
#include <cctype>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "boardCell.hpp"
#include "piecesIcon.hpp"

// GUI Constants
#define CELL_SIZE 70
#define PIECE_ICON_SIZE 60
#define CAPTURE_CELL_SIZE 30
#define MOVES_READ 32

// External variables used for file reading
extern "C" 
{
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
}