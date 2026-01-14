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
    QString label_style = "font-weight: bold; color: #f8e7bb;"
                        " font-size: 20px; margin-bottom: 5px;"
                        " padding-bottom: 2px;";

    QApplication app(argc, argv);
    Chess::GInterface *main_window = new Chess::GInterface(label_style);  

    Chess::Board *game_board = new Chess::Board();

    display_start_window(main_window, game_board);

    main_window->show();

    return app.exec();
}
