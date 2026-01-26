#pragma once
#include "interfaces.hpp"

#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QTimer>

#include <memory>

extern "C"
{
    #include "pawnMoves.h"
    #include "rookMoves.h"
    #include "knightMoves.h"
    #include "bishopMoves.h"
    #include "queenMoves.h"
    #include "kingMoves.h"
    #include "saveGame.h"
    
    extern char *loadPath;
}

class BoardCell : public QWidget 
{
    Q_OBJECT
    private:
        int row_pos, col_pos;
        std::shared_ptr<Chess::Board> game_board = nullptr;
    public:
        BoardCell(int r, int c, std::shared_ptr<Chess::Board> game_board, QWidget *parent = nullptr);
        int getRow() { return row_pos; }
        int getCol() { return col_pos; }
    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
};
