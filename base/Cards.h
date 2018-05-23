#ifndef CARDS_H
#define CARDS_H

#include "CONST.h"
#include <vector>
#include <string>
using namespace std;

extern vector<int> LeadNums;

class Card;

struct Row
{
    PLAYER mPlayer;
    eRows mRowInfo;

    Row(eRows r, PLAYER p = cMe) :mPlayer(p), mRowInfo(r) {}
};

struct DetailedCardPos
{
    Row mRow;
    int LeftNo; // The position Counted from the first one of the left. 从左边数起第几个

    DetailedCardPos(Row r, int no) :mRow(r), LeftNo(no) {}
};

class Card
{
public:
//	The Type of Card
//	卡牌种类
    CardType		mCardType;
//	The kind of card
//	卡牌类型（领袖、天气等）
    CardKind		mCardKind;
//	The level of card
//	卡牌等级（金银铜）
    CardLevel		mCardLevel;
    //	The origin blood of Card
//	卡牌初始血量
    CardBloodOrigin	mCardBloodOrigin;
//	Now blood of Card
//	卡牌当前血量
    CardBloodNow	mCardBloodNow;
//	The position of Card
//	卡牌位置
    CardPosition	mCardPosition;
//	The position you can put a card in the game
//	卡牌在场上呆的位置
    CardCanPutPosition	mCardPutPosition;
    int mCardShield;
    bool mCardSuperShield;
    bool mLostAfterDie;
    int mWaitInterval;
    int mWaitNow;

public:
    Card() : mCardType(cCard_Base), mCardKind(cUnit), mCardLevel(cBronze), mCardBloodOrigin(0), mCardBloodNow(0), mCardPosition(cGrave), mCardPutPosition(cPutNone), mCardShield(0), mCardSuperShield(false), mLostAfterDie(false), mWaitInterval(0), mWaitNow(0) {}
    virtual ~Card() {}
//	Get Row of the Card
//	获取卡牌当前在第几列
    Row GetRow() throw(string);
//  Get the index of card
//  获取类的编号
    virtual int GetIndex() = 0;
//	Called when you set the card(choosing other cards and so on)
//	放置牌时的效果（包括选择其他卡牌作为攻击、补充的目标、选择其他卡片抽取，执行血量增减）
    virtual void Set() = 0;
//	Called when the round is over
//	一局结束后时的效果
    virtual void EndRound() {};
//	Called when you need to play a card
//	在自己出牌开始时的效果
    virtual void Start() {};
//	Called when your enemy need to play a card
//	在敌方出牌开始时的效果
    virtual void EnemyStart() {};
//	Called when a new round is started
//	开始新一轮时的效果
    virtual void NewRound() {};
//  Get Class Name
//  获取类名
    virtual string GetCardClassName() { return "Card_None"; };

    friend class DeckBuildingChoose;
};

// 破晓
class Card_First_Light: public Card
{
public:
    Card_First_Light()
    {
        mCardType = cCard_First_Light;
        mCardKind = cSpecialNormal;
    }

    // 生成“晴空”或“调遣”
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 刺骨冰霜
class Card_Biting_Frost : public Card
{
public:
    Card_Biting_Frost()
    {
        mCardType = cCard_Biting_Frost;
        mCardKind = cWeather;
    }

    // 在对方单排降下“刺骨冰霜”。刺骨冰霜：每当己方回合开始时，对所在排最弱的单个单位造成2点伤害。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 蔽日浓雾
class Card_Impenetrable_Fog : public Card
{
public:
    Card_Impenetrable_Fog()
    {
        mCardType = cCard_Impenetrable_Fog;
        mCardKind = cWeather;
    }

    // 在对方单排降下“蔽日浓雾”。蔽日浓雾：每当己方回合开始时，对所在排最强的单位造成2点伤害。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 倾盆大雨
class Card_Torrential_Rain : public Card
{
public:
    Card_Torrential_Rain()
    {
        mCardType = cCard_Torrential_Rain;
        mCardKind = cWeather;
    }

    // 在对方单排降下“倾盆大雨”。倾盆大雨：每当己方回合开始时，对所在排最多5个最弱的单位造成1点伤害。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 撕裂
class Card_Lacerate : public Card
{
public:
    Card_Lacerate()
    {
        mCardType = cCard_Lacerate;
        mCardKind = cSpecialNormal;
    }

