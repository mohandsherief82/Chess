
#include "piecesIcon.hpp"

DraggablePiece::DraggablePiece(QWidget *parent, int row_pos, int col_pos, PieceColor color, PieceType symbol) 
    : QLabel(parent)
{
    this->color = color;
    this->symbol = symbol;

    this->row_pos = row_pos;
    this->col_pos = col_pos;
    
    this->setAttribute(Qt::WA_TransparentForMouseEvents, false);
}


void DraggablePiece::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        this->drag_start_pos = event->pos();

    QLabel::mousePressEvent(event);
}


void DraggablePiece::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton)) return;

    if ((event->pos() - drag_start_pos).manhattanLength() < QApplication::startDragDistance()) {
        return;
    }

    if (!pixmap().isNull())
    {
        QMimeData *mimeData = new QMimeData;

        mimeData->setText(this->objectName()); 

        QDrag *drag = new QDrag(this);

        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap());
        
        drag->setHotSpot(drag_start_pos); 

        this->hide();

        if (drag->exec(Qt::MoveAction) != Qt::MoveAction) 
            this->show();
    }
}
