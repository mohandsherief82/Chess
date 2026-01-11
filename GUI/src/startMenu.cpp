#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>

#include <unordered_map>
#include <string>
#include <iostream>

#include "guiBoard.hpp"
#include "startMenu.hpp"

extern "C" 
{
    #include "Board/include/board.h"
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
    #include "GameEnd/include/saveGame.h"
}


void load_game(QMainWindow *main_window, char**& board, Player *ply1, Player *ply2
                , Captured *ply1_captures, Captured *ply2_captures, int *whiteEP, int *blackEP)
{
    // Ensure the board is initialized
    if (board == nullptr) board = initializeBoard();

    // Pass the pointers directly to loadGame
    int player_turn = loadGame(board, ply1, ply2, ply1_captures, 
            ply2_captures, whiteEP, blackEP);

    updateBoard(board, *ply1, *ply2);
    display_board(main_window, board, ply1_captures, ply2_captures, player_turn);

    return;
}


void start_game(QMainWindow *main_window, char**& board, Player *ply1, Player *ply2
                          , Captured *ply1_captures, Captured *ply2_captures) 
{
    board = initializeBoard();

    // Sync board state using the data stored in the map
    updateBoard(board, *ply1, *ply2);

    // Render the board
    display_board(main_window, board, ply1_captures, ply2_captures);

    return;
}


void display_start_window(QMainWindow *main_window, Player *ply1, Player *ply2
                          , Captured *ply1_captures, Captured *ply2_captures,
                            char**& board, int *whiteEP, int *blackEP)
{
    QWidget *master_container = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout(master_container);
    QHBoxLayout *button_layout = new QHBoxLayout();
    
    QLabel *start_msg = new QLabel("Welcome To Chess");
    QPushButton *start_button = new QPushButton("Start a New Game");
    QPushButton *load_button = new QPushButton("Load a Game");

    // Button Functionality with lambda functions
    QObject::connect(start_button, &QPushButton::clicked, [=, &board]() 
    {
        start_game(main_window, board, ply1, ply2, ply1_captures, ply2_captures);
    });

    QObject::connect(load_button, &QPushButton::clicked, [=, &board]() 
    {
        load_game(main_window, board, ply1, ply2, ply1_captures, ply2_captures, whiteEP, blackEP);
    });

    QString button_style = 
            "QPushButton { background-color: #004474; color: #f8e7bb; "
            "font-size: 18px; font-weight: bold; border-radius: 8px; "
            "padding: 20px; min-width: 250px; }"
            "QPushButton:hover { background-color: #0c7ace; color: white; }";

    start_button->setStyleSheet(button_style);
    load_button->setStyleSheet(button_style);

    start_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 60px; margin-bottom: 20px;");
    start_msg->setAlignment(Qt::AlignCenter);

    // Layout assembly
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
}
