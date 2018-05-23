#include "Cards.h"
#include "Functions.h"

using namespace std;

vector<int> LeadNums = {10, 24};

Card* GetNewCard(int id)
{
    switch (id)
    {
    case 1:
        return new Card_First_Light();
    case 2:
        return new Card_Biting_Frost();
    case 3:
        return new Card_Impenetrable_Fog();
    case 4:
        return new Card_Torrential_Rain();
    case 5:
        return new Card_Lacerate();
    case 6:
        return new Card_Thunderbolt_Potion();
    case 7:
        return new Card_Commander_s_Horn();
    case 8:
        return new Card_Bekker_s_Twisted_Mirror();
    case 9:
        return new Card_Geralt__Igni();
    case 10:
        return new Card_Dagon();
    case 11:
        return new Card_Foglet();
    case 12:
        return new Card_Ge_els();
    case 13:
        return new Card_Celaeno_Harpy();
    case 14:
        return new Card_Roach();
    case 15:
        return new Card_Woodland_Spirit();
    case 16:
        return new Card_Earth_Elemental();
    case 17:
        return new Card_Crone__Weavess();
    case 18:
        return new Card_Crone__Whispess();
    case 19:
        return new Card_Crone__Brewess();
    case 20:
        return new Card_Archgriffin();
    case 21:
        return new Card_Wild_Hunt_Rider();
    case 22:
        return new Card_Caranthir();
    case 23:
        return new Card_Frightener();
    case 24:
        return new Card_Unseen_Elder();
    case 25:
        return new Card_Arachas();
    case 26:
        return new Card_Vran_Warrior();
    case 27:
        return new Card_Arachas_Behemoth();
    case 500:
        return new Card_Clear_Sky();
    case 501:
        return new Card_Get_New();
    case 502:
        return new Card_Eggs();
    case 503:
        return new Card_Angry_Wolf();
    case 504:
        return new Card_Little_Girl();
    case 505:
        return new Card_Low_Ground();
    case 506:
        return new Card_Little_Spider();
    default:
        return nullptr;
    }
}

// Get Row of the Card
// 获取卡牌当前在第几列
Row Card::GetRow() throw(string)
{
    switch (mCardPosition)
    {
    case cCenterNearest:
        return Row(cFirstRow);
        break;
    case cMedium:
        return Row(cSecondRow);
        break;
    case cFarest:
        return Row(cThirdRow);
        break;
    default:
        throw("The Card Havn't Been Put!");
        break;
    }
}




////////////////////////////////////////////////////////
////////////////////////重载Set区////////////////////////
////////////////////////////////////////////////////////




// 生成“晴空”或“调遣”
void Card_First_Light::Set()
{
    switch (ChooseFromCards(this, 1))
    {
    case 0:
        ClearSky(cMe);
        break;
    default:
        GetNewUnitAndPlay(1);
        break;
    }
}

// 在对方单排降下“刺骨冰霜”。刺骨冰霜：每当己方回合开始时，对所在排最弱的单个单位造成2点伤害。
void Card_Biting_Frost::Set()
{
    SetFrost(ChooseFromRows(cEnemy));
}

// 在对方单排降下“蔽日浓雾”。蔽日浓雾：每当己方回合开始时，对所在排最强的单位造成2点伤害。
void Card_Impenetrable_Fog::Set()
{
    SetFog(ChooseFromRows(cEnemy));
}

// 在对方单排降下“倾盆大雨”。倾盆大雨：每当己方回合开始时，对所在排最多5个最弱的单位造成1点伤害。
void Card_Torrential_Rain::Set()
{
    SetRain(ChooseFromRows(cEnemy));
}

// 对单排所有单位造成3点伤害。
void Card_Lacerate::Set()
{
    HitUnit(ChooseFromRows(cEnemy), 3);
}

// 使 3 个相邻单位获得 2 点护甲和 3 点增益。
void Card_Thunderbolt_Potion::Set()
{
    auto t = SelectNeighbor(cAll, 3);
    GetArmor(t, 3);
    GetBonus(t, 3);
}

// 使5个相邻单位获得4点增益。
void Card_Commander_s_Horn::Set()
{
    GetBonus(SelectNeighbor(cAll, 5), 4);
}

// 将最强和最弱单位的战力互换。
void Card_Bekker_s_Twisted_Mirror::Set()
{
    SwapBlood(MaxBlood(), MinBlood());
}

