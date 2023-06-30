#ifndef SOUTHWEST_H
#define SOUTHWEST_H
#include <QtSerialPort/QSerialPort>
#include <QWidget>
#include <QTranslator>
#include "mainwindow.h"
class MainWindow;
namespace Ui {
class Southwest;
}

class Southwest : public QWidget
{
    Q_OBJECT

public:
    explicit Southwest(QWidget *parent = nullptr);
    ~Southwest();


//    void handleCheckBoxStateChanged();

private slots:


private:
    Ui::Southwest *ui;
    MainWindow *MainWindow;
    QTranslator tran;
    QApplication *qapp;


protected:
void paintEvent(QPaintEvent *event) override;
     void changeEvent(QEvent *e) override;


private slots:


signals:

};

#endif // SOUTHWEST_H
