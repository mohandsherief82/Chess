#pragma once

#include <QDialog>
#include <QCloseEvent>
#include <QKeyEvent>

class PersistentDialog : public QDialog 
{
    Q_OBJECT
    public:
        explicit PersistentDialog(QWidget *parent = nullptr);
        ~PersistentDialog() = default;

    protected:
        void closeEvent(QCloseEvent *event) override;

        void keyPressEvent(QKeyEvent *event) override;
};
