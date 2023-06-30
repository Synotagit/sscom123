#include "mid.h"
#include "ui_mid.h"
#include <QPainter>

Mid::Mid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Mid)
{
    ui->setupUi(this);



    //english勾选
    connect(ui->english, &QCheckBox::stateChanged, this, [this](int state) {
        if(state == Qt::Checked){
            tran.load(":/english.qm");
        }else if(state == Qt::Unchecked){
            tran.load(":/chinese.qm");
        }
        //发送event信号
        qapp->installTranslator(&tran);
    });



}





Mid::~Mid()
{
    delete ui;
}

//信号处理函数
void Mid::changeEvent(QEvent *e)
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
void Mid::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawRect(0,0,width()-1,height()-1);
}
