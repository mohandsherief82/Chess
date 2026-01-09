#pragma once
#include <QLabel>
#include <QMouseEvent>

class DraggablePiece : public QLabel
{
    Q_OBJECT // Added for Qt Meta-Object support
    public:
        DraggablePiece(QWidget *parent = nullptr);

    protected:
        void mousePressEvent(QMouseEvent *event) override;
};
