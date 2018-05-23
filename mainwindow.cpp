#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "base/Cards.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    g(new GameController),
    m_load(nullptr),
    m_login(nullptr),
    m_choosing(nullptr),

    m_friends(nullptr),
    m_deckbuilder(nullptr),
    m_playing(nullptr),
    m_openkeg(nullptr),
    m_buykeg(nullptr),
    m_settings(nullptr),
    m_reward(nullptr),
    m_collection(nullptr),

    m_choosefromlist(nullptr),
    m_deckbuildingchoose(nullptr),

    m_cardshowclass(nullptr),
    m_switchhandcard(nullptr),

    m_ansshow(nullptr)
{
    // 禁用掉最大化，因为现在写的这个控制长宽比的会导致最大化的时候出现问题
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);

    ui->setupUi(this);

    connect(g, SIGNAL(IOStartGame()), this, SLOT(LoadPage()));
    connect(g, SIGNAL(IOLogIn()), this, SLOT(LogInPage()));
    connect(g, SIGNAL(IOLoggedIn()), this, SLOT(ChoosingPage()));

    connect(g, SIGNAL(IOFriends(PlayerInfo*)), this, SLOT(FriendsPage(PlayerInfo*)));
    connect(g, SIGNAL(IODeckBuilding(PlayerInfo*)), this, SLOT(DeckBuildingPage(PlayerInfo*)));
    connect(g, SIGNAL(IOPlaying(PlayerInfo*)), this, SLOT(PlayingPage(PlayerInfo*)));
    connect(g, SIGNAL(IOOpenKeg(PlayerInfo*)), this, SLOT(OpenKegPage(PlayerInfo*)));
    connect(g, SIGNAL(IOBuyKeg(PlayerInfo*)), this, SLOT(BuyKegPage(PlayerInfo*)));
    connect(g, SIGNAL(IOSetting(PlayerInfo*)), this, SLOT(SettingPage(PlayerInfo*)));
    connect(g, SIGNAL(IORewarding(PlayerInfo*)), this, SLOT(RewardingPage(PlayerInfo*)));
    connect(g, SIGNAL(IOCollection(PlayerInfo*)), this, SLOT(CollectionPage(PlayerInfo*)));

    connect(g, SIGNAL(IOChooseLeader(PlayerInfo*)), this, SLOT(ChooseLeaderPage(PlayerInfo*)));
    connect(g, SIGNAL(IODeckBuildingChoose(int, PlayerInfo*)), this, SLOT(DeckBuildingChoosePage(int, PlayerInfo*)));

    connect(g, SIGNAL(IODeckChange(PlayerInfo*, int)), this, SLOT(DeckChange(PlayerInfo*, int)));

    connect(this, SIGNAL(IOPrepareToPlay()), g, SLOT(PrepareToPlay()));
    connect(g, SIGNAL(IOChooseDeck(vector<string>, vector<string>)), this, SLOT(ChooseDeckPage(vector<string>, vector<string>)));
    connect(g, SIGNAL(IOChosenDeck(int, PlayerInfo*)), this, SLOT(ChangeHandCardPre(int, PlayerInfo*)));
    connect(this, SIGNAL(ChangedCards(Playing*)), g, SLOT(ShowMainPlayingInfo(Playing*)));

    connect(g, SIGNAL(Set_01_Choose()), this, SLOT(Set_01_ChooseList()));
    connect(g, SIGNAL(ChooseFromWeatherCards_10()), this, SLOT(Set_10_ChooseFromWeathers()));
    connect(g, SIGNAL(ReceivedChosenCard()), this, SLOT(DeleteChooseList()));
    connect(g, SIGNAL(Set_12_Choose(vector<string>)), this, SLOT(Set_12_MainChoose(vector<string>)));

    connect(g, SIGNAL(Set20_ChooseFromMyGrave(vector<string>)), this, SLOT(Set_20_MainChoose1(vector<string>)));
    connect(g, SIGNAL(Set20_ChooseFromEnemyGrave(vector<string>)), this, SLOT(Set_20_MainChoose2(vector<string>)));

    connect(g, SIGNAL(GameOver(bool, int, int, int, int, int, int, int, int)), this, SLOT(ShowAnsPage(bool, int, int, int, int, int, int, int, int)));
    connect(g, SIGNAL(EmitChangeHandCardOnly(int, PlayerInfo*)), this, SLOT(ChangeHandCardOnly(int, PlayerInfo*)));

    g->NewGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HideAllWidget()
{
    if(m_load)
    {
        m_load->hide();
        delete m_load;
        m_load = nullptr;
    }
    if(m_login)
    {
        m_login->hide();
        delete m_login;
        m_login = nullptr;
    }
    if(m_choosing)
    {
        m_choosing->hide();
        delete m_choosing;
        m_choosing = nullptr;
    }

    if(m_friends)
    {
        m_friends->hide();
        delete m_friends;
        m_friends = nullptr;
    }
    if(m_deckbuilder)
    {
        m_deckbuilder->hide();
        delete m_deckbuilder;
        m_deckbuilder = nullptr;
    }
    if(m_playing)
    {
        m_playing->hide();
        delete m_playing;
        m_playing = nullptr;
    }
    if(m_openkeg)
    {
        m_openkeg->hide();
        delete m_openkeg;
        m_openkeg = nullptr;
    }
    if(m_buykeg)
    {
        m_buykeg->hide();
        delete m_buykeg;
        m_buykeg = nullptr;
    }
    if(m_settings)
    {
        m_settings->hide();
        delete m_settings;
        m_settings = nullptr;
    }
    if(m_reward)
    {
        m_reward->hide();
        delete m_reward;
        m_reward = nullptr;
    }
    if(m_collection)
    {
        m_collection->hide();
        delete m_collection;
        m_collection = nullptr;
    }

    if(m_choosefromlist)
    {
        m_choosefromlist->hide();
        delete m_choosefromlist;
        m_choosefromlist = nullptr;
    }
    if(m_deckbuildingchoose)
    {
        m_deckbuildingchoose->hide();
        delete m_deckbuildingchoose;
        m_deckbuildingchoose = nullptr;
    }
    if(m_cardshowclass)
    {
        m_cardshowclass->hide();
        delete m_cardshowclass;
        m_cardshowclass = nullptr;
    }
    if(m_switchhandcard)
    {
        m_switchhandcard->hide();
        delete m_switchhandcard;
        m_switchhandcard = nullptr;
    }
    if(m_ansshow)
    {
        m_ansshow->hide();
        delete m_ansshow;
        m_ansshow = nullptr;
    }
}

