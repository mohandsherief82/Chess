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

    QWidget *master_container = new QWidget();
    QVBoxLayout *master_layout = new QVBoxLayout(master_container);

    master_layout->setContentsMargins(20, 20, 20, 20);
    master_layout->setSpacing(10);

    QWidget *container_top = new QWidget();
    QWidget *container_central = new QWidget();
    QWidget *container_bottom = new QWidget();

    QGridLayout *main_grid = new QGridLayout(container_central);
    
    QLabel *player2_msg = new QLabel("Player 2 (Black)");
    QLabel *player1_msg = new QLabel("Player 1 (White)");

    // Set font
    QFont font;

    font.setBold(true);

    // Set Styles to the text
    player2_msg->setFont(font);
    player1_msg->setFont(font);
    player2_msg->setStyleSheet("color: #f8e7bb; font-size: 20px; margin-bottom: 5px;"); 
    player1_msg->setStyleSheet("color: #f8e7bb; font-size: 20px; margin-bottom: 5px;");


    // Add pieces and the board to the main grid
    display_board(main_grid, board, player1_msg, player2_msg);

    // Add widgets to the main layout
    master_layout->addWidget(container_top);
    master_layout->addWidget(container_central, 1); 
    master_layout->addWidget(container_bottom);

    // Add Containers to the main window
    main_window->setCentralWidget(master_container);
    main_window->setStyleSheet("background-color: #0A1118;");

    main_window->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());

    main_window->show();

    return app.exec();
}
