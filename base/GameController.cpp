#include "GameController.h"
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include "playing.h"
#include <QMessageBox>

void GameController::UpdatePlayingShowNow()
{
    DealWithDie();
    m_playing->ShowPlayingInfoMe(&mMe);
    m_playing->ShowPlayingInfoEnemy(&mEnemy);
}

void GameController::NewTurn(PlayerInfo* p)
{
    if(timer != nullptr)
    {
        delete timer;
    }
    CountDown = 30;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));
    timer->start(1000);
    if(p == &mMe)
    {
        mEnemy.ApplyWeatherHit();
        UpdatePlayingShowNow();
        if(mMe.mPassed)
        {
            m_playing->SetLabelText("已经放弃，另一方出牌！");
            NewTurn(&mEnemy);
            return;
        }
        emit(NewMeTurn());
        if(mMe.mHand.size() == 0 && mMe.HaveUsedLeader)
        {
            PressedPass();
        }
        if(mMe.mUserName == "")
        {
            m_playing->SetLabelText("B方回合，请选择一张手牌");
        }
        else
        {
            m_playing->SetLabelText("A方回合，请选择一张手牌");
        }
        mNowGameState = cChoosingCards;
        return;
    }
    mMe.ApplyWeatherHit();
    auto temp = mMe;
    mMe = mEnemy;
    mEnemy = temp;
    if(mMe.mPassed)
    {
        m_playing->SetLabelText("已经放弃，另一方出牌！");
        NewTurn(&mEnemy);
        return;
    }
    UpdatePlayingShowNow();
    emit(NewMeTurn());
    if(mMe.mHand.size() == 0 && mMe.HaveUsedLeader)
    {
        PressedPass();
    }
    if(mMe.mUserName == "")
    {
        m_playing->SetLabelText("B方回合，请选择一张手牌");
    }
    else
    {
        m_playing->SetLabelText("A方回合，请选择一张手牌");
    }
    mNowGameState = cChoosingCards;
}

PlayerInfo* GameController::ChooseWhoFirst()
{
    connect(m_playing, SIGNAL(PlayingClickedCard(Card*)), this, SLOT(ChosenCard(Card*)));
    connect(m_playing, SIGNAL(PlayingMouseClicked(int,int)), this, SLOT(MouseClickedEmpty(int, int)));
    if(rand() % 2)
    {
        return &mMe;
    }
    m_playing->SetLabelText("敌方先出牌！");
    return &mEnemy;
}

void GameController::NewGame()
{
	mNowStates = cLoading;
    emit IOStartGame();
}

void GameController::LogIn()
{
    mNowStates = cLogingIn;
    emit IOLogIn();
}

void GameController::LoggedIn(string user, string psw)
{
    mNowStates = cChoosing;
    mMe.SetUserName(user);
    mMe.SetUserPassword(psw);
    auto info = IOLoadInfo(user, psw);
    mMe.SetUserInfo(info);
    mMe.GetCardInfoFromString(info);
    mMe.CardVectorToMap();
    mMe.GetLeaderList();
    emit IOLoggedIn();
}


void GameController::Friends()
{
    mNowStates = cFriends;
    emit IOFriends(&mMe);
}

void GameController::DeckBuild()
{
    mNowStates = cDeckBuilding;
    emit IODeckBuilding(&mMe);
}

void GameController::MainGaming()
{
    mNowStates = cPlaying;
    emit IOPlaying(&mMe);
}

void GameController::OpenKegs()
{
    mNowStates = cOpenKeg;
    emit IOOpenKeg(&mMe);
}

void GameController::BuyKegs()
{
    mNowStates = cBuyKeg;
    emit IOBuyKeg(&mMe);
}

void GameController::Settings()
{
    mNowStates = cSetting;
    emit IOSetting(&mMe);
}

void GameController::Rewarding()
{
    mNowStates = cRewarding;
    emit IORewarding(&mMe);
}

void GameController::Collection()
{
    mNowStates = cCollection;
    emit IOCollection(&mMe);
}

void GameController::ChooseLeaderPage()
{
    emit IOChooseLeader(&mMe);
}

void GameController::ChoosePagePressed(ChooseFunctionFrom fromid, int tar)
{
    switch (fromid)
    {
    case cBuildPageChooseLeader:
        emit IODeckBuildingChoose(tar, &mMe);
        qDebug("t");
        break;
    case cChooseDeck:
        emit IOChosenDeck(tar, &mMe);
        qDebug("ChooseDeck");
        break;
    default:
        break;
    }
    qDebug("m");
}

void GameController::SaveNewDeck(DeckInfo t)
{
    mMe.mDecks.push_back(t);
    IOUpdateDataAll(mMe.mUserName, mMe.mPassword, mMe.UpdateData());
    emit IODeckBuilding(&mMe);
}

void GameController::UpdateUserDataOnly()
{
    IOUpdateDataAll(mMe.mUserName, mMe.mPassword, mMe.UpdateData());
}

void GameController::UpdatePasswordOnly(string oldPass)
{
    IOUpdateDataAll(mMe.mUserName, oldPass, mMe.UpdateData());
}

void GameController::ChangeDecksInfo(int index)
{
    emit IODeckChange(&mMe, index);
}

void GameController::SaveChangedDeck(DeckInfo t, int index)
{
    mMe.mDecks.insert(mMe.mDecks.erase(mMe.mDecks.begin() + index), t);
    IOUpdateDataAll(mMe.mUserName, mMe.mPassword, mMe.UpdateData());
    emit IODeckBuilding(&mMe);
}

void GameController::PrepareToPlay()
{
    if(timer != nullptr)
    {
        delete timer;
        timer = nullptr;
    }

    vector<string> list;
    vector<string> info;

    for (auto deck : mMe.mDecks)
    {
        list.push_back(deck.mLeadCard);
        info.push_back(deck.mDeckIntro);
    }

    emit IOChooseDeck(list, info);
}

