#include <QGridLayout>
#include <QWidget>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>

#include <cctype>

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
    }

    return QString(":/icons/icons/%1_%2.svg").arg(colorStr).arg(typeStr);
}


void add_piece_to_cell(QWidget *cell, char pieceChar) 
{
    QString iconPath = getIconPath(pieceChar);
    if (iconPath.isEmpty()) return;

    QVBoxLayout *layout = new QVBoxLayout(cell);
    
    layout->setContentsMargins(5, 5, 5, 5); 

    QLabel *pieceLabel = new QLabel();
    
    QIcon icon(iconPath);
    QPixmap pixmap = icon.pixmap(QSize(60, 60));
    
    pieceLabel->setPixmap(pixmap);
    pieceLabel->setScaledContents(true);
    pieceLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(pieceLabel);

    return;
}


void display_board(QGridLayout *gl, const char (&board)[8][8]) 
{
    if (!gl) return;

    QLayoutItem *item;
    while ((item = gl->takeAt(0)) != nullptr)
    {
        if (item->widget()) delete item->widget();
        delete item;
    }

    gl->setSpacing(0);
    gl->setContentsMargins(0, 0, 0, 0);
    gl->setAlignment(Qt::AlignCenter);

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

            if (board[i][j] != '-' && board[i][j] != '.') add_piece_to_cell(cell, board[i][j]);

            gl->addWidget(cell, i, j);
        }
    }
}
