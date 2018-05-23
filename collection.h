#ifndef COLLECTION_H
#define COLLECTION_H

#include <QWidget>
#include "showncard.h"
#include "base/PlayerInfo.h"

namespace Ui {
class Collection;
}

class Collection : public QWidget
{
    Q_OBJECT

public:
    explicit Collection(PlayerInfo *m, QWidget *parent = 0);
    ~Collection();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_buy_clicked();

private:
    Ui::Collection *ui;
    int m_page;
    ShownCard *s[10];
    QWidget *widget[10];
    int pageMax;
    PlayerInfo *mMe;
    int WindowWidth;
    int WindowHeight;

    void LoadPicture();
    void unLoadPicture();

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();
    void PressedBuy();
};

#endif // COLLECTION_H
