#ifndef PLAYING_H
#define PLAYING_H

#include <QWidget>
#include "base/PlayerInfo.h"
#include "showncard.h"
#include <QMouseEvent>

namespace Ui {
class Playing;
}

class Playing : public QWidget
{
    Q_OBJECT

public:
    explicit Playing(PlayerInfo *m, QWidget *parent = 0);
    ~Playing();
    void mousePressEvent(QMouseEvent *event);


private:
    Ui::Playing *ui;
    PlayerInfo *mMe;

    ShownCard *RightShow;

    int NowWindowWidth;
    int NowWindowHeight;

    int mHandCardWidth;
    int mHandCardHeight;
    int mInnerCardWidth;
    int mInnerCardHeight;

    int mInnerCardHeightLeftTop1;
    int mInnerCardHeightLeftTop2;
    int mInnerCardHeightLeftTop3;
    int mInnerCardHeightLeftTop4;
    int mInnerCardHeightLeftTop5;
    int mInnerCardHeightLeftTop6;

    int mHandCardHeightEnemyTop;
    int mHandCardHeightMeTop;

    int mCardInnerWidthAll;
    int mCardHandWidthAll;

    int mCardInnerLeft;
    int mCardHandLeft;

    vector<ShownCard*> sMeHand;
    vector<ShownCard*> sMeNearestSelf;
    vector<ShownCard*> sMeMediumSelf;
    vector<ShownCard*> sMeFarestSelf;
    vector<ShownCard*> sEnemyFarestSelf;
    vector<ShownCard*> sEnemyMediumSelf;
    vector<ShownCard*> sEnemyNearestSelf;
    vector<ShownCard*> sEnemyHand;

    ShownCard *sMeLeader;
    ShownCard *sEnemyLeader;

    void ShowMeWeather(PlayerInfo *mMe);
    void ShowMeScore(PlayerInfo *mMe);
    void ShowMeLeader(PlayerInfo *mMe);
    void ShowMeHandCard(PlayerInfo *mMe);
    void ShowMeNearestSelf(PlayerInfo *mMe);
    void ShowMeMediumSelf(PlayerInfo *mMe);
    void ShowMeFarestSelf(PlayerInfo *mMe);

    void ShowEnemyWeather(PlayerInfo *mEnemy);
    void ShowEnemyScore(PlayerInfo *mEnemy);
    void ShowEnemyLeader(PlayerInfo *mEnemy);
    void ShowEnemyHandCard(PlayerInfo *mEnemy);
    void ShowEnemyNearestSelf(PlayerInfo *mEnemy);
    void ShowEnemyMediumSelf(PlayerInfo *mEnemy);
    void ShowEnemyFarestSelf(PlayerInfo *mEnemy);

    void ChangeMeHandCardPosition();
    void ChangeMeNearestSelfCardPosition();
    void ChangeMeMediumSelfCardPosition();
    void ChangeMeFarestSelfCardPosition();

    void ChangeEnemyHandCardPosition();
    void ChangeEnemyNearestSelfCardPosition();
    void ChangeEnemyMediumSelfCardPosition();
    void ChangeEnemyFarestSelfCardPosition();

public slots:
    void ChangeSize(int, int);
    void ShowPlayingInfoMe(PlayerInfo *mMe);
    void ShowPlayingInfoEnemy(PlayerInfo *mEnemy);

    void ClickedCard(Card*);
    void RightShowCard(Card*);

signals:
    void PlayingClickedCard(Card*);
    void PlayingMouseClicked(int, int);
    void PassPressed();

    friend class GameController;

public:
    int GetInnerPositionIndex(int rownum, int x);
    void SetLabelText(QString text);
    int GetMeScore(PlayerInfo *mMe);
    int GetEnemyScore(PlayerInfo *mEnemy);
    void ShowTimer(int CountDown);

private slots:
    void on_pushButton_clicked();
};

#endif // PLAYING_H