// 部署：若对方同排总战力不低于20，则摧毁其上所有最强的单位。
void Card_Geralt__Igni::Set()
{
    Row targetRow = GetRow();
    targetRow.mPlayer = cEnemy;
    if (CountAllBlood(targetRow) >= 20)
    {
        KillUnit(MaxBlood(targetRow));
    }
}

// 生成“达冈”。部署：生成“刺骨冰霜”、“蔽日浓雾”或“倾盆大雨”。
void Card_Dagon::Set()
{
    switch (ChooseFromCards(this, 1))
    {
    case 0:
        GetNewUnitAndPlay(new Card_Biting_Frost());
        break;
    case 1:
        GetNewUnitAndPlay(new Card_Impenetrable_Fog());
        break;
    default:
        GetNewUnitAndPlay(new Card_Biting_Frost());
        break;
    }
}

void Card_Foglet::Set() {}

// 部署：从牌组顶端各抽1张金色牌和银色牌。打出1张，将另1张置于牌组顶端。
void Card_Ge_els::Set()
{
    Card* g = GetGoldCardFromTop();
    Card* s = GetSilverCardFromTop();
    vector<Card*> got = { g, s };
    switch (ChooseFromCards(got, 1))
    {
    case 0:
        PlayCard(g);
        PutToTop(s);
        break;
    default:
        PlayCard(s);
        PutToTop(g);
        break;
    }
}

// 部署：在自身左侧生成2个“鹰身女妖蛋”。
void Card_Celaeno_Harpy::Set()
{
    LeftTwoEggs();
}

// 己方每次从手牌打出金色牌时（领袖牌除外），便将牌组中的“萝卜”在其能力结算之前召唤至随机位置。
void Card_Roach::Set()
{
}

// 后来换为别的实现方式，未使用此方式
void Card_Woodland_Spirit::Set()
{
}

void Card_Earth_Elemental::Set()
{
}

void Card_Crone__Weavess::Set()
{
}

void Card_Crone__Whispess::Set()
{
}

void Card_Crone__Brewess::Set()
{
}

void Card_Archgriffin::Set()
{
}

void Card_Wild_Hunt_Rider::Set()
{
}

void Card_Caranthir::Set()
{
}

void Card_Frightener::Set()
{
}

void Card_Unseen_Elder::Set()
{
}

void Card_Arachas::Set()
{
}

void Card_Vran_Warrior::Set()
{
}

void Card_Arachas_Behemoth::Set()
{
}

void Card_Clear_Sky::Set()
{
}

void Card_Get_New::Set()
{
}

void Card_Eggs::Set()
{
}

void Card_Angry_Wolf::Set()
{
}

void Card_Little_Girl::Set()
{
}

void Card_Low_Ground::Set()
{
}

void Card_Little_Spider::Set()
{
}

////////////////////////////////////////////////////////
/////////////////////重载GetIndex区/////////////////////
////////////////////////////////////////////////////////




int Card_First_Light::GetIndex()
{
    return 1;
}

int Card_Biting_Frost::GetIndex()
{
    return 2;
}

int Card_Impenetrable_Fog::GetIndex()
{
    return 3;
}

int Card_Torrential_Rain::GetIndex()
{
    return 4;
}

int Card_Lacerate::GetIndex()
{
    return 5;
}

int Card_Thunderbolt_Potion::GetIndex()
{
    return 6;
}

int Card_Commander_s_Horn::GetIndex()
{
    return 7;
}

int Card_Bekker_s_Twisted_Mirror::GetIndex()
{
    return 8;
}

int Card_Geralt__Igni::GetIndex()
{
    return 9;
}

int Card_Dagon::GetIndex()
{
    return 10;
}

int Card_Foglet::GetIndex()
{
    return 11;
}

int Card_Ge_els::GetIndex()
{
    return 12;
}

int Card_Celaeno_Harpy::GetIndex()
{
    return 13;
}

int Card_Roach::GetIndex()
{
    return 14;
}

int Card_Woodland_Spirit::GetIndex()
{
    return 15;
}

int Card_Earth_Elemental::GetIndex()
{
    return 16;
}

int Card_Crone__Weavess::GetIndex()
{
    return 17;
}

int Card_Crone__Whispess::GetIndex()
{
    return 18;
}

int Card_Crone__Brewess::GetIndex()
{
    return 19;
}

int Card_Archgriffin::GetIndex()
{
    return 20;
}

int Card_Wild_Hunt_Rider::GetIndex()
{
    return 21;
}

int Card_Caranthir::GetIndex()
{
    return 22;
}

