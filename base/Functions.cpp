#include "Functions.h"
using namespace std;

// Choose num cards from a vector formed according to Card*
// 根据Card* from的属性选择一堆牌让用户抽取num张，抽到之后是否打出要看具体实现
ChooseReturnType ChooseFromCards(const Card* from, CardNum num)
{
    return 1;
}

ChooseReturnType ChooseFromCards(const vector<Card*> cards, CardNum num)
{
    return 1;
}

// Clear Sky, Cancel all the weather effort
// 天晴，去除所有天气的影响
void ClearSky(PLAYER)
{

}

// Get New Units by random, in use of Card_First_Light
// 随机抽取几张卡牌，并且打出
Card* GetNewUnitAndPlay(CardNum)
{
    return NULL;
}

Card* GetNewUnitAndPlay(Card*)
{
    return NULL;
}

// Choose from the six (2 x 3) rows
// 从场上的六列之中选择一列，参数为限制条件（在哪里选择，己方、敌方还是都可以）
Row ChooseFromRows(PLAYER)
{
    return Row(cRowsNum);
}

// Set frost to Position
// 放上冰霜
void SetFrost(Row)
{

}

// Set fog to Position
// 放上雾
void SetFog(Row)
{

}

// Set rain to Position
// 放上雨
void SetRain(Row)
{

}

// Hit units
// 降血
void HitUnit(Row, int)
{

}

void HitUnit(vector<Card*>, int)
{

}

void HitUnit(Card*, int)
{

}

// Kill units
// 打死单位
void KillUnit(Row)
{

}

void KillUnit(vector<Card*>)
{

}

void KillUnit(Card*)
{

}

// Give Armor to units
// 给护甲
void GetArmor(Row, int)
{

}

void GetArmor(vector<Card*>, int)
{

}

void GetArmor(Card*, int)
{

}

// Give bonus to units
// 给回血
void GetBonus(Row, int)
{

}

void GetBonus(vector<Card*>, int)
{

}

void GetBonus(Card*, int)
{

}

// Select the neighboring n cards
// 选择相邻的几个卡牌并且返回其vector
vector<Card*> SelectNeighbor(PLAYER, int)
{
    return vector<Card*>();
}

// Swap the blood of the two vectors
// 交换两个vector里面单位的血量
void SwapBlood(vector<Card*>, vector<Card*>)
{

}

// Get the max Blood unit of all
// 获取全场所有的单位中血量最多的（默认全场）
vector<Card*> MaxBlood(PLAYER)
{
    return vector<Card*>();
}

vector<Card*> MaxBlood(Row)
{
    return vector<Card*>();
}

// Get the min Blood unit of all
// 获取全场所有的单位中血量最少的（默认全场）
vector<Card*> MinBlood(PLAYER)
{
    return vector<Card*>();
}

vector<Card*> MinBlood(Row)
{
    return vector<Card*>();
}

// Get the sum blood of all the units
// 获取参数代表的所有单位的血量总和
int CountAllBlood(Row)
{
    return 0;
}

int CountAllBlood(PLAYER)
{
    return 0;
}

// Play the card
// 打出这张卡牌
void PlayCard(Card*)
{

}

// Put the card to the top of deck
// 把牌放回牌堆
void PutToTop(Card*)
{

}

// From the top of deck take a gold card
// 在牌堆顶上抽一张金卡
Card* GetGoldCardFromTop()
{
    return NULL;
}

// From the top of deck take a silver card
// 在牌堆顶上抽一张银卡
Card* GetSilverCardFromTop()
{
    return NULL;
}

// Get two eggs in the left of Card_Celaeno_Harpy
// 左侧生成两个鹰身女妖蛋
void LeftTwoEggs()
{

}

// Run the last wish of cards
// 触发遗愿
void RunLastWish(vector<Card*>)
{

}

// Get the neighbors card vector
// 获取邻居卡牌
vector<Card*> Neighbors(Card*)
{
    return vector<Card*>();
}


int ChooseOneCardFromHand()
{
    return 0;
}
