#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QDebug>

#include <memory>
#include <string>
#include <iostream>

#include "guiBoard.hpp"
#include "startMenu.hpp"
#include "helpers.hpp"

extern "C" 
{
    #include "Board/include/board.h"
    #include "Pieces/include/player.h"
    #include "Pieces/include/captures.h"
    #include "GameEnd/include/saveGame.h"
}

namespace fs = std::filesystem;


void load_game(std::unique_ptr<Chess::GInterface> &main_window, std::shared_ptr<Chess::Board> &game_board)
{
    std::string game_path = helpers::load_menu(main_window.get(), loadPath);

    if (game_path.empty()) return;

    char ***board_ptr = game_board->get_board_ptr();

    Player *ply1 = game_board->get_player(PLAYER1);
    Player *ply2 = game_board->get_player(PLAYER2);

    Captured *ply1_captures = game_board->get_player_captures(PLAYER1);
    Captured *ply2_captures = game_board->get_player_captures(PLAYER2);

    int *whiteEP = game_board->get_player_EP(PLAYER1);
    int *blackEP = game_board->get_player_EP(PLAYER2);

    if (*board_ptr == nullptr) *board_ptr = initializeBoard();

    int player_turn = loadGame(board_ptr, ply1, ply2, ply1_captures, 
            ply2_captures, whiteEP, blackEP, game_path.c_str());

    game_board->update_board();
    game_board->udpate_game_path(game_path);
    
    main_window->update();

    return;
}


void start_game(std::unique_ptr<Chess::GInterface> &main_window, std::shared_ptr<Chess::Board> &game_board) 
{
    char ***board_ptr = game_board->get_board_ptr();
    *board_ptr = initializeBoard();

    Player *ply1 = game_board->get_player(PLAYER1);
    Player *ply2 = game_board->get_player(PLAYER2);

    std::string game_path { loadPath }
            , time { helpers::get_formatted_time() }
            , redo_path { redoPath };
    
    game_path.append( time );
    redo_path.append( time );

    { 
        std::ofstream game_file(game_path, std::ios::binary); 
        std::ofstream redo_file(redo_path, std::ios::binary); 
    }

    game_board->udpate_game_path(game_path);
    game_board->udpate_redo_path(redo_path);

    // Sync board state using the data stored in the map
    updateBoard(*board_ptr, ply1, ply2);

    // Render the board
    main_window->update();

    return;
}


void display_start_window(std::unique_ptr<Chess::GInterface> &main_window, std::shared_ptr<Chess::Board> &game_board)
{
    QWidget *master_container = new QWidget();
    
    QVBoxLayout *main_layout = new QVBoxLayout(master_container);
    QHBoxLayout *button_layout = new QHBoxLayout();
    
    QLabel *start_msg = new QLabel("Welcome To Chess");
    
    QPushButton *start_button = new QPushButton("Start a New Game");
    QPushButton *load_button = new QPushButton("Load a Game");

    QString button_style = 
            "QPushButton { background-color: #004474; color: #f8e7bb; "
            "font-size: 18px; font-weight: bold; border-radius: 8px; "
            "padding: 20px; min-width: 250px; }"
            "QPushButton:hover { background-color: #0c7ace; color: white; }";

    start_button->setStyleSheet(button_style);
    load_button->setStyleSheet(button_style);

    start_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 60px; margin-bottom: 20px;");
    start_msg->setAlignment(Qt::AlignCenter);

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

    QObject::connect(start_button, &QPushButton::clicked, [&]() 
            {
                start_game(main_window, game_board);
            }
    );

    QObject::connect(load_button, &QPushButton::clicked, [&]() 
            {
                load_game(main_window, game_board);
            }
    );

    return;
}
