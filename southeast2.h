#ifndef SOUTHEAST2_H
#define SOUTHEAST2_H

#include <QWidget>

namespace Ui {
class Southeast2;
}

class Southeast2 : public QWidget
{
    Q_OBJECT

public:
    explicit Southeast2(QWidget *parent = nullptr);
    ~Southeast2();

private:
    Ui::Southeast2 *ui;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif // SOUTHEAST2_H
