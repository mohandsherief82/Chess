#include <QWidget>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QGridLayout>
#include <QLabel>
#include <QDebug>

extern "C"
{
    #include "pawnMoves.h"
    #include "rookMoves.h"
    #include "knightMoves.h"
    #include "bishopMoves.h"
    #include "queenMoves.h"
    #include "kingMoves.h"
}

class BoardCell : public QWidget 
{
    Q_OBJECT
    private:
        int row_pos, col_pos;
        char ***board_ptr;
    public:
        BoardCell(int r, int c, char ***board_ptr = nullptr, QWidget *parent = nullptr);
        int getRow() { return row_pos; }
        int getCol() { return col_pos; }
        char **getBoard() { return (board_ptr != nullptr) ? *board_ptr : nullptr; }
    protected:
        void dragEnterEvent(QDragEnterEvent *event) override;
        void dropEvent(QDropEvent *event) override;
        void dragMoveEvent(QDragMoveEvent *event) override;
};
