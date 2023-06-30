#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "form.h"
#include "southwest.h"
#include <QtGui>
#include <QFile>
#include <QTime>
#include <QStyle>
#include <QDebug>
#include <QTimer>
#include <QScreen>
#include <QWindow>
#include <QScreen>
#include <QDateTime>
#include <QComboBox>
#include <ui_form.h>
#include <QListView>
#include <QMessageBox>
#include <QPushButton>
#include <QFileDialog>
#include <QTranslator>
#include <QStringList>
#include <QMessageBox>
#include <QPushButton>
#include <QApplication>
#include <QDesktopWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //初始化
    initpointer();
    loadSettingsFromIniFile();

    //信号与槽
    ssfunction();

}


MainWindow::~MainWindow()
{
    delete ui;
}





//初始化
void MainWindow::initpointer(){

    //隐藏进度栏
    ui->progressBar->hide();

    //新建串口
    serial = new QSerialPort;

    //发送数据定时器
    TimerSend = new QTimer(this);



    //定时发送定时器

    //定时器超时发送数据
    timeSend = new QTimer(this);
        connect(timeSend, &QTimer::timeout, this, [=](){
            if(openbtn->text() == "关闭串口"||openbtn->text() == "CloseCom")
            sendbtn();
            else{
                QMessageBox::warning(this,"警告","串口未打开");
               timesend->setChecked(false);
            }
        });

    //100ms读取一次数据
    rtimer = new QTimer;
    rtimer->start(10);

    //ok按键附上指针
    nwindow = new Form();
    ok = nwindow->findChild<QPushButton*>("ok");
    connect(ok, SIGNAL(clicked()), this, SLOT(okPush()));


    //将1区控件附上指针,并连接上槽函数
    Southwest* onepart = ui->widget_4;

    openbtn = onepart->findChild<QPushButton*>("openserialbtn");
    connect(openbtn, &QPushButton::clicked, this, &MainWindow::openserialbtn);

    senbtn = onepart->findChild<QPushButton*>("sendbtn");
    connect(senbtn,&QPushButton::clicked,this,&MainWindow::sendbtn);

    moresbtn = onepart->findChild<QPushButton*>("moreset");
    connect(moresbtn,&QPushButton::clicked,this,&MainWindow::moreset);

    choosebit = onepart->findChild<QComboBox*>("choosebit");
    connect(choosebit, SIGNAL(currentIndexChanged(int)), this, SLOT(modifybaud(int)));

    portbox = onepart->findChild<QComboBox*>("portbox");
    connect(portbox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(modifyCom(const QString&)));



    //将2区控件附上指针,并连接上槽函数
    Southeast* twopart = ui->widget;

    hexshow = twopart->findChild<QCheckBox*>("hexshow");

    label = twopart->findChild<QLabel*>("label");

    rectofile = twopart->findChild<QCheckBox*>("rectofile");

    timepack = twopart->findChild<QCheckBox*>("timepack");

    savedata = twopart->findChild<QPushButton*>("savedata");
    connect(savedata,&QPushButton::clicked,this,&MainWindow::savedatacao);

    latetime = twopart->findChild<QLineEdit*>("latetime");


    //将3区控件附上指针,并连接上槽函数

    Southeast1* threepart = ui->widget_2;

    hexsend = threepart->findChild<QCheckBox*>("hexsend");

    timesend = threepart->findChild<QCheckBox*>("timesend");

    exchange = threepart->findChild<QCheckBox*>("exchange");

    whatbit = threepart->findChild<QLineEdit*>("whatbit");

    whattime = threepart->findChild<QLineEdit*>("whattime");

    number = threepart->findChild<QComboBox*>("number");

    wei = threepart->findChild<QComboBox*>("wei");

    recog = threepart->findChild<QLineEdit*>("recog");

    di = threepart->findChild<QLabel*>("di");

    zijie = threepart->findChild<QLabel*>("zijie");

    zhi = threepart->findChild<QLabel*>("zhi");

    jiajiaoyan = threepart->findChild<QLabel*>("jiajiaoyan");


    //将4区控件附上指针,并连接上槽函数

    Southeast2* fourpart = ui->widget_3;

    sendedit = fourpart->findChild<QTextEdit*>("recedit");


    //将5区控件附上指针,并连接上槽函数

    Mid* fivepart = ui->widget_5;

    clearwin = fivepart->findChild<QPushButton*>("clearwin");
    connect(clearwin,&QPushButton::clicked,this,&MainWindow::clearwincao);

    openfile = fivepart->findChild<QPushButton*>("openfile");
    connect(openfile,&QPushButton::clicked,this,&MainWindow::openfilecao);

    sendfile = fivepart->findChild<QPushButton*>("sendfile");
    connect(sendfile,&QPushButton::clicked,this,&MainWindow::sendfilecao);

    filepath = fivepart->findChild<QLineEdit*>("filepath");

    stop = fivepart->findChild<QPushButton*>("stop");
    connect(stop,&QPushButton::clicked,this,&MainWindow::stopcao);

    clearsend = fivepart->findChild<QPushButton*>("clearsend");
    connect(clearsend,&QPushButton::clicked,this,&MainWindow::clearsendcao);

    zuiqian = fivepart->findChild<QCheckBox*>("zuiqian");

    english = fivepart->findChild<QCheckBox*>("english");

    savepara = fivepart->findChild<QPushButton*>("savepara");
    connect(savepara,&QPushButton::clicked,this,&MainWindow::saveparacao);

    expand = fivepart->findChild<QPushButton*>("expand");
    connect(expand,&QPushButton::clicked,this,&MainWindow::expandcao);

    widghide = fivepart->findChild<QPushButton*>("widghide");
    connect(widghide,&QPushButton::clicked,this,&MainWindow::widghidecao);


    //初始化串口
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts()){
        qDebug()<<info.portName()<<info.description();
        portbox->addItem(info.portName());
        QSerialPort serial(info);

        //测试串口是否空闲
        if(serial.open(QIODevice::ReadWrite)){
            portbox->addItem(info.portName());
            serial.close();
        }else{
            portbox->addItem(info.portName()+"(被占用)");
        }

        //1秒更新一次串口，更新串口下拉框
        timer = new QTimer;
        connect(timer,&QTimer::timeout,this,&MainWindow::UpdatePort);
        timer->start(1000);

}


//        //用单位内的点数判断缩放的大小
//        qreal beginwid = 698;                             //初始设置屏幕缩放100%时主窗口宽度为698
//        qreal beginhei = 582;                             //初始设置屏幕缩放100%时主窗口高度为582
//        qreal begindot = 96;                              //初始设置屏幕缩放100%时点数为96

//        QScreen *screen = qApp->primaryScreen();
//        qreal dotsPerInch = screen->logicalDotsPerInch(); //获取当前屏幕点数

        // 创建 QListView 对象并设置其属性
        listView = new QListView(number);
        listView->setMinimumWidth(200); // 设置下拉框的最小宽度

        QString styleSheet = "QListView { font-size: 9px; }"; // 自定义字体大小
        listView->setStyleSheet(styleSheet);

        // 设置 QComboBox 使用 QListView 作为下拉框
        number->setView(listView);


//        if(dotsPerInch!=begindot){                        //修改控件大小
//        filepath->setFixedSize(filepath->width()*dotsPerInch/begindot,filepath->height()*dotsPerInch/135);
//        whattime->setFixedSize(whattime->width()*dotsPerInch/begindot,whattime->height()*dotsPerInch/begindot);
//        latetime->setFixedSize(whattime->width(),whattime->height());
//        recog->setFixedSize(whattime->width(),whattime->height());
//        whatbit->setFixedSize(whatbit->width()*dotsPerInch/(begindot+1),whatbit->height()*dotsPerInch/(begindot+1));
//        number->setFixedSize(number->width()*dotsPerInch/(begindot+1),number->height()*dotsPerInch/(begindot+1));
//        wei->setFixedSize(wei->width()*dotsPerInch/(begindot+1),wei->height()*dotsPerInch/(begindot+1));

