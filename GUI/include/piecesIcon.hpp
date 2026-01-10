#pragma once
#include <QLabel>
#include <QMouseEvent>

class DraggablePiece : public QLabel
{
    Q_OBJECT // Added for Qt Meta-Object support
    public:
        DraggablePiece(QWidget *parent = nullptr);
        DraggablePiece(int row_pos, int col_pos);
        int col_pos, row_pos;
    protected:
        void mousePressEvent(QMouseEvent *event) override;
};
