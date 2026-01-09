#include "piecesIcon.hpp"

#include <QDrag>
#include <QMimeData>

// 1. Removed "= nullptr" here
DraggablePiece::DraggablePiece(QWidget *parent) : QLabel(parent) 
{}

// 2. Removed "override" and the semicolon before the brace
void DraggablePiece::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && !pixmap().isNull())
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(this->objectName()); 

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(pixmap());
        drag->setHotSpot(event->pos()); 

        this->hide();

        if (drag->exec(Qt::MoveAction) != Qt::MoveAction) this->show();
    }
}