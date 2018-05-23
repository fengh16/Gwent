#ifndef PLAYERINFO_H
#define PLAYERINFO_H

#include "iobase.h"
#include "Functions.h"
#include "CONST.h"
#include "DataControl.h"
#include <map>

using namespace std;

#include <QMessageBox>
struct DeckInfo
{
    string mDeckIntro;
    int mCardNum;
    vector<string> mCardIndexs;
    vector<Card*> mCards;
    string mLeadCard;

    DeckInfo() : mDeckIntro("NEW"), mCardNum(0), mLeadCard("0") {}

    DeckInfo(string intro, int cardnum, vector<string>& indexs, string lead) throw(string) : mDeckIntro(intro), mCardNum(cardnum), mCardIndexs(indexs), mLeadCard(lead)
    {
        bool flag = false;
        bool leadOK = false;

        for (auto i : LeadNums)
        {
            if(getstring(i) == mLeadCard)
            {
                leadOK = true;
            }
        }

        if(!leadOK)
        {
            throw("LeadError!");
        }

        for (vector<string>::iterator k = indexs.begin(); k != indexs.end(); ++k)
        {
            // You can't have lead card in the card list
            // 这一堆牌里面不能有领袖牌
            for (auto i : LeadNums)
            {
                if(getstring(i) == *k)
                {
                    throw("MultiLeadError!");
                }
            }
            // flag here is used to prevent the things that the first one isn't available
            // 为了防止第一个就出错……只能立一个flag了
            if (flag)
            {
                --k;
                flag = false;
            }
            if (getint(*k) <= 0 || getint(*k) >= cCard_Num)
            {
                --cardnum;
                k = indexs.erase(k);
                flag = true;
                if (k == indexs.end())
                {
                    // In case that the last one isn't available and cause out of range error
                    // 如果是最后一个的话，如果不退出会再次++，这样会越界
                    break;
                }
            }
        }
    }

    ~DeckInfo()
    {
        if (mCards.size() > 0)
        {
            for (auto t : mCards)
            {
                delete t;
                t = nullptr;
            }
        }
    }

    void SetCards()
    {
        if (mCards.size() > 0)
        {
            for (auto t : mCards)
            {
                delete t;
                t = nullptr;
            }
        }
        mCards.clear();
        for (auto i : mCardIndexs)
        {
            mCards.push_back(GetNewCard(getint(i)));
        }
    }
};

#define Weather_Fog 1
#define Weather_Rain 2
#define Weather_Frost 3

struct Weather
{
    int kind;
    int Hit;

    Weather(int mKind) : kind(mKind)
    {
        if(kind == 1)
        {
            Hit = 2;
        }
        if(kind == 2)
        {
            Hit = 1;
        }
        if(kind == 3)
        {
            Hit = 2;
        }
    }

    Weather()
    {
        kind = 0;
        Hit = 0;
    }
};

class PlayerInfo
{
private:
    vector<Card*> mHand;
    vector<Card*> mFirstRow;
    vector<Card*> mSecondRow;
    vector<Card*> mThirdRow;
    vector<Card*> mGrave;
    vector<Card*> mDeckLeft;
    vector<DeckInfo> mDecks;
    vector<string> mCardList;
    string mUserName;
    string mUserInfo;
    string mPassword;
    UserLevel mUserLevel;
    UserScore mUserScore;
    UserId mUserId;
    map<string, int> mCardMap;
    vector<string> leaderList;
    int mKegsnum;
    int mMoney;
    vector<Card*> temp;
    bool HaveUsedLeader;
    Card* mLeader;
    int mWinNum;
    bool mPassed;
    int mNowRoundScore[3];

    Weather FirstWeather;
    Weather SecondWeather;
    Weather ThirdWeather;

    void PrePareForFirst(int deckIndex);

    // Get New Cards(Called every round)
    // 回合开始的抽卡（每一回合调用一次，回合开始进行抽卡）
    void GetNewCards(int round);

    // Get Card From Deck
    // 从卡堆中抽出卡牌
    void GetCardsFromDeck(int num);

    // Get deck info from the string of deck
    // 从记录里面的deck的string获得结构体
    DeckInfo GetDeckInfo(string src) const;

    // Set Deck info from the string of deck log
    // 将整个结构体变为PlayerInfo里面的变量
    void GetDeckInfoFromString(string src);

    // Set Card info from the string of card log
    // 将整个card结构体变为PlayerInfo里面的变量
    void GetCardInfoFromString(string src);

    // Change the vector the last function got into a map
    // 把上面那个函数最后生成的vector换成Map
    void CardVectorToMap();

    // Choose out leader cards from the data
    // 提取领袖牌的列表
    void GetLeaderList();

    // Change the hand card at p
    void ChangeCardAtP(int p);

    void ChangedCardsPutTemp();

    void ClearSky();
    void ApplyWeatherHit();

    void DecreaseCardBlood(Card* c, int b);

public:
    PlayerInfo(int kegsnum = 0, int money = 0, string password = "", UserId id = "", UserLevel level = 0, UserScore score = 0, string name = "", string info = "") :mUserName(name), mUserInfo(info), mPassword(password), mUserLevel(level), mUserScore(score), mUserId(id), mKegsnum(kegsnum), mMoney(money), HaveUsedLeader(false), mLeader(nullptr), mWinNum(0), mPassed(false)
    {
        mNowRoundScore[0] = mNowRoundScore[1] = mNowRoundScore[2] = 0;
    }
    friend class GameController;
    void SetUserName(string u);
    void SetUserPassword(string p);
    void SetUserInfo(string i);
    void SetKegsNum(string kegs);
    void SetMoney(string money);
    void SetId(string id);
    void SetLevel(string level);
    void SetScore(string score);
    string UpdateData();

    friend class MainWindow;
    friend class GameController;

    friend class Choosing;

    friend class Collection;
    friend class DeckBuilder;
    friend class Playing;
    friend class BuyKeg;
    friend class Friends;
    friend class OpenKeg;
    friend class Settings;
    friend class Reward;

    friend class DeckBuildingChoose;
    friend class SwitchHandCard;
};

#endif
