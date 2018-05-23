#ifndef SWITCHHANDCARD_H
#define SWITCHHANDCARD_H

#include <QWidget>
#include "showncard.h"
#include "base/PlayerInfo.h"
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class SwitchHandCard;
}

class SwitchHandCard : public QWidget
{
    Q_OBJECT

public:
    explicit SwitchHandCard(PlayerInfo *m, int CardsNum, QWidget *parent = 0, int nowPage = 1);
    ~SwitchHandCard();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();

    void on_pushButton_clicked();

private:
    Ui::SwitchHandCard *ui;
    PlayerInfo *mMe;
    int m_page;
    ShownCard *s[3];
    QWidget *widget[3];
    QLabel *labels[3];
    int pageMax;
    int totalNum;
    int SwitchCardNum;
    int WindowWidth;
    int WindowHeight;

    void LoadPicture();
    void unLoadPicture();

    bool HavePressed;

    void Pressed(int);

public slots:
    void Pressed1();
    void Pressed2();
    void Pressed3();
    void ChangeSize(int, int);

signals:
    void PressedInt(int, int, PlayerInfo*, int);
};

#endif // SWITCHHANDCARD_H
