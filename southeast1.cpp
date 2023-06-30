#include "southeast1.h"
#include "ui_southeast1.h"
#include <QPainter>



Southeast1::Southeast1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Southeast1)
{
    ui->setupUi(this);
}

Southeast1::~Southeast1()
{
    delete ui;
}


void Southeast1::handleCheckBoxStateChanged()
{
    if (MainWindow->english->isChecked())
    {
         tran.load(":/english.qm");
    }
    else
    {
        tran.load(":/chinese.qm");
    }
     qapp->installTranslator(&tran);
}


//信号处理函数
void Southeast1::changeEvent(QEvent *e)
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

//为分区添加灰色边框
void Southeast1::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawRect(0,0,width()-1,height()-1);
}