    // 对单排所有单位造成3点伤害。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 雷霆药水
class Card_Thunderbolt_Potion : public Card
{
public:
    Card_Thunderbolt_Potion()
    {
        mCardType = cCard_Thunderbolt_Potion;
        mCardKind = cSpecialNormal;
    }

    // 使 3 个相邻单位获得 2 点护甲和 3 点增益。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 指挥号角
class Card_Commander_s_Horn : public Card
{
public:
    Card_Commander_s_Horn()
    {
        mCardType = cCard_Commander_s_Horn;
        mCardKind = cSpecialNormal;
        mCardLevel = cSilver;
    }

    // 使5个相邻单位获得4点增益。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 贝克尔的扭曲之镜
class Card_Bekker_s_Twisted_Mirror : public Card
{
public:
    Card_Bekker_s_Twisted_Mirror()
    {
        mCardType = cCard_Bekker_s_Twisted_Mirror;
        mCardKind = cSpecialNormal;
        mCardLevel = cSilver;
    }

    // 将最强和最弱单位的战力互换。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 杰洛特：伊格尼法印
class Card_Geralt__Igni : public Card
{
public:
    Card_Geralt__Igni()
    {
        mCardType = cCard_Geralt__Igni;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 4;
        mCardLevel = cGold;
    }

    // 部署：若对方同排总战力不低于20，则摧毁其上所有最强的单位。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 达冈
class Card_Dagon : public Card
{
public:
    Card_Dagon()
    {
        mCardType = cCard_Dagon;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 6;
        mCardKind = cLeader;
        mCardLevel = cGold;
        mLostAfterDie = true;
    }

    // 生成“达冈”。部署：生成“刺骨冰霜”、“蔽日浓雾”或“倾盆大雨”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 小雾妖
class Card_Foglet : public Card
{
public:
    Card_Foglet()
    {
        mCardType = cCard_Foglet;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 2;
    }

    // 每当己方“蔽日浓雾”在敌排生效，便从牌组打出，或复活至己方同排。若场上不再存在“蔽日浓雾”效果，则被摧毁。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 盖尔
class Card_Ge_els : public Card
{
public:
    Card_Ge_els()
    {
        mCardType = cCard_Ge_els;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 2;
        mCardLevel = cGold;
    }

    // 部署：从牌组顶端各抽1张金色牌和银色牌。打出1张，将另1张置于牌组顶端。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 赛尔伊诺鹰身女妖
class Card_Celaeno_Harpy : public Card
{
public:
    Card_Celaeno_Harpy()
    {
        mCardType = cCard_Celaeno_Harpy;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 3;
    }

    // 部署：在自身左侧生成2个“鹰身女妖蛋”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 萝卜
class Card_Roach : public Card
{
public:
    Card_Roach()
    {
        mCardType = cCard_Roach;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 4;
        mCardLevel = cSilver;
    }

    // 己方每次从手牌打出金色牌时（领袖牌除外），便将牌组中的“萝卜”在其能力结算之前召唤至随机位置。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 林精
class Card_Woodland_Spirit : public Card
{
public:
    Card_Woodland_Spirit()
    {
        mCardType = cCard_Woodland_Spirit;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 5;
        mCardLevel = cGold;
    }

    // 部署：生成3只“暴怒的狼”，并在对方同排降下“蔽日浓雾”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 土元素
class Card_Earth_Elemental : public Card
{
public:
    Card_Earth_Elemental()
    {
        mCardType = cCard_Earth_Elemental;
        mCardPutPosition = cPutCenterNearest;
        mCardBloodOrigin = mCardBloodNow = 6;
    }

    // 部署：获得护盾。遗愿：在该排末位生成2个“次级土元素”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 老巫妪：织婆
class Card_Crone__Weavess : public Card
{
public:
    Card_Crone__Weavess()
    {
        mCardType = cCard_Crone__Weavess;
        mCardPutPosition = cPutFarest;
        mCardBloodOrigin = mCardBloodNow = 6;
        mCardLevel = cSilver;
    }

    // 部署：从牌组打出“煮婆”和“呢喃婆”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 老巫妪：呢喃婆
class Card_Crone__Whispess : public Card
{
public:
    Card_Crone__Whispess()
    {
        mCardType = cCard_Crone__Whispess;
        mCardPutPosition = cPutFarest;
        mCardBloodOrigin = mCardBloodNow = 6;
        mCardLevel = cSilver;
    }

