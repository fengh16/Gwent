#ifndef CARDSHOWCLASS_H
#define CARDSHOWCLASS_H

#include <QWidget>
#include "showncard.h"
#include "base/PlayerInfo.h"
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class CardShowClass;
}

class CardShowClass : public QWidget
{
    Q_OBJECT

public:
    explicit CardShowClass(vector<string> list, QWidget *parent = 0);
    ~CardShowClass();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();
    void on_pushButton_back_clicked();

private:
    Ui::CardShowClass *ui;
    vector<string> mList;
    int m_page;
    ShownCard *s[3];
    QWidget *widget[3];
    int pageMax;
    int totalNum;

    void LoadPicture();
    void unLoadPicture();

public slots:
    void ChangeSize(int, int);

signals:
    void PressedBack();
};

#endif // CARDSHOWCLASS_H
