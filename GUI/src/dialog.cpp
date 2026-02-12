
#include "dialog.hpp"

PersistentDialog::PersistentDialog(QWidget *parent) : QDialog(parent) 
{
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);

    this->setMinimumSize(500, 500);
    this->setStyleSheet("QDialog { background-color: #0f1a24; border: 2px solid #f8e7bb; } "
                        "QLabel { color: #f8e7bb; }");
    
    setModal(true);
}

void PersistentDialog::closeEvent(QCloseEvent *event) 
{
    event->ignore();
}

void PersistentDialog::keyPressEvent(QKeyEvent *event) 
{
    if (event->key() == Qt::Key_Escape) event->ignore(); // Don't allow Escape to close it either
    else QDialog::keyPressEvent(event);
}