#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>

#include <iostream>
#include <string>
#include <memory>

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
    std::unique_ptr<Chess::GInterface> main_window = std::make_unique<Chess::GInterface>(label_style);  

    std::shared_ptr<Chess::Board> game_board = std::make_shared<Chess::Board>();

    display_start_window(main_window, game_board);

    main_window->show();

    int result = app.exec();
    
    return result;
}
