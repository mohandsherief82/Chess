#include "guiBoard.hpp"
#include "piecesIcon.hpp"

QString getIconPath(char piece) 
{
    QString colorStr = (std::islower(piece)) ? "white" : "black";
    QString typeStr;

    switch (std::tolower(piece)) 
    {
        case 'p':   
            typeStr = "pawn";   
            break;
        case 'r':   
            typeStr = "rook";   
            break;
        case 'n': 
            typeStr = "knight"; 
            break;
        case 'b': 
            typeStr = "bishop"; 
            break;
        case 'q':  
            typeStr = "queen";  
            break;
        case 'k':   
            typeStr = "king";   
            break;
        default:
            return QString("");
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


void add_piece_to_cell(QWidget *cell, char pieceChar, int row, int col) 
{
    QString iconPath = getIconPath(pieceChar);
    if (iconPath.isEmpty()) return;

    // Use a layout if it doesn't already have one
    QVBoxLayout *layout = static_cast<QVBoxLayout*>(cell->layout());

    if (!layout) 
    {
        layout = new QVBoxLayout(cell);
        layout->setContentsMargins(5, 5, 5, 5);
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

    // Use our new DraggablePiece class
    DraggablePiece *pieceLabel = new DraggablePiece(row, col, color, piece_type);
    
    // Store the pieceChar in the objectName so we know what we are dragging
    pieceLabel->setObjectName(QString(pieceChar));
    
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(QSize(60, 60));
    
    pieceLabel->setPixmap(pixmap);
    pieceLabel->setScaledContents(true);
    pieceLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(pieceLabel);
}


void display_board(QMainWindow *main_window, char **board
        , QLabel *player1_label, QLabel *player2_label, int player_turn) 
{
    QWidget *master_container = new QWidget();
    QVBoxLayout *master_layout = new QVBoxLayout(master_container);

    master_layout->setContentsMargins(20, 20, 20, 20);
    master_layout->setSpacing(10);

    QWidget *container_top = new QWidget();
    QWidget *container_central = new QWidget();
    QWidget *container_bottom = new QWidget();

    QGridLayout *gl = new QGridLayout(container_central);

    clear_items(gl);

    gl->setSpacing(0);
    gl->setContentsMargins(0, 0, 0, 0);
    gl->setAlignment(Qt::AlignCenter);

    if (player_turn == 1) 
    {
        gl->addWidget(player2_label, 0, 0, 1, 8, Qt::AlignLeft);
        for (int i = 0; i < 8; i++) 
        {
            for (int j = 0; j < 8; j++) 
            {
                QWidget *cell = new QWidget();
                cell->setFixedSize(70, 70);

                QString color = ((i + j) % 2 == 0) ? "#f8e7bb" : "#004474";

                cell->setStyleSheet(QString(
                    "background-color: %1;"
                    "border: none;"
                    "margin: 0px;"
                ).arg(color));

                if (board[i][j] != '-' && board[i][j] != '.') add_piece_to_cell(cell, board[i][j], i, j);

                gl->addWidget(cell, i + 1, j);
            }
        }

        gl->addWidget(player1_label, 9, 0, 1, 8, Qt::AlignLeft);
    }
    else
    {
        gl->addWidget(player1_label, 0, 0, 1, 8, Qt::AlignLeft);
        for (int i = 0; i < 8; i++) 
        {
            for (int j = 0; j < 8; j++) 
            {
                QWidget *cell = new QWidget();
                cell->setFixedSize(70, 70);

                QString color = ((i + j) % 2 == 0) ? "#f8e7bb" : "#004474";

                cell->setStyleSheet(QString(
                    "background-color: %1;"
                    "border: none;"
                    "margin: 0px;"
                ).arg(color));

                if (board[i][j] != '-' && board[i][j] != '.') add_piece_to_cell(cell, board[i][j], i, j);

                gl->addWidget(cell, (7 - i) + 1, j);
            }
        }

        gl->addWidget(player2_label, 9, 0, 1, 8, Qt::AlignLeft);
    }

    // Add widgets to the main layout
    master_layout->addWidget(container_top);
    master_layout->addWidget(container_central); 
    master_layout->addWidget(container_bottom);

    main_window->setCentralWidget(master_container);

    return;
}
