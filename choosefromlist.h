#ifndef CHOOSEFROMLIST_H
#define CHOOSEFROMLIST_H

#include <QWidget>
#include "showncard.h"
#include "base/PlayerInfo.h"
#include <vector>
#include <string>

using namespace std;

namespace Ui {
class ChooseFromList;
}

class ChooseFromList : public QWidget
{
    Q_OBJECT

public:
    explicit ChooseFromList(ChooseFunctionFrom fromid, vector<string> list, QWidget *parent = 0);
    explicit ChooseFromList(ChooseFunctionFrom fromid, vector<string> list, vector<string> info, QWidget *parent = 0);
    ~ChooseFromList();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();

private:
    Ui::ChooseFromList *ui;
    vector<string> mList;
    vector<string> mInfo;
    int m_page;
    ShownCard *s[3];
    QWidget *widget[3];
    QLabel *labels[3];
    int pageMax;
    int totalNum;
    ChooseFunctionFrom mFromid;
    int WindowWidth;
    int WindowHeight;

    void LoadPicture();
    void unLoadPicture();

    // 不知道是不是这里引发了bug……重复发送重复删除？
    bool HavePressed;

    void Pressed(int);

public slots:
    void Pressed1();
    void Pressed2();
    void Pressed3();
    void ChangeSize(int, int);

signals:
    void PressedInt(ChooseFunctionFrom, int);
};

#endif // CHOOSEFROMLIST_H