void MainWindow::LoadPage()
{
    HideAllWidget();
    // 一定要现场创建！不能提前创建！
    m_load = new Load(this);
    m_load->show();
    connect(m_load, SIGNAL(Pressed()), g, SLOT(LogIn()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_load, SLOT(ChangeSize(int, int)));
}

void MainWindow::LogInPage()
{
    HideAllWidget();
    m_login = new Login(this);
    m_login->show();
    connect(m_login, SIGNAL(Passed(std::string,std::string)), g, SLOT(LoggedIn(std::string, std::string)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_login, SLOT(ChangeSize(int, int)));
}

void MainWindow::FriendsPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_friends = new Friends(mMe, this);
    m_friends->show();
    connect(this, SIGNAL(ChangedSize(int,int)), m_friends, SLOT(ChangeSize(int, int)));
}

void MainWindow::DeckBuildingPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_deckbuilder = new DeckBuilder(mMe, this);
    m_deckbuilder->show();

    connect(m_deckbuilder, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(m_deckbuilder, SIGNAL(PressedBuy()), g, SLOT(BuyKegs()));
    connect(m_deckbuilder, SIGNAL(ChooseLeader()), g, SLOT(ChooseLeaderPage()));
    connect(m_deckbuilder, SIGNAL(HaveDeleted()), g, SLOT(UpdateUserDataOnly()));
    connect(m_deckbuilder, SIGNAL(HaveDeleted()), g, SLOT(DeckBuild()));
    connect(m_deckbuilder, SIGNAL(ChangeDeck(int)), g, SLOT(ChangeDecksInfo(int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_deckbuilder, SLOT(ChangeSize(int, int)));
}

void MainWindow::PlayingPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_playing = new Playing(mMe, this);
    m_playing->show();
    emit IOPrepareToPlay();

    connect(m_playing, SIGNAL(PassPressed()), g, SLOT(PressedPass()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_playing, SLOT(ChangeSize(int, int)));
}

void MainWindow::OpenKegPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_openkeg = new OpenKeg(mMe, this);
    m_openkeg->show();

    connect(m_openkeg, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(m_openkeg, SIGNAL(PressedOpen()), g, SLOT(UpdateUserDataOnly()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_openkeg, SLOT(ChangeSize(int, int)));
}

void MainWindow::BuyKegPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_buykeg = new BuyKeg(mMe, this);
    m_buykeg->show();

    connect(m_buykeg, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(m_buykeg, SIGNAL(PressedBuy()), g, SLOT(UpdateUserDataOnly()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_buykeg, SLOT(ChangeSize(int, int)));
}

void MainWindow::SettingPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_settings = new Settings(mMe, this);
    m_settings->show();

    connect(m_settings, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(m_settings, SIGNAL(PressedSave(string)), g, SLOT(UpdatePasswordOnly(string)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_settings, SLOT(ChangeSize(int, int)));
}

void MainWindow::RewardingPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_reward = new Reward(mMe, this);
    m_reward->show();

    connect(m_reward, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_reward, SLOT(ChangeSize(int, int)));
}

void MainWindow::CollectionPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_collection = new Collection(mMe, this);
    m_collection->show();

    connect(m_collection, SIGNAL(PressedBack()), this, SLOT(ChoosingPage()));
    connect(m_collection, SIGNAL(PressedBuy()), g, SLOT(BuyKegs()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_collection, SLOT(ChangeSize(int, int)));
}

void MainWindow::ChoosingPage()
{
    HideAllWidget();
    m_choosing = new Choosing(this);
    m_choosing->show();

    connect(m_choosing, SIGNAL(pushButtFriends_clicked()), g, SLOT(Friends()));
    connect(m_choosing, SIGNAL(pushButtDeckBuilder_clicked()), g, SLOT(DeckBuild()));
    connect(m_choosing, SIGNAL(pushButtPlaying_clicked()), g, SLOT(MainGaming()));
    connect(m_choosing, SIGNAL(pushButtOpenKeg_clicked()), g, SLOT(OpenKegs()));
    connect(m_choosing, SIGNAL(pushButtBuyKeg_clicked()), g, SLOT(BuyKegs()));
    connect(m_choosing, SIGNAL(pushButtSettings_clicked()), g, SLOT(Settings()));
    connect(m_choosing, SIGNAL(pushButtReward_clicked()), g, SLOT(Rewarding()));
    connect(m_choosing, SIGNAL(pushButtCollection_clicked()), g, SLOT(Collection()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosing, SLOT(ChangeSize(int, int)));
}

void MainWindow::ChooseLeaderPage(PlayerInfo* mMe)
{
    HideAllWidget();
    m_choosefromlist = new ChooseFromList(cBuildPageChooseLeader, mMe->leaderList, this);
    m_choosefromlist->show();
    // Here #define got ChooseFunctionFrom doesn't work
    // 在这里经过宏定义后的ChooseFunctionFrom不能工作，也许是因为这里还没有进行宏定义，找的时候ChooseFromList类里面已经替换了，所以只能在这里用原始的名字
    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(ChoosePagePressed(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::DeckBuildingChoosePage(int tar, PlayerInfo* mMe)
{
    HideAllWidget();
    m_deckbuildingchoose = new DeckBuildingChoose(tar, mMe, this);
    m_deckbuildingchoose->show();

    connect(this, SIGNAL(ChangedSize(int,int)), m_deckbuildingchoose, SLOT(ChangeSize(int, int)));
    connect(m_deckbuildingchoose, SIGNAL(PressedBack()), g, SLOT(ChooseLeaderPage()));
    connect(m_deckbuildingchoose, SIGNAL(PressedSave(DeckInfo)), g, SLOT(SaveNewDeck(DeckInfo)));
//    connect(this, SIGNAL(ChangedSize(int,int)), m_deckbuildingchoose, SLOT(ChangeSize(int, int)));
}

void MainWindow::DeckChange(PlayerInfo* mMe, int index)
{
    HideAllWidget();
    m_deckbuildingchoose = new DeckBuildingChoose(mMe, index, this);
    m_deckbuildingchoose->show();

//    connect(m_deckbuildingchoose, SIGNAL(PressedBack()), g, SLOT(ChooseLeaderPage()));
    connect(m_deckbuildingchoose, SIGNAL(PressedChangeBack()), g, SLOT(DeckBuild()));
    connect(m_deckbuildingchoose, SIGNAL(ChangePressedSave(DeckInfo, int)), g, SLOT(SaveChangedDeck(DeckInfo, int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_deckbuildingchoose, SLOT(ChangeSize(int, int)));
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    if(this->height() != width() * 9 / 16)
    {
        resize(this->width(), this->width() * 9 / 16);
        emit ChangedSize(this->width(), this->width() * 9 / 16);
    }
}

void MainWindow::ChooseDeckPage(vector<string> list, vector<string> info)
{
    // 不需要把其他窗口关掉
    m_choosefromlist = new ChooseFromList(cChooseDeck, list, info, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(ChoosePagePressed(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::ChangeHandCardPre(int tar, PlayerInfo* mMe)
{
    m_choosefromlist->hide();
    delete m_choosefromlist;
    m_choosefromlist = nullptr;

    mMe->PrePareForFirst(tar);
    mMe->GetNewCards(1);

    ChangeHandCard(3, mMe, 1);
}

void MainWindow::ChangeHandCardOnly(int nums, PlayerInfo* mMe)
{
    if(m_choosefromlist)
    {
        m_choosefromlist->hide();
        delete m_choosefromlist;
        m_choosefromlist = nullptr;
    }
    if(m_switchhandcard)
    {
        m_switchhandcard->hide();
        delete m_switchhandcard;
        m_switchhandcard = nullptr;
    }

    ChangeHandCard(nums, mMe, 1);
}

void MainWindow::GotSwitchInfo(int tar, int nums, PlayerInfo* mMe, int m_page)
{
    m_switchhandcard->hide();
    delete m_switchhandcard;
    m_switchhandcard = nullptr;

    if(tar < 0 || nums == 0)
    {
        mMe->ChangedCardsPutTemp();
        emit(ChangedCards(m_playing));
        return;
    }
    mMe->ChangeCardAtP(tar);
    if(nums >= 1)
    {
        ChangeHandCard(nums - 1, mMe, m_page);
        return;
    }
    mMe->ChangedCardsPutTemp();
    emit(ChangedCards(m_playing));
}

void MainWindow::ChangeHandCard(int nums, PlayerInfo* mMe, int m_page)
{
    m_switchhandcard = new SwitchHandCard(mMe, nums, this, m_page);
    m_switchhandcard->show();

    connect(m_switchhandcard, SIGNAL(PressedInt(int, int, PlayerInfo*, int)), this, SLOT(GotSwitchInfo(int, int, PlayerInfo*, int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_switchhandcard, SLOT(ChangeSize(int, int)));
}

void MainWindow::Set_01_ChooseList()
{
    vector<string> temp = {"500", "501"};
    m_choosefromlist = new ChooseFromList(cChooseSet01, temp, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(Set01Chosen(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::Set_10_ChooseFromWeathers()
{
    vector<string> temp = {"2", "3", "4"};
    m_choosefromlist = new ChooseFromList(cChooseSet10, temp, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(Set10Chosen(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::Set_12_MainChoose(vector<string> temp)
{
    m_choosefromlist = new ChooseFromList(cChooseSet12, temp, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(Set20Chosen1(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::Set_20_MainChoose1(vector<string> temp)
{
    m_choosefromlist = new ChooseFromList(cChooseSet20, temp, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(Set20Chosen2(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::Set_20_MainChoose2(vector<string> temp)
{
    m_choosefromlist = new ChooseFromList(cChooseSet20, temp, this);
    m_choosefromlist->show();

    connect(m_choosefromlist, SIGNAL(PressedInt(eChooseFunctionFrom,int)), g, SLOT(Set12Chosen(eChooseFunctionFrom,int)));
    connect(this, SIGNAL(ChangedSize(int,int)), m_choosefromlist, SLOT(ChangeSize(int, int)));
}

void MainWindow::DeleteChooseList()
{
    if(m_choosefromlist)
    {
        m_choosefromlist->hide();
        delete m_choosefromlist;
        m_choosefromlist = nullptr;
    }
}

void MainWindow::ShowAnsPage(bool a, int b, int c, int d, int e, int f, int g, int h, int i)
{
    HideAllWidget();
    m_ansshow = new AnsShow(a, b, c, d, e, f, g, h, i, this);
    m_ansshow->show();

    connect(m_ansshow, SIGNAL(ClickedClose()), this, SLOT(ClosedAllWidget()));
    connect(this, SIGNAL(ChangedSize(int,int)), m_ansshow, SLOT(ChangeSize(int, int)));
}

void MainWindow::ClosedAllWidget()
{
    g->clearAllData();
    ChoosingPage();
}
