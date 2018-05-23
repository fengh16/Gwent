#ifndef CONST_H
#define CONST_H
#include <string>

#define CardType			eCardType
#define CardBloodOrigin		int
#define CardBloodNow		int
#define CardPosition		eCardPosition
#define CardCanPutPosition	eCardCanPutPosition
#define CardKind			eCardKind
#define CardLevel			eCardLevel
#define PLAYER				ePlayer
#define ChooseReturnType	int
#define CardNum				int
#define UserLevel			int
#define UserScore			int
#define UserId				string
#define GameStates			eGameStates
#define NowStates			eNowStates
#define ChooseFunctionFrom  eChooseFunctionFrom

#define CardWidthNormal     367
#define CardHeightNormal    420
#define PlayGroundWidth     4192
#define PlayGroundHeight    2416
#define HandCardHeight      357
#define InnerCardHeight     209
#define InnerPosition1      488
#define InnerPosition2      718
#define InnerPosition3      948
#define InnerPosition4      1248
#define InnerPosition5      1475
#define InnerPosition6      1709
#define HandCardPosition    2057
#define UseLessLeftPosition 1307
#define UsefulWidth         1594
#define HandUseLessLeft     50
#define HandUsefullWidth    3500

// Use the following to show where did the choose list function called
// 用于判断在哪里进行的从列表中选一个的调用
enum eChooseFunctionFrom
//  (From Deck Building to choose a leader, Choose from decks)
//  （建立牌堆页面，用于选择一个领袖牌,
    { cBuildPageChooseLeader,               cChooseDeck,    cChooseSet01,   cChooseSet10,   cChooseSet12,
      cChooseSet20, cChooseFunctionFromNum };

// The Players	(Me,	Enemy,		All)
// 玩家			（我，	对手，		双方都有）
enum ePlayer	{ cMe,	cEnemy,		cAll, cPlayerNum };

// All the rows	(First(near the center,		Second,			Third(Near the player))
// 所有行		（第一行（最靠近中间的），		第二行，			第三行（最靠近玩家的））
enum eRows		{ cFirstRow,				cSecondRow,		cThirdRow, cRowsNum };

// Type of Playing	(Play with friends,		with AI)
// 游戏模式			（双人，					人机）
enum ePlayType		{ cMultiPlay,			cSinglePlay,	cPlayTypeNum };

// States of the game	(Loading Game,	Choose what to do,	Log in,		Friends,
// 游戏状态				（加载中，		选择页面（即主页），	登录界面，	好友界面，
enum eNowStates			{ cLoading, 	cChoosing,			cLogingIn,	cFriends,
//	Build Deck,		PlayingGame,	Open Kegs,	Get Kegs,	Change Settings,
//	建立牌堆，		游戏过程中，		开牌桶，		获得牌桶，	设置，
    cDeckBuilding,	cPlaying,		cOpenKeg,	cBuyKeg,	cSetting,
//	Rewards Gotten,	All the cards you have,	End of game)
//	彩蛋收集，		已有的卡牌，				游戏结束结算环节）
    cRewarding,		cCollection,			cFinishedPlaying,	cNowStatesNum };

// States of the game(playing)	(Choose cards to change,	Decide which side to play,
// 游戏进行中的状态				（换卡，						投色子确定谁先走，
enum eGameStates				{ cChangingCards,			cDecidingWho,
//	Choosing with card to play,	End of a round)
//	选择出牌，					回合结束）
    cChoosingCards,				cEnd,           cChooseCardsOnlyEn1,    cChooseCardsOnlyEn, cChooseCardsOnlyMe, cChoosingPlaceToPut,    cChoosingEnemyPlaceToPut,
                                  cChoosingTargetCards,
    cEnemyChoose,               cChooseEnemyRowForFrost,                cChooseEnemyRowForFog,
    cChooseEnemyRowForRain,     cChooseRowFor3Hit,             cChooseNeighbor3For3And3,
    cChooseNeighbor5For4,       cSet22ChooseEnemyCard,         cUnseenElder_Eat3,
                                  cUnseenElder_Eat2, cUnseenElder_Eat1, cEmegency, cGameStatesNum };

// All the kinds of cards	(leadercard,	Weather,	Special card (not weather),	ordinary unit)
// 卡牌属性					（领袖，			天气，		除了天气之外的其他技能牌，		普通单位）
enum eCardKind				{ cLeader,		cWeather,	cSpecialNormal,				cUnit, cCardKindNum };

// The level of cards	(Gold,		Silver,		Bronze)
// 卡牌等级				（金，		银，			铜）
enum eCardLevel			{ cGold,	cSilver,	cBronze, cCardLevelNum};

