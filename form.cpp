#include "form.h"
#include "ui_form.h"
#include <QSerialPort>
#include <QStringList>
#include <mainwindow.h>
#include <QSerialPortInfo>



Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    //串口一秒更新一次串口号
    time = new QTimer;
    connect(time,&QTimer::timeout,this,&Form::port);
    time->start(1000);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}


Form::~Form()
{
    delete ui;
}



//确定之后将下拉框的参数传至主界面
void Form::on_ok_clicked()
{

    emit sendData(ui->baud->currentIndex(),ui->stop->currentIndex(),
                  ui->datab->currentIndex(),ui->parity->currentIndex());
    this->close();
}



//点击cancel关闭
void Form::on_cancel_clicked()
{

    this->close();
}



//串口设置界面的串口更新
void Form::port(){
    // 可使用的串口赋值给newPortStringList
    QStringList newPortStringList;
    static QStringList oldPortStringList; // 添加static关键字
    const auto infos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : infos){
        newPortStringList.append(info.portName());}
    // 把新串口加入portbox
    if (newPortStringList.size() != oldPortStringList.size())
    {
        oldPortStringList = newPortStringList;
        ui->port->clear();
        ui->port->addItems(oldPortStringList);
    }
}
