#include "guiBoard.hpp"
#include "piecesIcon.hpp"
#include "boardCell.hpp"

#include <iostream>
#include <array>
#include <fstream>

#include <QLabel>
#include <QScrollArea>


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
                hLayout->addSpacing(95); 
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


    void GInterface::add_captures(QVBoxLayout *ply_data, QLabel *ply_msg, Captured *ply_captures)
    {
        ply_data->addWidget(ply_msg);

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

        this->add_captures(ply1_data, player1_msg, this->game_board->get_player_captures(PLAYER1));
        this->add_captures(ply2_data, player2_msg, this->game_board->get_player_captures(PLAYER2));

        this->setCentralWidget(master_container);

        if (player_turn == PLAYER1)
        {
            captures = this->game_board->get_player_captures(1);
            gl->addLayout(ply2_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(ply1_data, 9, 0, 1, 8, Qt::AlignLeft);
        }
        else
        {
            captures = this->game_board->get_player_captures(2);
            gl->addLayout(ply1_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(ply2_data, 9, 0, 1, 8, Qt::AlignLeft);
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