    // 部署：从牌组打出“煮婆”和“织婆”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 老巫妪：煮婆
class Card_Crone__Brewess : public Card
{
public:
    Card_Crone__Brewess()
    {
        mCardType = cCard_Crone__Brewess;
        mCardPutPosition = cPutFarest;
        mCardBloodOrigin = mCardBloodNow = 8;
        mCardLevel = cSilver;
    }

    // 部署：从牌组打出“呢喃婆”和“织婆”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 大狮鹫
class Card_Archgriffin : public Card
{
public:
    Card_Archgriffin()
    {
        mCardType = cCard_Archgriffin;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 7;
    }

    // 部署：移除所在排的天气效果。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 狂猎骑士
class Card_Wild_Hunt_Rider : public Card
{
public:
    Card_Wild_Hunt_Rider()
    {
        mCardType = cCard_Wild_Hunt_Rider;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 8;
        mCardLevel = cBronze;
    }

    // 使对方此排的“刺骨冰霜”伤害提高 1 点
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 卡兰希尔
class Card_Caranthir : public Card
{
public:
    Card_Caranthir()
    {
        mCardType = cCard_Caranthir;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 9;
        mCardLevel = cGold;
    }

    // 部署：将1个敌军单位移至对方同排，并在此排降下“刺骨冰霜”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 畏惧者
class Card_Frightener : public Card
{
public:
    Card_Frightener()
    {
        mCardType = cCard_Frightener;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 10;
        mCardLevel = cSilver;
        mLostAfterDie = true;
    }

    // 部署：从牌组顶端抽1张牌。将所在战场其他排的1个单位移至自身的同一排。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 暗影长者
class Card_Unseen_Elder : public Card
{
public:
    Card_Unseen_Elder()
    {
        mCardType = cCard_Unseen_Elder;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 5;
        mCardKind = cLeader;
        mCardLevel = cGold;
        mLostAfterDie = true;
    }

    // 部署：吞噬3个友军单位，吸收它们的战力为自身的基础战力。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 蟹蜘蛛
class Card_Arachas : public Card
{
public:
    Card_Arachas()
    {
        mCardType = cCard_Arachas;
        mCardPutPosition = cPutMedium;
        mCardBloodOrigin = mCardBloodNow = 3;
    }

    // 部署：从牌组打出所有“蟹蜘蛛”。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 蜥蜴人战士
class Card_Vran_Warrior : public Card
{
public:
    Card_Vran_Warrior()
    {
        mCardType = cCard_Vran_Warrior;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 5;
        mWaitInterval = mWaitNow = 2;
    }

    // 部署：吞噬右侧单位。每2回合，己方回合开始时，吞噬右侧单位。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

// 蟹蜘蛛巨兽
class Card_Arachas_Behemoth : public Card
{
public:
    Card_Arachas_Behemoth()
    {
        mCardType = cCard_Arachas_Behemoth;
        mCardPutPosition = cPutFarest;
        mCardBloodOrigin = mCardBloodNow = 6;
    }

    // 每当有友军单位吞噬1张牌，便生成1只“蟹蜘蛛”，随后受到1点伤害（无视护甲）。部 署：获得2点护甲。
    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Clear_Sky : public Card
{
public:
    Card_Clear_Sky()
    {
        mCardType = cCard_Clear_Sky;
        mCardKind = cSpecialNormal;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Get_New : public Card
{
public:
    Card_Get_New()
    {
        mCardType = cCard_Get_New;
        mCardKind = cSpecialNormal;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Eggs : public Card
{
public:
    Card_Eggs()
    {
        mCardType = cCard_Eggs;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 1;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Angry_Wolf : public Card
{
public:
    Card_Angry_Wolf()
    {
        mCardType = cCard_Angry_Wolf;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 1;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Little_Girl : public Card
{
public:
    Card_Little_Girl()
    {
        mCardType = cCard_Little_Girl;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 1;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Low_Ground : public Card
{
public:
    Card_Low_Ground()
    {
        mCardType = cCard_Low_Ground;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 2;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

class Card_Little_Spider : public Card
{
public:
    Card_Little_Spider()
    {
        mCardType = cCard_Little_Spider;
        mCardPutPosition = cPutAll;
        mCardBloodOrigin = mCardBloodNow = 3;
        mLostAfterDie = true;
    }

    void Set() override final;

    int GetIndex() override final;
    string GetCardClassName() override final;
};

extern Card* GetNewCard(int id);


#endif
