#include <QApplication>
#include <QMainWindow>
#include <QLabel>
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
    
    // Initialize Labels
    QLabel *player2_msg = new QLabel("Player 2 (Black)");
    QLabel *player1_msg = new QLabel("Player 1 (White)");

    // Set Styles for the labels
    QString label_style = "font-weight: bold; color: #f8e7bb; font-size: 20px; margin-bottom: 5px; padding-bottom: 2px;";
    player2_msg->setStyleSheet(label_style); 
    player1_msg->setStyleSheet(label_style);

    std::unordered_map<std::string, PlayerData> ply_data;
    char **board = nullptr;
    int whiteEP {-1}, blackEP {-1};

    display_start_window(main_window, player1_msg, player2_msg, ply_data, board, &whiteEP, &blackEP);

    main_window->show();

    return app.exec();
}
