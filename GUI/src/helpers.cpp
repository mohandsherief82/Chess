#include "helpers.hpp"

namespace helpers
{
    std::string get_filename_without_ext(const std::string& full_path)
    {
        return fs::path(full_path).stem().string();
    }

    std::string load_menu(QWidget* parent, const std::string& folder_path, const std::string& exclude_name_no_ext) 
    {
        QDialog dialog(parent);

        dialog.setWindowTitle("Load Game");
        dialog.setFixedSize(350, 450);

        QVBoxLayout* layout = new QVBoxLayout(&dialog);
        layout->addWidget(new QLabel("Select a save file to load:"));

        QListWidget* list_widget = new QListWidget(&dialog);
        
        try 
        {
            if (fs::exists(folder_path) && fs::is_directory(folder_path)) 
            {
                for (const auto& entry : fs::directory_iterator(folder_path)) 
                {
                    if (entry.is_regular_file() && entry.path().extension() == ".bin") 
                    {
                        std::string stem = entry.path().stem().string();
                        if (stem != exclude_name_no_ext)
                        {
                            QListWidgetItem* item = new QListWidgetItem(QString::fromStdString(stem));
                            item->setData(Qt::UserRole, QString::fromStdString(entry.path().filename().string()));
                            list_widget->addItem(item);
                        }
                    }
                }
            }
        } 
        catch (const fs::filesystem_error& e) 
        { 
            layout->addWidget(new QLabel("Error accessing folder!")); 
        }

        bool has_files = list_widget->count() > 0;
        
        if (!has_files) list_widget->addItem("No other .bin files found.");
        else
        {
            list_widget->sortItems(Qt::AscendingOrder);
            list_widget->setCurrentRow(0);
        }

        layout->addWidget(list_widget);

        QPushButton* load_btn = new QPushButton("Load Selection", &dialog);
        load_btn->setEnabled(has_files);
        layout->addWidget(load_btn);

        std::string final_path = "";
        
        auto on_confirm = [&]() 
        {
            if (list_widget->currentItem() && has_files) 
            {
                QString actual_filename = list_widget->currentItem()->data(Qt::UserRole).toString();
                fs::path p = fs::path(folder_path) / actual_filename.toStdString();
                
                final_path = p.string();
                dialog.accept();
            }
        };

        QObject::connect(load_btn, &QPushButton::clicked, on_confirm);
        QObject::connect(list_widget, &QListWidget::itemDoubleClicked, on_confirm);

        if (dialog.exec() == QDialog::Accepted) return final_path;

        return ""; 
    }


    QString getIconPath(char piece)
    {
        QString colorStr = (std::islower(piece)) ? "white" : "black";
        QString type_str;

        switch (std::tolower(piece))
        {
            case 'p': type_str = "pawn"; break;
            case 'r': type_str = "rook"; break;
            case 'n': type_str = "knight"; break;
            case 'b': type_str = "bishop"; break;
            case 'q': type_str = "queen"; break;
            case 'k': type_str = "king"; break;

            case 'd': return QString(":/icons/redo.svg");
            case 'u': return QString(":/icons/undo.svg");
            case 's': return QString(":/icons/save.svg");
            case 'l': return QString(":/icons/load.svg");
            case 'a': return QString(":/icons/start.svg");
            case 'g': return QString(":/icons/resign.svg");
            case 'x': return QString(":/icons/exit.svg");

            default: return QString("");
        }

        return QString(":/icons/%1_%2.svg").arg(colorStr).arg(type_str);
    }


