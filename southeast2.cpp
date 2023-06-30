#include "southeast2.h"
#include "ui_southeast2.h"
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QtSerialPort/QSerialPort>

Southeast2::Southeast2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Southeast2)
{
    ui->setupUi(this);
}



Southeast2::~Southeast2()
{
    delete ui;
}



//为分区添加灰色边框
void Southeast2::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawRect(0,0,width()-1,height()-1);
}
