#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>

#include <iostream>
#include <string>

#include "guiBoard.hpp"
#include "startMenu.hpp"
#include "interfaces.hpp"

extern "C" 
{
    #include "Board/include/board.h"
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow *main_window = new QMainWindow();

    // Main Window Settings
    main_window->setStyleSheet("background-color: #0A1118;");
    main_window->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());

    Chess::Board *board = new Chess::Board();

    main_window->show();

    return app.exec();
}
