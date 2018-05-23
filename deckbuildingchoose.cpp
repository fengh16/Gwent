#include <QtGui>
#include "deckbuildingchoose.h"
#include "ui_deckbuildingchoose.h"

DeckBuildingChoose::DeckBuildingChoose(PlayerInfo *m, int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckBuildingChoose),
    mTargetLead(0),
    m_page(1),
    pageMax(666),
    mMe(m),
    mLead(nullptr),
    mNowGoldNum(0),
    mNowSilverNum(0),
    mNowBronzeNum(0),
    mNowAllNum(0),
    mTotalCardNum(8888),
    firstpress(true),
    mNewDeckBuilding(false),
    mIndex(index),
    mInit(true)
{
    qDebug("DeckBuildingChange!");

    auto nowdeck = mMe->mDecks[index];

    mLead = GetNewCard(getint(nowdeck.mLeadCard));

    for (auto deckCardStr : nowdeck.mCardIndexs)
    {
        mChosenCard[getint(deckCardStr)]++;
        auto a = GetNewCard(getint(deckCardStr));
        if (a->mCardLevel == cGold)
        {
            mNowGoldNum++;
        }
        else if (a->mCardLevel == cSilver)
        {
            mNowSilverNum++;
        }
        else
        {
            mNowBronzeNum++;
        }
        mNowAllNum++;
        delete a;
    }

    qDebug("a");
    for(int i = 0; i < 8; i++)
    {
        s[i] = nullptr;
    }

    ui->setupUi(this);
    qDebug("b");
    qDebug("c");
    ui->pushButton_l->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed.png)}");
    ui->pushButton_r->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow_right.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move_right.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed_right.png)}");
    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_confirm->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/save.png)}"
                                          "QPushButton:hover{border-image: url(:/pics/arrow/save_move.png)}"
                                          "QPushButton:pressed{border-image: url(:/pics/arrow/save_click.png)}");
    ui->pushButton_save->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                       "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                       "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    slead = new ShownCard(mLead, ui->widget_lead);
    slead->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
    GetAvailableCards();
    LoadPicture();
    UpdateText();

    mInit = false;

    ui->label->setStyleSheet("color: white");
    ui->label_gold->setStyleSheet("color: white");
    ui->label_silver->setStyleSheet("color: white");
    ui->label_bronze->setStyleSheet("color: white");
    ui->label_all->setStyleSheet("color: white");
    ui->label_name->setStyleSheet("color: white");

    qDebug("DeckBuildingChange2!");

    ChangeSize(parent->width(), parent->height());
}

DeckBuildingChoose::DeckBuildingChoose(int tarlead, PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckBuildingChoose),
    mTargetLead(tarlead),
    m_page(1),
    pageMax(666),
    mMe(m),
    mLead(nullptr),
    mNowGoldNum(0),
    mNowSilverNum(0),
    mNowBronzeNum(0),
    mNowAllNum(0),
    mTotalCardNum(8888),
    firstpress(true),
    mNewDeckBuilding(true),
    mIndex(0),
    mInit(false)
{
    qDebug("a");
    mLead = GetNewCard(getint(mMe->leaderList[mTargetLead]));

    for(int i = 0; i < 8; i++)
    {
        s[i] = nullptr;
    }

    ui->setupUi(this);
    qDebug("b");
    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/Wood_Background.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.
    qDebug("c");
    ui->pushButton_l->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed.png)}");
    ui->pushButton_r->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow_right.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move_right.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed_right.png)}");
    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_confirm->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/save.png)}"
                                          "QPushButton:hover{border-image: url(:/pics/arrow/save_move.png)}"
                                          "QPushButton:pressed{border-image: url(:/pics/arrow/save_click.png)}");
    ui->pushButton_save->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                       "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                       "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    qDebug("d");
    slead = new ShownCard(mLead, ui->widget_lead);
    slead->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
    qDebug("e");
    GetAvailableCards();
    qDebug("f");
    LoadPicture();
    qDebug("g");
    UpdateText();

    ui->label->setStyleSheet("color: white");
    ui->label_gold->setStyleSheet("color: white");
    ui->label_silver->setStyleSheet("color: white");
    ui->label_bronze->setStyleSheet("color: white");
    ui->label_all->setStyleSheet("color: white");
    ui->label_name->setStyleSheet("color: white");

    ChangeSize(parent->width(), parent->height());
}

DeckBuildingChoose::~DeckBuildingChoose()
{
    qDebug("~DeckBuilding");
    slead->hide();
    if(slead->GetCard() != nullptr)
    {
        delete slead->GetCard();
    }
    delete slead;
    slead = nullptr;
    unLoadPicture();
    delete ui;
}

void DeckBuildingChoose::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void DeckBuildingChoose::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void DeckBuildingChoose::on_pushButton_back_clicked()
{
    if(mNewDeckBuilding)
    {
        emit PressedBack();
    }
    else
    {
        emit PressedChangeBack();
    }
}

