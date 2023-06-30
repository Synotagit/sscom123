#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QSerialPort>
#include <QCloseEvent>
#include <QSerialPortInfo>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();

    void port();


signals:
    void sendData(int baud,int stop,int data,int pari);


private slots:
    void on_ok_clicked();

    void on_cancel_clicked();


private:
    Ui::Form *ui;
    QTimer *time;


};

#endif // FORM_H
