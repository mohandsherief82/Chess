#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QScreen>
#include <QFont>
#include <QLabel>

#include "guiBoard.hpp"
#include "startMenu.hpp"

extern "C" 
{
    const char board[8][8] = 
    {
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'-', '.', '-', '.', '-', '.', '-', '.'}, 
        {'.', '-', '.', '-', '.', '-', '.', '-'},
        {'-', '.', '-', '.', '-', '.', '-', '.'}, 
        {'.', '-', '.', '-', '.', '-', '.', '-'}, 
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, 
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}  
    };
}


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow *main_window = new QMainWindow();

    // Main Window Main Settings
    main_window->setStyleSheet("background-color: #0A1118;");
    main_window->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());
    
    QLabel *player2_msg = new QLabel("Player 2 (Black)");
    QLabel *player1_msg = new QLabel("Player 1 (White)");

    int player_turn {1};

    // Set Styles to the text
    player2_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 20px; margin-bottom: 5px;"); 
    player1_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 20px; margin-bottom: 5px;");

    // Add pieces and the board to the main grid
    display_board(main_window, board, player1_msg, player2_msg, player_turn);

    display_start_window(main_window);

    main_window->show();

    return app.exec();
}
