#include "PlayerInfo.h"
#include <algorithm>
#include "iobase.h"

void PlayerInfo::PrePareForFirst(int tar)
{
    mHand.clear();
    mFirstRow.clear();
    mSecondRow.clear();
    mThirdRow.clear();
    mGrave.clear();
    mDeckLeft.clear();
    mDecks[tar].SetCards();
    mLeader = GetNewCard(getint(mDecks[tar].mLeadCard));
    for (auto i : mDecks[tar].mCards)
    {
        i->mCardPosition = cDeck;
        mDeckLeft.push_back(i);
    }
    mDecks[tar].mCards.clear();
}

// Get New Cards from deck to Hand(Called every round)
// 从卡堆里面抽卡到手牌中（每一回合调用一次，回合开始进行抽卡）
void PlayerInfo::GetNewCards(int round)
{
    temp.clear();
    switch (round)
    {
    case 1:
        // The other radom_shuffles are put behinde SwitchCards()
        // 第一轮抽牌之前首先洗牌，之后的洗牌放在了换牌之后
        random_shuffle(mDeckLeft.begin(), mDeckLeft.end());
        GetCardsFromDeck(11);
        break;
    case 2:
        GetCardsFromDeck(2);
        break;
    default:
        GetCardsFromDeck(1);
        break;
    }
}

// Get Card From Deck
// 从卡堆中抽出卡牌
void PlayerInfo::GetCardsFromDeck(int num)
{
    vector<Card*>::iterator fromIt = mDeckLeft.begin();
    for (int i = 0; i < num; i++)
    {
        mHand.push_back(*fromIt);
        (*fromIt)->mCardPosition = cHand;
        fromIt = mDeckLeft.erase(fromIt);
    }
}

void PlayerInfo::ChangeCardAtP(int p)
{
    mHand[p]->mCardPosition = cDeck;
    temp.push_back(mHand[p]);
    mHand[p] = *(mDeckLeft.begin());
    mHand[p]->mCardPosition = cHand;
    mDeckLeft.erase(mDeckLeft.begin());
}

void PlayerInfo::ChangedCardsPutTemp()
{
    for (auto t : temp)
    {
        mDeckLeft.push_back(t);
    }
    temp.clear();
    random_shuffle(mDeckLeft.begin(), mDeckLeft.end()); // 抽牌之前首先洗牌
}

void PlayerInfo::SetUserName(string u)
{
    mUserName = u;
}

void PlayerInfo::SetUserPassword(string p)
{
    mPassword = p;
}

void PlayerInfo::SetUserInfo(string i)
{
    mUserInfo = i;
    GetDeckInfoFromString(i);
    SetKegsNum(GetTag(i, "kegs"));
    SetMoney(GetTag(i, "money"));
    SetId(GetTag(i, "id"));
    SetLevel(GetTag(i, "level"));
    SetScore(GetTag(i, "score"));
}

void PlayerInfo::SetKegsNum(string kegs)
{
    mKegsnum = getint(kegs);
}

void PlayerInfo::SetMoney(string money)
{
    mMoney = getint(money);
}

void PlayerInfo::SetId(string id)
{
    mUserId = getint(id);
}

void PlayerInfo::SetLevel(string level)
{
    mUserLevel = getint(level);
}

void PlayerInfo::SetScore(string score)
{
    mUserScore = getint(score);
}

string PlayerInfo::UpdateData()
{
    string t = "<info><user>";
    t += mUserName;
    t += "</user><psw>";
    t += mPassword;
    t += "</psw><userinfo><kegs>";
    t += getstring(mKegsnum);
    t += "</kegs><money>";
    t += getstring(mMoney);
    t += "</money><id>";
    t += mUserId;
    t += "</id><level>";
    t += getstring(mUserLevel);
    t += "</level><score>";
    t += getstring(mUserScore);
    t += "</score><cardlist>";
    for (auto i : mCardMap)
    {
        for (auto i2 = 0; i2 < i.second; i2++)
        {
            t += "<card>" + i.first + "</card>";
        }
    }
    t += "</cardlist>";
    for (auto i : mDecks)
    {
        t += "<deck><deckname>";
        t += i.mDeckIntro;
        t += "</deckname><deckleader>";
        t += i.mLeadCard;
        t += "</deckleader>";
        for (auto m : i.mCardIndexs)
        {
            t += "<card>" + m + "</card>";
        }
        t += "</deck>";
    }
    t += "</userinfo></info>";
    return t;
}

// Get deck info from the string of deck
// 从记录里面的deck的string获得结构体
DeckInfo PlayerInfo::GetDeckInfo(string src) const
{
    auto temp = GetTagAll(src, "card");
    return DeckInfo(GetTag(src, "deckname"), temp.size(), temp, GetTag(src, "deckleader"));
}

// Set Deck info from the string of deck log
// 将整个结构体变为PlayerInfo里面的变量
void PlayerInfo::GetDeckInfoFromString(string src)
{
    auto temp = GetTagAll(src, "deck");
    mDecks.clear();
    for (auto i : temp)
    {
        mDecks.push_back(GetDeckInfo(i));
    }
}

