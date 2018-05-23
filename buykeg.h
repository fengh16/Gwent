#ifndef BUYKEG_H
#define BUYKEG_H

#include <QWidget>
#include "base/PlayerInfo.h"

namespace Ui {
class BuyKeg;
}

class BuyKeg : public QWidget
{
    Q_OBJECT

public:
    explicit BuyKeg(PlayerInfo *m, QWidget *parent = 0);
    ~BuyKeg();

private slots:
    void on_pushButton_back_clicked();
    void on_pushButton_buy_clicked();

private:
    Ui::BuyKeg *ui;
    PlayerInfo *mMe;

    void ShowInfo();

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();
    void PressedBuy();
};

#endif // BUYKEG_H
