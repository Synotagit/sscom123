#include "southeast.h"
#include "ui_southeast.h"
#include "mid.h"
#include <QPainter>


Southeast::Southeast(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Southeast)
{
    ui->setupUi(this);



}

Southeast::~Southeast()
{
    delete ui;
}

void Southeast::handleCheckBoxStateChanged()
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
void Southeast::changeEvent(QEvent *e)
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
void Southeast::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawRect(0, 0, width() - 1, height() - 1);
    QWidget::paintEvent(event);
}
