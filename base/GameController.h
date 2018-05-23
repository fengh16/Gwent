#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "CONST.h"
#include "PlayerInfo.h"
#include "DataControl.h"
#include <QObject>
#include "iobase.h"
#include "playing.h"
#include <QTimer>

class GameController : public QObject
{
    Q_OBJECT

public:
    GameStates mNowGameState;
    NowStates mNowStates;
    PlayerInfo mMe;
    PlayerInfo mEnemy;
    int mNowRound;
    Playing *m_playing;
    Card* m_nowusecard;
    Card* m_eatCard;
    PlayerInfo *FirstPerson;
    QTimer *timer;
    int CountDown;

    void clearAllData();

public:
    GameController() : mNowGameState(cGameStatesNum), mNowStates(cLoading), mMe(PlayerInfo(0, 0, "", "", 0, 0, "", "")), mEnemy(PlayerInfo(0, 0, "", "", 0, 0, "", "")), mNowRound(0), m_eatCard(nullptr), FirstPerson(nullptr), timer(nullptr)
    {
        CountDown = 30;
        srand((unsigned int)time(NULL));
        connect(this, SIGNAL(PutOutGoldCard()), this, SLOT(FindAllRoaches()));
        connect(this, SIGNAL(UseSetOver()), this, SLOT(GotUseSetOver()));
        connect(this, SIGNAL(EatOthers()), this, SLOT(FindBigSpider()));
        connect(this, SIGNAL(ClearedFog()), this, SLOT(CheckClearedAllFogs()));
        connect(this, SIGNAL(PlacedFog(int)), this, SLOT(FindAllFoglets(int)));
        connect(this, SIGNAL(NewMeTurn()), this, SLOT(FindAllVranWarrior()));
        connect(this, SIGNAL(EndRound()), this, SLOT(NewRoundCheck()));
    }
    bool FindCardIn(Card* i, vector<Card*> b);
    void SelectAndPut(Card*);
    void PutCard(int row, int index, Card* card);
    void PutCardOnly(int row, int index, Card* card);
    void Set22GotEnemyCard(Card* card, int row);
    void UpdatePlayingShowNow();

    void Set_01_01();
    void Set_01_02();
    void Set_20_01();
    void Set_20_02_FromMe();
    void Set_20_02_FromEnemy();
    void Game_26_Eat_Right(Card* c);
    void Set_24_Eat_2Friend(Card*);
    void Set_24_Eat_1Friend(Card*);

    void PutTwoEggs(int row, int index);

    void Set_01_ChooseACard();
    void Set_02_ChooseWhichRow();
    void Set_03_ChooseWhichRow();
    void Set_04_ChooseWhichRow();
    void Set_05_ChooseWhichRowToHit();
    void Set_06_ChooseWhichCard_Neighboring3();
    void Set_07_ChooseWhichCard_Neighboring5();
    void Set_08_ChangeBiggestAndSmallest();
    void Set_09_CheckEnemyRowAllBlood_20(Card*);
    void Set_10_SwitchFromWeathers();
    void Set_12_GetGoldAndSilver();
    void Set_13_Get2Eggs(Card*);
    void Set_15_Get3AngryWolf(Card*);
    void Set_16_GetShield(Card*);
    void Set_17_Send_Crone();
    void Set_18_Send_Crone();
    void Set_19_Send_Crone();
    void Set_20_Remove_Weather(Card*);
    void Set_21_Rise_Weather(Card*);
    void Set_22_Move_1Unit(Card*);
    void Set_23_Get_OneNewCard(Card*);
    void Set_24_Eat_3Friend(Card*);
    void Set_25_Putout_Arachas();
    void Set_26_Eat_Right(Card*);
    void Set_27_Get_2Shield(Card*);
    void Set_506_Put_All_Spider();

    void SendOutCrone(vector<Card*> Crone);

    void DecreaseCardBlood(Card*, int);
    void GiveCardShieldAndBlood(Card* chosen, int Shield, int Blood, int NeighborLeftNum);

    void DealWithDie();
    void ToGrave(Card*);

    void StartNewRound(PlayerInfo*);
    void ClearForAnotherRound();

public slots:
    void NewGame();
    void LogIn();
    void LoggedIn(std::string, std::string);

    void Friends();
    void DeckBuild();
    void MainGaming();
    void OpenKegs();
    void BuyKegs();
    void Settings();
    void Rewarding();
    void Collection();

    void ChooseLeaderPage();
    void ChoosePagePressed(ChooseFunctionFrom,int);

    void SaveNewDeck(DeckInfo);
    void SaveChangedDeck(DeckInfo, int);

    void ChangeDecksInfo(int);

    void UpdateUserDataOnly();
    void UpdatePasswordOnly(string);

    void PrepareToPlay();

    void ShowMainPlayingInfo(Playing*);

    PlayerInfo* ChooseWhoFirst();

    void NewTurn(PlayerInfo*);
    void ChosenCard(Card*);
    void MouseClickedEmpty(int, int);

    void UseSet(Card*);

    void GotUseSetOver();

    void Set01Chosen(eChooseFunctionFrom,int);
    void Set10Chosen(eChooseFunctionFrom,int);
    void Set12Chosen(eChooseFunctionFrom, int p);
    void Set20Chosen1(eChooseFunctionFrom,int p);
    void Set20Chosen2(eChooseFunctionFrom,int p);

    void FindBigSpider();
    void FindAllVranWarrior();
    void FindAllRoaches();
    void FindAllFoglets(int);
    void CheckClearedAllFogs();

    void PressedPass();
    void NewRoundCheck();

    void timerDone();

signals:
    void IOStartGame();
    void IOLogIn();
    void IOLoggedIn();

    void IOFriends(PlayerInfo*);
    void IODeckBuilding(PlayerInfo*);
    void IOPlaying(PlayerInfo*);
    void IOOpenKeg(PlayerInfo*);
    void IOBuyKeg(PlayerInfo*);
    void IOSetting(PlayerInfo*);
    void IORewarding(PlayerInfo*);
    void IOCollection(PlayerInfo*);

    void IOChooseLeader(PlayerInfo*);
    void IODeckBuildingChoose(int, PlayerInfo*);

    void IODeckChange(PlayerInfo*, int);

    void IOChooseDeck(vector<string>, vector<string>);
    void IOChosenDeck(int, PlayerInfo*);

    void UseSetOver();
    void NewMeTurn();
    void PutOutGoldCard();
    void ClearedFog();
    void PlacedFog(int);
    void EatOthers();

    void Set_01_Choose();
    void ChooseFromWeatherCards_10();
    void Set_12_Choose(vector<string>);
    void Set20_ChooseFromMyGrave(vector<string>);
    void Set20_ChooseFromEnemyGrave(vector<string>);

    void ReceivedChosenCard();
    void EndRound();
    void GameOver(bool, int, int, int, int, int, int, int, int);

    void EmitChangeHandCardOnly(int, PlayerInfo*);

    friend class MainWindow;
};

#endif
