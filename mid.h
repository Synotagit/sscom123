#ifndef MID_H
#define MID_H

#include <QWidget>
#include <QTranslator>

namespace Ui {
class Mid;
}

class Mid : public QWidget
{
    Q_OBJECT

public:
    explicit Mid(QWidget *parent = nullptr);
    ~Mid();


private:
    Ui::Mid *ui;
    QTranslator tran;
    QApplication *qapp;

private slots:
    void changeEvent(QEvent *e) override;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // MID_H