void GameController::ShowMainPlayingInfo(Playing* playing)
{
    if(mNowGameState == cChooseCardsOnlyEn1)
    {
        m_playing->SetLabelText("游戏即将开始！");

        NewTurn(ChooseWhoFirst());
        return;
    }
    else if(mNowGameState == cChooseCardsOnlyMe)
    {
        QMessageBox box(QMessageBox::Critical, "提示", "进入第二个人（B）的换卡阶段！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
        mNowGameState = cChooseCardsOnlyEn;
        emit(EmitChangeHandCardOnly(1, &mEnemy));
        return;
    }
    else if(mNowGameState == cChooseCardsOnlyEn)
    {
        m_playing->SetLabelText("游戏即将开始！");
        NewTurn(FirstPerson);
        return;
    }
    m_playing = playing;
    mNowRound = 1;
    for (auto i : mMe.mHand)
    {
        mEnemy.mDeckLeft.push_back(GetNewCard(i->GetIndex()));
    }
    for (auto i : mMe.mDeckLeft)
    {
        mEnemy.mDeckLeft.push_back(GetNewCard(i->GetIndex()));
    }
    random_shuffle(mEnemy.mDeckLeft.begin(), mEnemy.mDeckLeft.end());
    for(int i = 0; i < 11; i++)
    {
        mEnemy.mHand.push_back(mEnemy.mDeckLeft[mEnemy.mDeckLeft.size() - 1]);
        mEnemy.mDeckLeft.erase(mEnemy.mDeckLeft.end() - 1);
    }
    mEnemy.mLeader = GetNewCard(mMe.mLeader->GetIndex());
    UpdatePlayingShowNow();
    QMessageBox box(QMessageBox::Critical, "提示", "进入第二个人（B）的换卡阶段！");
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    box.exec();
    mNowGameState = cChooseCardsOnlyEn1;
    emit(EmitChangeHandCardOnly(3, &mEnemy));
}

#include <QDebug>
void GameController::ChosenCard(Card* chosen)
{
    qDebug() << "ChosenCard!";
    m_playing->RightShowCard(chosen);
    switch (mNowGameState) {
    case cChoosingCards:       
        if(chosen == mMe.mLeader)
        {
            mMe.HaveUsedLeader = true;
        }
        if(FindCardIn(chosen, mMe.mHand) || chosen == mMe.mLeader)
        {
            mNowGameState = cChoosingPlaceToPut;
            SelectAndPut(chosen);
            return;
        }
        break;
    case cChooseNeighbor3For3And3:
        GiveCardShieldAndBlood(chosen, 2, 3, 1);
        break;
    case cChooseNeighbor5For4:
        GiveCardShieldAndBlood(chosen, 0, 4, 2);
        break;
    case cSet22ChooseEnemyCard:
        if(FindCardIn(chosen, mEnemy.mFirstRow))
        {
            Set22GotEnemyCard(chosen, 1);
            break;
        }
        else if(FindCardIn(chosen, mEnemy.mSecondRow))
        {
            Set22GotEnemyCard(chosen, 2);
            break;
        }
        else if(FindCardIn(chosen, mEnemy.mThirdRow))
        {
            Set22GotEnemyCard(chosen, 3);
            break;
        }
        break;
    case cUnseenElder_Eat1:
        if(chosen->GetIndex() == 24)
        {
            return;
        }
        if(FindCardIn(chosen, mMe.mFirstRow) || FindCardIn(chosen, mMe.mSecondRow) || FindCardIn(chosen, mMe.mThirdRow))
        {
            if(chosen->GetIndex() == 502)
            {
                m_eatCard->mCardBloodNow += 4;
            }
            m_eatCard->mCardBloodNow += chosen->mCardBloodNow;
            chosen->mCardBloodNow = 0;
            UpdatePlayingShowNow();
            emit(EatOthers());

            return;
        }
        return;
    case cUnseenElder_Eat2:
        if(chosen->GetIndex() == 24)
        {
            return;
        }
        if(FindCardIn(chosen, mMe.mFirstRow) || FindCardIn(chosen, mMe.mSecondRow) || FindCardIn(chosen, mMe.mThirdRow))
        {
            if(chosen->GetIndex() == 502)
            {
                m_eatCard->mCardBloodNow += 4;
            }
            m_eatCard->mCardBloodNow += chosen->mCardBloodNow;
            chosen->mCardBloodNow = 0;
            mNowGameState = cUnseenElder_Eat1;
            UpdatePlayingShowNow();
            Set_24_Eat_1Friend(m_eatCard);
            emit(EatOthers());
            return;
        }
        return;
    case cUnseenElder_Eat3:
        if(chosen->GetIndex() == 24)
        {
            return;
        }
        if(FindCardIn(chosen, mMe.mFirstRow) || FindCardIn(chosen, mMe.mSecondRow) || FindCardIn(chosen, mMe.mThirdRow))
        {
            if(chosen->GetIndex() == 502)
            {
                m_eatCard->mCardBloodNow += 4;
            }
            m_eatCard->mCardBloodNow += chosen->mCardBloodNow;
            chosen->mCardBloodNow = 0;
            mNowGameState = cUnseenElder_Eat2;
            UpdatePlayingShowNow();
            Set_24_Eat_2Friend(m_eatCard);
            emit(EatOthers());
            return;
        }
        return;
    default:
        break;
    }
}

void GameController::MouseClickedEmpty(int x, int y)
{
    qDebug("ClickedRecieved!");
    int m;
    switch (mNowGameState) {
    case cChoosingPlaceToPut:
        qDebug("ClickedPutCard!");
        // 距离自己最远的那排，第四排
        if((m_nowusecard->mCardPutPosition == cPutAll || m_nowusecard->mCardPutPosition == cPutCenterNearest) && x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop4 && y < m_playing->mInnerCardHeightLeftTop4 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(4, x);
            PutCard(4, m, m_nowusecard);
        }
        // 第五排
        if((m_nowusecard->mCardPutPosition == cPutAll || m_nowusecard->mCardPutPosition == cPutMedium) && x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop5 && y < m_playing->mInnerCardHeightLeftTop5 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(5, x);
            PutCard(5, m, m_nowusecard);
        }
        // 第六排，距离自己最近
        if((m_nowusecard->mCardPutPosition == cPutAll || m_nowusecard->mCardPutPosition == cPutFarest) && x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop6 && y < m_playing->mInnerCardHeightLeftTop6 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(6, x);
            PutCard(6, m, m_nowusecard);
        }
        break;
    case cChoosingEnemyPlaceToPut:
        // 间谍单位
        qDebug() << "spy";
        // 距离敌方最远的那排，第三排
        if(x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop3 && y < m_playing->mInnerCardHeightLeftTop3 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(3, x);
            PutCard(3, m, m_nowusecard);
        }
        // 第二排
        if(x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop2 && y < m_playing->mInnerCardHeightLeftTop2 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(2, x);
            PutCard(2, m, m_nowusecard);
        }
        // 第一排，距离敌方最近
        if(x > m_playing->mCardInnerLeft - 200 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 200 && y > m_playing->mInnerCardHeightLeftTop1 && y < m_playing->mInnerCardHeightLeftTop1 + m_playing->mInnerCardHeight)
        {
            m = m_playing->GetInnerPositionIndex(1, x);
            PutCard(1, m, m_nowusecard);
        }
        break;
    case cChooseEnemyRowForFrost:
        // 距离敌方最远的那排，第三排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop3 && y < m_playing->mInnerCardHeightLeftTop3 + m_playing->mInnerCardHeight)
        {
            mEnemy.FirstWeather.kind = Weather_Frost;
            mEnemy.FirstWeather.Hit = 2;
        }
        // 第二排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop2 && y < m_playing->mInnerCardHeightLeftTop2 + m_playing->mInnerCardHeight)
        {
            mEnemy.SecondWeather.kind = Weather_Frost;
            mEnemy.SecondWeather.Hit = 2;
        }
        // 第一排，距离敌方最近
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop1 && y < m_playing->mInnerCardHeightLeftTop1 + m_playing->mInnerCardHeight)
        {
            mEnemy.ThirdWeather.kind = Weather_Frost;
            mEnemy.ThirdWeather.Hit = 2;
        }
        emit(UseSetOver());
        break;
    case cChooseEnemyRowForFog:
        // 距离敌方最远的那排，第三排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop3 && y < m_playing->mInnerCardHeightLeftTop3 + m_playing->mInnerCardHeight)
        {
            mEnemy.FirstWeather.kind = Weather_Fog;
            mEnemy.FirstWeather.Hit = 2;
            emit(PlacedFog(3));
        }
        // 第二排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop2 && y < m_playing->mInnerCardHeightLeftTop2 + m_playing->mInnerCardHeight)
        {
            mEnemy.SecondWeather.kind = Weather_Fog;
            mEnemy.SecondWeather.Hit = 2;
            emit(PlacedFog(2));
        }
        // 第一排，距离敌方最近
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop1 && y < m_playing->mInnerCardHeightLeftTop1 + m_playing->mInnerCardHeight)
        {
            mEnemy.ThirdWeather.kind = Weather_Fog;
            mEnemy.ThirdWeather.Hit = 2;
            emit(PlacedFog(1));
        }
        emit(UseSetOver());
        break;
    case cChooseEnemyRowForRain:
        // 距离敌方最远的那排，第三排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop3 && y < m_playing->mInnerCardHeightLeftTop3 + m_playing->mInnerCardHeight)
        {
            mEnemy.FirstWeather.kind = Weather_Rain;
            mEnemy.FirstWeather.Hit = 1;
        }
        // 第二排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop2 && y < m_playing->mInnerCardHeightLeftTop2 + m_playing->mInnerCardHeight)
        {
            mEnemy.SecondWeather.kind = Weather_Rain;
            mEnemy.SecondWeather.Hit = 1;
        }
        // 第一排，距离敌方最近
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop1 && y < m_playing->mInnerCardHeightLeftTop1 + m_playing->mInnerCardHeight)
        {
            mEnemy.ThirdWeather.kind = Weather_Rain;
            mEnemy.ThirdWeather.Hit = 1;
        }
        emit(UseSetOver());
        break;
    case cChooseRowFor3Hit:
        // 距离敌方最远的那排，第一排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop3 && y < m_playing->mInnerCardHeightLeftTop3 + m_playing->mInnerCardHeight)
        {
            for (auto i : mEnemy.mFirstRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        // 第二排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop2 && y < m_playing->mInnerCardHeightLeftTop2 + m_playing->mInnerCardHeight)
        {
            for (auto i : mEnemy.mSecondRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        // 第三排，距离敌方最近
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop1 && y < m_playing->mInnerCardHeightLeftTop1 + m_playing->mInnerCardHeight)
        {
            for (auto i : mEnemy.mThirdRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        // 距离自己最远的那排，第四排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop4 && y < m_playing->mInnerCardHeightLeftTop4 + m_playing->mInnerCardHeight)
        {
            for (auto i : mMe.mFirstRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        // 第五排
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop5 && y < m_playing->mInnerCardHeightLeftTop5 + m_playing->mInnerCardHeight)
        {
            for (auto i : mMe.mSecondRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        // 第六排，距离自己最近
        if(x > m_playing->mCardInnerLeft - 600 && x < m_playing->mCardInnerLeft + m_playing->mCardInnerWidthAll + 600 && y > m_playing->mInnerCardHeightLeftTop6 && y < m_playing->mInnerCardHeightLeftTop6 + m_playing->mInnerCardHeight)
        {
            for (auto i : mMe.mThirdRow)
            {
                DecreaseCardBlood(i, 3);
            }
        }
        emit(UseSetOver());
        break;
    default:
        break;
    }
}

bool GameController::FindCardIn(Card* i, vector<Card*> b)
{
    for (auto t : b)
    {
        if(t == i)
        {
            return true;
        }
    }
    return false;
}

void GameController::SelectAndPut(Card* tar)
{
    mNowGameState = cChoosingPlaceToPut;
    m_playing->SetLabelText("请将这张牌放在场上某一行");
    m_playing->RightShowCard(tar);
    m_nowusecard = tar;
    if(tar->mCardBloodNow == 0)
    {
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == tar)
            {
                (*(m_playing->sMeHand.begin() + i))->hide();
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        m_playing->ShowPlayingInfoMe(&mMe);
        m_playing->ShowPlayingInfoEnemy(&mEnemy);
        mNowGameState = cChoosingTargetCards;
        UseSet(tar);
    }
    if(tar->GetIndex() == 23)
    {
        mNowGameState = cChoosingEnemyPlaceToPut;
        m_nowusecard = tar;
        m_playing->SetLabelText("请将这张牌放在敌方场上某一行");
    }
    if(tar->mCardLevel == cGold && tar->mCardKind != cLeader)
    {
        emit(PutOutGoldCard());
    }
}

void GameController::PutCardOnly(int row, int index, Card* card)
{
    switch (row) {
    case 3:
        card->mCardPosition = cCenterNearest;
        mEnemy.mFirstRow.insert(mEnemy.mFirstRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                qDebug() << "a";
                mMe.mHand.erase(mMe.mHand.begin() + i);
                qDebug() << "b";
                m_playing->sEnemyFarestSelf.insert(m_playing->sEnemyFarestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                qDebug() << "c";
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                qDebug() << "d";
                break;
            }
        }
        break;
    case 2:
        card->mCardPosition = cMedium;
        mEnemy.mSecondRow.insert(mEnemy.mSecondRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sEnemyMediumSelf.insert(m_playing->sEnemyMediumSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 1:
        card->mCardPosition = cFarest;
        mEnemy.mThirdRow.insert(mEnemy.mThirdRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sEnemyNearestSelf.insert(m_playing->sEnemyNearestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 4:
        card->mCardPosition = cCenterNearest;
        mMe.mFirstRow.insert(mMe.mFirstRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeFarestSelf.insert(m_playing->sMeFarestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 5:
        card->mCardPosition = cMedium;
        mMe.mSecondRow.insert(mMe.mSecondRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeMediumSelf.insert(m_playing->sMeMediumSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 6:
        card->mCardPosition = cFarest;
        mMe.mThirdRow.insert(mMe.mThirdRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeNearestSelf.insert(m_playing->sMeNearestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    default:
        break;
    }
}

void GameController::PutCard(int row, int index, Card* card)
{
    switch (row) {
    case 3:
        card->mCardPosition = cCenterNearest;
        mEnemy.mFirstRow.insert(mEnemy.mFirstRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                qDebug() << "a";
                mMe.mHand.erase(mMe.mHand.begin() + i);
                qDebug() << "b";
                m_playing->sEnemyFarestSelf.insert(m_playing->sEnemyFarestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                qDebug() << "c";
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                qDebug() << "d";
                break;
            }
        }
        break;
    case 2:
        card->mCardPosition = cMedium;
        mEnemy.mSecondRow.insert(mEnemy.mSecondRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sEnemyMediumSelf.insert(m_playing->sEnemyMediumSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 1:
        card->mCardPosition = cFarest;
        mEnemy.mThirdRow.insert(mEnemy.mThirdRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sEnemyNearestSelf.insert(m_playing->sEnemyNearestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 4:
        card->mCardPosition = cCenterNearest;
        mMe.mFirstRow.insert(mMe.mFirstRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeFarestSelf.insert(m_playing->sMeFarestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 5:
        card->mCardPosition = cMedium;
        mMe.mSecondRow.insert(mMe.mSecondRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeMediumSelf.insert(m_playing->sMeMediumSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    case 6:
        card->mCardPosition = cFarest;
        mMe.mThirdRow.insert(mMe.mThirdRow.begin() + index, card);
        for(auto i = 0; i < mMe.mHand.size(); i++)
        {
            if(mMe.mHand[i] == card)
            {
                mMe.mHand.erase(mMe.mHand.begin() + i);
                m_playing->sMeNearestSelf.insert(m_playing->sMeNearestSelf.begin() + index, *(m_playing->sMeHand.begin() + i));
                m_playing->sMeHand.erase(m_playing->sMeHand.begin() + i);
                break;
            }
        }
        break;
    default:
        break;
    }
    UpdatePlayingShowNow();
    mNowGameState = cChoosingTargetCards;
    UseSet(card);
}

void GameController::Set22GotEnemyCard(Card* card, int row)
{
    switch (row) {
    case 1:
        for (auto i = 0; i < mEnemy.mFirstRow.size(); i++)
        {
            if(mEnemy.mFirstRow[i] == card)
            {
                mEnemy.mFirstRow.erase(mEnemy.mFirstRow.begin() + i);
            }
        }
        break;
    case 2:
        for (auto i = 0; i < mEnemy.mSecondRow.size(); i++)
        {
            if(mEnemy.mSecondRow[i] == card)
            {
                mEnemy.mSecondRow.erase(mEnemy.mSecondRow.begin() + i);
            }
        }
        break;
    default:
        for (auto i = 0; i < mEnemy.mThirdRow.size(); i++)
        {
            if(mEnemy.mThirdRow[i] == card)
            {
                mEnemy.mThirdRow.erase(mEnemy.mThirdRow.begin() + i);
            }
        }
        break;
    }
    if(FindCardIn(m_nowusecard, mMe.mFirstRow))
    {
        mEnemy.mFirstRow.push_back(card);
    }
    else if(FindCardIn(m_nowusecard, mMe.mSecondRow))
    {
        mEnemy.mSecondRow.push_back(card);
    }
    else if(FindCardIn(m_nowusecard, mMe.mThirdRow))
    {
        mEnemy.mThirdRow.push_back(card);
    }
    else if(FindCardIn(m_nowusecard, mEnemy.mFirstRow))
    {
        mEnemy.mFirstRow.push_back(card);
    }
    else if(FindCardIn(m_nowusecard, mEnemy.mSecondRow))
    {
        mEnemy.mSecondRow.push_back(card);
    }
    else if(FindCardIn(m_nowusecard, mEnemy.mThirdRow))
    {
        mEnemy.mThirdRow.push_back(card);
    }
    emit(UseSetOver());
}

void GameController::Set_01_ChooseACard()
{
    emit(Set_01_Choose());
}

void GameController::Set01Chosen(eChooseFunctionFrom,int p)
{
    if(p == 0)
    {
        Set_01_01();
        emit(ReceivedChosenCard());
        return;
    }
    Set_01_02();
    emit(ReceivedChosenCard());
}

void GameController::Set10Chosen(eChooseFunctionFrom, int p)
{
    Card* m = GetNewCard(p + 2);
    UseSet(m);
    emit(ReceivedChosenCard());
}

void GameController::Set12Chosen(eChooseFunctionFrom, int p)
{
    Card* temp = nullptr;
    if(p == 0)
    {
        mMe.mHand[mMe.mHand.size() - 1]->mCardPosition = cDeck;
        mMe.mDeckLeft.insert(mMe.mDeckLeft.begin(), mMe.mHand[mMe.mHand.size() - 1]);
        mMe.mHand.erase(mMe.mHand.begin() + mMe.mHand.size() - 1);
        temp = mMe.mHand[mMe.mHand.size() - 1];
        mMe.mHand.erase(mMe.mHand.begin() + mMe.mHand.size() - 1);
        emit(ReceivedChosenCard());
        mNowGameState = cChoosingPlaceToPut;
        SelectAndPut(temp);
        return;
    }
    temp = mMe.mHand[mMe.mHand.size() - 1];
    mMe.mHand.erase(mMe.mHand.begin() + mMe.mHand.size() - 1);
    mMe.mHand[mMe.mHand.size() - 1]->mCardPosition = cDeck;
    mMe.mDeckLeft.insert(mMe.mDeckLeft.begin(), mMe.mHand[mMe.mHand.size() - 1]);
    mMe.mHand.erase(mMe.mHand.begin() + mMe.mHand.size() - 1);
    emit(ReceivedChosenCard());
    mNowGameState = cChoosingPlaceToPut;
    SelectAndPut(temp);
    return;
}

void GameController::Set20Chosen1(eChooseFunctionFrom,int p)
{
    emit(ReceivedChosenCard());
    for (auto i = 0; i < mMe.mGrave.size(); i++)
    {
        if(mMe.mGrave[i]->mCardLevel == cBronze)
        {
            if(p <= 0)
            {
                mEnemy.mGrave.push_back(mMe.mGrave[i]);
                mMe.mGrave.erase(mMe.mGrave.begin() + i);
                m_playing->SetLabelText("已经把你选定的牌移入对方的墓地！");
                emit(UseSetOver());
                return;
            }
            p--;
        }
    }
    emit(UseSetOver());
    return;
}

void GameController::Set20Chosen2(eChooseFunctionFrom,int p)
{
    emit(ReceivedChosenCard());
    for (auto i = 0; i < mEnemy.mGrave.size(); i++)
    {
        if(mEnemy.mGrave[i]->mCardLevel == cBronze)
        {
            if(p <= 0)
            {
                mMe.mGrave.push_back(mEnemy.mGrave[i]);
                mEnemy.mGrave.erase(mEnemy.mGrave.begin() + i);
                m_playing->SetLabelText("已经把你选定的牌移入自己的墓地！");
                emit(UseSetOver());
                return;
            }
            p--;
        }
    }
    emit(UseSetOver());
    return;
}

void GameController::Set_01_01()
{
    m_playing->SetLabelText("您使用了晴空技能！");
    if(mMe.FirstWeather.kind == Weather_Fog || mMe.SecondWeather.kind == Weather_Fog || mMe.ThirdWeather.kind == Weather_Fog)
    {
        emit ClearedFog();
    }
    mMe.ClearSky();
    emit UseSetOver();
}

void GameController::Set_01_02()
{
    m_playing->SetLabelText("您获得了一张新的牌！请把它找个位置放下吧！");
    for(auto i = 0; i < mMe.mDeckLeft.size(); i++)
    {
        if(mMe.mDeckLeft[i]->mCardKind == cUnit && mMe.mDeckLeft[i]->mCardLevel == cBronze)
        {
            mMe.mHand.push_back(mMe.mDeckLeft[i]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + i);
            break;
        }
    }
    m_playing->ShowMeHandCard(&mMe);
    SelectAndPut(mMe.mHand[mMe.mHand.size() - 1]);
}

void GameController::Set_02_ChooseWhichRow()
{
    m_playing->SetLabelText("选择对方的一列，放下你的刺骨冰霜！");
    mNowGameState = cChooseEnemyRowForFrost;
}

void GameController::Set_03_ChooseWhichRow()
{
    m_playing->SetLabelText("选择对方的一列，放下你的蔽日浓雾！");
    mNowGameState = cChooseEnemyRowForFog;
}

void GameController::Set_04_ChooseWhichRow()
{
    m_playing->SetLabelText("选择对方的一列，放下你的倾盆大雨！");
    mNowGameState = cChooseEnemyRowForRain;
}

void GameController::Set_05_ChooseWhichRowToHit()
{
    m_playing->SetLabelText("选择一列，对这一列所有单位造成3点伤害！");
    mNowGameState = cChooseRowFor3Hit;
}

void GameController::Set_06_ChooseWhichCard_Neighboring3()
{
    m_playing->SetLabelText("选择一个单位，相邻三个单位提供2护甲3增益！");
    mNowGameState = cChooseNeighbor3For3And3;
    if(mMe.mFirstRow.size() == 0 && mMe.mSecondRow.size() == 0 && mMe.mThirdRow.size() == 0 &&
            mEnemy.mFirstRow.size() == 0 && mEnemy.mSecondRow.size() == 0 && mEnemy.mThirdRow.size() == 0)
    {
        emit(UseSetOver());
    }
}

void GameController::Set_07_ChooseWhichCard_Neighboring5()
{
    m_playing->SetLabelText("选择一个单位，相邻五个单位提供4增益！");
    mNowGameState = cChooseNeighbor5For4;
    if(mMe.mFirstRow.size() == 0 && mMe.mSecondRow.size() == 0 && mMe.mThirdRow.size() == 0 &&
            mEnemy.mFirstRow.size() == 0 && mEnemy.mSecondRow.size() == 0 && mEnemy.mThirdRow.size() == 0)
    {
        emit(UseSetOver());
    }
}

void GameController::Set_08_ChangeBiggestAndSmallest()
{
    vector<Card*> Max;
    vector<Card*> Min;
    vector<Card*> allCard;
    for(auto i : mMe.mFirstRow)
    {
        allCard.push_back(i);
    }
    for(auto i : mMe.mSecondRow)
    {
        allCard.push_back(i);
    }
    for(auto i :mMe.mThirdRow)
    {
        allCard.push_back(i);
    }
    for(auto i : mEnemy.mFirstRow)
    {
        allCard.push_back(i);
    }
    for(auto i : mEnemy.mSecondRow)
    {
        allCard.push_back(i);
    }
    for(auto i :mEnemy.mThirdRow)
    {
        allCard.push_back(i);
    }
    if(allCard.size() == 0)
    {
        emit(UseSetOver());
    }
    Max.push_back(allCard[0]);
    Min.push_back(allCard[0]);
    for(auto i : allCard)
    {
        if(i == allCard[0])
            continue;
        if(i->mCardBloodNow == Max[0]->mCardBloodNow)
        {
            Max.push_back(i);
        }
        if(i->mCardBloodNow > Max[0]->mCardBloodNow)
        {
            Max.clear();
            Max.push_back(i);
        }
        if(i->mCardBloodNow == Min[0]->mCardBloodNow)
        {
            Min.push_back(i);
        }
        if(i->mCardBloodNow < Min[0]->mCardBloodNow)
        {
            Min.clear();
            Min.push_back(i);
        }
    }
    int minB = rand() % Min.size();
    int maxB = rand() % Max.size();
    int temp = Min[minB]->mCardBloodNow;
    Min[minB]->mCardBloodNow = Max[maxB]->mCardBloodNow;
    Max[maxB]->mCardBloodNow = temp;
    m_playing->SetLabelText("已经随机交换了场上最大血量和最小血量的两张牌的血量！");
    emit(UseSetOver());
}

void GameController::Set_09_CheckEnemyRowAllBlood_20(Card *c)
{
    int total = 0;
    vector<Card*> Biggest;
    if(FindCardIn(c, mMe.mFirstRow))
    {
        for(auto i : mEnemy.mFirstRow)
        {
            total += i->mCardBloodNow;
        }
        if(total > 20)
        {
            Biggest.push_back(mEnemy.mFirstRow[0]);
            for(auto i = 1; i < mEnemy.mFirstRow.size(); i++)
            {
                if((Biggest[0])->mCardBloodNow < mEnemy.mFirstRow[i]->mCardBloodNow)
                {
                    Biggest.clear();
                    Biggest.push_back(mEnemy.mFirstRow[i]);
                }
                else if((Biggest[0])->mCardBloodNow == mEnemy.mFirstRow[i]->mCardBloodNow)
                {
                    Biggest.push_back(mEnemy.mFirstRow[i]);
                }
            }
            for(auto i:Biggest)
            {
                i->mCardBloodNow = 0;
            }
        }
        emit(UseSetOver());
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        for(auto i : mEnemy.mSecondRow)
        {
            total += i->mCardBloodNow;
        }
        if(total > 20)
        {
            Biggest.push_back(mEnemy.mSecondRow[0]);
            for(auto i = 1; i < mEnemy.mSecondRow.size(); i++)
            {
                if(Biggest[0]->mCardBloodNow < mEnemy.mSecondRow[i]->mCardBloodNow)
                {
                    Biggest.clear();
                    Biggest.push_back(mEnemy.mSecondRow[i]);
                }
                else if(Biggest[0]->mCardBloodNow == mEnemy.mSecondRow[i]->mCardBloodNow)
                {
                    Biggest.push_back(mEnemy.mSecondRow[i]);
                }
            }
            for(auto i:Biggest)
            {
                i->mCardBloodNow = 0;
            }
        }
        emit(UseSetOver());
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        for(auto i : mEnemy.mThirdRow)
        {
            total += i->mCardBloodNow;
        }
        if(total > 20)
        {
            Biggest.push_back(mEnemy.mThirdRow[0]);
            for(auto i = 1; i < mEnemy.mThirdRow.size(); i++)
            {
                if(Biggest[0]->mCardBloodNow < mEnemy.mThirdRow[i]->mCardBloodNow)
                {
                    Biggest.clear();
                    Biggest.push_back(mEnemy.mThirdRow[i]);
                }
                else if(Biggest[0]->mCardBloodNow == mEnemy.mThirdRow[i]->mCardBloodNow)
                {
                    Biggest.push_back(mEnemy.mThirdRow[i]);
                }
            }
            for(auto i:Biggest)
            {
                i->mCardBloodNow = 0;
            }
        }
        emit(UseSetOver());
    }
}

void GameController::Set_10_SwitchFromWeathers()
{
    m_playing->SetLabelText("请选择并放置一个天气牌！");
    emit(ChooseFromWeatherCards_10());
}

void GameController::Set_12_GetGoldAndSilver()
{
    m_playing->SetLabelText("请选择一张牌并打出，另一张将放在牌组顶端！");
    int tempGold = -1;
    int tempSilver = -1;
    Card* tempUseCard = nullptr;
    Card* tempUseCard2 = nullptr;
    for (auto i = 0; i < mMe.mDeckLeft.size(); i++)
    {
        if(mMe.mDeckLeft[i]->mCardLevel == cGold && tempGold < 0)
        {
            tempGold = i;
            if(tempSilver >= 0)
            {
                break;
            }
        }
        if(mMe.mDeckLeft[i]->mCardLevel == cSilver && tempSilver < 0)
        {
            tempSilver = i;
            if(tempGold >= 0)
            {
                break;
            }
        }
    }
    if(tempSilver < 0 && tempGold < 0)
    {
        QMessageBox box(QMessageBox::Critical, "提示", "牌组中已经没有金卡银卡，跳过技能阶段！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
        emit(UseSetOver());
        return;
    }
    if(tempGold < 0)
    {
        QMessageBox box1(QMessageBox::Critical, "提示", "牌组中已经没有金卡，直接选择银卡打出！");
        box1.setStandardButtons (QMessageBox::Ok);
        box1.setButtonText (QMessageBox::Ok,QString("确 定"));
        box1.exec();
        tempUseCard = mMe.mDeckLeft[tempSilver];
        mMe.mHand.push_back(tempUseCard);
        tempUseCard->mCardPosition = cHand;
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempSilver);
        mNowGameState = cChoosingPlaceToPut;
        SelectAndPut(tempUseCard);
        return;
    }
    if(tempSilver < 0)
    {
        QMessageBox box2(QMessageBox::Critical, "提示", "牌组中已经没有银卡，直接选择金卡打出！");
        box2.setStandardButtons (QMessageBox::Ok);
        box2.setButtonText (QMessageBox::Ok,QString("确 定"));
        box2.exec();
        tempUseCard = mMe.mDeckLeft[tempGold];
        mMe.mHand.push_back(tempUseCard);
        tempUseCard->mCardPosition = cHand;
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempGold);
        mNowGameState = cChoosingPlaceToPut;
        SelectAndPut(tempUseCard);
        return;
    }
    tempUseCard = mMe.mDeckLeft[tempGold];
    tempUseCard2 = mMe.mDeckLeft[tempSilver];
    tempUseCard->mCardPosition = cHand;
    tempUseCard2->mCardPosition = cHand;
    if(tempGold < tempSilver)
    {
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempSilver);
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempGold);
    }
    else
    {
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempGold);
        mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + tempSilver);
    }
    mMe.mHand.push_back(tempUseCard);
    mMe.mHand.push_back(tempUseCard2);
    vector<string> s = {getstring(tempUseCard->GetIndex()), getstring(tempUseCard2->GetIndex())};
    emit(Set_12_Choose(s));
}

void GameController::Set_13_Get2Eggs(Card* c)
{
    if(FindCardIn(c, mMe.mFirstRow))
    {
        for(auto i = 0; i < mMe.mFirstRow.size(); i++)
        {
            if(mMe.mFirstRow[i] == c)
            {
                PutTwoEggs(4, i);
                return;
            }
        }
    }
    if(FindCardIn(c, mMe.mSecondRow))
    {
        for(auto i = 0; i < mMe.mSecondRow.size(); i++)
        {
            if(mMe.mSecondRow[i] == c)
            {
                PutTwoEggs(5, i);
                return;
            }
        }
    }
    if(FindCardIn(c, mMe.mThirdRow))
    {
        for(auto i = 0; i < mMe.mThirdRow.size(); i++)
        {
            if(mMe.mThirdRow[i] == c)
            {
                PutTwoEggs(6, i);
                return;
            }
        }
    }
}

void GameController::PutTwoEggs(int row, int index)
{
    Card* temp = GetNewCard(502);
    PutCardOnly(row, index, temp);
    temp = GetNewCard(502);
    PutCardOnly(row, index, temp);
    emit(UseSetOver());
}

void GameController::Set_15_Get3AngryWolf(Card* c)
{
    int i = 0;
    if(FindCardIn(c, mMe.mFirstRow))
    {
        mEnemy.FirstWeather.kind = Weather_Fog;
        mEnemy.FirstWeather.Hit = 2;
        for(i = 0; i < mMe.mFirstRow.size(); i++)
        {
            if(mMe.mFirstRow[i] == c)
            {
                break;
            }
        }
        PutCardOnly(4, i, GetNewCard(503));
        PutCardOnly(4, i, GetNewCard(503));
        PutCardOnly(4, i, GetNewCard(503));
        emit(PlacedFog(3));
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        mEnemy.SecondWeather.kind = Weather_Fog;
        mEnemy.SecondWeather.Hit = 2;
        for(i = 0; i < mMe.mSecondRow.size(); i++)
        {
            if(mMe.mSecondRow[i] == c)
            {
                break;
            }
        }
        PutCardOnly(5, i, GetNewCard(503));
        PutCardOnly(5, i, GetNewCard(503));
        PutCardOnly(5, i, GetNewCard(503));
        emit(PlacedFog(2));
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        mEnemy.ThirdWeather.kind = Weather_Fog;
        mEnemy.ThirdWeather.Hit = 2;
        for(i = 0; i < mMe.mThirdRow.size(); i++)
        {
            if(mMe.mThirdRow[i] == c)
            {
                break;
            }
        }
        PutCardOnly(6, i, GetNewCard(503));
        PutCardOnly(6, i, GetNewCard(503));
        PutCardOnly(6, i, GetNewCard(503));
        emit(PlacedFog(1));
    }
    emit(UseSetOver());
}

void GameController::Set_16_GetShield(Card *c)
{
    c->mCardSuperShield = true;
    emit(UseSetOver());
}

void GameController::Set_17_Send_Crone()
{
    vector<Card*> Crone;
    for(auto t = 0; t < mMe.mDeckLeft.size(); t++)
    {
        if(mMe.mDeckLeft[t]->GetIndex() == 18 || mMe.mDeckLeft[t]->GetIndex() == 19)
        {
            mMe.mDeckLeft[t]->mCardPosition = cHand;
            Crone.push_back(mMe.mDeckLeft[t]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + t);
        }
    }
    SendOutCrone(Crone);
}

void GameController::Set_18_Send_Crone()
{
    vector<Card*> Crone;
    for(auto t = 0; t < mMe.mDeckLeft.size(); t++)
    {
        if(mMe.mDeckLeft[t]->GetIndex() == 17 || mMe.mDeckLeft[t]->GetIndex() == 19)
        {
            mMe.mDeckLeft[t]->mCardPosition = cHand;
            Crone.push_back(mMe.mDeckLeft[t]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + t);
        }
    }
    SendOutCrone(Crone);
}

void GameController::Set_19_Send_Crone()
{
    vector<Card*> Crone;
    for(auto t = 0; t < mMe.mDeckLeft.size(); t++)
    {
        if(mMe.mDeckLeft[t]->GetIndex() == 18 || mMe.mDeckLeft[t]->GetIndex() == 17)
        {
            mMe.mDeckLeft[t]->mCardPosition = cHand;
            Crone.push_back(mMe.mDeckLeft[t]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + t);
        }
    }
    SendOutCrone(Crone);
}

void GameController::SendOutCrone(vector<Card*> Crone)
{
    if(Crone.size() == 0)
    {
        emit(UseSetOver());
        return;
    }
    for (auto c : Crone)
    {
        SelectAndPut(c);
    }
}

void GameController::Set_20_Remove_Weather(Card* c)
{
    if(FindCardIn(c, mMe.mFirstRow))
    {
        if(mMe.FirstWeather.kind == Weather_Fog)
        {
            emit(ClearedFog());
        }
        mMe.FirstWeather.Hit = 0;
        mMe.FirstWeather.kind = 0;
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        if(mMe.SecondWeather.kind == Weather_Fog)
        {
            emit(ClearedFog());
        }
        mMe.SecondWeather.Hit = 0;
        mMe.SecondWeather.kind = 0;
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        if(mMe.ThirdWeather.kind == Weather_Fog)
        {
            emit(ClearedFog());
        }
        mMe.ThirdWeather.Hit = 0;
        mMe.ThirdWeather.kind = 0;
    }
    Set_20_01();
}

void GameController::Set_20_01()
{
    QMessageBox box(QMessageBox::Warning, "提示", "您想选择谁的墓地中的铜卡移出？");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("我 方"));
    box.setButtonText (QMessageBox::Cancel,QString("敌 方"));
    if(box.exec() == QMessageBox::Ok)
    {
        Set_20_02_FromMe();
        return;
    }
    Set_20_02_FromEnemy();
}

void GameController::Set_20_02_FromMe()
{
    vector<string> targetCards;
    for(auto i : mMe.mGrave)
    {
        if(i->mCardLevel == cBronze)
        {
            targetCards.push_back(getstring(i->GetIndex()));
        }
    }
    if(targetCards.size() == 0)
    {
        QMessageBox box1(QMessageBox::Critical, "提示", "您的墓地中已经没有铜卡，无法进行操作！");
        box1.setStandardButtons (QMessageBox::Ok);
        box1.setButtonText (QMessageBox::Ok,QString("确 定"));
        box1.exec();
        emit(UseSetOver());
        return;
    }
    emit(Set20_ChooseFromMyGrave(targetCards));
}

void GameController::Set_20_02_FromEnemy()
{
    vector<string> targetCards;
    for(auto i : mEnemy.mGrave)
    {
        if(i->mCardLevel == cBronze)
        {
            targetCards.push_back(getstring(i->GetIndex()));
        }
    }
    if(targetCards.size() == 0)
    {
        QMessageBox box1(QMessageBox::Critical, "提示", "对方的墓地中已经没有铜卡，无法进行操作！");
        box1.setStandardButtons (QMessageBox::Ok);
        box1.setButtonText (QMessageBox::Ok,QString("确 定"));
        box1.exec();
        emit(UseSetOver());
        return;
    }
    emit(Set20_ChooseFromEnemyGrave(targetCards));
}

void GameController::Set_21_Rise_Weather(Card *c)
{
    if(FindCardIn(c, mMe.mFirstRow))
    {
        if(mEnemy.FirstWeather.kind == Weather_Frost)
        {
            m_playing->SetLabelText("已经提高对方冰霜伤害一点！");
            mEnemy.FirstWeather.Hit++;
        }
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        if(mEnemy.SecondWeather.kind == Weather_Frost)
        {
            m_playing->SetLabelText("已经提高对方冰霜伤害一点！");
            mEnemy.SecondWeather.Hit++;
        }
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        if(mEnemy.ThirdWeather.kind == Weather_Frost)
        {
            m_playing->SetLabelText("已经提高对方冰霜伤害一点！");
            mEnemy.ThirdWeather.Hit++;
        }
    }
    else
    {
        m_playing->SetLabelText("对方该排没有冰霜，无法执行技能！");
    }
    emit(UseSetOver());
}

void GameController::Set_22_Move_1Unit(Card*c)
{
    m_nowusecard = c;
    if(FindCardIn(c, mMe.mFirstRow))
    {
        mEnemy.FirstWeather.kind = Weather_Frost;
        mEnemy.FirstWeather.Hit = 2;
        if(mEnemy.mSecondRow.size() + mEnemy.mThirdRow.size() == 0)
        {
            emit(UseSetOver());
            return;
        }
        m_playing->SetLabelText("请选择对方一个兵放到与自己同排的位置！");
        mNowGameState = cSet22ChooseEnemyCard;
    }
    if(FindCardIn(c, mMe.mSecondRow))
    {
        mEnemy.SecondWeather.kind = Weather_Frost;
        mEnemy.SecondWeather.Hit = 2;
        if(mEnemy.mFirstRow.size() + mEnemy.mThirdRow.size() == 0)
        {
            emit(UseSetOver());
            return;
        }
        m_playing->SetLabelText("请选择对方一个兵放到与自己同排的位置！");
        mNowGameState = cSet22ChooseEnemyCard;
    }
    if(FindCardIn(c, mMe.mThirdRow))
    {
        mEnemy.ThirdWeather.kind = Weather_Frost;
        mEnemy.ThirdWeather.Hit = 2;
        if(mEnemy.mSecondRow.size() + mEnemy.mFirstRow.size() == 0)
        {
            emit(UseSetOver());
            return;
        }
        m_playing->SetLabelText("请选择对方一个兵放到与自己同排的位置！");
        mNowGameState = cSet22ChooseEnemyCard;
    }
}

void GameController::Set_23_Get_OneNewCard(Card* c)
{
    mMe.mDeckLeft[0]->mCardPosition = cHand;
    mMe.mHand.push_back(mMe.mDeckLeft[0]);
    mMe.mDeckLeft.erase(mMe.mDeckLeft.begin());
    UpdatePlayingShowNow();
    if((FindCardIn(c, mEnemy.mFirstRow) && mEnemy.mSecondRow.size() + mEnemy.mThirdRow.size() > 0) ||
            (FindCardIn(c, mEnemy.mSecondRow) && mEnemy.mFirstRow.size() + mEnemy.mThirdRow.size() > 0) ||
            (FindCardIn(c, mEnemy.mThirdRow) && mEnemy.mSecondRow.size() + mEnemy.mFirstRow.size() > 0))

    {
        m_playing->SetLabelText("请选择对方一个兵放到与自己同排的位置！");
        mNowGameState = cSet22ChooseEnemyCard;
    }
    else
    {
        m_playing->SetLabelText("敌方没有可供移动的兵！");
        emit(UseSetOver());
    }
}

void GameController::Set_24_Eat_3Friend(Card* c)
{
    m_eatCard = c;
    if(mMe.mFirstRow.size() + mMe.mSecondRow.size() + mMe.mThirdRow.size() <= 1)
    {
        m_playing->SetLabelText("场上没有可以让暗影长者吞噬的友军了！");
        emit(UseSetOver());
        return;
    }
    m_playing->SetLabelText("暗影长者还可以吞噬三个友军，请选择！");
    mNowGameState = cUnseenElder_Eat3;
}

void GameController::Set_24_Eat_2Friend(Card* c)
{
    UpdatePlayingShowNow();
    m_eatCard = c;
    if(mMe.mFirstRow.size() + mMe.mSecondRow.size() + mMe.mThirdRow.size() <= 1)
    {
        m_playing->SetLabelText("场上没有可以让暗影长者吞噬的友军了！");
        emit(UseSetOver());
        return;
    }
    m_playing->SetLabelText("暗影长者还可以吞噬两个友军，请选择！");
    mNowGameState = cUnseenElder_Eat2;
}

void GameController::Set_24_Eat_1Friend(Card* c)
{
    UpdatePlayingShowNow();
    m_eatCard = c;
    if(mMe.mFirstRow.size() + mMe.mSecondRow.size() + mMe.mThirdRow.size() <= 1)
    {
        m_playing->SetLabelText("场上没有可以让暗影长者吞噬的友军了！");
        emit(UseSetOver());
        return;
    }
    m_playing->SetLabelText("暗影长者还可以吞噬一个友军，请选择！");
    mNowGameState = cUnseenElder_Eat1;
}

void GameController::Set_25_Putout_Arachas()
{
    Card* temp = nullptr;
    for(auto i = (int)(mMe.mDeckLeft.size()) - 1; i >= 0 ; i--)
    {
        if(mMe.mDeckLeft[i]->GetIndex() == 25)
        {
            temp = mMe.mDeckLeft[i];
            temp->mCardPosition = cHand;
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + i);
            SelectAndPut(temp);
            return;
        }
    }
    emit(UseSetOver());
}

void GameController::Game_26_Eat_Right(Card* c)
{
    if(FindCardIn(c, mMe.mFirstRow))
    {
        for (auto i = 0; i < mMe.mFirstRow.size(); i++)
        {
            if(mMe.mFirstRow[i] == c)
            {
                if(i >= mMe.mFirstRow.size() - 1)
                {
                    return;
                }
                emit(EatOthers());
                if(mMe.mFirstRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mFirstRow[i]->mCardBloodNow += 4;
                }
                mMe.mFirstRow[i]->mCardBloodNow += mMe.mFirstRow[i + 1]->mCardBloodNow;
                mMe.mFirstRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        for (auto i = 0; i < mMe.mSecondRow.size(); i++)
        {
            if(mMe.mSecondRow[i] == c)
            {
                if(i >= mMe.mSecondRow.size() - 1)
                {
                    return;
                }
                emit(EatOthers());
                if(mMe.mSecondRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mSecondRow[i]->mCardBloodNow += 4;
                }
                mMe.mSecondRow[i]->mCardBloodNow += mMe.mSecondRow[i + 1]->mCardBloodNow;
                mMe.mSecondRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        for (auto i = 0; i < mMe.mThirdRow.size(); i++)
        {
            if(mMe.mThirdRow[i] == c)
            {
                if(i >= mMe.mThirdRow.size() - 1)
                {
                    return;
                }
                emit(EatOthers());
                if(mMe.mThirdRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mThirdRow[i]->mCardBloodNow += 4;
                }
                mMe.mThirdRow[i]->mCardBloodNow += mMe.mThirdRow[i + 1]->mCardBloodNow;
                mMe.mThirdRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
}

void GameController::Set_26_Eat_Right(Card* c)
{
    if(FindCardIn(c, mMe.mFirstRow))
    {
        for (auto i = 0; i < mMe.mFirstRow.size(); i++)
        {
            if(mMe.mFirstRow[i] == c)
            {
                if(i >= mMe.mFirstRow.size() - 1)
                {
                    emit(UseSetOver());
                    return;
                }
                emit(EatOthers());
                if(mMe.mFirstRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mFirstRow[i]->mCardBloodNow += 4;
                }
                mMe.mFirstRow[i]->mCardBloodNow += mMe.mFirstRow[i + 1]->mCardBloodNow;
                mMe.mFirstRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
    else if(FindCardIn(c, mMe.mSecondRow))
    {
        for (auto i = 0; i < mMe.mSecondRow.size(); i++)
        {
            if(mMe.mSecondRow[i] == c)
            {
                if(i >= mMe.mSecondRow.size() - 1)
                {
                    emit(UseSetOver());
                    return;
                }
                emit(EatOthers());
                if(mMe.mSecondRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mSecondRow[i]->mCardBloodNow += 4;
                }
                mMe.mSecondRow[i]->mCardBloodNow += mMe.mSecondRow[i + 1]->mCardBloodNow;
                mMe.mSecondRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
    else if(FindCardIn(c, mMe.mThirdRow))
    {
        for (auto i = 0; i < mMe.mThirdRow.size(); i++)
        {
            if(mMe.mThirdRow[i] == c)
            {
                if(i >= mMe.mThirdRow.size() - 1)
                {
                    emit(UseSetOver());
                    return;
                }
                emit(EatOthers());
                if(mMe.mThirdRow[i + 1]->GetIndex() == 502)
                {
                    mMe.mThirdRow[i]->mCardBloodNow += 4;
                }
                mMe.mThirdRow[i]->mCardBloodNow += mMe.mThirdRow[i + 1]->mCardBloodNow;
                mMe.mThirdRow[i + 1]->mCardBloodNow = 0;
                UpdatePlayingShowNow();
                return;
            }
        }
    }
    emit(UseSetOver());
}

void GameController::Set_27_Get_2Shield(Card *c)
{
    c->mCardShield += 2;
    emit(UseSetOver());
}

void GameController::Set_506_Put_All_Spider()
{
    Card* temp;
    for (auto i = 0; i < mMe.mDeckLeft.size(); i++)
    {
        if(mMe.mDeckLeft[i]->GetIndex() == 25)
        {
            temp = mMe.mDeckLeft[i];
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + i);
            SelectAndPut(temp);
            return;
        }
    }
    emit(UseSetOver());
}

void GameController::UseSet(Card * c)
{
    m_playing->SetLabelText("现在要开始使用这张牌的技能了！");
    switch (c->GetIndex())
    {
    case 1:
        // Card_First_Light
        Set_01_ChooseACard();
        break;
    case 2:
        // Card_Biting_Frost
        Set_02_ChooseWhichRow();
        break;
    case 3:
        // Card_Impenetrable_Fog
        Set_03_ChooseWhichRow();
        break;
    case 4:
        // Card_Torrential_Rain
        Set_04_ChooseWhichRow();
        break;
    case 5:
        // Card_Lacerate
        Set_05_ChooseWhichRowToHit();
        break;
    case 6:
        // Card_Thunderbolt_Potion
        Set_06_ChooseWhichCard_Neighboring3();
        break;
    case 7:
        // Card_Commander_s_Horn
        Set_07_ChooseWhichCard_Neighboring5();
        break;
    case 8:
        // Card_Bekker_s_Twisted_Mirror
        Set_08_ChangeBiggestAndSmallest();
        break;
    case 9:
        // Card_Geralt__Igni
        Set_09_CheckEnemyRowAllBlood_20(c);
        break;
    case 10:
        // Card_Dagon
        Set_10_SwitchFromWeathers();
        break;
    case 11:
        // Card_Foglet
        emit UseSetOver();
        break;
    case 12:
        // Card_Ge_els
        Set_12_GetGoldAndSilver();
        break;
    case 13:
        // Card_Celaeno_Harpy
        Set_13_Get2Eggs(c);
        break;
    case 14:
        // Card_Roach
        emit UseSetOver();
        break;
    case 15:
        // Card_Woodland_Spirit
        Set_15_Get3AngryWolf(c);
        break;
    case 16:
        // Card_Earth_Elemental
        Set_16_GetShield(c);
        break;
    case 17:
        // Card_Crone__Weavess
        Set_17_Send_Crone();
        break;
    case 18:
        // Card_Crone__Whispess
        Set_18_Send_Crone();
        break;
    case 19:
        // Card_Crone__Brewess
        Set_19_Send_Crone();
        break;
    case 20:
        // Card_Archgriffin
        Set_20_Remove_Weather(c);
        break;
    case 21:
        // Card_Wild_Hunt_Rider
        Set_21_Rise_Weather(c);
        break;
    case 22:
        // Card_Caranthir
        Set_22_Move_1Unit(c);
        break;
    case 23:
        // Card_Frightener
        Set_23_Get_OneNewCard(c);
        break;
    case 24:
        // Card_Unseen_Elder
        Set_24_Eat_3Friend(c);
        break;
    case 25:
        // Card_Arachas
        Set_25_Putout_Arachas();
        break;
    case 26:
        // Card_Vran_Warrior
        Set_26_Eat_Right(c);
        break;
    case 27:
        // Card_Arachas_Behemoth
        Set_27_Get_2Shield(c);
        break;
    case 506:
        Set_506_Put_All_Spider();
        break;
    default:
        emit(UseSetOver());
        break;
    }
}

void GameController::GotUseSetOver()
{
    if(mNowGameState == cEmegency)
    {
        return;
    }
    delete timer;
    timer = nullptr;
//    UpdatePlayingShowNow();
    m_playing->SetLabelText("接下来交换用户进行游戏！");
    NewTurn(&mEnemy);
}

void GameController::DecreaseCardBlood(Card* c, int b)
{
    if(b > 0 && c->mCardSuperShield)
    {
        c->mCardSuperShield = false;
        return;
    }
    if(c->mCardShield > 0 && b > 0)
    {
        if(b > c->mCardShield)
        {
            c->mCardShield = 0;
            c->mCardBloodNow -= (b - c->mCardShield);
            return;
        }
        c->mCardShield -= b;
        return;
    }
    c->mCardBloodNow -= b;
}

void GameController::GiveCardShieldAndBlood(Card* chosen, int Shield, int Blood, int NeighborLeftNum)
{
    if(FindCardIn(chosen, mMe.mFirstRow))
    {
        for(auto i = 0; i < mMe.mFirstRow.size(); i++)
        {
            if(mMe.mFirstRow[i] == chosen)
            {
                mMe.mFirstRow[i]->mCardBloodNow += Blood;
                mMe.mFirstRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mMe.mFirstRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mMe.mFirstRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mMe.mFirstRow.size()) - 1 - temp)
                    {
                        mMe.mFirstRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mMe.mFirstRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
    if(FindCardIn(chosen, mMe.mSecondRow))
    {
        for(auto i = 0; i < mMe.mSecondRow.size(); i++)
        {
            if(mMe.mSecondRow[i] == chosen)
            {
                mMe.mSecondRow[i]->mCardBloodNow += Blood;
                mMe.mSecondRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mMe.mSecondRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mMe.mSecondRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mMe.mSecondRow.size()) - 1 - temp)
                    {
                        mMe.mSecondRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mMe.mSecondRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
    if(FindCardIn(chosen, mMe.mThirdRow))
    {
        for(auto i = 0; i < mMe.mThirdRow.size(); i++)
        {
            if(mMe.mThirdRow[i] == chosen)
            {
                mMe.mThirdRow[i]->mCardBloodNow += Blood;
                mMe.mThirdRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mMe.mThirdRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mMe.mThirdRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mMe.mThirdRow.size()) - 1 - temp)
                    {
                        mMe.mThirdRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mMe.mThirdRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
    if(FindCardIn(chosen, mEnemy.mFirstRow))
    {
        for(auto i = 0; i < mEnemy.mFirstRow.size(); i++)
        {
            if(mEnemy.mFirstRow[i] == chosen)
            {
                mEnemy.mFirstRow[i]->mCardBloodNow += Blood;
                mEnemy.mFirstRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mEnemy.mFirstRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mEnemy.mFirstRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mEnemy.mFirstRow.size()) - 1 - temp)
                    {
                        mEnemy.mFirstRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mEnemy.mFirstRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
    if(FindCardIn(chosen, mEnemy.mSecondRow))
    {
        for(auto i = 0; i < mEnemy.mSecondRow.size(); i++)
        {
            if(mEnemy.mSecondRow[i] == chosen)
            {
                mEnemy.mSecondRow[i]->mCardBloodNow += Blood;
                mEnemy.mSecondRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mEnemy.mSecondRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mEnemy.mSecondRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mEnemy.mSecondRow.size()) - 1 - temp)
                    {
                        mEnemy.mSecondRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mEnemy.mSecondRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
    if(FindCardIn(chosen, mEnemy.mThirdRow))
    {
        for(auto i = 0; i < mEnemy.mThirdRow.size(); i++)
        {
            if(mEnemy.mThirdRow[i] == chosen)
            {
                mEnemy.mThirdRow[i]->mCardBloodNow += Blood;
                mEnemy.mThirdRow[i]->mCardShield += Shield;
                for (int temp = 0; temp < NeighborLeftNum; temp++)
                {
                    if(i > temp)
                    {
                        mEnemy.mThirdRow[i - 1 - temp]->mCardBloodNow += Blood;
                        mEnemy.mThirdRow[i - 1 - temp]->mCardShield += Shield;
                    }
                    if(i < (int)(mEnemy.mThirdRow.size()) - 1 - temp)
                    {
                        mEnemy.mThirdRow[i + 1 + temp]->mCardBloodNow += Blood;
                        mEnemy.mThirdRow[i + 1 + temp]->mCardShield += Shield;
                    }
                }
                emit(UseSetOver());
                return;
            }
        }
    }
}

void GameController::DealWithDie()
{
    for (auto i = (int)(mMe.mFirstRow.size()) - 1; i >= 0; i--)
    {
        if(mMe.mFirstRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mMe.mFirstRow[i];
            ToGrave(temp);
            if(mMe.mFirstRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mMe.mFirstRow[i]->mCardPosition = cGrave;
            if(!mMe.mFirstRow[i]->mLostAfterDie)
            {
                mMe.mGrave.push_back(mMe.mFirstRow[i]);
            }
            mMe.mFirstRow.erase(mMe.mFirstRow.begin() + i);
        }
    }
    for (auto i = (int)(mMe.mSecondRow.size()) - 1; i >= 0; i--)
    {
        if(mMe.mSecondRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mMe.mSecondRow[i];
            ToGrave(temp);
            if(mMe.mSecondRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mMe.mSecondRow[i]->mCardPosition = cGrave;
            if(!mMe.mSecondRow[i]->mLostAfterDie)
            {
                mMe.mGrave.push_back(mMe.mSecondRow[i]);
            }
            mMe.mSecondRow.erase(mMe.mSecondRow.begin() + i);
        }
    }
    for (auto i = (int)(mMe.mThirdRow.size()) - 1; i >= 0; i--)
    {
        if(mMe.mThirdRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mMe.mThirdRow[i];
            ToGrave(temp);
            if(mMe.mThirdRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mMe.mThirdRow[i]->mCardPosition = cGrave;
            if(!mMe.mThirdRow[i]->mLostAfterDie)
            {
                mMe.mGrave.push_back(mMe.mThirdRow[i]);
            }
            mMe.mThirdRow.erase(mMe.mThirdRow.begin() + i);
        }
    }
    for (auto i = (int)(mEnemy.mFirstRow.size()) - 1; i >= 0; i--)
    {
        if(mEnemy.mFirstRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mEnemy.mFirstRow[i];
            ToGrave(temp);
            if(mEnemy.mFirstRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mEnemy.mFirstRow[i]->mCardPosition = cGrave;
            if(!mEnemy.mFirstRow[i]->mLostAfterDie)
            {
                mEnemy.mGrave.push_back(mEnemy.mFirstRow[i]);
            }
            mEnemy.mFirstRow.erase(mEnemy.mFirstRow.begin() + i);
        }
    }
    for (auto i = (int)(mEnemy.mSecondRow.size()) - 1; i >= 0; i--)
    {
        if(mEnemy.mSecondRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mEnemy.mSecondRow[i];
            ToGrave(temp);
            if(mEnemy.mSecondRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mEnemy.mSecondRow[i]->mCardPosition = cGrave;
            if(!mEnemy.mSecondRow[i]->mLostAfterDie)
            {
                mEnemy.mGrave.push_back(mEnemy.mSecondRow[i]);
            }
            mEnemy.mSecondRow.erase(mEnemy.mSecondRow.begin() + i);
        }
    }
    for (auto i = (int)(mEnemy.mThirdRow.size()) - 1; i >= 0; i--)
    {
        if(mEnemy.mThirdRow[i]->mCardBloodNow <= 0)
        {
            auto temp = mEnemy.mThirdRow[i];
            ToGrave(temp);
            if(mEnemy.mThirdRow[i]->mCardBloodNow > 0)
            {
                i++;
            }
            mEnemy.mThirdRow[i]->mCardPosition = cGrave;
            if(!mEnemy.mThirdRow[i]->mLostAfterDie)
            {
                mEnemy.mGrave.push_back(mEnemy.mThirdRow[i]);
            }
            mEnemy.mThirdRow.erase(mEnemy.mThirdRow.begin() + i);
        }
    }
}

void GameController::ToGrave(Card* c)
{
    if(c->GetIndex() == 502)
    {
        if(FindCardIn(c, mMe.mFirstRow) || FindCardIn(c, mMe.mSecondRow) || FindCardIn(c, mMe.mThirdRow))
        {
            PutCardOnly(4 + rand() % 3, 0, GetNewCard(504));
            return;
        }
        PutCardOnly(1 + rand() % 3, 0, GetNewCard(504));
        return;
    }
    else if(c->GetIndex() == 16)
    {
        Card* temp3 = GetNewCard(505);
        Card* temp2 = GetNewCard(505);

        if(FindCardIn(c, mMe.mSecondRow))
        {
            PutCardOnly(5, mMe.mSecondRow.size(), temp2);
            PutCardOnly(5, mMe.mSecondRow.size(), temp3);
        }
        else if(FindCardIn(c, mMe.mThirdRow))
        {
            PutCardOnly(6, mMe.mThirdRow.size(), temp2);
            PutCardOnly(6, mMe.mThirdRow.size(), temp3);
        }
        else if(FindCardIn(c, mMe.mFirstRow))
        {
            PutCardOnly(4, mMe.mFirstRow.size(), temp2);
            PutCardOnly(4, mMe.mFirstRow.size(), temp3);
        }
        else if(FindCardIn(c, mEnemy.mSecondRow))
        {
            PutCardOnly(2, mEnemy.mSecondRow.size(), temp2);
            PutCardOnly(2, mEnemy.mSecondRow.size(), temp3);
        }
        else if(FindCardIn(c, mEnemy.mThirdRow))
        {
            PutCardOnly(1, mEnemy.mThirdRow.size(), temp2);
            PutCardOnly(1, mEnemy.mThirdRow.size(), temp3);
        }
        else if(FindCardIn(c, mEnemy.mFirstRow))
        {
            PutCardOnly(3, mEnemy.mFirstRow.size(), temp2);
            PutCardOnly(3, mEnemy.mFirstRow.size(), temp3);
        }
    }
}

void GameController::FindBigSpider()
{
    for (auto i : mMe.mFirstRow)
    {
        if(i->GetIndex() == 27)
        {
            i->mCardBloodNow -= 1;
            PutCardOnly(4 + rand() % 3, 0, GetNewCard(506));
            UpdatePlayingShowNow();
            Set_506_Put_All_Spider();
        }
    }
    for (auto i : mMe.mSecondRow)
    {
        if(i->GetIndex() == 27)
        {
            i->mCardBloodNow -= 1;
            PutCardOnly(4 + rand() % 3, 0, GetNewCard(506));
            UpdatePlayingShowNow();
            Set_506_Put_All_Spider();
        }
    }
    for (auto i : mMe.mThirdRow)
    {
        if(i->GetIndex() == 27)
        {
            i->mCardBloodNow -= 1;
            PutCardOnly(4 + rand() % 3, 0, GetNewCard(506));
            UpdatePlayingShowNow();
            Set_506_Put_All_Spider();
        }
    }
    emit(UseSetOver());
}

void GameController::FindAllVranWarrior()
{
    for (auto i : mMe.mFirstRow)
    {
        if(i->GetIndex() == 26)
        {
            i->mWaitNow = i->mWaitNow % i->mWaitInterval + 1;
            if(i->mWaitNow == i->mWaitInterval)
            {
                Game_26_Eat_Right(i);
            }
        }
    }
    for (auto i : mMe.mSecondRow)
    {
        if(i->GetIndex() == 26)
        {
            i->mWaitNow = i->mWaitNow % i->mWaitInterval + 1;
            if(i->mWaitNow == i->mWaitInterval)
            {
                Game_26_Eat_Right(i);
            }
        }
    }
    for (auto i : mMe.mThirdRow)
    {
        if(i->GetIndex() == 26)
        {
            i->mWaitNow = i->mWaitNow % i->mWaitInterval + 1;
            if(i->mWaitNow == i->mWaitInterval)
            {
                Game_26_Eat_Right(i);
            }
        }
    }
}

void GameController::FindAllRoaches()
{
    for(auto i  = 0; i < mMe.mDeckLeft.size(); i++)
    {
        if(mMe.mDeckLeft[i]->GetIndex() == 14)
        {
            PutCardOnly(4 + rand() % 3, 0, mMe.mDeckLeft[i]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + i);
            return;
        }
    }
}

void GameController::FindAllFoglets(int row)
{
    for (auto i = 0; i < mMe.mDeckLeft.size(); i++)
    {
        if(mMe.mDeckLeft[i]->GetIndex() == 11)
        {
            PutCardOnly(7 - row, 0, mMe.mDeckLeft[i]);
            mMe.mDeckLeft.erase(mMe.mDeckLeft.begin() + i);
            return;
        }
    }
    for (auto i = 0; i < mMe.mGrave.size(); i++)
    {
        if(mMe.mGrave[i]->GetIndex() == 11)
        {
            PutCardOnly(7 - row, 0, mMe.mGrave[i]);
            mMe.mGrave.erase(mMe.mGrave.begin() + i);
            return;
        }
    }
}

void GameController::CheckClearedAllFogs()
{
    if(mMe.FirstWeather.kind != 1 && mMe.SecondWeather.kind != 1 && mMe.ThirdWeather.kind != 1
            && mEnemy.FirstWeather.kind != 1 && mEnemy.SecondWeather.kind != 1 && mEnemy.ThirdWeather.kind != 1)
    {
        for (auto i : mMe.mFirstRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        for (auto i : mMe.mSecondRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        for (auto i : mMe.mThirdRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        for (auto i : mEnemy.mFirstRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        for (auto i : mEnemy.mSecondRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        for (auto i : mEnemy.mThirdRow)
        {
            if(i->GetIndex() == 11)
            {
                i->mCardBloodNow = 0;
            }
        }
        UpdatePlayingShowNow();
    }
}

void GameController::PressedPass()
{
    delete timer;
    timer = nullptr;
    mMe.mPassed = true;
    if(mEnemy.mPassed)
    {
        emit(EndRound());
        return;
    }
    emit(UseSetOver());
}

void GameController::NewRoundCheck()
{
    auto temp0 = mMe;
    if(mMe.mUserName == "")
    {
        mMe = mEnemy;
        mEnemy = temp0;
    }
    QString Ans = "第" + QString::number(mNowRound) + "局结束！";
    QString Ans2 = "先手！A先换牌！";
    PlayerInfo *FirstMove = &mMe;
    mMe.mNowRoundScore[mNowRound - 1] = m_playing->GetMeScore(&mMe);
    mEnemy.mNowRoundScore[mNowRound - 1] = m_playing->GetEnemyScore(&mEnemy);
    if(mMe.mUserName == "" && mMe.mNowRoundScore[mNowRound - 1] < mEnemy.mNowRoundScore[mNowRound - 1])
    {
        Ans += "A胜利！比分" + QString::number(mEnemy.mNowRoundScore[mNowRound - 1]) + "比" + QString::number(mMe.mNowRoundScore[mNowRound - 1]) + "！";
        FirstMove = &mEnemy;
        Ans2 = "A" + Ans2;
    }
    else if(mMe.mUserName != "" && mMe.mNowRoundScore[mNowRound - 1] > mEnemy.mNowRoundScore[mNowRound - 1])
    {
        Ans += "A胜利！比分" + QString::number(mMe.mNowRoundScore[mNowRound - 1]) + "比" + QString::number(mEnemy.mNowRoundScore[mNowRound - 1]) + "！";
        Ans2 = "A" + Ans2;
    }
    else if(mMe.mNowRoundScore[mNowRound - 1] == mEnemy.mNowRoundScore[mNowRound - 1])
    {
        Ans += "AB平局！得分均为" + QString::number(mMe.mNowRoundScore[mNowRound - 1]) + "！";
        if(rand() % 2)
        {
            FirstMove = &mEnemy;
        }
        if(FirstMove->mUserName == "")
        {
            Ans2 = "B" + Ans2;
        }
        else
        {
            Ans2 = "A" + Ans2;
        }
    }
    else if(mMe.mUserName == "")
    {
        Ans += "B胜利！比分" + QString::number(mMe.mNowRoundScore[mNowRound - 1]) + "比" + QString::number(mEnemy.mNowRoundScore[mNowRound - 1]) + "！";
        Ans2 = "B" + Ans2;
    }
    else
    {
        Ans += "B胜利！比分" + QString::number(mEnemy.mNowRoundScore[mNowRound - 1]) + "比" + QString::number(mMe.mNowRoundScore[mNowRound - 1]) + "！";
        FirstMove = &mEnemy;
        Ans2 = "B" + Ans2;
    }
    m_playing->SetLabelText(Ans);
    m_playing->SetLabelText(Ans2);
    if(mMe.mNowRoundScore[mNowRound - 1] < mEnemy.mNowRoundScore[mNowRound - 1])
    {
        mEnemy.mWinNum++;
    }
    else if(mMe.mNowRoundScore[mNowRound - 1] > mEnemy.mNowRoundScore[mNowRound - 1])
    {
        mMe.mWinNum++;
    }
    else
    {
        mMe.mWinNum++;
        mEnemy.mWinNum++;
    }
    mNowRound++;
    if(mMe.mWinNum >= 2 || mEnemy.mWinNum >= 2)
    {
        emit(GameOver(mMe.mUserName == "", mMe.mWinNum, mEnemy.mWinNum,
                      mMe.mNowRoundScore[0], mMe.mNowRoundScore[1], mMe.mNowRoundScore[2],
                      mEnemy.mNowRoundScore[0], mEnemy.mNowRoundScore[1], mEnemy.mNowRoundScore[2]));
        return;
    }
    ClearForAnotherRound();
    StartNewRound(FirstMove);
}

void GameController::StartNewRound(PlayerInfo *FirstMove)
{
    FirstPerson = FirstMove;

    mMe.GetNewCards(mNowRound);
    mEnemy.GetNewCards(mNowRound);

    UpdatePlayingShowNow();

    mNowGameState = cChooseCardsOnlyMe;
    emit(EmitChangeHandCardOnly(1, &mMe));
}

void GameController::clearAllData()
{
    mNowGameState = cEnd;
    mNowStates = cChoosing;
    mNowRound = 0;
    m_playing = nullptr;
    m_nowusecard = nullptr;
    m_eatCard = nullptr;
    FirstPerson = nullptr;

    if(mMe.mUserName == "")
    {
        auto temp = mMe;
        mMe = mEnemy;
        mEnemy = temp;
    }
    mMe.mDeckLeft.clear();
    mMe.mGrave.clear();
    mMe.mFirstRow.clear();
    mMe.mSecondRow.clear();
    mMe.mThirdRow.clear();
    mMe.mHand.clear();
    mMe.ClearSky();
    mMe.temp.clear();
    mMe.HaveUsedLeader = false;
    mMe.mLeader = nullptr;
    mMe.mWinNum = 0;
    mMe.mPassed = false;
    mMe.mNowRoundScore[0] = mMe.mNowRoundScore[1] = mMe.mNowRoundScore[2] = 0;

    mEnemy.mDeckLeft.clear();
    mEnemy.mGrave.clear();
    mEnemy.mFirstRow.clear();
    mEnemy.mSecondRow.clear();
    mEnemy.mThirdRow.clear();
    mEnemy.mHand.clear();
    mEnemy.ClearSky();
    mEnemy.temp.clear();
    mEnemy.HaveUsedLeader = false;
    mEnemy.mLeader = nullptr;
    mEnemy.mWinNum = 0;
    mEnemy.mPassed = false;
    mEnemy.mNowRoundScore[0] = mEnemy.mNowRoundScore[1] = mEnemy.mNowRoundScore[2] = 0;
}

void GameController::ClearForAnotherRound()
{
    mNowGameState = cChoosingCards;

    mMe.ClearSky();
    mMe.temp.clear();
    mMe.mPassed = false;
    for(auto i : mMe.mFirstRow)
    {
        i->mCardBloodNow = 0;
    }
    for(auto i : mMe.mSecondRow)
    {
        i->mCardBloodNow = 0;
    }
    for(auto i : mMe.mThirdRow)
    {
        i->mCardBloodNow = 0;
    }

    mEnemy.ClearSky();
    mEnemy.temp.clear();
    mEnemy.mPassed = false;
    for(auto i : mEnemy.mFirstRow)
    {
        i->mCardBloodNow = 0;
    }
    for(auto i : mEnemy.mSecondRow)
    {
        i->mCardBloodNow = 0;
    }
    for(auto i : mEnemy.mThirdRow)
    {
        i->mCardBloodNow = 0;
    }

    UpdatePlayingShowNow();
}

void GameController::timerDone()
{
    CountDown--;
    if(CountDown == 10)
    {
        QMessageBox box(QMessageBox::Critical, "提示", "只剩十秒，抓紧！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
    }
    m_playing->ShowTimer(CountDown);
    if(CountDown <= 0)
    {
        delete timer;
        timer = nullptr;
        PressedPass();
    }
}
