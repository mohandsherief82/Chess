
#include "helpers.hpp"

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


    std::array<Move, MOVES_READ> read_moves(std::string path) 
    {
        std::array<Move, MOVES_READ> lastMoves = {};

        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open()) return lastMoves;

        std::streamsize fileSize = file.tellg();

        int totalMoves = fileSize / sizeof(Move);
        int countToRead = (totalMoves < MOVES_READ) ? totalMoves : MOVES_READ;

        if (countToRead > 0) 
        {
            file.seekg(-(static_cast<std::streamoff>(countToRead) * sizeof(Move)), std::ios::end);
            
            file.read(reinterpret_cast<char*>(lastMoves.data()), countToRead * sizeof(Move));
        }

        file.close();

        return lastMoves;
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

    std::string get_formatted_time()
    {
        auto now = std::chrono::system_clock::now();

        std::time_t now_c = std::chrono::system_clock::to_time_t(now);

        std::tm now_tm = *std::localtime(&now_c);

        std::stringstream ss;
        
        ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S") << ".bin";

        return ss.str();
    }
}
