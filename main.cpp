#include <QApplication>
#include <QMainWindow>
#include <QScreen>
#include <QGuiApplication>

#include <unordered_map>
#include <string>

#include "guiBoard.hpp"
#include "startMenu.hpp"

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
    
    Player ply1 { createPlayer(COLOR_WHITE) };
    Player ply2 { createPlayer(COLOR_BLACK) };

    Captured white_captured { initializeCapture(COLOR_WHITE) };
    Captured BLACK_captured { initializeCapture(COLOR_BLACK) };

    char **board = initializeBoard();

    int whiteEP {-1}, blackEP {-1};

    display_start_window(main_window, &ply1, &ply2, &white_captured, &BLACK_captured
                            , board, &whiteEP, &blackEP);

    main_window->show();

    return app.exec();
}
