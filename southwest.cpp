
#include "southwest.h"
#include "ui_southwest.h"
#include <QPainter>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QTimer>

Southwest::Southwest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Southwest)
{



    ui->setupUi(this);


}

Southwest::~Southwest()
{
    delete ui;
}


//void Southwest::handleCheckBoxStateChanged()
//{
//    if (MainWindow->english->isChecked()){

////        if (ui->openserialbtn->text() == "打开串口") {
////                ui->openserialbtn->setText(tr("OpenCom"));
////            } else if (ui->openserialbtn->text() == "关闭串口") {
////                ui->openserialbtn->setText(tr("CloseCom"));
//            }


//    else
//    {

//    }
//     qapp->installTranslator(&tran);
//}


void Southwest::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
                if (ui->openserialbtn->text() == "打开串口") {
                        ui->openserialbtn->setText("OpenCom");
                    } else if (ui->openserialbtn->text() == "关闭串口") {
                        ui->openserialbtn->setText("CloseCom");
                    }else if (ui->openserialbtn->text() == "OpenCom") {
                    ui->openserialbtn->setText("打开串口");
                }else if (ui->openserialbtn->text() == "CloseCom") {
                    ui->openserialbtn->setText("关闭串口");
                }
                ui->moreset->setText(tr("更多串口设置"));
                ui->label->setText(tr("端口号："));
                ui->label_2->setText(tr("波特率："));
                ui->sendbtn->setText(tr("发送"));


//            ui->retranslateUi(this); // 重新翻译用户界面
            break;
        default:
            break;

}
}

//为分区添加灰色边框
void Southwest::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawRect(0, 0, width() - 1, height() - 1);
    QWidget::paintEvent(event);
}






