#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>

#include <vector>

#include "guiBoard.hpp"
#include "startMenu.hpp"


extern "C"
{
    #include "Board/include/board.h"
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
}

std::vector<PlayerData> start_game(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label)
{
    char **board = initializeBoard();

    PlayerData ply1_data, ply2_data;

    ply1_data.player = createPlayer(COLOR_WHITE);
    ply1_data.ply_captures = initializeCapture(COLOR_WHITE);
    ply2_data.player = createPlayer(COLOR_BLACK);
    ply2_data.ply_captures = initializeCapture(COLOR_BLACK);

    std::vector<PlayerData> ply_data {ply1_data, ply2_data};

    updateBoard(board, player1, player2);

    display_board(main_window, board, player1_label, player2_label);

    return ply_data;
}


void display_start_window(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label)
{
    QWidget *master_container = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout(master_container);
    QHBoxLayout *button_layout = new QHBoxLayout();
    
    QLabel *start_msg = new QLabel("Welcome To Chess");
    QPushButton *start_button = new QPushButton("Start a New Game");
    QPushButton *load_button = new QPushButton("Load a Game");

    // Add button functionality
    QObject::connect(start_button, &QPushButton::clicked, [main_window, player1_label, player2_label]() {
        start_game(main_window, player1_label, player2_label);
    });

    // Unified Button Style
    QString button_style = 
        "QPushButton {"
        "   background-color: #004474;"
        "   color: #f8e7bb;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   padding: 20px;"
        "   min-width: 250px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0c7ace;"
        "   color: white;"
        "}";

    start_button->setStyleSheet(button_style);
    load_button->setStyleSheet(button_style);

    start_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 60px; margin-bottom: 20px;");
    start_msg->setAlignment(Qt::AlignCenter);

    // Build the Menu Stack
    main_layout->addStretch(2);
    main_layout->addWidget(start_msg);
    main_layout->addSpacing(40);
    
    button_layout->addStretch(1);

    button_layout->addWidget(start_button, 0, Qt::AlignCenter);
    button_layout->addSpacing(15);

    button_layout->addWidget(load_button, 0, Qt::AlignCenter);
    button_layout->addStretch(1);
    
    main_layout->addLayout(button_layout);
    main_layout->addStretch(3);

    main_window->setCentralWidget(master_container);

    main_window->setStyleSheet("background-color: #0A1118;");
}
