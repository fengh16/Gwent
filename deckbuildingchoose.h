#ifndef DECKBUILDINGCHOOSE_H
#define DECKBUILDINGCHOOSE_H

#include <QWidget>
#include "showncard.h"
#include <QMessageBox>
#include "base/PlayerInfo.h"
#include <map>
#include <vector>

using namespace std;

namespace Ui {
class DeckBuildingChoose;
}

class DeckBuildingChoose : public QWidget
{
    Q_OBJECT

public:
    explicit DeckBuildingChoose(int tarlead, PlayerInfo *m, QWidget *parent = 0);
    explicit DeckBuildingChoose(PlayerInfo *m, int index, QWidget *parent = 0);
    ~DeckBuildingChoose();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_confirm_clicked();

private:
    Ui::DeckBuildingChoose *ui;
    int mTargetLead;
    int m_page;
    ShownCard *s[8];
    ShownCard *slead;
    QWidget *widget[8];
    int pageMax;
    PlayerInfo *mMe;
    Card *mLead;
    int mNowGoldNum;
    int mNowSilverNum;
    int mNowBronzeNum;
    int mNowAllNum;
    int mTotalCardNum;
    vector<int> mAvailable;
    map<int, int> mChosenCard;
    bool firstpress;
    bool mNewDeckBuilding;
    int mIndex;
    bool mInit;
    int WindowWidth;
    int WindowHeight;

    void LoadPicture();
    void unLoadPicture();
    bool Checked();
    void GetAvailableCards();
    void UpdateText();
    void Save();

    void Pressed(int i);

signals:
    void PressedBack();
    void PressedChangeBack();
    void PressedSave(DeckInfo);
    void ChangePressedSave(DeckInfo, int);

public slots:
    void Pressed1();
    void Pressed2();
    void Pressed3();
    void Pressed4();
    void Pressed5();
    void Pressed6();
    void Pressed7();
    void Pressed8();
    void ChangeSize(int, int);
};

#endif // DECKBUILDINGCHOOSE_H
