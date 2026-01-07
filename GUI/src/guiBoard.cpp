#include <QGridLayout>
#include <QPushButton>


void create_board(QGridLayout *gl) 
{
    if (!gl) return;

    gl->setSpacing(0);
    gl->setContentsMargins(0, 0, 0, 0);
    gl->setAlignment(Qt::AlignAbsolute);

    for (int i = 0; i < 8; i++) 
    {
        for (int j = 0; j < 8; j++) 
        {
            QPushButton *btn = new QPushButton();
            
            btn->setFixedSize(80, 80);

            QString color = ((i + j) % 2 == 0) ? "#f8e7bb" : "#004474";

            btn->setStyleSheet(QString(
                "background-color: %1;"
                "border: none;"
                "margin: 0px;"
            ).arg(color));

            gl->addWidget(btn, i, j);
        }
    }
}