// All the Cards types
// 所有的卡牌，每一种会单独做一个类
enum eCardType
{
    cCard_Base,
    cCard_First_Light,      //破晓
    cCard_Biting_Frost,     //刺骨冰霜
    cCard_Impenetrable_Fog, //蔽日浓雾
    cCard_Torrential_Rain,  //倾盆大雨
    cCard_Lacerate,			//撕裂
    cCard_Thunderbolt_Potion,       //雷霆药水
    cCard_Commander_s_Horn, //指挥号角
    cCard_Bekker_s_Twisted_Mirror,  //贝克尔的扭曲之镜
    cCard_Geralt__Igni,     //杰洛特：伊格尼法印
    cCard_Dagon,            //达冈
    cCard_Foglet,           //小雾妖
    cCard_Ge_els,           //盖尔
    cCard_Celaeno_Harpy,    //赛尔伊诺鹰身女妖
    cCard_Roach,            //萝卜
    cCard_Woodland_Spirit,  //林精
    cCard_Earth_Elemental,  //土元素
    cCard_Crone__Weavess,   //老巫妪：织婆
    cCard_Crone__Whispess,  //老巫妪：呢喃婆
    cCard_Crone__Brewess,   //老巫妪：煮婆
    cCard_Archgriffin,      //大狮鹫
    cCard_Wild_Hunt_Rider,	//狂猎骑士
    cCard_Caranthir,        //卡兰希尔
    cCard_Frightener,       //畏惧者
    cCard_Unseen_Elder,     //暗影长者
    cCard_Arachas,			//蟹蜘蛛
    cCard_Vran_Warrior,     //蜥蜴人战士
    cCard_Arachas_Behemoth, //蟹蜘蛛巨兽
    cCard_Num,
    cCard_Clear_Sky = 500,  //By 1
    cCard_Get_New,          //By 1
    cCard_Eggs,             //
    cCard_Angry_Wolf,
    cCard_Little_Girl,
    cCard_Low_Ground,
    cCard_Little_Spider
};

// Position of the card	(In the deck,	in your hand,	in the graveyard,	in the center,
// 牌所在的位置			（牌堆里面，		在自己手里，		坟墓里，				在第一排（中间），
enum eCardPosition		{ cDeck,		cHand,			cGrave,				cCenterNearest,
//	in the medium row,	in the farest row(near the player))
//	第二排，				第三排（最靠近自己））
    cMedium,			cFarest,	cCardPositionNum };

// Position a card can be placed	(in the center of window,	in the second row,
// 卡牌被使用时可以放的位置			（在第一排（场地中间），		第二排，
enum eCardCanPutPosition				{ cPutCenterNearest,		cPutMedium,
//	Nearest to the player,	All the three row,	You can't place it anywhere)
//	第三排（最靠近自己），	三排都可以放，		效果排（不放在场上的））
    cPutFarest,				cPutAll,			cPutNone,	cCardCanPutPositionNum };

