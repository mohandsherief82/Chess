#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSpacerItem>

void display_start_window(QMainWindow *main_window)
{
    QWidget *master_container = new QWidget();

    QVBoxLayout *main_layout = new QVBoxLayout(master_container);
    QHBoxLayout *button_layout = new QHBoxLayout();
    
    QLabel *start_msg = new QLabel("Welcome To Chess");

    QPushButton *start_button = new QPushButton("Start a New Game");
    QPushButton *load_button = new QPushButton("Load a Game");

    // Unified Button Style
    QString button_style = 
        "QPushButton {"
        "   background-color: #004474;"
        "   color: #f8e7bb;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   border-radius: 8px;"
        "   padding: 20px;"
        "   min-width: 250px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0c7ace;"
        "   color: white;"
        "}";

    start_button->setStyleSheet(button_style);
    load_button->setStyleSheet(button_style);

    start_msg->setStyleSheet("font-weight: bold; color: #f8e7bb; font-size: 60px; margin-bottom: 20px;");
    start_msg->setAlignment(Qt::AlignCenter);

    // Build the Menu Stack
    main_layout->addStretch(2);
    main_layout->addWidget(start_msg);
    main_layout->addSpacing(40);
    
    button_layout->addStretch(1);
    button_layout->addWidget(start_button, 0, Qt::AlignCenter);
    button_layout->addSpacing(15);
    button_layout->addWidget(load_button, 0, Qt::AlignCenter);
    button_layout->addStretch(1);
    
    main_layout->addLayout(button_layout);
    main_layout->addStretch(3);

    main_window->setCentralWidget(master_container);

    main_window->setStyleSheet("background-color: #0A1118;");
}
