#ifndef OPENKEG_H
#define OPENKEG_H

#include <QWidget>
#include "base/PlayerInfo.h"

namespace Ui {
class OpenKeg;
}

class OpenKeg : public QWidget
{
    Q_OBJECT

public:
    explicit OpenKeg(PlayerInfo *m, QWidget *parent = 0);
    ~OpenKeg();

private slots:
    void on_pushButton_open_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::OpenKeg *ui;
    PlayerInfo *mMe;

    void ShowInfo();

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();
    void PressedOpen();
};

#endif // OPENKEG_H