void DeckBuildingChoose::on_pushButton_save_clicked()
{
    Save();
}

void DeckBuildingChoose::on_pushButton_confirm_clicked()
{
    Save();
}

bool DeckBuildingChoose::Checked()
{
    mNowAllNum = mNowGoldNum + mNowSilverNum + mNowBronzeNum;
    if(mNowAllNum < 25 || mNowAllNum > 40 || mNowGoldNum > 4 || mNowSilverNum > 6)
    {
        return false;
    }
    return true;
}

void DeckBuildingChoose::LoadPicture()
{
    qDebug("Loada");
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;
    widget[3] = ui->widget_4;
    widget[4] = ui->widget_5;
    widget[5] = ui->widget_6;
    widget[6] = ui->widget_7;
    widget[7] = ui->widget_8;
    qDebug("Loadaaa");
    ui->label->setText(QString::number(m_page));
    int addid = m_page * 8 - 8;
    Card *temp;
    qDebug("Loadaab");
    if(mTotalCardNum == 0)
    {
        return;
    }
    for (int i = 0; i < 8; i++)
    {
        try
        {
            if(i + addid >= mTotalCardNum)
            {
                qDebug("Loadabc");
                break;
            }
            qDebug("Loadccc");
            temp = GetNewCard(mAvailable[i + addid]);
            s[i] = new ShownCard(temp, widget[i]);
            s[i]->setFont(QFont("Myanmar Text", 50, QFont::Bold));
            s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
            s[i]->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
            s[i]->show();
        }
        catch(...)
        {
            qDebug("bug");
        }
    }
    qDebug("Loadb");
    if(s[0] != nullptr)
    {
        qDebug("set0");
        connect(s[0], SIGNAL(clicked()), this, SLOT(Pressed1()));
    }
    if(s[1] != nullptr)
    {
        connect(s[1], SIGNAL(clicked()), this, SLOT(Pressed2()));
    }
    if(s[2] != nullptr)
    {
        connect(s[2], SIGNAL(clicked()), this, SLOT(Pressed3()));
    }
    if(s[3] != nullptr)
    {
        connect(s[3], SIGNAL(clicked()), this, SLOT(Pressed4()));
    }
    if(s[4] != nullptr)
    {
        connect(s[4], SIGNAL(clicked()), this, SLOT(Pressed5()));
    }
    if(s[5] != nullptr)
    {
        connect(s[5], SIGNAL(clicked()), this, SLOT(Pressed6()));
    }
    if(s[6] != nullptr)
    {
        connect(s[6], SIGNAL(clicked()), this, SLOT(Pressed7()));
    }
    if(s[7] != nullptr)
    {
        connect(s[7], SIGNAL(clicked()), this, SLOT(Pressed8()));
    }
}

void DeckBuildingChoose::unLoadPicture()
{
    for (int i = 0; i < 8; i++)
    {
        if(s[i] != nullptr)
        {
            s[i]->hide();
            if(s[i]->GetCard() != nullptr)
            {
                delete s[i]->GetCard();
            }
            delete s[i];
            s[i] = nullptr;
        }
    }
}

void DeckBuildingChoose::GetAvailableCards()
{
    mAvailable.clear();
    bool needContinue = false;
    for(auto e: mMe->mCardMap)
    {
        if(e.second > 0)
        {
            auto t = getint(e.first);
            if(t <= 0 || t >= cCard_Num)
            {
                continue;
            }
            for(auto k : LeadNums)
            {
                if(k == t)
                {
                    needContinue = true;
                    break;
                }
            }
            if(needContinue)
            {
                needContinue = false;
                continue;
            }
            mAvailable.push_back(getint(e.first));
        }
    }
    mTotalCardNum = mAvailable.size();
    pageMax = (mTotalCardNum + 7) / 8;
}

void DeckBuildingChoose::Pressed1()
{
    qDebug("0");
    Pressed(0);
}

void DeckBuildingChoose::Pressed2()
{
    qDebug("1");
    Pressed(1);
}

void DeckBuildingChoose::Pressed3()
{
    qDebug("2");
    Pressed(2);
}

void DeckBuildingChoose::Pressed4()
{
    qDebug("3");
    Pressed(3);
}

void DeckBuildingChoose::Pressed5()
{
    qDebug("4");
    Pressed(4);
}

void DeckBuildingChoose::Pressed6()
{
    qDebug("5");
    Pressed(5);
}

void DeckBuildingChoose::Pressed7()
{
    qDebug("6");
    Pressed(6);
}

void DeckBuildingChoose::Pressed8()
{
    qDebug("7");
    Pressed(7);
}