int Card_Frightener::GetIndex()
{
    return 23;
}

int Card_Unseen_Elder::GetIndex()
{
    return 24;
}

int Card_Arachas::GetIndex()
{
    return 25;
}

int Card_Vran_Warrior::GetIndex()
{
    return 26;
}

int Card_Arachas_Behemoth::GetIndex()
{
    return 27;
}

int Card_Clear_Sky::GetIndex()
{
    return 500;
}

int Card_Get_New::GetIndex()
{
    return 501;
}

int Card_Eggs::GetIndex()
{
    return 502;
}

int Card_Angry_Wolf::GetIndex()
{
    return 503;
}

int Card_Little_Girl::GetIndex()
{
    return 504;
}

int Card_Low_Ground::GetIndex()
{
    return 505;
}

int Card_Little_Spider::GetIndex()
{
    return 506;
}

////////////////////////////////////////////////////////
/////////////////重载GetCardClassName区//////////////////
////////////////////////////////////////////////////////




string Card_First_Light::GetCardClassName()
{
    return "Card_First_Light";
}

string Card_Biting_Frost::GetCardClassName()
{
    return "Card_Biting_Frost";
}

string Card_Impenetrable_Fog::GetCardClassName()
{
    return "Card_Impenetrable_Fog";
}

string Card_Torrential_Rain::GetCardClassName()
{
    return "Card_Torrential_Rain";
}

string Card_Lacerate::GetCardClassName()
{
    return "Card_Lacerate";
}

string Card_Thunderbolt_Potion::GetCardClassName()
{
    return "Card_Thunderbolt_Potion";
}

string Card_Commander_s_Horn::GetCardClassName()
{
    return "Card_Commander_s_Horn";
}

string Card_Bekker_s_Twisted_Mirror::GetCardClassName()
{
    return "Card_Bekker_s_Twisted_Mirror";
}

string Card_Geralt__Igni::GetCardClassName()
{
    return "Card_Geralt__Igni";
}

string Card_Dagon::GetCardClassName()
{
    return "Card_Dagon";
}

string Card_Foglet::GetCardClassName()
{
    return "Card_Foglet";
}

string Card_Ge_els::GetCardClassName()
{
    return "Card_Ge_els";
}

string Card_Celaeno_Harpy::GetCardClassName()
{
    return "Card_Celaeno_Harpy";
}

string Card_Roach::GetCardClassName()
{
    return "Card_Roach";
}

string Card_Woodland_Spirit::GetCardClassName()
{
    return "Card_Woodland_Spirit";
}

string Card_Earth_Elemental::GetCardClassName()
{
    return "Card_Earth_Elemental";
}

string Card_Crone__Weavess::GetCardClassName()
{
    return "Card_Crone__Weavess";
}

string Card_Crone__Whispess::GetCardClassName()
{
    return "Card_Crone__Whispess";
}

string Card_Crone__Brewess::GetCardClassName()
{
    return "Card_Crone__Brewess";
}

string Card_Archgriffin::GetCardClassName()
{
    return "Card_Archgriffin";
}

string Card_Wild_Hunt_Rider::GetCardClassName()
{
    return "Card_Wild_Hunt_Rider";
}

string Card_Caranthir::GetCardClassName()
{
    return "Card_Caranthir";
}

string Card_Frightener::GetCardClassName()
{
    return "Card_Frightener";
}

string Card_Unseen_Elder::GetCardClassName()
{
    return "Card_Unseen_Elder";
}

string Card_Arachas::GetCardClassName()
{
    return "Card_Arachas";
}

string Card_Vran_Warrior::GetCardClassName()
{
    return "Card_Vran_Warrior";
}

string Card_Arachas_Behemoth::GetCardClassName()
{
    return "Card_Arachas_Behemoth";
}

string Card_Clear_Sky::GetCardClassName()
{
    return "Card_Clear_Sky";
}

string Card_Get_New::GetCardClassName()
{
    return "Card_Get_New";
}

string Card_Eggs::GetCardClassName()
{
    return "Card_Eggs";
}

string Card_Angry_Wolf::GetCardClassName()
{
    return "Card_Angry_Wolf";
}

string Card_Little_Girl::GetCardClassName()
{
    return "Card_Little_Girl";
}

string Card_Low_Ground::GetCardClassName()
{
    return "Card_Low_Ground";
}

string Card_Little_Spider::GetCardClassName()
{
    return "Card_Little_Spider";
}
