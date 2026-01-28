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
#include "server.hpp"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    std::shared_ptr<Chess::Board> game_board = std::make_shared<Chess::Board>();
    
    std::unique_ptr<Chess::GInterface> main_window = std::make_unique<Chess::GInterface>(game_board);
    std::unique_ptr<Chess::AIOpponent> opponent = std::make_unique<Chess::AIOpponent>();
    
    game_board->addObserver(main_window.get());

    display_start_window(main_window, game_board);

    main_window->show();

    int result = app.exec();
    
    return result;
}
