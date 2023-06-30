#ifndef SOUTHEAST_H
#define SOUTHEAST_H

#include <QWidget>
#include <QTranslator>
#include "mainwindow.h"

namespace Ui {
class Southeast;
}

class Southeast : public QWidget
{
    Q_OBJECT

public:
    explicit Southeast(QWidget *parent = nullptr);
    ~Southeast();

    void handleCheckBoxStateChanged();

private:
    Ui::Southeast *ui;
    MainWindow *MainWindow;
    QTranslator tran;
    QApplication *qapp;
protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *e) override;
};

#endif // SOUTHEAST_H
