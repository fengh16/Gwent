#ifndef REWARD_H
#define REWARD_H

#include <QWidget>
#include "base/PlayerInfo.h"

namespace Ui {
class Reward;
}

class Reward : public QWidget
{
    Q_OBJECT

public:
    explicit Reward(PlayerInfo *m, QWidget *parent = 0);
    ~Reward();

private:
    Ui::Reward *ui;
    PlayerInfo *mMe;

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();

private slots:
    void on_pushButton_back_clicked();
};

#endif // REWARD_H
