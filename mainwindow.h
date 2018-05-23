#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "base/GameController.h"
#include "load.h"
#include "login.h"
#include "choosing.h"

#include "friends.h"
#include "deckbuilder.h"
#include "playing.h"
#include "openkeg.h"
#include "buykeg.h"
#include "settings.h"
#include "reward.h"
#include "collection.h"

#include "choosefromlist.h"
#include "deckbuildingchoose.h"

#include "cardshowclass.h"
#include "switchhandcard.h"

#include "ansshow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameController *g;
    Load *m_load;
    Login *m_login;
    Choosing *m_choosing;

    Friends *m_friends;
    DeckBuilder *m_deckbuilder;
    Playing *m_playing;
    OpenKeg *m_openkeg;
    BuyKeg *m_buykeg;
    Settings *m_settings;
    Reward *m_reward;
    Collection *m_collection;

    ChooseFromList *m_choosefromlist;
    DeckBuildingChoose *m_deckbuildingchoose;

    CardShowClass *m_cardshowclass;
    SwitchHandCard *m_switchhandcard;

    AnsShow *m_ansshow;

    void HideAllWidget();
    void resizeEvent(QResizeEvent *event);

public slots:
    void LoadPage();
    void LogInPage();
    void ChoosingPage();

    void FriendsPage(PlayerInfo* mMe);
    void DeckBuildingPage(PlayerInfo* mMe);
    void PlayingPage(PlayerInfo* mMe);
    void OpenKegPage(PlayerInfo* mMe);
    void BuyKegPage(PlayerInfo* mMe);
    void SettingPage(PlayerInfo* mMe);
    void RewardingPage(PlayerInfo* mMe);
    void CollectionPage(PlayerInfo* mMe);

    void ChooseLeaderPage(PlayerInfo* mMe);
    void DeckBuildingChoosePage(int, PlayerInfo*);
    void DeckChange(PlayerInfo*, int);

    void ChooseDeckPage(vector<string>, vector<string>);
    void ChangeHandCardPre(int, PlayerInfo*);
    void ChangeHandCard(int, PlayerInfo*, int);
    void GotSwitchInfo(int, int, PlayerInfo*, int);

    void Set_01_ChooseList();
    void Set_10_ChooseFromWeathers();
    void Set_12_MainChoose(vector<string>);
    void Set_20_MainChoose1(vector<string>);
    void Set_20_MainChoose2(vector<string>);

    void ShowAnsPage(bool, int, int, int, int, int, int, int, int);

    void DeleteChooseList();

    void ClosedAllWidget();

    void ChangeHandCardOnly(int nums, PlayerInfo* mMe);

signals:
    void ChangedSize(int, int);
    void IOPrepareToPlay();
    void ChangedCards(Playing*);
};

#endif // MAINWINDOW_H