// Set Card info from the string of card log
// 将整个card结构体变为PlayerInfo里面的变量
void PlayerInfo::GetCardInfoFromString(string src)
{
    auto t = GetTag(src, "cardlist");
    auto temp = GetTagAll(t, "card");
    mCardList.clear();
    for (auto i : temp)
    {
        if (getint(i) <= 0 || getint(i) >= cCard_Num)
        {
            continue;
        }
        mCardList.push_back(i);
    }
}

// Change the vector the last function got into a map
// 把上面那个函数最后生成的vector换成Map
void PlayerInfo::CardVectorToMap()
{
    mCardMap.clear();
    for (auto i : mCardList)
    {
        mCardMap[i]++;
    }
}

// Choose out leader cards from the data
// 提取领袖牌的列表
void PlayerInfo::GetLeaderList()
{
    leaderList.clear();
    for (auto leadNum : LeadNums)
    {
        for (auto i : mCardMap)
        {
            if(i.first == getstring(leadNum))
            {
                leaderList.push_back(i.first);
            }
        }
    }
    if(leaderList.size() == 0)
    {
        throw("NoLeaderError");
    }
}

void PlayerInfo::ClearSky()
{
    FirstWeather.kind = 0;
    FirstWeather.Hit = 0;
    SecondWeather.kind = 0;
    SecondWeather.Hit = 0;
    ThirdWeather.kind = 0;
    ThirdWeather.Hit = 0;
}

void PlayerInfo::DecreaseCardBlood(Card* c, int b)
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

void PlayerInfo::ApplyWeatherHit()
{
    Card* temp;
    vector<Card*> tempVec;
    if(FirstWeather.Hit && mFirstRow.size() > 0)
    {
        switch (FirstWeather.kind)
        {
        case Weather_Fog:
            temp = mFirstRow[0];
            for (auto t : mFirstRow)
            {
                if(t->mCardBloodNow > temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, FirstWeather.Hit);
            break;
        case Weather_Rain:
            temp = mFirstRow[0];
            tempVec.push_back(temp);
            for (auto t : mFirstRow)
            {
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                    tempVec.clear();
                    tempVec.push_back(temp);
                }
                else if(t->mCardBloodNow == temp->mCardBloodNow)
                {
                    if(tempVec.size() < 5)
                    {
                        tempVec.push_back(t);
                    }
                }
            }
            for (auto t : tempVec)
            {
                DecreaseCardBlood(t, FirstWeather.Hit);
            }
            tempVec.clear();
            break;
        case Weather_Frost:
            temp = mFirstRow[0];
            for (auto t : mFirstRow)
            {
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, FirstWeather.Hit);
            break;
        default:
            break;
        }
    }
    if(SecondWeather.Hit && mSecondRow.size() > 0)
    {
        switch (SecondWeather.kind)
        {
        case Weather_Fog:
            temp = mSecondRow[0];
            for (auto t : mSecondRow)
            {
                if(t->mCardBloodNow > temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, SecondWeather.Hit);
            break;
        case Weather_Rain:
            temp = mSecondRow[0];
            tempVec.push_back(temp);
            for (auto t : mSecondRow)
            {
                if(t == temp)
                {
                    continue;
                }
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                    tempVec.clear();
                    tempVec.push_back(temp);
                }
                else if(t->mCardBloodNow == temp->mCardBloodNow)
                {
                    if(tempVec.size() < 5)
                    {
                        tempVec.push_back(t);
                    }
                }
            }
            for (auto t : tempVec)
            {
                DecreaseCardBlood(t, SecondWeather.Hit);
            }
            tempVec.clear();
            break;
        case Weather_Frost:
            temp = mSecondRow[0];
            for (auto t : mSecondRow)
            {
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, SecondWeather.Hit);
            break;
        default:
            break;
        }
    }
    if(ThirdWeather.Hit && mThirdRow.size() > 0)
    {
        switch (ThirdWeather.kind)
        {
        case Weather_Fog:
            temp = mThirdRow[0];
            for (auto t : mThirdRow)
            {
                if(t->mCardBloodNow > temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, ThirdWeather.Hit);
            break;
        case Weather_Rain:
            temp = mThirdRow[0];
            tempVec.push_back(temp);
            for (auto t : mThirdRow)
            {
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                    tempVec.clear();
                    tempVec.push_back(temp);
                }
                else if(t->mCardBloodNow == temp->mCardBloodNow)
                {
                    if(tempVec.size() < 5)
                    {
                        tempVec.push_back(t);
                    }
                }
            }
            for (auto t : tempVec)
            {
                DecreaseCardBlood(t, ThirdWeather.Hit);
            }
            tempVec.clear();
            break;
        case Weather_Frost:
            temp = mThirdRow[0];
            for (auto t : mThirdRow)
            {
                if(t->mCardBloodNow < temp->mCardBloodNow)
                {
                    temp = t;
                }
            }
            DecreaseCardBlood(temp, ThirdWeather.Hit);
            break;
        default:
            break;
        }
    }
}