//        resize(beginwid*dotsPerInch/begindot, beginhei*dotsPerInch/begindot);
//}

        //状态栏
        QStatusBar *statusBar = new QStatusBar(this);
        setStatusBar(statusBar);

        QLabel *spacealabel = new QLabel(" ", this);
        statusBar->addPermanentWidget(spacealabel);

        // 创建用于显示发送数据字节的标签
        sendLabel = new QLabel("S: 0   ", this);
        statusBar->addPermanentWidget(sendLabel);

        // 创建用于显示接收数据字节的标签
        receiveLabel = new QLabel("R: 0   ", this);
        statusBar->addPermanentWidget(receiveLabel);

        // 创建用于显示串口信息状态的标签
        serialStatusLabel = new QLabel("COM1:已关闭 9600bps,8,1,None", this);
        statusBar->addPermanentWidget(serialStatusLabel);

        statusBar->addPermanentWidget(new QWidget(), 1);


        //hex显示颜色变化
        connect(hexshow, &QCheckBox::stateChanged, this, [this](int state) {
            if (state == Qt::Checked) {
                hexshow->setStyleSheet("QCheckBox { background-color: yellow; } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            } else {
                hexshow->setStyleSheet("QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            }
        });

        //接收数据到文件颜色变化
        connect(rectofile, &QCheckBox::stateChanged, this, [this](int state) {
            if (state == Qt::Checked) {
                rectofile->setStyleSheet("QCheckBox { background-color: yellow; } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");

            } else {
                rectofile->setStyleSheet("QCheckBox { color:rgb(0, 0, 255); } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            }
        });

        //hex发送颜色变化
        connect(hexsend, &QCheckBox::stateChanged, this, [this](int state) {
            if (state == Qt::Checked) {
                hexsend->setStyleSheet("QCheckBox { background-color: yellow; } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            } else {
                hexsend->setStyleSheet("QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            }
        });

        //定时发送
        connect(timesend, &QCheckBox::stateChanged, this, &MainWindow::timeSen);

        //时间戳和分包显示颜色变化
        connect(timepack, &QCheckBox::stateChanged, this, [this](int state) {
            if (state == Qt::Checked) {
                label->setStyleSheet("");
                timepack->setStyleSheet("QCheckBox { background-color: yellow; } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            } else {
                timepack->setStyleSheet("");
                label->setStyleSheet("color:rgb(213, 213, 213);");
                timepack->setStyleSheet("QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            }
        });

        //回车换行显示颜色变化
        connect(exchange, &QCheckBox::stateChanged, this, [this](int state) {

            if (state == Qt::Checked) {
                exchange->setStyleSheet("QCheckBox { background-color: yellow; } QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            } else {
                exchange->setStyleSheet("");
                exchange->setStyleSheet("QCheckBox::indicator { width: 12px; height: 12px; }QCheckBox { spacing: 5px; padding-left: 0px; }");
            }
        });

        //校验行行显示颜色变化
        connect(wei, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
            switch (index) {
            case 0:
                di->setStyleSheet("");
                zijie->setStyleSheet("");
                zhi->setStyleSheet("");
                jiajiaoyan->setStyleSheet("");
                whatbit->setStyleSheet("");
                number->setStyleSheet("");
                wei->setStyleSheet("");
                recog->setStyleSheet("");
                break;
            case 1:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 2:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 3:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 4:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 5:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 6:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;
            case 7:
                di->setStyleSheet("background-color: yellow;");
                zijie->setStyleSheet("background-color: yellow;");
                zhi->setStyleSheet("background-color: yellow;");
                jiajiaoyan->setStyleSheet("background-color: yellow;");
                whatbit->setStyleSheet("background-color: yellow;");
                number->setStyleSheet("background-color: yellow;");
                wei->setStyleSheet("background-color: yellow;");
                recog->setStyleSheet("background-color: yellow;");
                break;

            }
        });

        //窗口置顶勾选
        connect(zuiqian, &QCheckBox::stateChanged, this, [this](int state) {
            if (state == Qt::Unchecked) {
                // 取消置顶
                ::SetWindowPos(reinterpret_cast<HWND>(this->winId()), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
            } else if (state == Qt::Checked) {
                // 设置为置顶
                ::SetWindowPos(reinterpret_cast<HWND>(this->winId()), HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE);
            }
        });


        //english勾选
        connect(english, &QCheckBox::stateChanged, this, [this](int state) {

            if(state == Qt::Checked){
                tran.load(":/english.qm");
            }else if(state == Qt::Unchecked){
                tran.load(":/chinese.qm");
            }
            //发送event信号
            qApp->installTranslator(&tran);
        });

        //发送文件定时器
        FileSendTimer = new QTimer(this);
        connect(FileSendTimer,SIGNAL(timeout()),this,SLOT(timsend()));

        //设置初始分包超时时间和定时发送间隔时间
        latetime->setText("20");
        whattime->setText("900");


        statusBar->setStyleSheet("font-size: 13px; font-family: SimSun; font-weight: 470;");

        elapsedTime_.start();

}



//modbuscrc16校验显示
void MainWindow::showModbus(){
if(!hexsend->isChecked()){

 QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
 int p = string.length();
 int n = whatbit->text().toInt();  // 获取文本编辑框的内容
 int m = number->currentIndex();
 QString text = string.mid(n-1,p-m-n+1);  // 提取从第n个字符开始的子串

 // 将文本内容转换为十六进制字符串
 QString hexString;
 for (QChar c : text) {
     hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
 }

 // 将十六进制字符串转换为 QByteArray
 QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

 // 计算 CRC 校验
 uint16_t wcrc = ModbusCRC16(ba);
 QString crcString = QString("%1 %2")
         .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
         .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

 // 在 QLineEdit 控件中显示 CRC 码结果
 recog->setText(crcString);
}

else if (hexsend->isChecked()) {
    QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
    string.remove(' ');                  // 去除字符串中的空格
    int p = string.length();
    int n = whatbit->text().toInt();  // 获取从哪开始
    int m = number->currentIndex();
    QString hexString = string.mid(n*2-2 , p - (m*2-1) - (n*2-1));  // 提取从第n个字符开始的子串
    QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

    uint16_t wcrc = ModbusCRC16(ba);
    QString crcString = QString("%1 %2")
            .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
            .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

    // 在 QLineEdit 控件中显示 CRC 码结果
    recog->setText(crcString);



}
}



//crc16校验显示
void MainWindow::showCrc16(){
    if(!hexsend->isChecked()){

        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取文本编辑框的内容
        int m = number->currentIndex();
        QString text = string.mid(n-1,p-m-n+1);  // 提取从第n个字符开始的子串

     // 将文本内容转换为十六进制字符串
     QString hexString;
     for (QChar c : text) {
         hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
     }

     // 将十六进制字符串转换为 QByteArray
     QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

     // 计算 CRC 校验
     uint16_t wcrc = CCITTCRC16(ba);
     QString crcString = QString("%1 %2")
             .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
             .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

     // 在 QLineEdit 控件中显示 CRC 码结果
     recog->setText(crcString);
    }

    else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        string.remove(' ');                  // 去除字符串中的空格
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取从哪开始
        int m = number->currentIndex();
        QString hexString = string.mid(n*2-2 , p - (m*2-1) - (n*2-1));  // 提取从第n个字符开始的子串

        QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

        uint16_t wcrc = CCITTCRC16(ba);
        QString crcString = QString("%1 %2")
                .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
                .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

        // 在 QLineEdit 控件中显示 CRC 码结果
        recog->setText(crcString);
    }
}



//ADD16校验显示
void MainWindow::showADD16(){
    if(!hexsend->isChecked()){

        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取文本编辑框的内容
        int m = number->currentIndex();
        QString text = string.mid(n-1,p-m-n+1);  // 提取从第n个字符开始的子串
     // 将文本内容转换为十六进制字符串
     QString hexString;
     for (QChar c : text) {
         hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
     }

     // 将十六进制字符串转换为 QByteArray
     QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

     // 计算 CRC 校验
     uint16_t wcrc = ADD16(ba);
     QString crcString = QString("%1 %2")
             .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
             .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

     // 在 QLineEdit 控件中显示 CRC 码结果
     recog->setText(QString("%1 %2")
                        .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'))
                        .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0')));
    }

    else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        string.remove(' ');                  // 去除字符串中的空格
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取从哪开始
        int m = number->currentIndex();
        QString hexString = string.mid(n*2-2 , p - (m*2-1) - (n*2-1));  // 提取从第n个字符开始的子串
        QByteArray ba = QByteArray::fromHex(hexString.toLatin1());


        uint16_t wcrc = ADD16(ba);
        QString crcString = QString("%1 %2")
                .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0'))
                .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'));

        // 在 QLineEdit 控件中显示 CRC 码结果
        recog->setText(QString("%1 %2")
                           .arg(QString::number((wcrc >> 8) & 0xFF, 16).toUpper().rightJustified(2, '0'))
                           .arg(QString::number(wcrc & 0xFF, 16).toUpper().rightJustified(2, '0')));
  }
}



//0-ADD8校验显示
void MainWindow::showADD8_0(){
    if(!hexsend->isChecked()){

        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取文本编辑框的内容
        int m = number->currentIndex();
        QString text = string.mid(n-1,p-m-n+1);  // 提取从第n个字符开始的子串
        // 将文本内容转换为十六进制字符串
                QString hexString;
                for (QChar c : text) {
                    hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
                }

                // 将十六进制字符串转换为 QByteArray
                QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

                // 计算 0-ADD8 校验和
                unsigned char checksum = ADD8_0(ba);

                // 在 QLineEdit 控件中显示校验和结果
                recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
            }

    else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        string.remove(' ');                  // 去除字符串中的空格
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取从哪开始
        int m = number->currentIndex();
        // 提取从第n个字符开始的子串
                QString hexString = string.mid(n * 2 - 2, p - (m * 2 - 1) - (n * 2 - 1));

                // 将十六进制字符串转换为 QByteArray
                QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

                // 计算 0-ADD8 校验和
                unsigned char checksum = ADD8_0(ba);

                // 在 QLineEdit 控件中显示校验和结果
                recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
            }

}



//ADD8校验显示
void MainWindow::showADD8()
{
    if (!hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取文本编辑框的内容
        int m = number->currentIndex();
        QString text = string.mid(n - 1, p - m - n + 1);  // 提取从第n个字符开始的子串

        // 将文本内容转换为十六进制字符串
        QString hexString;
        for (QChar c : text) {
            hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
        }

        // 将十六进制字符串转换为 QByteArray
        QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

        // 计算 ADD8 校验和
        unsigned char checksum = ADD8(ba);

        // 在 QLineEdit 控件中显示校验和结果
        recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
    }
    else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        string.remove(' ');                  // 去除字符串中的空格
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取从哪开始
        int m = number->currentIndex();
        // 提取从第n个字符开始的子串
        QString hexString = string.mid(n * 2 - 2, p - m * 2 - n * 2 + 2);

        // 将十六进制字符串转换为 QByteArray
        QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

        // 计算 ADD8 校验和
        unsigned char checksum = ADD8(ba);

        // 在 QLineEdit 控件中显示校验和结果
        recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
    }
}



//XOR8校验显示
void MainWindow::showXOR8()
{
    if (!hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取文本编辑框的内容
        int m = number->currentIndex();
        QString text = string.mid(n - 1, p - m - n + 1);  // 提取从第n个字符开始的子串

        // 将文本内容转换为十六进制字符串
        QString hexString;
        for (QChar c : text) {
            hexString.append(QString("%1").arg(c.unicode(), 2, 16, QChar('0')));
        }

        // 将十六进制字符串转换为 QByteArray
        QByteArray ba = QByteArray::fromHex(hexString.toUtf8());

        // 计算 XOR8 校验和
        unsigned char checksum = XOR8(ba);

        // 在 QLineEdit 控件中显示校验和结果
        recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
    }
    else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();  // 获取文本编辑框的内容
        string.remove(' ');                  // 去除字符串中的空格
        int p = string.length();
        int n = whatbit->text().toInt();  // 获取从哪开始
        int m = number->currentIndex();
        // 提取从第n个字符开始的子串
        QString hexString = string.mid(n * 2 - 2, p - m * 2 - n * 2 + 2);

        // 将十六进制字符串转换为 QByteArray
        QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

        // 计算 XOR8 校验和
        unsigned char checksum = XOR8(ba);

        // 在 QLineEdit 控件中显示校验和结果
        recog->setText(QString("%1").arg(checksum, 2, 16, QChar('0')).toUpper());
    }
}



//CRC32校验显示
void MainWindow::showCRC32() {
    if (!hexsend->isChecked()) {
        QString string = sendedit->toPlainText();
        int p = string.length();
        int n = whatbit->text().toInt();
        int m = number->currentIndex();
        QString text = string.mid(n - 1, p - m - n + 1);

        QByteArray ba = text.toUtf8();

        uint32_t crc32 = CRC32(ba);
        QString crcString = QString("%1").arg(crc32, 8, 16, QChar('0')).toUpper();

        // 逆转 CRC32 结果字符串中的每两个字符
        QString reversedCrcString;
        for (int i = 0; i < crcString.length(); i += 2) {
            reversedCrcString = crcString.mid(i, 2) + reversedCrcString;
        }

        recog->setText(reversedCrcString);
    } else if (hexsend->isChecked()) {
        QString string = sendedit->toPlainText();
        string.remove(' ');
        int p = string.length();
        int n = whatbit->text().toInt();
        int m = number->currentIndex();
        QString hexString = string.mid(n * 2 - 2, p - m * 2 - n * 2 + 2);

        QByteArray ba = QByteArray::fromHex(hexString.toLatin1());

        uint32_t crc32 = CRC32(ba);
        QString crcString = QString("%1").arg(crc32, 8, 16, QChar('0')).toUpper();

        // 逆转 CRC32 结果字符串中的每两个字符
        QString reversedCrcString;
        for (int i = 0; i < crcString.length(); i += 2) {
            reversedCrcString = crcString.mid(i, 2) + reversedCrcString;
        }

        recog->setText(reversedCrcString);
    }
}



//信号处理函数
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}



//校验位颜色变化
bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == wei->view())
    {
        if (event->type() == QEvent::Enter)
        {
            // 鼠标进入事件，设置选项背景为黄色
            QAbstractItemView* view = static_cast<QAbstractItemView*>(obj);
            QModelIndex index = view->currentIndex();
            view->model()->setData(index, QColor(Qt::yellow), Qt::BackgroundRole);
        }
        else if (event->type() == QEvent::Leave)
        {
            // 鼠标离开事件，恢复选项背景
//            QAbstractItemView* view = static_cast<QAbstractItemView*>(obj);
//            view->model()->setData(QModelIndex(), QVariant(), Qt::BackgroundRole);
        }
    }
    return false;
}



//信号与槽
void MainWindow::ssfunction(){

    //更多串口设置槽函数
    connect(nwindow,SIGNAL(sendData(int,int,int,int)),this,SLOT(recdata(int,int,int,int)));

    //判断数据的合法性
    connect(sendedit,SIGNAL(textChanged()),this,SLOT(examing()));

    //HEX发送chexkBox信号槽
    connect(hexsend,&QCheckBox::stateChanged,this,[=](int state){
        if (SendTextEditStr.isEmpty())
        {
            if (state == Qt::Checked)
            {
                exchange->setCheckState(Qt::Unchecked);
                exchange->setEnabled(false);
            }
            else
            {
                exchange->setEnabled(true);
            }
            return;
        }
        //asccii与hex转换
        if (state == Qt::Checked)
        {
            //转换成QByteArray -> 转换成16进制数，按空格分开 -> 转换为大写
            SendTextEditBa = SendTextEditBa.toHex(' ').toUpper();
            sendedit->document()->setPlainText(SendTextEditBa);
            exchange->setEnabled(false);
        }
        else
        {
            //从QByteArray转换为QString
            SendTextEditStr = SendTextEditBa.fromHex(SendTextEditBa);
            sendedit->document()->setPlainText(SendTextEditStr);
            exchange->setEnabled(true);
        }
    });

    //定时器超时发送数据
    connect(TimerSend, &QTimer::timeout, this, [=](){
        SerialSendData(SendTextEditBa);
        qDebug()<<b;
    });

    //串口combobox改变信息传递
    connect(this, SIGNAL(reflashSignal()), this, SLOT(reflashSlot()));

//    QTextCursor cursor = ui->textEdit->textCursor();
//    cursor.movePosition(QTextCursor::End);                                                                               ////
//    ui->textEdit->setTextCursor(cursor);

    //数据接收
//    connect(rtimer,&QTimer::timeout,[this](){
//    QTextCursor cursor = ui->textEdit->textCursor();                                                                     ////
//    cursor.movePosition(QTextCursor::End);
//    ui->textEdit->setTextCursor(cursor);
//});
     connect(qApp, &QApplication::aboutToQuit, this, &MainWindow::saveSettingsOnExit);

}



//接收数据槽函数
void MainWindow::SerialPortReadyRead_slot() {
//    QTextCursor cursor = ui->textEdit->textCursor();
//    cursor.movePosition(QTextCursor::End);                                                                               ////
//    ui->textEdit->setTextCursor(cursor);
    if (openbtn->text() == "关闭串口"||openbtn->text() == "CloseCom") {
        setserial();
        QString framedata;
        QByteArray bytedata = serial->readAll();

        if (!bytedata.isEmpty()) {
            if (hexshow->isChecked()) {
                //hex显示
                            framedata = bytedata.toHex(' ').trimmed().toUpper();

                            int dataSize = bytedata.size();
                            // 累加这次接收的字节数到总数上
                            totalReceiveBytes += dataSize;
                            // 更新接收数据字节的标签
                            QString receiveText = QString("R: %1   ").arg(totalReceiveBytes);
                            receiveLabel->setText(receiveText);
            } else {
                //ascii显示
                            framedata = QString(bytedata);

                            int dataSize = bytedata.size();
                            // 累加这次接收的字节数到总数上
                            totalReceiveBytes += dataSize;
                            // 更新接收数据字节的标签
                            QString receiveText = QString("R: %1   ").arg(totalReceiveBytes);
                            receiveLabel->setText(receiveText);
            }

            //是否显示时间戳
                    if (timepack->isChecked())
                    {
//                        QTextCursor cursor = ui->textEdit->textCursor();
//                        cursor.movePosition(QTextCursor::End);                                                                               ////
//                        ui->textEdit->setTextCursor(cursor);
                        framedata = QString("[%1]RX -> %2").arg
                                (QTime::currentTime().toString("HH:mm:ss:zzz")).arg(framedata);
                        ui->textEdit->append(framedata);

                    }
                    else
                    {
//                        QTextCursor cursor = ui->textEdit->textCursor();
//                        cursor.movePosition(QTextCursor::End);                                                                               ////
//                        ui->textEdit->setTextCursor(cursor);
                        ui->textEdit->insertPlainText(framedata);

                    }


            // 更新接收数据字节的标签
            QString receiveText = QString("R: %1   ").arg(totalReceiveBytes);
            receiveLabel->setText(receiveText);

            // 更新状态栏的文本
            statusBar()->showMessage(receiveText);

            if (rectofile->isChecked()) {
                QString fileName = QString("ReceivedToFile-%1-%2.dat")
                        .arg(serial->portName())
                        .arg(QDateTime::currentDateTime().toString("yyyy_MM_d_H-m-s"));

                QFile file(fileName);
                if (file.open(QIODevice::WriteOnly)) {
                    file.write(bytedata);
                    file.close();
                    qDebug() << "Data saved to" << fileName;
                } else {
                    qDebug() << "Failed to save data to" << fileName;
                }
            }

//            // 时间戳和分包接收处理
//            if(timepack->isChecked()){framedata = QString("[%1]:RX -> %2").arg
//                        (QTime::currentTime().toString("HH:mm:ss:zzz")).arg(framedata);

//                                if (interval > whattime->text().toInt()) {
//                                        QString hexData = QString(bytedata.toHex());  // 将字节数组转换为十六进制字符串
//                                        QString currentTime = QTime::currentTime().toString("hh:mm:ss.zzz");
//                                        QString receivedText = QString("[%1]:RX -> %2").arg
//                                                (QTime::currentTime().toString("HH:mm:ss:zzz")).arg(framedata);
////                                                QString("[%1]:RX -> %2").arg(currentTime).arg(hexData);
//                                        ui->textEdit->append(receivedText);
//                                    }
//                                    QString receiveText = QString("R: %1   ").arg(totalReceiveBytes);
//                                    receiveLabel->setText(receiveText);
//                                   }
                        }


    QString styleSheet = "QComboBox:: { background-color: yellow; }"
                         "QComboBox QAbstractItemView::item:hover { background-color: yellow; }";
    wei->setStyleSheet(styleSheet);

    QAbstractItemView* view = wei->view();
    view->installEventFilter(this);



    //校验
    QObject::connect(senbtn, &QPushButton::clicked, [&]() {

        switch (wei->currentIndex()) {

        case 0:
            recog->clear();
            break;

        case 1:                                                                                     //ModbusCRC16校验
            showModbus();
            break;

        case 2:
            showCrc16();
            break;

        case 3:
            showCRC32();
            break;
        case 4:
            showADD8_0();
            break;
        case 5:
            showADD8();
            break;
        case 6:
            showXOR8();
            break;
        case 7:
            showADD16();
            break;
    }

    });
}

//     cursor = ui->textEdit->textCursor();
//    cursor.movePosition(QTextCursor::End);                                                                               ////
//    ui->textEdit->setTextCursor(cursor);
}



//字符判断
void MainWindow::examing(){
        //获取发送框字符
        SendTextEditStr = sendedit->document()->toPlainText();
        if (SendTextEditStr.isEmpty())
        {
            return;
        }
        //勾选hex发送则判断是否有非法hex字符
        if (hexsend->isChecked())
        {
            char ch;
            bool flag = false;
            uint32_t i, len;
            //去掉无用符号
            SendTextEditStr = SendTextEditStr.replace(' ',"");
            SendTextEditStr = SendTextEditStr.replace(',',"");
            SendTextEditStr = SendTextEditStr.replace('\r',"");
            SendTextEditStr = SendTextEditStr.replace('\n',"");
            SendTextEditStr = SendTextEditStr.replace('\t',"");
            SendTextEditStr = SendTextEditStr.replace("0x","");
            SendTextEditStr = SendTextEditStr.replace("0X","");
            //判断数据合法性
            for(i = 0, len = SendTextEditStr.length(); i < len; i++)
            {
                ch = SendTextEditStr.at(i).toLatin1();
                if (ch >= '0' && ch <= '9') {
                    flag = false;
                } else if (ch >= 'a' && ch <= 'f') {
                    flag = false;
                } else if (ch >= 'A' && ch <= 'F') {
                    flag = false;
                } else {
                    flag = true;
                }
            }
            if(flag) QMessageBox::warning(this,"警告","输入内容包含非法16进制字符");
        }
        //QString转QByteArray
        SendTextEditBa = SendTextEditStr.toUtf8();
}



//设置串口信息
void MainWindow::setserial(){

       //设置波特率
       switch (setbaud)
       {
        case 0:
            serial->setBaudRate(QSerialPort::Baud9600);
            break;
        case 1:
            serial->setBaudRate(QSerialPort::Baud38400);
            break;
        case 2:
            serial->setBaudRate(QSerialPort::Baud115200);
            break;
        }

        //设置校验位
        switch (setpari)
        {
        case 0:
            serial->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serial->setParity(QSerialPort::OddParity);
            break;
        case 2:
            serial->setParity(QSerialPort::EvenParity);
            break;
        }

        //设置数据位
        switch (setdata)
        {
        case 0:
            serial->setDataBits(QSerialPort::Data5);
            break;
        case 1:
            serial->setDataBits(QSerialPort::Data6);
            break;
        case 2:
            serial->setDataBits(QSerialPort::Data7);
            break;
        case 3:
            serial->setDataBits(QSerialPort::Data8);
            break;
        }

        //设置停止位
        switch (setstop)
        {
        case 0:
            serial->setStopBits(QSerialPort::OneStop);
            break;
        case 1:
            serial->setStopBits(QSerialPort::TwoStop);
            break;
        case 2:
            serial->setStopBits(QSerialPort::OneAndHalfStop);
            break;
        }

}



//更多串口设置界面显示
void MainWindow::moreset()
{
    nwindow->setWindowModality(Qt::ApplicationModal);
    nwindow->setWindowFlags(Qt::WindowStaysOnTopHint);
    nwindow->show();
    nwindow->move(this->geometry().center()-nwindow->rect().center());
}



//清除接收框
void MainWindow::clearwincao()
{
    ui->textEdit->clear();
}



// 打开文件
void MainWindow::openfilecao() {
    // 选择并打开文件
    QString initialName;
    QString fileName = QFileDialog::getOpenFileName(this, tr("choose file"), initialName);
    fileName = QDir::toNativeSeparators(fileName);

    // 路径不为空
    if (!fileName.isEmpty()) {
        filepath->setText(fileName);
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            qDebug() << "Can't open the file!" ;
        }

        // 读取文件内容并指定编码
        QTextStream in(&file);
        in.setCodec("UTF-8"); // 根据实际文件编码调整
        data = in.readAll().toUtf8(); // 转换为UTF-8编码

        // 关闭文件
        ui->textEdit->clear();
        file.close();
    }

    // 显示文件内容
    QString previewData;

    if (hexshow->isChecked()) {
        previewData = data.toHex(' ').toUpper();
    }
    else {
        previewData = data;
    }

    if (previewData.length() > 4000) {
        previewData = previewData.left(4000);
        // 显示文件大小
        QFileInfo info(fileName);
        fileSizeS = QString::number(info.size());
        ui->textEdit->append("文件大小：" + fileSizeS + "字节，下面是预览的前4000字节内容：\n");
    }
    else {
        // 显示文件大小
        QFileInfo info(fileName);
        fileSizeS = QString::number(info.size());
        ui->textEdit->append("文件大小：" + fileSizeS + "字节，下面是预览的前" + fileSizeS + "字节内容：\n");
    }

    ui->textEdit->append(previewData);

    // 设置显示焦点在最顶部
//    ui->textEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor);                                                                               ////

    // 标记待发送文件
    isSendFile = true;
    FrameCount = 0;
    ProgressBarValue = 0;
    ui->progressBar->setValue(0);
}



//发送文件按键槽
void MainWindow::sendfilecao(){
    setserial();
    ui->progressBar->show();
    if(openbtn->text()!="打开串口"){
            sendFile();
                totalSendBytes = fileSizeS.toUInt()+totalSendBytes;
                QString sendText = QString("S: %1   ").arg(totalSendBytes);
                sendLabel->setText(sendText);
    }else{
        QMessageBox::information(this,"tips","serial is not open");
    }
}



//发送文件函数
void MainWindow::sendFile(){
    //发送文件延时
    framelen = 256;
    framegap = 100;

    int textlen = MainWindow::data.size();

    //设定时间和长度发送
    framenumber = textlen / framelen;
    lastframelen = textlen % framelen;

    //设置进度条的最大值为发送的总字节数
    ui->progressBar->setMaximum(textlen);

    //进度条步进值
    ProgressBarstep = 1;
//    totalReceiveBytes = 0;


    //设置定时器
    FileSendTimer->start(framegap);
}



//发送文件槽函数
void MainWindow::timsend(){

    if(FrameCount<framenumber){
        QByteArray frameData = data.mid(FrameCount * framelen, framelen);
        serial->write(frameData);
        FrameCount++;

        //更新进度条的当前值
        ui->progressBar->setValue(FrameCount * framelen);

        // 更新状态栏的文本
//        int sentDataSize = FrameCount * framelen + lastframelen;

//               QString dataSize = QString("S：%1   ").arg(sentDataSize);
//               sendLabel->setText(dataSize);
//         statusBar()->showMessage(dataSize);

//        int sentDataSize = FrameCount * framelen + lastframelen;
//               qint64 totalSentBytes = totalFileBytes + sentDataSize;
//               QString dataSize = QString("S: %1   ").arg(totalSentBytes);
//               sendLabel->setText(dataSize);
//               statusBar()->showMessage(dataSize);

    }else{
        if (lastframelen > 0) {
               QByteArray lastFrameData = data.mid(FrameCount * framelen, lastframelen);
               serial->write(lastFrameData);
               ui->progressBar->setValue(data.size());
                }

        //发送完成
        FileSendTimer->stop();
        FrameCount = 0;
        ProgressBarValue = 0;
        framenumber = 0;
        lastframelen = 0;
        QMessageBox::information(this,"tips","transfer succeeded");
        ui->progressBar->hide();
//        // 更新状态栏的文本（发送完成后）
//                qint64 totalSentBytes = totalFileBytes + data.size();
//                QString dataSize = QString("S: %1   ").arg(totalSentBytes);
//                sendLabel->setText(dataSize);
//                statusBar()->showMessage(dataSize);

    }

}



//停止发送文件键
void MainWindow::stopcao()
{
    FileSendTimer->stop();
}



//清除发送框
void MainWindow::clearsendcao()
{
   sendedit->clear();
}



//隐藏控件
void MainWindow::widghidecao(){
            if(widghide->text() == "-"){
                QWidget* hide[] = {ui->widget,ui->widget_2,ui->widget_3,ui->widget_4};
                for(unsigned int i = 0; i < sizeof(hide)/sizeof(QWidget*) ; i++){
                    hide[i]->setVisible(false);
                    widghide->setText("|");
                }}
                    else{
                    QWidget* hide[] = {ui->widget,ui->widget_2,ui->widget_3,ui->widget_4};
                    for(unsigned int i = 0; i < sizeof(hide)/sizeof(QWidget*); i++){
                        hide[i]->setVisible(true);
                       widghide->setText("-");
                }
    }
//            qDebug()<<ui->widget_5->width();

            //使缩放控件大小不变
//            ui->textEdit->resize( ui->widget_5->width(),ui->textEdit->height()+ui->widget_4->height());
            ui->textEdit->resize( 2000,3000);
}



//保存数据按键槽
void MainWindow::savedatacao(){
    QString text = ui->textEdit->toPlainText();

        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString dateTimeString = currentDateTime.toString("yyyy_MM_d_h-m-s");

        // 保存内容到.txt文件
        QString fileNameTxt = "SaveWindows" + dateTimeString + ".TXT";  // 修改文件名格式
        QFile fileTxt(fileNameTxt);
        if (fileTxt.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&fileTxt);
            out << text;
            fileTxt.close();
            qDebug() << "Text saved to" << fileNameTxt;
        } else {
            qDebug() << "Failed to save text to" << fileNameTxt;
        }

        // 保存原始数据到.dat文件
        QByteArray data = text.toUtf8();
        QString fileNameDat = "SAVE" + dateTimeString + ".dat";  // 修改文件名格式
        QFile fileDat(fileNameDat);
        if (fileDat.open(QIODevice::WriteOnly)) {
            fileDat.write(data);
            fileDat.close();
            qDebug() << "Data saved to" << fileNameDat;
        } else {
            qDebug() << "Failed to save data to" << fileNameDat;
        }

        QString currentPath = QDir::currentPath();
        qDebug() << currentPath;

//        QMessageBox::information(this, "数据保存路径", currentPath);
        if(!english->isChecked()){
            QMessageBox box(QMessageBox::Information,"数据保存路径",currentPath);
            box.setButtonText(QMessageBox::Ok,QString("确定"));
            box.exec();
        }else{
            QMessageBox box(QMessageBox::Information,"save path",currentPath);
            box.setButtonText(QMessageBox::Ok,QString("OK"));
            box.exec();
        }
}



//ok按键按下波特率窗口更新显示
void MainWindow::okPush(){

    //更新主界面波特率下拉框
     switch (setbaud)
       {
        case 2:
            choosebit->setCurrentIndex(2);
            break;
        case 1:
            choosebit->setCurrentIndex(1);
            break;
        case 0:
            choosebit->setCurrentIndex(0);
            break;
        }

          QString parityStr;
          switch (setpari)
          {
          case 0:
              parityStr = "None";
                  break;
              case 1:
              parityStr = "Odd";
                  break;
              case 2:
              parityStr = "Even";
                  break;
              case 3:
              parityStr = "Mark";
                  break;
              case 4:
              parityStr = "Space";
              break;
          }



      QString stopBitsStr;
      switch (setstop)
      {
      case 0:
          stopBitsStr = "1";
              break;
          case 1:
          stopBitsStr = "1.5";
              break;
          case 2:
          stopBitsStr = "2";
          break;
      }




      QString dataBitsStr;
      switch (setdata)
      {
      case 0:
          dataBitsStr = "8";
              break;
          case 1:
          dataBitsStr = "7";
              break;
          case 2:
          dataBitsStr = "6";
              break;
          case 3:
          dataBitsStr = "5";
          break;
      }



      serialStatusLabel->setText(portbox->currentText() + ":已打开 " + choosebit->currentText() + "bps"  + ","  +  dataBitsStr + "," + stopBitsStr + "," + parityStr);
      a = dataBitsStr; b = stopBitsStr;c = parityStr;

}



//发送串口改变信号
void MainWindow::modifyCom(const QString& text){
    if(text==portbox->currentText())
       emit reflashSignal();
  }



//串口改变更新状态栏
void MainWindow::reflashSlot() {
     serialStatusLabel->setText(portbox->currentText() + ":已关闭 " + choosebit->currentText() + "bps"  + ","  +  a + "," + b + "," + c);
}



//波特率combobox设置波特率
void MainWindow::modifybaud(int index){
    //设置波特率
    switch (index)
    {
     case 0:
         setbaud=0;
         break;
     case 1:
         setbaud=1;
         break;
     case 2:
         setbaud=2;
         break;
     }
  emit reflashSignal();
}



//用接收到的下拉框内容设置串口信息
void MainWindow::recdata(int baud,int stop,int data,int pari){

        //保存波特率
        switch (baud)
       {
        case 0:setbaud = 0;
            break;
        case 1:setbaud = 1;
            break;
        case 2:setbaud = 2;
            break;
        }

        //保存校验位
        switch (pari)
        {
        case 0:setpari = 0;
            break;
        case 1:setpari = 1;
            break;
        case 2:setpari = 2;
            break;
        case 3:setpari = 3;
            break;
        case 4:setpari = 4;
            break;
        }

        //保存数据位
        switch (data)
        {
        case 0:setdata = 0;
            break;
        case 1:setdata = 1;
            break;
        case 2:setdata = 2;
            break;
        case 3:setdata = 3;
            break;
        }

        //保存停止位
        switch (stop)
        {
        case 0:setstop = 0;
            break;
        case 1:setstop = 1;
            break;
        case 2:setstop = 2;
            break;
        }

    }



//发送数据函数
void MainWindow::SerialSendData(QByteArray badata){

    if(badata.isEmpty()!=true){

        //是否加回车换行
        if(exchange->isChecked()){
            badata.append("\r\n");
            yon=1;
        }else{
            yon=0;
        }

        //hex发送
        if(hexsend->isChecked()){
            if (badata.size() % 2 != 0) {

                // 当发送的数据长度为奇数时，丢弃最后一个字节
                badata = badata.left(badata.size() - 1);
            }
            badata = badata.fromHex(badata);
            if(wei->currentIndex() == 1)
                showModbus();
            else if(wei->currentIndex() == 2)
                showCrc16();
            else if(wei->currentIndex() == 7)
                showADD16();
            else if(wei->currentIndex() == 3)
                showCRC32();
            else if(wei->currentIndex() == 4)
                showADD8_0();
            else if(wei->currentIndex() == 5)
                showADD8();
            else if(wei->currentIndex() == 6)
                showXOR8();



            if(wei->currentIndex() == 1 ||wei->currentIndex() == 2||wei->currentIndex() == 7){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第二位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(highByte.toInt(0, 16)));
                    }
                } else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");
                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据末尾添加校验位
                        badata.append(static_cast<char>(highByte.toInt(0, 16)));
                        badata.append(static_cast<char>(lowByte.toInt(0, 16)));
                    }
                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(highByte.toInt(0, 16)));

                    }
                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(highByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(highByte.toInt(0, 16)));
                    }
                }
            }


            if(wei->currentIndex() == 4 ||wei->currentIndex() == 5||wei->currentIndex() == 6){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                        // 在发送数据的倒数第二位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(crcString.toInt(0, 16)));

                } else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                        badata.append(static_cast<char>(crcString.toInt(0, 16)));

                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    QString crcString = recog->text();

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(crcString.toInt(0, 16)));

                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                           QString crcString = recog->text();

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(crcString.toInt(0, 16)));
                    }
                else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();

                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(crcString.toInt(0, 16)));
                    }

            }



            if(wei->currentIndex() == 3){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第二位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据末尾添加校验位
                        badata.append(static_cast<char>(onByte.toInt(0, 16)));
                        badata.append(static_cast<char>(twByte.toInt(0, 16)));
                        badata.append(static_cast<char>(thByte.toInt(0, 16)));
                        badata.append(static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);


                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }

}


            serial->write(badata);

            //是否显示时间戳
            if(timepack->isChecked()){
                QString strdata = badata.toHex(' ').trimmed().toUpper();
                ui->textEdit->setTextColor(QColor("blue"));
//                ui->textEdit->append(QString("[%1]TX -> ").arg(QTime::currentTime().toString("HH:mm:ss:zzz")));
//                ui->textEdit->setTextColor(QColor("black"));
//                ui->textEdit->append(strdata);

                strdata = QString("[%1]TX -> %2").arg
                        (QTime::currentTime().toString("HH:mm:ss:zzz")).arg(strdata);
                ui->textEdit->append(strdata);
            }
        }

        //ascii发送
        else{
            if(wei->currentIndex() == 1)
                showModbus();
            else if(wei->currentIndex() == 2)
                showCrc16();
            else if(wei->currentIndex() == 7)
                showADD16();
            else if(wei->currentIndex() == 3)
                showCRC32();
            else if(wei->currentIndex() == 4)
                showADD8_0();
            else if(wei->currentIndex() == 5)
                showADD8();
            else if(wei->currentIndex() == 6)
                showXOR8();
            if(wei->currentIndex() == 1 ||wei->currentIndex() == 2||wei->currentIndex() == 7){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(highByte.toInt(0, 16)));
                    }
                } else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");
                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据末尾添加校验位
                        badata.append(static_cast<char>(highByte.toInt(0, 16)));
                        badata.append(static_cast<char>(lowByte.toInt(0, 16)));
                    }
                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(highByte.toInt(0, 16)));

                    }
                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(highByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();
                    QStringList crcList = crcString.split(" ");

                    // 检查校验位数据是否有效
                    if (crcList.length() == 2) {
                        QString highByte = crcList.at(0);
                        QString lowByte = crcList.at(1);

                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(lowByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(highByte.toInt(0, 16)));
                    }
                }
            }

            if(wei->currentIndex() == 4 ||wei->currentIndex() == 5||wei->currentIndex() == 6){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                        // 在发送数据的倒数第二位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(crcString.toInt(0, 16)));

                } else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                        badata.append(static_cast<char>(crcString.toInt(0, 16)));

                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    QString crcString = recog->text();

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(crcString.toInt(0, 16)));

                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                           QString crcString = recog->text();

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(crcString.toInt(0, 16)));
                    }
                else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    // 在recog中添加校验位数据
                    QString crcString = recog->text();

                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(crcString.toInt(0, 16)));
                    }

            }


            if(wei->currentIndex() == 3){
                // 在recog中添加校验位数据
                if (number->currentIndex() == 2 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第二位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex - 1, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(lastIndex - 1, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 0 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据末尾添加校验位
                        badata.append(static_cast<char>(onByte.toInt(0, 16)));
                        badata.append(static_cast<char>(twByte.toInt(0, 16)));
                        badata.append(static_cast<char>(thByte.toInt(0, 16)));
                        badata.append(static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if(number->currentIndex() == 1 && !recog->text().isEmpty()){
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第一位之前插入低字节和高字节的值
                        int lastIndex = badata.size() - 1;
                        badata.insert(lastIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(lastIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 3 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);

                        // 在发送数据的倒数第三位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 3;
                        badata.insert(insertIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }else if (number->currentIndex() == 4 && !recog->text().isEmpty()) {
                    QString crcString = recog->text();

                    // 检查校验位数据是否有效
                    if (crcString.length() == 8) {
                        QString onByte = crcString.mid(6, 2);
                        QString twByte = crcString.mid(4, 2);
                        QString thByte = crcString.mid(2, 2);
                        QString foByte = crcString.mid(0, 2);


                        // 在发送数据的倒数第四位之前插入低字节和高字节的值
                        int insertIndex = badata.size() - 4;
                        badata.insert(insertIndex, static_cast<char>(onByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(twByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(thByte.toInt(0, 16)));
                        badata.insert(insertIndex, static_cast<char>(foByte.toInt(0, 16)));
                    }
                }

}

            serial->write(badata);

            //是否显示时间戳
            if(timepack->isChecked()){
                //移动光标到末尾
//                sendedit->moveCursor(QTextCursor::End);
                QString strdata = QString(badata);
                ui->textEdit->setTextColor(QColor("red"));
//                ui->textEdit->append(QString("[%1]TX -> ").arg(QTime::currentTime().toString("HH:mm:ss:zzz")));
//                ui->textEdit->setTextColor(QColor("black"));
//                ui->textEdit-> insertPlainText(strdata);

                strdata = QString("[%1]TX -> %2").arg
                        (QTime::currentTime().toString("HH:mm:ss:zzz")).arg(strdata);
                ui->textEdit->append(strdata);

            }
        }

    }
    else{
        QMessageBox::warning(this,"warning","data is empty");
    }
}



//定时发送
void MainWindow::timeSen(int state){
    // 获取复选框状态，未选为0，选中为2
    if(state == 0){
        timeSend->stop();
        // 时间输入框恢复可选
         whattime->setEnabled(true);
    }else{
        // 对输入的值做限幅，小于10ms会弹出对话框提示
        if(whattime->text().toInt() >= 10){
            timeSend->start(whattime->text().toInt());// 设置定时时长，重新计数
            // 让时间输入框不可选，避免误操作（输入功能不可用，控件背景为灰色）
            whattime->setEnabled(false);
        }else{
            timesend->setCheckState(Qt::Unchecked);
            QMessageBox::critical(this, "错误提示", "定时发送的最小间隔为 10ms\r\n请确保输入的值 >=10");
        }
    }
}



//打开串口槽函数
void MainWindow::openserialbtn(){
    //如果串口为关闭状态时按下
        if(openbtn->text()=="打开串口"||openbtn->text()=="OpenCom")
        {

            //串口为当前串口box显示的串口
            serial = new QSerialPort(portbox->currentText());

            //使能发送按钮
            senbtn->setEnabled(true);
            sendfile->setEnabled(true);

            senbtn->setStyleSheet("color:rgb(0, 139, 152); font-weight: bold;");
            sendfile->setStyleSheet("color: black;");

            //不使能portbox
            portbox->setEnabled(false);



            //如果串口正在使用
            if(!serial->open(QIODevice::ReadWrite)){
                QMessageBox::warning(this,"tips","串口打开失败");
                serial->deleteLater();
                return;
            }

            //初始化
             switch (setbaud)
               {
                case 0:
                    serial->setBaudRate(QSerialPort::Baud9600);
                    break;
                case 1:
                    serial->setBaudRate(QSerialPort::Baud38400);
                    break;
                case 2:
                    serial->setBaudRate(QSerialPort::Baud115200);
                    break;
                }

              //设置校验位
              switch (setpari)
              {
              case 0:
                  serial->setParity(QSerialPort::NoParity);
                  break;
              case 1:
                  serial->setParity(QSerialPort::OddParity);
                  break;
              case 2:
                  serial->setParity(QSerialPort::EvenParity);
                  break;
              case 3:
                  serial->setParity(QSerialPort::MarkParity);
                  break;
              case 4:
                  serial->setParity(QSerialPort::SpaceParity);
                  break;
              }

              //设置数据位
              switch (setdata)
              {
              case 0:
                  serial->setDataBits(QSerialPort::Data8);
                  break;
              case 1:
                  serial->setDataBits(QSerialPort::Data7);
                  break;
              case 2:
                  serial->setDataBits(QSerialPort::Data6);
                  break;
              case 3:
                  serial->setDataBits(QSerialPort::Data5);
                  break;
              }

              //设置停止位
              switch (setstop)
              {
              case 0:
                  serial->setStopBits(QSerialPort::OneStop);
                  break;
              case 1:
                  serial->setStopBits(QSerialPort::OneAndHalfStop);
                  break;
              case 2:
                  serial->setStopBits(QSerialPort::TwoStop);
                  break;
              }

//            serial->setBaudRate(QSerialPort::Baud9600);
//            serial->setDataBits(QSerialPort::Data8);
//            serial->setStopBits(QSerialPort::OneStop);
//            serial->setParity(QSerialPort::NoParity);
            serial->setFlowControl(QSerialPort::NoFlowControl);

            QString stopBitsStr;
            if (serial->stopBits() == QSerialPort::OneStop)
                stopBitsStr = "1";
            else if (serial->stopBits() == QSerialPort::OneAndHalfStop)
                stopBitsStr = "1.5";
            else if (serial->stopBits() == QSerialPort::TwoStop)
                stopBitsStr = "2";

            QString parityStr;
            if (serial->parity() == QSerialPort::NoParity)
                parityStr = "None";
            else if (serial->parity() == QSerialPort::OddParity)
                parityStr = "Odd";
            else if (serial->parity() == QSerialPort::EvenParity)
                parityStr = "Even";
            else if (serial->parity() == QSerialPort::MarkParity)
                parityStr = "Mark";
            else if (serial->parity() == QSerialPort::SpaceParity)
                parityStr = "Space";


            QString dataBitsStr;
            if (serial->dataBits() == QSerialPort::Data8)
                dataBitsStr = "8";
            else if (serial->dataBits() == QSerialPort::Data7)
                dataBitsStr = "7";
            else if (serial->dataBits() == QSerialPort::Data6)
                dataBitsStr = "6";
            else if (serial->dataBits() == QSerialPort::Data5)
                dataBitsStr = "5";



            serialStatusLabel->setText(portbox->currentText() + ":已打开 " + choosebit->currentText() + "bps"  + ","  +  dataBitsStr + "," + stopBitsStr + "," + parityStr);
            a = dataBitsStr; b = stopBitsStr;c = parityStr;

            connect(serial, &QSerialPort::readyRead, this, &MainWindow::SerialPortReadyRead_slot);

             //不使能portbox
            portbox->setEnabled(false);


            if(openbtn->text() == "打开串口")
            openbtn->setText("关闭串口");
            else
                openbtn->setText("CloseCom");
        }

        //串口处于打开状态
        else{
            //释放串口
            serial->clear();
            serial->close();
            serial->deleteLater();

            //使能portbox
            portbox->setEnabled(true);

            serialStatusLabel->setText(portbox->currentText() + ":已关闭 " + choosebit->currentText() + "bps"  + ","  +  a + "," + b + "," + c);

            //不使能发送按钮
            senbtn->setEnabled(false);
            sendfile->setEnabled(false);

            senbtn->setStyleSheet("color: gray;");
            sendfile->setStyleSheet("color: gray;");
            if(openbtn->text() == "关闭串口")
            openbtn->setText("打开串口");
            else

            openbtn->setText("OpenCom");

}
}



//串口刷新函数
void MainWindow::UpdatePort()
{
    //可使用的串口赋值给newPortStringList
    QStringList newPortStringList;
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        newPortStringList += info.portName();
    }

    //把新串口加入portbox
    if(newPortStringList.size() != oldPortStringList.size())
    {
        oldPortStringList = newPortStringList;
        portbox->clear();
        portbox->addItems(oldPortStringList);
    }

}



//发送按键槽函数
void MainWindow::sendbtn(){

    if(openbtn->text()=="打开串口")
    {
        //串口为当前串口box显示的串口
        serial = new QSerialPort(portbox->currentText());

        //使能发送按钮
        senbtn->setEnabled(true);
        sendfile->setEnabled(true);

        senbtn->setStyleSheet("color:rgb(0, 139, 152); font-weight: bold;");
        sendfile->setStyleSheet("color: black;");

        //不使能portbox
        portbox->setEnabled(false);
        openbtn->setText("关闭串口");




        //如果串口正在使用
        if(!serial->open(QIODevice::ReadWrite)){
            QMessageBox::warning(this,"tips","串口打开失败");
            serial->deleteLater();
            return;
        }


        }
        setserial();

        SerialSendData(SendTextEditBa);

        if(!hexsend->isChecked()){
        QString hexString = recog->text().toLower();
        int hexSize = hexString.length() / 2;  // 每两个字符为一个字节，因此除以2得到字节数
        if(wei->currentIndex()!=0){
        int dataSize = SendTextEditBa.size() + hexSize;
        totalSendBytes += dataSize;}
        else {
        int dataSize = SendTextEditBa.size();
            totalSendBytes += dataSize;
            if(yon == 1){
                totalSendBytes += 2;
            }
}
        // 更新总的发送字节数

        }

        if(hexsend->isChecked()){
        QString hexString = recog->text().toLower();
        int hexSize = hexString.length() / 2;  // 每两个字符为一个字节，因此除以2得到字节数

        if(wei->currentIndex()!=0){
            int dataSize = SendTextEditBa.size()/2 + hexSize;

            // 更新总的发送字节数
            totalSendBytes += dataSize;
        }else {
                int dataSize = SendTextEditBa.size()/2 ;

                // 更新总的发送字节数
                totalSendBytes += dataSize;
}

        }

        // 更新发送数据字节的标签

        QString sendText = QString("S: %1   ").arg(totalSendBytes);


        sendLabel->setText(sendText);


}




void MainWindow::updateStatusBar()
{
    // 计算已发送的总字节数
    qint64 sentDataSize = FrameCount * framelen + lastframelen;
    qint64 totalSentBytes = totalFileBytes + sentDataSize;

    // 更新状态栏的文本
    QString dataSize = QString("S: %1   ").arg(totalSentBytes);
    sendLabel->setText(dataSize);
    statusBar()->showMessage(dataSize);
}



//保存参数按键槽
void MainWindow::saveparacao(){
    saveSettingsToIniFile();
}



//扩展
void MainWindow::expandcao(){}



//保存参数信息
void MainWindow::saveSettingsToIniFile()
{

    QString filePath = QCoreApplication::applicationDirPath() + "/sscom.ini"; // 拼接当前路径和文件名

    QSettings settings(filePath, QSettings::IniFormat);

    // 保存窗口大小
    settings.setValue("Section/MainWindowSize", size());

    // 保存CheckBox状态
    bool isChecked1 = english->isChecked();
    settings.setValue("Section/english", isChecked1);
    bool isChecked2 = zuiqian->isChecked();
    settings.setValue("Section/zuiqian", isChecked2);
    bool isChecked3 = hexshow->isChecked();
    settings.setValue("Section/hexshow", isChecked3);
    bool isChecked4 = rectofile->isChecked();
    settings.setValue("Section/rectofile", isChecked4);
    bool isChecked5 = hexsend->isChecked();
    settings.setValue("Section/hexsend", isChecked5);
    bool isChecked6 = timepack->isChecked();
    settings.setValue("Section/timepack", isChecked6);
    bool isChecked7 = timesend->isChecked();
    settings.setValue("Section/timesend", isChecked7);
    bool isChecked8 = exchange->isChecked();
    settings.setValue("Section/exchange", isChecked8);

    // 保存ComboBox当前选项
    int currentIndex1 = wei->currentIndex();
    settings.setValue("Section/wei", currentIndex1);
    int currentIndex2 = number->currentIndex();
    settings.setValue("Section/number", currentIndex2);
    int currentIndex3 = portbox->currentIndex();
    settings.setValue("Section/portbox", currentIndex3);
    int currentIndex4 = choosebit->currentIndex();
    settings.setValue("Section/choosebit", currentIndex4);

    // 保存LineEdit文本内容
    QString text1 = filepath->text();
    settings.setValue("Section/filepath", text1);
    QString text2 = whattime->text();
    settings.setValue("Section/whattime", text2);
    QString text3 = whatbit->text();
    settings.setValue("Section/whatbit", text3);
    QString text4 = latetime->text();
    settings.setValue("Section/latetime", text4);

    // 将数据写入配置文件
    settings.sync();
}



//读取参数信息
void MainWindow::loadSettingsFromIniFile()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/sscom.ini"; // 拼接当前路径和文件名

        QSettings settings(filePath, QSettings::IniFormat);

    // 检查 INI 文件是否存在
    if (!QFile::exists(filePath)) {
        ui->textEdit->append("检测到您第一次使用本软件");
        ui->textEdit->append("没有找到 INI 配置文件");
        ui->textEdit->append("本次退出时会自动生成");
        return;
    }else{

    // 获取窗口大小
    QSize windowSize = settings.value("Section/MainWindowSize", QSize(800, 600)).toSize();
    resize(windowSize);

    // 获取CheckBox状态
    bool isChecked1 = settings.value("Section/english", false).toBool();
    english->setChecked(isChecked1);
    bool isChecked2 = settings.value("Section/zuiqian", false).toBool();
    zuiqian->setChecked(isChecked2);
    bool isChecked3 = settings.value("Section/hexshow", false).toBool();
    hexshow->setChecked(isChecked3);
    bool isChecked4 = settings.value("Section/rectofile", false).toBool();
    rectofile->setChecked(isChecked4);
    bool isChecked5 = settings.value("Section/hexsend", false).toBool();
    hexsend->setChecked(isChecked5);
    bool isChecked6 = settings.value("Section/timepack", false).toBool();
    timepack->setChecked(isChecked6);
    bool isChecked7 = settings.value("Section/timesend", false).toBool();
    timesend->setChecked(isChecked7);
    bool isChecked8 = settings.value("Section/exchange", false).toBool();
    exchange->setChecked(isChecked8);


    // 获取ComboBox当前选项
    int currentIndex1 = settings.value("Section/wei", 0).toInt();
    wei->setCurrentIndex(currentIndex1);
    int currentIndex2 = settings.value("Section/number", 0).toInt();
    number->setCurrentIndex(currentIndex2);
    int currentIndex3 = settings.value("Section/portbox", 0).toInt();
    portbox->setCurrentIndex(currentIndex3);
    int currentIndex4 = settings.value("Section/choosebit", 0).toInt();
    choosebit->setCurrentIndex(currentIndex4);

    // 获取LineEdit文本内容
    QString text1 = settings.value("Section/filepath", "").toString();
    filepath->setText(text1);
    QString text2 = settings.value("Section/whattime", "").toString();
    whattime->setText(text2);
    QString text3 = settings.value("Section/whatbit", "").toString();
    whatbit->setText(text3);
    QString text4= settings.value("Section/latetime", "").toString();
    latetime->setText(text4);
    }
}

