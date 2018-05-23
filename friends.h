#ifndef FRIENDS_H
#define FRIENDS_H

#include <QWidget>
#include "base/PlayerInfo.h"

namespace Ui {
class Friends;
}

class Friends : public QWidget
{
    Q_OBJECT

public:
    explicit Friends(PlayerInfo *m, QWidget *parent = 0);
    ~Friends();

private:
    Ui::Friends *ui;
    PlayerInfo *mMe;

public slots:
    void ChangeSize(int, int);
};

#endif // FRIENDS_H
