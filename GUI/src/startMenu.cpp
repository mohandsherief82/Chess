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


void load_game(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label
            , char**& board, std::unordered_map<std::string, PlayerData>& ply_map, int *whiteEP, int *blackEP)
{
    // Ensure the board is initialized
    if (board == nullptr) board = initializeBoard();

    Player ply1 = {};
    Player ply2 = {};

    Captured white_captures = {};
    Captured black_captures = {};

    // Pass the pointers directly to loadGame
    int player_turn = loadGame(board, &ply1, &ply2, &white_captures, 
            &black_captures, whiteEP, blackEP);

    if (ply_map.count("Player 1")) freePlayer(ply_map["Player 1"].player);
    if (ply_map.count("Player 2")) freePlayer(ply_map["Player 2"].player);

    ply_map.clear();

    PlayerData p1_data, p2_data;

    p1_data.player = ply1;
    p1_data.ply_captures = white_captures;

    p2_data.player = ply2;
    p2_data.ply_captures = black_captures;

    ply_map["Player 1"] = p1_data;
    ply_map["Player 2"] = p2_data;

    updateBoard(board, ply_map["Player 1"].player, ply_map["Player 2"].player);
    display_board(main_window, board, player1_label, player2_label, player_turn);

    return;
}


void start_game(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label, char**& board
    , std::unordered_map<std::string, PlayerData>& ply_map) 
{
    board = initializeBoard(); 

    PlayerData ply1, ply2;

    ply1.player = createPlayer(COLOR_WHITE);
    ply1.ply_captures = initializeCapture(COLOR_WHITE);
    
    ply2.player = createPlayer(COLOR_BLACK);
    ply2.ply_captures = initializeCapture(COLOR_BLACK);

    ply_map.clear();
    ply_map["Player 1"] = ply1;
    ply_map["Player 2"] = ply2;

    // Sync board state using the data stored in the map
    updateBoard(board, ply_map["Player 1"].player, ply_map["Player 2"].player);

    // Render the board
    display_board(main_window, board, player1_label, player2_label);
}


void display_start_window(QMainWindow *main_window, QLabel *player1_label, QLabel *player2_label, 
                          std::unordered_map<std::string, PlayerData>& ply_map
                          , char**& board, int *whiteEP, int *blackEP)
{
    QWidget *master_container = new QWidget();
    QVBoxLayout *main_layout = new QVBoxLayout(master_container);
    QHBoxLayout *button_layout = new QHBoxLayout();
    
    QLabel *start_msg = new QLabel("Welcome To Chess");
    QPushButton *start_button = new QPushButton("Start a New Game");
    QPushButton *load_button = new QPushButton("Load a Game");

    // Button Functionality with lambda functions
    QObject::connect(start_button, &QPushButton::clicked, [=, &ply_map, &board]() 
    {
        start_game(main_window, player1_label, player2_label, board, ply_map);
    });

    QObject::connect(load_button, &QPushButton::clicked, [=, &ply_map, &board]() 
    {
            load_game(main_window, player1_label, player2_label, board, ply_map, whiteEP, blackEP);
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