void DeckBuildingChoose::Pressed(int i)
{
    if(firstpress)
    {
        QMessageBox box(QMessageBox::Information, "提示", "如果想删除卡牌，请多点几次，个数就会清零！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
        firstpress = false;
    }
    int addid = m_page * 8 - 8;
    auto level = s[i]->GetCard()->mCardLevel;
    int temp;
//    套牌由最少25 张，最多40 张牌组成，其中铜色牌中同名牌可以放3 张；银色牌最多放6 张且
//    同名牌只能放1 张；金色牌只能放4 张，同名牌只能放1 张．
    if(mChosenCard[mAvailable[addid + i]] >= mMe->mCardMap[getstring(mAvailable[i + addid])])
    {
        temp = mChosenCard[mAvailable[addid + i]];
        mChosenCard[mAvailable[addid + i]] = 0;
        switch (level) {
        case cGold:
            mNowGoldNum -= temp;
            break;
        case cSilver:
            mNowSilverNum -= temp;
            break;
        default:
            mNowBronzeNum -= temp;
            break;
        }
        mNowAllNum -= temp;
        s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
        UpdateText();
        return;
    }
    if(mChosenCard[mAvailable[addid + i]] >= 3)
    {
        temp = mChosenCard[mAvailable[addid + i]];
        mChosenCard[mAvailable[addid + i]] = 0;
        mNowBronzeNum -= temp;
        mNowAllNum -= temp;
        s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
        UpdateText();
        return;
    }
    if(level == cGold)
    {
        if(mChosenCard[mAvailable[addid + i]])
        {
            temp = mChosenCard[mAvailable[addid + i]];
            mChosenCard[mAvailable[addid + i]] = 0;
            mNowGoldNum -= temp;
            mNowAllNum -= temp;
            s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
            UpdateText();
            return;
        }
        if(mNowGoldNum >= 4)
        {
            return;
        }
    }
    if(level == cSilver)
    {
        if(mChosenCard[mAvailable[addid + i]])
        {
            temp = mChosenCard[mAvailable[addid + i]];
            mChosenCard[mAvailable[addid + i]] = 0;
            mNowSilverNum -= temp;
            mNowAllNum -= temp;
            s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
            UpdateText();
            return;
        }
        if(mNowSilverNum >= 6)
        {
            return;
        }
    }
    mChosenCard[mAvailable[addid + i]]++;
    s[i]->setText(QString::fromStdString(getstring(mChosenCard[mAvailable[i + addid]]) + "/" + getstring(mMe->mCardMap[getstring(mAvailable[i + addid])])));
    mNowAllNum++;
    if(level == cGold)
    {
        mNowGoldNum++;
    }
    if(level == cSilver)
    {
        mNowSilverNum++;
    }
    if(level == cBronze)
    {
        mNowBronzeNum++;
    }
    UpdateText();
}

void DeckBuildingChoose::UpdateText()
{
    if(mInit && !mNewDeckBuilding)
    {
        ui->lineEdit->setText(QString::fromLocal8Bit(mMe->mDecks[mIndex].mDeckIntro.data()));
    }
    ui->label_gold->setText("金卡数目：" + QString::number(mNowGoldNum) + "/4");
    ui->label_silver->setText("银卡数目：" + QString::number(mNowSilverNum) + "/6");
    ui->label_bronze->setText("铜卡数目：" + QString::number(mNowBronzeNum) + "/40");
    ui->label_all->setText("总数：" + QString::number(mNowAllNum) + "（" + (mNowAllNum < 25 ? QString::fromLocal8Bit("<font color=\"#FF0000\">25</font>") : QString::fromLocal8Bit("25")) + QString::fromLocal8Bit("-") + (mNowAllNum > 40 ? QString::fromLocal8Bit("<font color=\"#FF0000\">40</font>") : "40") + "）");
}

void DeckBuildingChoose::Save()
{
    if (Checked())
    {
        auto reDeck = DeckInfo();
        reDeck.mDeckIntro = std::string(ui->lineEdit->text().toLocal8Bit());
        reDeck.mCardNum = mNowAllNum;
        if(mNewDeckBuilding)
        {
            reDeck.mLeadCard = mMe->leaderList[mTargetLead];
        }
        else
        {
            reDeck.mLeadCard = mMe->mDecks[mIndex].mLeadCard;
        }
        for(auto t : mChosenCard)
        {
            for(auto i = 0; i < t.second; i++)
            {
                reDeck.mCardIndexs.push_back(getstring(t.first));
            }
        }
        if(mNewDeckBuilding)
        {
            emit PressedSave(reDeck);
        }
        else
        {
            emit ChangePressedSave(reDeck, mIndex);
        }
        return;
    }
    QMessageBox box(QMessageBox::Warning, "提示", "您的选择不符合要求，请检查！");
    box.setStandardButtons (QMessageBox::Ok);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    box.exec();
}

void DeckBuildingChoose::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    WindowWidth = a;
    WindowHeight = b;

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/Wood_Background.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.

    slead->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
    for (auto i = 0; i < 8; i++)
    {
        s[i]->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
    }
}
