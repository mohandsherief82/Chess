#include <QApplication>
#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QScreen>

#include "guiBoard.hpp"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QMainWindow main_window;

    QWidget *container = new QWidget();
    
    QGridLayout *main_grid = new QGridLayout(container);

    QTextEdit *text = new QTextEdit{"Hello"};

    // Set Styles
    create_board(main_grid);

    // Resize all objects
    main_window.setFixedSize(QGuiApplication::primaryScreen()->availableGeometry().size());

    // Add objects to main window
    main_window.setCentralWidget(container);

    main_window.show();

    return app.exec();
}