const std::string CARDSTRING[] =
{
    "Base",

    "<card id = \"16\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/115.png</img><cname>破晓</cname><ename>First Light</ename><role>中立</role><use>生成“晴空”或“调遣”。</use><pro>属性：特殊</pro></card>",

    "<card id = \"8\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/27.png</img><cname>刺骨冰霜</cname><ename>Biting Frost</ename><role>中立</role><use>在对方单排降下“刺骨冰霜”。刺骨冰霜：每当己方回合开始时，对所在排最弱的单个单位造成2 点伤害。</use><pro>属性：特殊，天气</pro></card>",

    "<card id = \"18\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/143.png</img><cname>蔽日浓雾</cname><ename>Impenetrable Fog</ename><role>中立</role><use>在对方单排降下“蔽日浓雾”。蔽日浓雾：每当己方回合开始时，对所在排最强的单位造成2点伤害。</use><pro>属性：特殊，天气</pro></card>",

    "<card id = \"41\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/263.png</img><cname>倾盆大雨</cname><ename>Torrential Rain</ename><role>中立</role><use>在对方单排降下“倾盆大雨”。倾盆大雨：每当己方回合开始时，对所在排最多5个最弱的单位造成1点伤害。</use><pro>属性：特殊，天气</pro></card>",

    "<card id = \"19\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/166.png</img><cname>撕裂</cname><ename>Lacerate</ename><role>中立</role><use>对单排所有单位造成3点伤害。</use><pro>属性：特殊</pro></card>",

    "<card id = \"17\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/142.png</img><cname>免疫增强</cname><ename>Immune Boost</ename><role>中立</role><use>使3个相邻单位获得3点护甲和3点增益。</use><pro>属性：特殊</pro></card>",

    "<card id = \"10\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/65.png</img><cname>指挥号角</cname><ename>Commander's Horn</ename><role>中立</role><use>使5个相邻单位获得4点增益。</use><pro>属性：特殊</pro></card>",

    "<card id = \"7\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/24.png</img><cname>贝克尔的扭曲之镜</cname><ename>Bekker's Twisted Mirror</ename><role>中立</role><use>将最强和最弱单位的战力互换。</use><pro>属性：特殊</pro></card>",

    "<card id = \"101\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/125.png</img><cname>杰洛特：伊格尼法印</cname><ename>Geralt: Igni</ename><role>中立</role><use>部署：若对方同排总战力不低于20，则摧毁其上所有最强的单位。</use><pro>属性：猎魔人</pro></card>",

    "<card id = \"183\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/75.png</img><cname>达冈</cname><ename>Dagon</ename><role>怪物</role><use>生成“达冈”。部署：生成“刺骨冰霜”、“蔽日浓雾”或“倾盆大雨”。</use><pro>复制名称到剪贴板获取卡牌图片</pro></card>",

    "<card id = \"57\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/116.png</img><cname>小雾妖</cname><ename>Foglet</ename><role>怪物</role><use>每当己方“蔽日浓雾”在敌排生效，便从牌组打出，或复活至己方同排。若场上不再存在“蔽日浓雾”效果，则被摧毁。</use><pro>属性：食腐生物</pro></card>",

    "<card id = \"58\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/122.png</img><cname>盖尔</cname><ename>Ge'els</ename><role>怪物</role><use>部署：从牌组顶端各抽1张金色牌和银色牌。打出1张，将另1张置于牌组顶端。</use><pro>属性：狂猎</pro></card>",

    "<card id = \"66\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/43.png</img><cname>赛尔伊诺鹰身女妖</cname><ename>Celaeno Harpy</ename><role>怪物</role><use>部署：在自身左侧生成2个“鹰身女妖蛋”。</use><pro>属性：野兽</pro></card>",

    "<card id = \"75\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/132.png</img><cname>鹰身女妖</cname><ename>Harpy</ename><role>怪物</role><use>部署：触发相邻单位的遗愿能力。</use><pro>属性：野兽</pro></card>",

    "<card id = \"169\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/297.png</img><cname>林精</cname><ename>Woodland Spirit</ename><role>怪物</role><use>部署：生成3只“暴怒的狼”，并在对方同排降下“蔽日浓雾”。</use><pro>属性：残物</pro></card>",

    "<card id = \"188\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/98.png</img><cname>土元素</cname><ename>Earth Elemental</ename><role>怪物</role><use>部署：获得护盾。遗愿：在该排末位生成2个“次级土元素”。</use><pro>属性：构造体</pro></card>",

    "<card id = \"181\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/70.png</img><cname>老巫妪：织婆</cname><ename>Crone: Weavess</ename><role>怪物</role><use>部署：从牌组打出“煮婆”和“呢喃婆”。</use><pro>属性：法师</pro></card>",

    "<card id = \"182\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/71.png</img><cname>老巫妪：呢喃婆</cname><ename>Crone: Whispess</ename><role>怪物</role><use>部署：从牌组打出“煮婆”和“织婆”。</use><pro>属性：法师</pro></card>",

    "<card id = \"256\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/69.png</img><cname>老巫妪：煮婆</cname><ename>Crone: Brewess</ename><role>怪物</role><use>部署：从牌组打出“呢喃婆”和“织婆”。</use><pro>属性：法师</pro></card>",

    "<card id = \"225\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/16.png</img><cname>大狮鹫</cname><ename>Archgriffin</ename><role>怪物</role><use>部署：移除所在排的天气效果。</use><pro>属性：野兽</pro></card>",

    "<card id = \"236\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/198.png</img><cname>尼斯里拉</cname><ename>Nithral</ename><role>怪物</role><use>对方战场上的“刺骨冰霜”伤害提升至3点。</use><pro>属性：狂猎</pro></card>",

    "<card id = \"281\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/40.png</img><cname>卡兰希尔</cname><ename>Caranthir</ename><role>怪物</role><use>部署：将3个敌军单位移至对方同排，并在此排降下“刺骨冰霜”。</use><pro>属性：狂猎，法师</pro></card>",

    "<card id = \"294\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/119.png</img><cname>畏惧者</cname><ename>Frightener</ename><role>怪物</role><use>部署：从牌组顶端抽1张牌。将所在战场其他排的1个单位移至自身的同一排。</use><pro>属性：构造体，佚亡</pro></card>",

    "<card id = \"166\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/272.png</img><cname>暗影长者</cname><ename>Unseen Elder</ename><role>怪物</role><use>部署：吞噬3个友军单位，吸收它们的战力为自身的基础战力。</use><pro>复制名称到剪贴 板获取卡牌图片</pro></card>",

    "<card id = \"63\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/13.png</img><cname>蟹蜘蛛</cname><ename>Arachas</ename><role>怪物</role><use>部署：从牌组打出所有“蟹蜘蛛”。</use><pro>属性：类虫生物</pro></card>",

    "<card id = \"167\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/283.png</img><cname>蜥蜴人战士</cname><ename>Vran Warrior</ename><role>怪物</role><use>部署：吞噬右侧单位。每2回合，己方回合开始时，吞噬右侧单位。</use><pro>属性：龙 兽</pro></card>",

    "<card id = \"171\"><img>http ://static.iyingdi.com/card/gwent/series/core/card/14.png</img><cname>蟹蜘蛛巨兽</cname><ename>Arachas Behemoth</ename><role>怪物</role><use>每当有友军单位吞噬1张牌，便生成1只“蟹蜘蛛”，随后受到1点伤害（无视护甲）。部 署：获得2点护甲。</use><pro>属性：类虫生物</pro></card>"
};

#endif
