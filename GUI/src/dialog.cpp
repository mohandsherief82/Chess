#include "dialog.hpp"

PersistentDialog::PersistentDialog(QWidget *parent) : QDialog(parent) 
{
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    
    setModal(true);
}

void PersistentDialog::closeEvent(QCloseEvent *event) 
{
    event->ignore();
}

void PersistentDialog::keyPressEvent(QKeyEvent *event) 
{
    if (event->key() == Qt::Key_Escape) event->accept();
    else QDialog::keyPressEvent(event);
}