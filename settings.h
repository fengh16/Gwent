#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "base/PlayerInfo.h"

namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(PlayerInfo *m, QWidget *parent = 0);
    ~Settings();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::Settings *ui;
    PlayerInfo *mMe;

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();
    void PressedSave(string);
};

#endif // SETTINGS_H
