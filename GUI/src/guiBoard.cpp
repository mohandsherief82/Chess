#include "guiBoard.hpp"


namespace Chess
{
    GInterface::GInterface(std::shared_ptr<Board> game_board)
        : game_board(game_board)
    {
        this->setStyleSheet("background-color: #0A1118;");
        this->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());
    }


    void GInterface::add_moves_view()
    {
        std::string path { this->game_board->get_game_path() };
        std::array<Move, MOVES_READ> moves { helpers::read_moves(path) };

        QWidget *central = this->centralWidget();
        if (!central || !central->layout()) return;

        QWidget *sidebar = central->findChild<QWidget*>("moveHistorySidebar");
        
        if (!sidebar) 
        {
            sidebar = new QWidget();
            sidebar->setObjectName("moveHistorySidebar");
            
            sidebar->setFixedWidth(380);
            sidebar->setFixedHeight(650);
            
            if (QHBoxLayout *hLayout = qobject_cast<QHBoxLayout*>(central->layout())) 
            {
                hLayout->addSpacing(50); 
                hLayout->addWidget(sidebar, 0, Qt::AlignVCenter);
            }
            
            QVBoxLayout *side_layout = new QVBoxLayout(sidebar);

            side_layout->setContentsMargins(0, 0, 0, 0);
            side_layout->setSpacing(5);

            QLabel *title = new QLabel("Moves");

            title->setObjectName("sidebarTitle");
            title->setAlignment(Qt::AlignCenter);

            side_layout->addWidget(title);

            QScrollArea *scroll = new QScrollArea();

            scroll->setWidgetResizable(true);
            scroll->setStyleSheet("background: transparent; border: none;");
            
            QWidget *table_container = new QWidget();

            table_container->setObjectName("tableContainer");

            new QGridLayout(table_container);
            
            scroll->setWidget(table_container);
            side_layout->addWidget(scroll);
        }

        QString label_style = "font-weight: bold; color: #f8e7bb;"
                            " font-size: 20px; margin-bottom: 5px;"
                            " padding-bottom: 2px;";

        QLabel *title = sidebar->findChild<QLabel*>("sidebarTitle");

        if (title) title->setStyleSheet(label_style + "font-size: 22px;");

        QWidget *table_container = sidebar->findChild<QWidget*>("tableContainer");

        QGridLayout *grid_view = qobject_cast<QGridLayout*>(table_container->layout());
        helpers::clear_items(grid_view);

        grid_view->setSpacing(2);
        grid_view->setContentsMargins(0, 0, 0, 0);

        grid_view->setColumnStretch(0, 1);
        grid_view->setColumnStretch(1, 1);

        QString headerStyle = label_style + "background-color: #1A2634; border-bottom: 2px solid #f8e7bb; padding: 10px;";
        
        QLabel *whiteHeader = new QLabel("White");
        QLabel *blackHeader = new QLabel("Black");

        whiteHeader->setStyleSheet(headerStyle);
        blackHeader->setStyleSheet(headerStyle);

        whiteHeader->setAlignment(Qt::AlignCenter);
        blackHeader->setAlignment(Qt::AlignCenter);

        grid_view->addWidget(whiteHeader, 0, 0);
        grid_view->addWidget(blackHeader, 0, 1);

        for (int i = 0; i < MOVES_READ; i++)
        {
            int row = (i / 2) + 1; 
            int col = i % 2;
            QString rowBg = (row % 2 == 0) ? "#0D1721" : "#121C26";

            QWidget *cell = new QWidget();
            cell->setStyleSheet(QString("background-color: %1;").arg(rowBg));

            QHBoxLayout *cell_layout = new QHBoxLayout(cell);
            cell_layout->setContentsMargins(10, 2, 10, 2);

            cell_layout->setSpacing(6);
            cell_layout->setAlignment(Qt::AlignLeft);

            if (moves[i].symbol != 0)
            {
                QLabel *iconLabel = new QLabel();
                QPixmap pix = QIcon(helpers::getIconPath(moves[i].symbol)).pixmap(24, 24);

                iconLabel->setPixmap(pix);
                cell_layout->addWidget(iconLabel);

                QString move_text = QString("%1%2 -> %3%4")
                    .arg((char)(moves[i].colPrev + 'A')).arg(8 - moves[i].rowPrev)
                    .arg((char)(moves[i].colNext + 'A')).arg(8 - moves[i].rowNext);
                
                QLabel *textLabel = new QLabel(move_text);

                textLabel->setStyleSheet(label_style + "font-size: 14px; background: transparent; padding: 0; margin: 0;");
                cell_layout->addWidget(textLabel);

                cell_layout->addStretch();
            }
            else
            {
                QLabel *emptyLabel = new QLabel("");

                emptyLabel->setStyleSheet(label_style + "font-size: 14px; background: transparent;");
                cell_layout->addWidget(emptyLabel);
            }

            grid_view->addWidget(cell, row, col);
        }

        return;
    }


    void GInterface::load_game()
    {
        std::string game_path { helpers::load_menu(this, loadPath) };

        if (game_path.empty()) return;

        char ***board_ptr = this->game_board->get_board_ptr();

        Player *ply1 = this->game_board->get_player(PLAYER1);
        Player *ply2 = this->game_board->get_player(PLAYER2);

        Captured *ply1_captures = this->game_board->get_player_captures(PLAYER1);
        Captured *ply2_captures = this->game_board->get_player_captures(PLAYER2);

        int *whiteEP = this->game_board->get_player_EP(PLAYER1);
        int *blackEP = this->game_board->get_player_EP(PLAYER2);

        if (*board_ptr == nullptr) *board_ptr = initializeBoard();

        int player_turn = loadGame(board_ptr, ply1, ply2, ply1_captures, 
                ply2_captures, whiteEP, blackEP, game_path.c_str());

        this->game_board->update_board();
        this->game_board->udpate_game_path(game_path);
        
        this->update();

        return;
    }


    void GInterface::start_game() 
    {
        char ***board_ptr = this->game_board->get_board_ptr();
        *board_ptr = initializeBoard();

        Player *ply1 = this->game_board->get_player(PLAYER1);
        Player *ply2 = this->game_board->get_player(PLAYER2);

        std::string game_path { loadPath }
                , time { helpers::get_formatted_time() }
                , redo_path { redoPath };
        
        game_path.append( time );
        redo_path.append( time );

        { 
            std::ofstream game_file(game_path, std::ios::binary); 
            std::ofstream redo_file(redo_path, std::ios::binary); 
        }

        this->game_board->udpate_game_path(game_path);
        this->game_board->udpate_redo_path(redo_path);

        // Sync board state using the data stored in the map
        updateBoard(*board_ptr, ply1, ply2);

        // Render the board
        this->update();

        return;
    }


    void GInterface::add_left_menu(QWidget *container)
    {
        QVBoxLayout *layout { new QVBoxLayout(container) };

        container->setFixedSize(250, 500); 
        container->setStyleSheet("background-color: #111c28; border-radius: 10px;");

        QString flat_style =
                    "QPushButton {"
                    "   color: #f8e7bb; "
                    "   border: 2px solid transparent;" 
                    "   background: transparent;"
                    "   font-size: 22px;" 
                    "   padding: 10px;" 
                    "   text-align: left;"
                    "}"
                    "QPushButton:hover {"
                    "   background-color: #1c2b3a;"
                    "   border-radius: 5px;"
                    "}";

        QPushButton *save_button { new QPushButton( QIcon( helpers::getIconPath('s') ), "  Save Game") };
        QPushButton *load_button { new QPushButton( QIcon( helpers::getIconPath('l') ), "  Load Game") };
        QPushButton *start_button { new QPushButton( QIcon( helpers::getIconPath('a') ), "  Start New Game") };

        save_button->setStyleSheet(flat_style);
        load_button->setStyleSheet(flat_style);
        start_button->setStyleSheet(flat_style);

        save_button->setFixedSize(200, 50);
        load_button->setFixedSize(200, 50);

        save_button->setIconSize(QSize(SAVE_BUTTON_SIZE, SAVE_BUTTON_SIZE));
        load_button->setIconSize(QSize(SAVE_BUTTON_SIZE, SAVE_BUTTON_SIZE));
        start_button->setIconSize(QSize(SAVE_BUTTON_SIZE, SAVE_BUTTON_SIZE));

        QObject::connect(load_button, &QPushButton::clicked, [=]()
                {
                    this->load_game();
                }  
        );

        QObject::connect(start_button, &QPushButton::clicked, [=]()
                {
                    this->start_game();
                }  
        );

        layout->setSpacing(20);
        layout->setContentsMargins(0, 0, 0, 0);

        layout->addStretch(1);

        layout->addWidget(save_button, 0, Qt::AlignCenter);
        layout->addWidget(load_button, 0, Qt::AlignCenter);
        layout->addWidget(start_button, 0, Qt::AlignCenter);

        layout->addStretch(1);
    }


    void GInterface::add_redo_undo(QHBoxLayout *box)
    {
        QString redo_icon_path { helpers::getIconPath('d') };   
        QString undo_icon_path { helpers::getIconPath('u') };

        QPushButton *undo_button = new QPushButton(QIcon(undo_icon_path), "Undo");
        QPushButton *redo_button = new QPushButton(QIcon(redo_icon_path), "Redo");

        QString flat_style = 
                    "QPushButton {"
                    "   color: #f8e7bb; "
                    "   border: none;" 
                    "   background: transparent;"
                    "   font-size: 20px;" 
                    "   padding: 0px;" 
                    "   text-align: left;"
                    "}"
                    "QPushButton:hover {"
                    "   background-color: #1c2b3a;"
                    "   border-radius: 5px;"
                    "}";

        undo_button->setStyleSheet(flat_style);
        redo_button->setStyleSheet(flat_style);

        undo_button->setFixedSize(REDO_BUTTON_SIZE * 3, REDO_BUTTON_SIZE);
        redo_button->setFixedSize(REDO_BUTTON_SIZE * 3, REDO_BUTTON_SIZE);

        undo_button->setIconSize(QSize(REDO_BUTTON_SIZE, REDO_BUTTON_SIZE));
        redo_button->setIconSize(QSize(REDO_BUTTON_SIZE, REDO_BUTTON_SIZE));
        
        QObject::connect(undo_button, &QPushButton::clicked, [=]()
            {
                undoLastMove(
                        this->game_board->get_board_ptr(),
                        this->game_board->get_player(PLAYER1),
                        this->game_board->get_player(PLAYER2),
                        this->game_board->get_player_captures(PLAYER1),
                        this->game_board->get_player_captures(PLAYER2),
                        this->game_board->get_player_EP(PLAYER1),
                        this->game_board->get_player_EP(PLAYER2),
                        (this->game_board->get_game_path()).c_str(),
                        (this->game_board->get_redo_path()).c_str()
                    );

                this->game_board->update_board();
            }
        );

        QObject::connect(redo_button, &QPushButton::clicked, [=]()
            {
                redoLastMove(
                        this->game_board->get_board_ptr(),
                        this->game_board->get_player(PLAYER1),
                        this->game_board->get_player(PLAYER2),
                        this->game_board->get_player_captures(PLAYER1),
                        this->game_board->get_player_captures(PLAYER2),
                        this->game_board->get_player_EP(PLAYER1),
                        this->game_board->get_player_EP(PLAYER2),
                        (this->game_board->get_game_path()).c_str(),
                        (this->game_board->get_redo_path()).c_str()
                    );

                this->game_board->update_board();
            }
        );

        box->setSpacing(0);
        box->setContentsMargins(0, 0, 0, 0);
        
        box->addStretch(1);

        box->addWidget(undo_button);
        box->addWidget(redo_button);

        undo_button->setVisible( fs::exists(this->game_board->get_game_path()) && fs::file_size(this->game_board->get_game_path()) > 0 );
        redo_button->setVisible( fs::exists(this->game_board->get_redo_path()) && fs::file_size(this->game_board->get_redo_path()) > 0 );

        box->addSpacing(10);
    }


    void GInterface::add_captures(QVBoxLayout *ply_data, QLabel *ply_msg, Captured *ply_captures, bool redo_flag)
    {
        QHBoxLayout *msg_box { new QHBoxLayout() };

        msg_box->addWidget(ply_msg);

        if (redo_flag) this->add_redo_undo(msg_box);
        
        ply_data->addLayout(msg_box);

        QGridLayout *gl = new QGridLayout();

        gl->setSpacing(0);
        gl->setContentsMargins(0, 0, 0, 0);

        gl->setAlignment(Qt::AlignLeft);

        for (int i = 0; i < MAXCAPTNUM; i++)
        {
            QWidget *cell = new QWidget();
            cell->setFixedSize(CAPTURE_CELL_SIZE, CAPTURE_CELL_SIZE);

            if (i < ply_captures->captureCount) helpers::add_piece_to_cell(cell, ply_captures->capturedSymbols[i]);

            gl->addWidget(cell, 1, i);
        }

        ply_data->addLayout(gl);
        ply_data->addSpacing(15);

        return;
    }


    void GInterface::update()
    {
        QWidget *master_container { new QWidget() };

        QWidget *container_central { new QWidget() };
        QWidget *container_left { new QWidget() };

        QHBoxLayout *master_layout { new QHBoxLayout(master_container) };
        
        QGridLayout *gl { new QGridLayout(container_central) };

        QString label_style = "font-weight: bold; color: #f8e7bb;"
                    " font-size: 20px; margin-bottom: 5px;"
                    " padding-bottom: 2px;";

        QLabel *player2_msg { new QLabel("Player 2 (Black)") };
        QLabel *player1_msg { new QLabel("Player 1 (White)") };

        player2_msg->setStyleSheet(label_style);
        player1_msg->setStyleSheet(label_style);

        QVBoxLayout *ply2_data { new QVBoxLayout() };
        QVBoxLayout *ply1_data { new QVBoxLayout() };

        gl->setSpacing(0);
        gl->setContentsMargins(0, 0, 0, 0);

        int player_turn = this->game_board->get_player_turn();

        char ***board_ptr = this->game_board->get_board_ptr();

        Player *ply = this->game_board->get_player(player_turn);

        Captured *captures = NULL;        

        this->setCentralWidget(master_container);

        if (player_turn == PLAYER1)
        {
            captures = this->game_board->get_player_captures(1);

            gl->addLayout(ply2_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(ply1_data, 9, 0, 1, 8, Qt::AlignLeft);

            this->add_captures(ply1_data, player1_msg, this->game_board->get_player_captures(PLAYER1), true);
            this->add_captures(ply2_data, player2_msg, this->game_board->get_player_captures(PLAYER2), false);
        }
        else
        {
            captures = this->game_board->get_player_captures(2);

            gl->addLayout(ply1_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(ply2_data, 9, 0, 1, 8, Qt::AlignLeft);

            this->add_captures(ply1_data, player1_msg, this->game_board->get_player_captures(PLAYER1), false);
            this->add_captures(ply2_data, player2_msg, this->game_board->get_player_captures(PLAYER2), true);
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                BoardCell *cell = new BoardCell(i, j, this->game_board);

                QString color = ((i + j) % 2 == 0) ? "#f8e7bb" : "#004474";
                cell->setStyleSheet(QString(
                    "background-color: %1; border: none; margin: 0px;"
                ).arg(color));

                bool isCurrentPlayerPiece = (player_turn == PLAYER1) ?
                                            std::islower((*board_ptr)[i][j]) :
                                            std::isupper((*board_ptr)[i][j]);

                if (!isEmpty(*board_ptr, i, j))
                {
                    if (isCurrentPlayerPiece) helpers::add_piece_to_cell(cell, (*board_ptr)[i][j], i, j);
                    else helpers::add_piece_to_cell(cell, (*board_ptr)[i][j]);
                }

                int displayRow = (player_turn == PLAYER1) ? (i + 1) : ((7 - i) + 1);
                gl->addWidget(cell, displayRow, j);
            }
        }

        master_layout->setContentsMargins(20, 10, 20, 10);
        master_layout->setSpacing(0);

        master_layout->addStretch(1);

        this->add_left_menu(container_left);
        master_layout->addWidget(container_left, 0, Qt::AlignLeft);

        master_layout->addStretch(1);
        master_layout->addWidget(container_central, 0, Qt::AlignCenter);
        
        this->add_moves_view();
        master_layout->addStretch(1);

        return;
    }


    void GInterface::keyPressEvent(QKeyEvent *event) 
    {
        if (event->matches(QKeySequence::Quit) || event->matches(QKeySequence::Close)) this->close();
    }
}
