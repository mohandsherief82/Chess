#include "guiBoard.hpp"
#include "piecesIcon.hpp"
#include "boardCell.hpp"

#include <iostream>

#include <QLabel>


namespace helpers
{
    QString getIconPath(char piece)
    {
        QString colorStr = (std::islower(piece)) ? "white" : "black";
        QString typeStr;

        switch (std::tolower(piece))
        {
            case 'p': typeStr = "pawn"; break;
            case 'r': typeStr = "rook"; break;
            case 'n': typeStr = "knight"; break;
            case 'b': typeStr = "bishop"; break;
            case 'q': typeStr = "queen"; break;
            case 'k': typeStr = "king"; break;
            default: return QString("");
        }

        return QString(":/icons/%1_%2.svg").arg(colorStr).arg(typeStr);
    }


    void clear_items(QLayout *gl)
    {
        if (!gl) return;

        QLayoutItem *item;
        while ((item = gl->takeAt(0)) != nullptr)
        {
            if (QWidget *widget = item->widget()) widget->deleteLater();
            delete item;
        }
    }


    void add_piece_to_cell(QWidget *cell, char pieceChar)
    {
        QString iconPath = getIconPath(pieceChar);
        if (iconPath.isEmpty()) return;

        QVBoxLayout *layout = static_cast<QVBoxLayout*>(cell->layout());

        if (!layout)
        {
            layout = new QVBoxLayout(cell);
            layout->setContentsMargins(0, 0, 0, 0);
        }

        QLabel *pieceLabel = new QLabel(cell);

        QIcon icon(iconPath);
        QPixmap pixmap = icon.pixmap(QSize(PIECE_ICON_SIZE, PIECE_ICON_SIZE));

        pieceLabel->setPixmap(pixmap);

        pieceLabel->setScaledContents(true);
        pieceLabel->setAlignment(Qt::AlignCenter);

        layout->addWidget(pieceLabel);

        return;
    }


    void add_piece_to_cell(BoardCell *cell, char pieceChar, int row, int col)
    {
        QString iconPath = getIconPath(pieceChar);
        if (iconPath.isEmpty()) return;

        QVBoxLayout *layout = static_cast<QVBoxLayout*>(cell->layout());

        if (!layout)
        {
            layout = new QVBoxLayout(cell);
            layout->setContentsMargins(0, 0, 0, 0);
        }

        PieceColor color = (std::islower(pieceChar)) ? COLOR_WHITE: COLOR_BLACK;
        PieceType piece_type;

        switch (std::tolower(pieceChar))
        {
            case 'p': piece_type = PAWN; break;
            case 'n': piece_type = KNIGHT; break;
            case 'r': piece_type = ROOK; break;
            case 'b': piece_type = BISHOP; break;
            case 'q': piece_type = QUEEN; break;
            case 'k': piece_type = KING; break;
        }

        DraggablePiece *piece_label = new DraggablePiece(cell, row,
                                                        col, color, piece_type);
        piece_label->setObjectName(QString(pieceChar));
        
        QIcon icon(iconPath);
        QPixmap pixmap = icon.pixmap(QSize(PIECE_ICON_SIZE, PIECE_ICON_SIZE));

        piece_label->setPixmap(pixmap);
        piece_label->setScaledContents(true);
        
        piece_label->setAlignment(Qt::AlignCenter);

        layout->addWidget(piece_label);
    }
}


namespace Chess
{
    GInterface::GInterface(QString label_style, std::shared_ptr<Board> game_board)
        : player2_msg(new QLabel("Player 2 (Black)")),
          player1_msg(new QLabel("Player 1 (White)")),
          master_container(new QWidget()),
          container_central(new QWidget()),
          gl(nullptr),
          master_layout(nullptr),
          ply2_data(new QVBoxLayout()),
          ply1_data(new QVBoxLayout()),
          game_board(game_board)
    {
        this->setStyleSheet("background-color: #0A1118;");
        this->setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());

        this->player2_msg->setStyleSheet(label_style);
        this->player1_msg->setStyleSheet(label_style);

        this->gl = new QGridLayout(container_central);

        this->master_layout = new QVBoxLayout(this->master_container);

        this->master_layout->setContentsMargins(20, 10, 20, 10);
        this->master_layout->setSpacing(5);

        this->ply1_data->addLayout(gl);
        this->ply2_data->addLayout(gl);

        this->master_layout->addWidget(container_central, 0, Qt::AlignCenter);
    }


    void GInterface::add_captures(int ply_num, Captured *ply_captures)
    {
        QVBoxLayout *&capture_box = (ply_num == PLAYER1) ? ply1_data : ply2_data;
        QLabel *ply_label = (ply_num == PLAYER1) ? player1_msg : player2_msg;

        capture_box->addWidget(ply_label);

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

        capture_box->addLayout(gl);
        capture_box->addSpacing(15);

        return;
    }


    void GInterface::update()
    {
        this->gl->setSpacing(0);
        this->gl->setContentsMargins(0, 0, 0, 0);

        int player_turn = this->game_board->get_player_turn();
        char ***board_ptr = this->game_board->get_board_ptr();
        Player *ply = this->game_board->get_player(player_turn);
        Captured *captures = NULL;

        this->add_captures(PLAYER1, this->game_board->get_player_captures(PLAYER1));
        this->add_captures(PLAYER2, this->game_board->get_player_captures(PLAYER2));

        if (player_turn == PLAYER1)
        {
            captures = this->game_board->get_player_captures(1);
            gl->addLayout(this->ply2_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(this->ply1_data, 9, 0, 1, 8, Qt::AlignLeft);
        }
        else
        {
            captures = this->game_board->get_player_captures(2);
            gl->addLayout(this->ply1_data, 0, 0, 1, 8, Qt::AlignLeft);
            gl->addLayout(this->ply2_data, 9, 0, 1, 8, Qt::AlignLeft);
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                // Use our custom class instead of QWidget
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

        this->setCentralWidget(this->master_container);

        return;
    }

    void GInterface::keyPressEvent(QKeyEvent *event) 
    {
        if (event->matches(QKeySequence::Quit) || event->matches(QKeySequence::Close)) this->close();
    }
}
