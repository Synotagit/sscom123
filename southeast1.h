#ifndef SOUTHEAST1_H
#define SOUTHEAST1_H

#include <QWidget>
#include <QTranslator>
#include "mainwindow.h"

namespace Ui {
class Southeast1;
}

class Southeast1 : public QWidget
{
    Q_OBJECT

public:
    explicit Southeast1(QWidget *parent = nullptr);
    ~Southeast1();
    void handleCheckBoxStateChanged();

private:
    Ui::Southeast1 *ui;
    MainWindow *MainWindow;
    QTranslator tran;
    QApplication *qapp;

protected:
    void paintEvent(QPaintEvent *event) override;
    void changeEvent(QEvent *e) override;
};

#endif // SOUTHEAST1_H