    std::vector<Move> read_moves(std::string path)
    {
        std::vector<Move> last_moves(MOVES_READ);

        for (int i = 0; i < MOVES_READ; i++)
        {
            last_moves[i].rowPrev = -1;
            last_moves[i].colPrev = -1;

            last_moves[i].rowNext = -1;
            last_moves[i].colNext = -1;
            
            last_moves[i].symbol = 0;
        }

        std::ifstream file(path, std::ios::binary | std::ios::ate);

        if (!file.is_open())
        {
            std::ofstream create_file(path, std::ios::binary);
            return last_moves;
        }

        std::streamsize file_size = file.tellg();
        int total_moves = static_cast<int>(file_size / sizeof(Move));

        int count_to_read = (total_moves < MOVES_READ) ? total_moves : MOVES_READ;

        if (count_to_read > 0)
        {
            file.seekg(-(static_cast<std::streamoff>(count_to_read) * sizeof(Move)), std::ios::end);
            file.read(reinterpret_cast<char*>(last_moves.data()), count_to_read * sizeof(Move));
        }

        file.close();

        if (total_moves % 2 != 0 && count_to_read != 0 && total_moves > MOVES_READ)
        {
            last_moves.erase(last_moves.begin());

            last_moves[count_to_read - 1].rowPrev = -1;
            last_moves[count_to_read - 1].colPrev = -1;

            last_moves[count_to_read - 1].rowNext = -1;
            last_moves[count_to_read - 1].colNext = -1;
            
            last_moves[count_to_read - 1].symbol = 0;
        }

        return last_moves;
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
    
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(cell->layout());
    
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
    
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(cell->layout());
    
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


    char promotion_menu(QWidget* parent, PieceColor player_color)
    {
        PersistentDialog dialog(parent);
        
        dialog.setWindowTitle("Pawn Promotion");
        dialog.setStyleSheet("background-color: #0A1118; border: 2px solid #f8e7bb;");

        QVBoxLayout* layout = new QVBoxLayout(&dialog);

        QLabel *title = new QLabel("Select your promotion piece:");
        title->setAlignment(Qt::AlignCenter);
        title->setStyleSheet("color: #f8e7bb; font-weight: bold; font-size: 18px; margin-top: 10px; border: none;");
        layout->addWidget(title);

        QListWidget* list_widget = new QListWidget(&dialog);
        
        QString list_style = "QListWidget { background-color: #111c28; border: 1px solid #f8e7bb; color: #f8e7bb; font-size: 16px; outline: none; }"
                             "QListWidget::item { padding: 15px; border-bottom: 1px solid #2a3f55; }"
                             "QListWidget::item:selected { background-color: #1c2b3a; color: #ffffff; }";
        
        list_widget->setStyleSheet(list_style);
        list_widget->setIconSize(QSize(40, 40));
        
        std::vector<PromoOption> options = {
            { (player_color == COLOR_WHITE) ? 'q' : 'Q', "  Queen" },
            { (player_color == COLOR_WHITE) ? 'r' : 'R', "  Rook" },
            { (player_color == COLOR_WHITE) ? 'b' : 'B', "  Bishop" },
            { (player_color == COLOR_WHITE) ? 'n' : 'N', "  Knight" }
        };

        for (const auto& opt : options)
        {
            QListWidgetItem* item = new QListWidgetItem(QIcon(getIconPath(opt.symbol)), opt.name);
            item->setData(Qt::UserRole, QVariant(opt.symbol));
            list_widget->addItem(item);
        }

        list_widget->setCurrentRow(0);
        layout->addWidget(list_widget);

        QPushButton* select_btn = new QPushButton("Confirm Promotion", &dialog);

        QString btn_style = "QPushButton { color: #f8e7bb; background-color: #1c2b3a; border: 1px solid #f8e7bb; padding: 12px; font-size: 14px; font-weight: bold; }"
                            "QPushButton:hover { background-color: #2a3f55; }"
                            "QPushButton:pressed { background-color: #0A1118; }";

        select_btn->setStyleSheet(btn_style);
        layout->addWidget(select_btn);

        char selected_symbol = (player_color == COLOR_WHITE) ? 'q' : 'Q';

        auto on_confirm = [&]() 
        {
            if (list_widget->currentItem()) 
            {
                selected_symbol = (char)list_widget->currentItem()->data(Qt::UserRole).toInt();
                dialog.accept();
            }
        };

        QObject::connect(select_btn, &QPushButton::clicked, on_confirm);
        QObject::connect(list_widget, &QListWidget::itemDoubleClicked, on_confirm);

        dialog.exec();

        return selected_symbol;
    }
}
