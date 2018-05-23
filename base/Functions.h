#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "CONST.h"
#include "Cards.h"
#include <vector>

using std::vector;

// Choose num cards from a vector formed according to Card*
// 根据Card* from的属性选择一堆牌让用户抽取num张，抽到之后是否打出要看具体实现
extern ChooseReturnType ChooseFromCards(const Card* from, CardNum num);
// Choose num cards from a vector cards
// 在vector中让用户抽取num张，返回抽到的牌
extern ChooseReturnType ChooseFromCards(const vector<Card*> cards, CardNum num);

// Clear Sky, Cancel all the weather effort
// 天晴，去除所有天气的影响
extern void ClearSky(PLAYER);

// Get New Units by random
// 随机抽取几张卡牌，并且打出
extern Card* GetNewUnitAndPlay(CardNum);
extern Card* GetNewUnitAndPlay(Card*);

// Choose from the six (2 x 3) rows
// 从场上的六列之中选择一列，参数为限制条件（在哪里选择，己方、敌方还是都可以）
extern Row ChooseFromRows(PLAYER);

// Set frost to Position
// 放上冰霜
extern void SetFrost(Row);

// Set fog to Position
// 放上雾
extern void SetFog(Row);

// Set rain to Position
// 放上雨
extern void SetRain(Row);

// Hit units
// 降血
extern void HitUnit(Row, int);
extern void HitUnit(vector<Card*>, int);
extern void HitUnit(Card*, int);

// Kill units
// 打死单位
extern void KillUnit(Row);
extern void KillUnit(vector<Card*>);
extern void KillUnit(Card*);

// Give Armor to units
// 给护甲
extern void GetArmor(Row, int);
extern void GetArmor(vector<Card*>, int);
extern void GetArmor(Card*, int);

// Give bonus to units
// 给回血
extern void GetBonus(Row, int);
extern void GetBonus(vector<Card*>, int);
extern void GetBonus(Card*, int);

// Select the neighboring n cards
// 选择相邻的几个卡牌并且返回其vector
extern vector<Card*> SelectNeighbor(PLAYER, int);

// Swap the blood of the two vectors
// 交换两个vector里面单位的血量
extern void SwapBlood(vector<Card*>, vector<Card*>);

// Get the max Blood unit of all
// 获取全场所有的单位中血量最多的（默认全场）
extern vector<Card*> MaxBlood(PLAYER = cAll);
extern vector<Card*> MaxBlood(Row);

// Get the min Blood unit of all
// 获取全场所有的单位中血量最少的（默认全场）
extern vector<Card*> MinBlood(PLAYER = cAll);
extern vector<Card*> MinBlood(Row);

// Get the sum blood of all the units
// 获取参数代表的所有单位的血量总和
extern int CountAllBlood(Row);
extern int CountAllBlood(PLAYER);

// Play the card
// 打出这张卡牌
extern void PlayCard(Card*);

// Put the card to the top of deck
// 把牌放回牌堆
extern void PutToTop(Card*);

// From the top of deck take a gold card
// 在牌堆顶上抽一张金卡
extern Card* GetGoldCardFromTop();

// From the top of deck take a silver card
// 在牌堆顶上抽一张银卡
extern Card* GetSilverCardFromTop();

// Get two eggs in the left of Card_Celaeno_Harpy
// 左侧生成两个鹰身女妖蛋
extern void LeftTwoEggs();

// Run the last wish of cards
// 触发遗愿
extern void RunLastWish(vector<Card*>);

// Get the neighbors card vector
// 获取邻居卡牌
extern vector<Card*> Neighbors(Card*);

// Choose one card from the cards in hand
// 从手牌中选出一张牌
extern int ChooseOneCardFromHand();

#endif
