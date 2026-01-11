#include "guiBoard.hpp"
#include "piecesIcon.hpp"
#include "boardCell.hpp"

#include <QLabel>

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
        default:  QString("");
    }

    return QString(":/icons/%1_%2.svg").arg(colorStr).arg(typeStr);
}


void clear_items(QGridLayout *gl) 
{
    if (!gl) return;

    QLayoutItem *item;
    while ((item = gl->takeAt(0)) != nullptr) 
    {
        if (QWidget *widget = item->widget()) 
        {
            widget->deleteLater(); 
        }
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
    QPixmap pixmap = icon.pixmap(QSize(60, 60));

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

    DraggablePiece *pieceLabel = new DraggablePiece(cell, row, col, color, piece_type);
    
    pieceLabel->setObjectName(QString(pieceChar));
    
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(QSize(60, 60));
    
    pieceLabel->setPixmap(pixmap);
    pieceLabel->setScaledContents(true);
    pieceLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(pieceLabel);
}


void add_captures(QVBoxLayout *capture_box, QLabel *ply_label, Captured *ply_captures)
{
    capture_box->addWidget(ply_label);

    QGridLayout *gl = new QGridLayout();

    gl->setSpacing(0);
    gl->setContentsMargins(0, 0, 0, 0);

    gl->setAlignment(Qt::AlignLeft);

    for (int i = 0; i < MAXCAPTNUM; i++)
    {
        QWidget *cell = new QWidget();

        cell->setFixedSize(30, 30);

        if (i < ply_captures->captureCount) add_piece_to_cell(cell, ply_captures->capturedSymbols[i]);

        gl->addWidget(cell, 1, i);
    }

    capture_box->addLayout(gl);
    capture_box->addSpacing(15);

    return;
}


void display_board(QMainWindow *main_window, char **board, Captured *ply1_captures,
     Captured *ply2_captures, int player_turn)
{
    QLabel *player2_msg = new QLabel("Player 2 (Black)");
    QLabel *player1_msg = new QLabel("Player 1 (White)");
    
    QString label_style = "font-weight: bold; color: #f8e7bb;"
                          " font-size: 20px; margin-bottom: 5px;"
                          " padding-bottom: 2px;";

    player2_msg->setStyleSheet(label_style); 
    player1_msg->setStyleSheet(label_style);

    QWidget *master_container = new QWidget();
    
    QVBoxLayout *master_layout = new QVBoxLayout(master_container);
    master_layout->setContentsMargins(20, 10, 20, 10);
    master_layout->setSpacing(5);

    QWidget *container_central = new QWidget();
    QGridLayout *gl = new QGridLayout(container_central);

    gl->setSpacing(0);
    gl->setContentsMargins(0, 0, 0, 0);

    QVBoxLayout *ply1_data = new QVBoxLayout();
    QVBoxLayout *ply2_data = new QVBoxLayout();

    add_captures(ply1_data, player1_msg, ply1_captures);
    add_captures(ply2_data, player2_msg, ply2_captures);

    if (player_turn == 1) 
    {
        gl->addLayout(ply2_data, 0, 0, 1, 8, Qt::AlignLeft);
        gl->addLayout(ply1_data, 9, 0, 1, 8, Qt::AlignLeft);
    }
    else
    {
        gl->addLayout(ply1_data, 0, 0, 1, 8, Qt::AlignLeft);
        gl->addLayout(ply2_data, 9, 0, 1, 8, Qt::AlignLeft);
    }

    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            // Use our custom class instead of QWidget
            BoardCell *cell = new BoardCell(i, j, &board); 
            cell->setFixedSize(70, 70);

            QString color = ((i + j) % 2 == 0) ? "#f8e7bb" : "#004474";
            cell->setStyleSheet(QString(
                "background-color: %1; border: none; margin: 0px;"
            ).arg(color));

            bool isCurrentPlayerPiece = (player_turn == 1) ? 
                                        std::islower(board[i][j]) : 
                                        std::isupper(board[i][j]);

            if (!isEmpty(board, i, j))
            {
                if (isCurrentPlayerPiece) add_piece_to_cell(cell, board[i][j], i, j);
                else add_piece_to_cell(cell, board[i][j]);
            }  

            int displayRow = (player_turn == 1) ? (i + 1) : ((7 - i) + 1);
            gl->addWidget(cell, displayRow, j);
        }
    }

    master_layout->addWidget(container_central, 0, Qt::AlignCenter);

    main_window->setCentralWidget(master_container);

    return;
}
