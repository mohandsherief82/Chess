#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScreen>
#include <QFont>

#include "guiBoard.hpp"

extern "C" {
    const char board[8][8] = {
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

    QFont font;

    QTextEdit *player1_msg = new QTextEdit("Player 1");
    QTextEdit *player2_msg = new QTextEdit("Player 2");

    font.setPointSize(24);
    font.setBold(true);

    player1_msg->setFont(font);
    player2_msg->setFont(font);

    QWidget *container = new QWidget();
    
    QGridLayout *main_grid = new QGridLayout(container);

    QTextEdit *text = new QTextEdit{"Hello"};

    // Set Styles
    display_board(main_grid, board);

    // Resize all objects
    main_window->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());

    // Add objects to containers
    // main_grid->addWidget(player1_msg);
    // main_grid->addWidget(player2_msg);

    main_window->setCentralWidget(container);

    main_window->show();

    return app.exec();
}
