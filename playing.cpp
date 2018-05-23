#include "playing.h"
#include "ui_playing.h"
#include <QDebug>

Playing::Playing(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Playing),
    mMe(m),
    RightShow(nullptr),
    sMeLeader(nullptr),
    sEnemyLeader(nullptr)
{
    ui->setupUi(this);

    ChangeSize(parent->width(), parent->height());

    ui->label->setStyleSheet("color:white");
    ui->label_2->setStyleSheet("color:white");
    ui->label_3->setStyleSheet("color:white");
    ui->label_4->setStyleSheet("color:white");
    ui->label_ScoreEn->setStyleSheet("color:white");
    ui->label_ScoreMe->setStyleSheet("color:white");
    ui->label_w1->setStyleSheet("color:white");
    ui->label_w2->setStyleSheet("color:white");
    ui->label_w3->setStyleSheet("color:white");
    ui->label_w4->setStyleSheet("color:white");
    ui->label_w5->setStyleSheet("color:white");
    ui->label_w6->setStyleSheet("color:white");
    ui->label_time->setStyleSheet("color:white");
}

Playing::~Playing()
{
    if(RightShow)
    {
        delete RightShow;
    }
    delete ui;
}

void Playing::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    NowWindowHeight = b;
    NowWindowWidth = a;

    mHandCardWidth = HandCardHeight * b * CardWidthNormal / CardHeightNormal / PlayGroundHeight;
    mHandCardHeight = HandCardHeight * b / PlayGroundHeight;
    mInnerCardWidth = InnerCardHeight * b * CardWidthNormal / CardHeightNormal / PlayGroundHeight;
    mInnerCardHeight = InnerCardHeight * b / PlayGroundHeight;

    mInnerCardHeightLeftTop1 = InnerPosition1 * b / PlayGroundHeight;
    mInnerCardHeightLeftTop2 = InnerPosition2 * b / PlayGroundHeight;
    mInnerCardHeightLeftTop3 = InnerPosition3 * b / PlayGroundHeight;
    mInnerCardHeightLeftTop4 = InnerPosition4 * b / PlayGroundHeight;
    mInnerCardHeightLeftTop5 = InnerPosition5 * b / PlayGroundHeight;
    mInnerCardHeightLeftTop6 = InnerPosition6 * b / PlayGroundHeight;

    mHandCardHeightEnemyTop = 0;
    mHandCardHeightMeTop = HandCardPosition * b / PlayGroundHeight;

    mCardInnerWidthAll = UsefulWidth * b / PlayGroundHeight;
    mCardHandWidthAll = HandUsefullWidth * b / PlayGroundHeight;

    mCardInnerLeft = UseLessLeftPosition * b / PlayGroundHeight;
    mCardHandLeft = HandUseLessLeft * b / PlayGroundHeight;

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/playground.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.

    ChangeMeHandCardPosition();
    ChangeMeNearestSelfCardPosition();
    ChangeMeMediumSelfCardPosition();
    ChangeMeFarestSelfCardPosition();
    ChangeEnemyHandCardPosition();
    ChangeEnemyNearestSelfCardPosition();
    ChangeEnemyMediumSelfCardPosition();
    ChangeEnemyFarestSelfCardPosition();

    if(RightShow)
    {
        RightShow->setGeometry(3321 * NowWindowHeight / PlayGroundHeight, 500 * NowWindowHeight / PlayGroundHeight, (4000 - 3321) * NowWindowHeight / PlayGroundHeight, (4000 - 3321) * NowWindowHeight * CardHeightNormal / CardWidthNormal / PlayGroundHeight);
    }

    if(sMeLeader)
    {
        sMeLeader->setGeometry(3600 * NowWindowHeight / PlayGroundHeight, 1982 * NowWindowHeight / PlayGroundHeight, 433 * NowWindowHeight * CardWidthNormal / CardHeightNormal / PlayGroundHeight, 433 * NowWindowHeight / PlayGroundHeight);
    }

    if(sEnemyLeader)
    {
        sEnemyLeader->setGeometry(3600 * NowWindowHeight / PlayGroundHeight, 0, 433 * NowWindowHeight * CardWidthNormal / CardHeightNormal / PlayGroundHeight, 433 * NowWindowHeight / PlayGroundHeight);
    }
}

void Playing::ShowPlayingInfoMe(PlayerInfo *mMe)
{
    qDebug() << "显示己方信息";
    ShowMeWeather(mMe);
    ShowMeScore(mMe);
    ShowMeLeader(mMe);
    ShowMeHandCard(mMe);
    ShowMeNearestSelf(mMe);
    ShowMeMediumSelf(mMe);
    ShowMeFarestSelf(mMe);
}

void Playing::ShowPlayingInfoEnemy(PlayerInfo *mEnemy)
{
    qDebug() << "显示对方信息";
    ShowEnemyWeather(mEnemy);
    ShowEnemyScore(mEnemy);
    ShowEnemyLeader(mEnemy);
    ShowEnemyHandCard(mEnemy);
    ShowEnemyNearestSelf(mEnemy);
    ShowEnemyMediumSelf(mEnemy);
    ShowEnemyFarestSelf(mEnemy);
}

void Playing::ShowMeHandCard(PlayerInfo *mMe)
{
    for (auto k : sMeHand)
    {
        k->hide();
        delete k;
    }
    sMeHand.clear();
    for (auto t : mMe->mHand)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sMeHand.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeMeHandCardPosition();
}

void Playing::ShowMeNearestSelf(PlayerInfo *mMe)
{
    //（firstrow是最靠近中间的）
    for (auto k : sMeNearestSelf)
    {
        k->hide();
        delete k;
    }
    sMeNearestSelf.clear();
    for (auto t : mMe->mThirdRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sMeNearestSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeMeNearestSelfCardPosition();
}

void Playing::ShowMeMediumSelf(PlayerInfo *mMe)
{
    for (auto k : sMeMediumSelf)
    {
        k->hide();
        delete k;
    }
    sMeMediumSelf.clear();
    for (auto t : mMe->mSecondRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sMeMediumSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeMeMediumSelfCardPosition();
}

void Playing::ShowMeFarestSelf(PlayerInfo *mMe)
{
    for (auto k : sMeFarestSelf)
    {
        k->hide();
        delete k;
    }
    sMeFarestSelf.clear();
    for (auto t : mMe->mFirstRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sMeFarestSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeMeFarestSelfCardPosition();
}








void Playing::ShowEnemyHandCard(PlayerInfo *mEnemy)
{
    for (auto k : sEnemyHand)
    {
        k->hide();
        delete k;
    }
    sEnemyHand.clear();
    for (auto t : mEnemy->mHand)
    {
        ShownCard* newShow = new ShownCard(nullptr, this);
        sEnemyHand.push_back(newShow);
    }

    ChangeEnemyHandCardPosition();
}

void Playing::ShowEnemyNearestSelf(PlayerInfo *mEnemy)
{
    for (auto k : sEnemyNearestSelf)
    {
        k->hide();
        delete k;
    }
    sEnemyNearestSelf.clear();
    for (auto t : mEnemy->mThirdRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sEnemyNearestSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeEnemyNearestSelfCardPosition();
}

void Playing::ShowEnemyMediumSelf(PlayerInfo *mEnemy)
{
    for (auto k : sEnemyMediumSelf)
    {
        k->hide();
        delete k;
    }
    sEnemyMediumSelf.clear();
    for (auto t : mEnemy->mSecondRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sEnemyMediumSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeEnemyMediumSelfCardPosition();
}

void Playing::ShowEnemyFarestSelf(PlayerInfo *mEnemy)
{
    for (auto k : sEnemyFarestSelf)
    {
        k->hide();
        delete k;
    }
    sEnemyFarestSelf.clear();
    for (auto t : mEnemy->mFirstRow)
    {
        ShownCard* newShow = new ShownCard(t, this);
        sEnemyFarestSelf.push_back(newShow);
        connect(newShow, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
    }

    ChangeEnemyFarestSelfCardPosition();
}







void Playing::ChangeEnemyHandCardPosition()
{
    int tempWidth = mHandCardWidth;
    int tempHeight = mHandCardHeight;
    if(sEnemyHand.size() * (unsigned int)(tempWidth) > mCardHandWidthAll)
    {
        tempWidth = mCardHandWidthAll / sEnemyHand.size();
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (unsigned int i = 0; i < sEnemyHand.size(); i++)
    {
        sEnemyHand[i]->setGeometry(mCardHandLeft + i * (tempWidth), mHandCardHeightEnemyTop, tempWidth, tempHeight);
        sEnemyHand[i]->show();
    }
}

void Playing::ChangeEnemyNearestSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sEnemyNearestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sEnemyNearestSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sEnemyNearestSelf.size(); i++)
    {
        sEnemyNearestSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop1, tempWidth, tempHeight);
        if(sEnemyNearestSelf[i]->GetCard()->mCardSuperShield)
        {
            sEnemyNearestSelf[i]->setText(QString::number(sEnemyNearestSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sEnemyNearestSelf[i]->GetCard()->mCardShield > 0)
        {
            sEnemyNearestSelf[i]->setText(QString::number(sEnemyNearestSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sEnemyNearestSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sEnemyNearestSelf[i]->setText(QString::number(sEnemyNearestSelf[i]->GetCard()->mCardBloodNow));
        }
        sEnemyNearestSelf[i]->show();
    }
}

void Playing::ChangeEnemyMediumSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sEnemyMediumSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sEnemyMediumSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sEnemyMediumSelf.size(); i++)
    {
        sEnemyMediumSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop2, tempWidth, tempHeight);
        if(sEnemyMediumSelf[i]->GetCard()->mCardSuperShield)
        {
            sEnemyMediumSelf[i]->setText(QString::number(sEnemyMediumSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sEnemyMediumSelf[i]->GetCard()->mCardShield > 0)
        {
            sEnemyMediumSelf[i]->setText(QString::number(sEnemyMediumSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sEnemyMediumSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sEnemyMediumSelf[i]->setText(QString::number(sEnemyMediumSelf[i]->GetCard()->mCardBloodNow));
        }
        sEnemyMediumSelf[i]->show();
    }
}

void Playing::ChangeEnemyFarestSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sEnemyFarestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sEnemyFarestSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sEnemyFarestSelf.size(); i++)
    {
        sEnemyFarestSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop3, tempWidth, tempHeight);
        if(sEnemyFarestSelf[i]->GetCard()->mCardSuperShield)
        {
            sEnemyFarestSelf[i]->setText(QString::number(sEnemyFarestSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sEnemyFarestSelf[i]->GetCard()->mCardShield > 0)
        {
            sEnemyFarestSelf[i]->setText(QString::number(sEnemyFarestSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sEnemyFarestSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sEnemyFarestSelf[i]->setText(QString::number(sEnemyFarestSelf[i]->GetCard()->mCardBloodNow));
        }
        sEnemyFarestSelf[i]->show();
    }
}








void Playing::ChangeMeHandCardPosition()
{
    int tempWidth = mHandCardWidth;
    int tempHeight = mHandCardHeight;
    if(sMeHand.size() * (tempWidth) > mCardHandWidthAll)
    {
        tempWidth = mCardHandWidthAll / sMeHand.size();
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sMeHand.size(); i++)
    {
        sMeHand[i]->setGeometry(mCardHandLeft + i * (tempWidth), mHandCardHeightMeTop, tempWidth, tempHeight);
        if(sMeHand[i]->GetCard()->mCardSuperShield)
        {
            sMeHand[i]->setText(QString::number(sMeHand[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sMeHand[i]->GetCard()->mCardShield > 0)
        {
            sMeHand[i]->setText(QString::number(sMeHand[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sMeHand[i]->GetCard()->mCardShield));
        }
        else
        {
            sMeHand[i]->setText(QString::number(sMeHand[i]->GetCard()->mCardBloodNow));
        }
        sMeHand[i]->show();
    }
}

void Playing::ChangeMeNearestSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sMeNearestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sMeNearestSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sMeNearestSelf.size(); i++)
    {
        sMeNearestSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop6, tempWidth, tempHeight);
        if(sMeNearestSelf[i]->GetCard()->mCardSuperShield)
        {
            sMeNearestSelf[i]->setText(QString::number(sMeNearestSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sMeNearestSelf[i]->GetCard()->mCardShield > 0)
        {
            sMeNearestSelf[i]->setText(QString::number(sMeNearestSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sMeNearestSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sMeNearestSelf[i]->setText(QString::number(sMeNearestSelf[i]->GetCard()->mCardBloodNow));
        }
        sMeNearestSelf[i]->show();
    }
}

void Playing::ChangeMeMediumSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sMeMediumSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sMeMediumSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sMeMediumSelf.size(); i++)
    {
        sMeMediumSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop5, tempWidth, tempHeight);
        if(sMeMediumSelf[i]->GetCard()->mCardSuperShield)
        {
            sMeMediumSelf[i]->setText(QString::number(sMeMediumSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sMeMediumSelf[i]->GetCard()->mCardShield > 0)
        {
            sMeMediumSelf[i]->setText(QString::number(sMeMediumSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sMeMediumSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sMeMediumSelf[i]->setText(QString::number(sMeMediumSelf[i]->GetCard()->mCardBloodNow));
        }
        sMeMediumSelf[i]->show();
    }
}

void Playing::ChangeMeFarestSelfCardPosition()
{
    int tempWidth = mInnerCardWidth;
    int tempHeight = mInnerCardHeight;
    if(sMeFarestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
    {
        tempWidth = mCardInnerWidthAll / sMeFarestSelf.size() - 10;
        tempHeight = tempWidth * CardWidthNormal / CardHeightNormal;
    }
    for (int i = 0; i < sMeFarestSelf.size(); i++)
    {
        sMeFarestSelf[i]->setGeometry(mCardInnerLeft + i * (tempWidth + 10) + 10, mInnerCardHeightLeftTop4, tempWidth, tempHeight);
        if(sMeFarestSelf[i]->GetCard()->mCardSuperShield)
        {
            sMeFarestSelf[i]->setText(QString::number(sMeFarestSelf[i]->GetCard()->mCardBloodNow) + "护甲");
        }
        else if(sMeFarestSelf[i]->GetCard()->mCardShield > 0)
        {
            sMeFarestSelf[i]->setText(QString::number(sMeFarestSelf[i]->GetCard()->mCardBloodNow) + "护" + QString::number(sMeFarestSelf[i]->GetCard()->mCardShield));
        }
        else
        {
            sMeFarestSelf[i]->setText(QString::number(sMeFarestSelf[i]->GetCard()->mCardBloodNow));
        }
        sMeFarestSelf[i]->show();
    }
}




int Playing::GetInnerPositionIndex(int rownum, int x)
{
    int tempWidth;
    int temp = 0;
    if(x < mCardInnerLeft)
    {
        return 0;
    }
    switch (rownum) {
    case 1:
        tempWidth = mInnerCardWidth;
        if(sEnemyNearestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sEnemyNearestSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sEnemyNearestSelf.size())
        {
            temp = sEnemyNearestSelf.size();
        }
        break;
    case 2:
        tempWidth = mInnerCardWidth;
        if(sEnemyMediumSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sEnemyMediumSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sEnemyMediumSelf.size())
        {
            temp = sEnemyMediumSelf.size();
        }
        break;
    case 3:
        tempWidth = mInnerCardWidth;
        if(sEnemyFarestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sEnemyFarestSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sEnemyFarestSelf.size())
        {
            temp = sEnemyFarestSelf.size();
        }
        break;
    case 6:
        tempWidth = mInnerCardWidth;
        if(sMeNearestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sMeNearestSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sMeNearestSelf.size())
        {
            temp = sMeNearestSelf.size();
        }
        break;
    case 5:
        tempWidth = mInnerCardWidth;
        if(sMeMediumSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sMeMediumSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sMeMediumSelf.size())
        {
            temp = sMeMediumSelf.size();
        }
        break;
    case 4:
        tempWidth = mInnerCardWidth;
        if(sMeFarestSelf.size() * (tempWidth + 10) + 10 > mCardInnerWidthAll)
        {
            tempWidth = mCardInnerWidthAll / sMeFarestSelf.size() - 10;
        }
        temp = (x - mCardInnerLeft) / (tempWidth + 10);
        if(temp > sMeFarestSelf.size())
        {
            temp = sMeFarestSelf.size();
        }
        break;
    default:
        break;
    }
    return temp;
}




void Playing::ClickedCard(Card *c)
{
    emit(PlayingClickedCard(c));
}

void Playing::mousePressEvent(QMouseEvent *event)
{
    emit(PlayingMouseClicked(event->x(), event->y()));
}

void Playing::RightShowCard(Card* tar)
{
    if(RightShow)
    {
        delete RightShow;
        RightShow = nullptr;
    }
    RightShow = new ShownCard(tar, this);
    RightShow->show();
    RightShow->setGeometry(3321 * NowWindowHeight / PlayGroundHeight, 500 * NowWindowHeight / PlayGroundHeight, (4000 - 3321) * NowWindowHeight / PlayGroundHeight, (4000 - 3321) * NowWindowHeight * CardHeightNormal / CardWidthNormal / PlayGroundHeight);
}

void Playing::SetLabelText(QString text)
{
    ui->label_4->setText(ui->label_3->text());
    ui->label_3->setText(ui->label_2->text());
    ui->label_2->setText(ui->label->text());
    ui->label->setText(text);
}

void Playing::ShowMeLeader(PlayerInfo *mMe)
{
    if(sMeLeader)
    {
        sMeLeader->hide();
        delete sMeLeader;
        sMeLeader = nullptr;
    }
    if(sMeLeader == nullptr && mMe->mLeader != nullptr)
    {
        sMeLeader = new ShownCard(mMe->mLeader, this);
        connect(sMeLeader, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
        sMeLeader->setGeometry(3600 * NowWindowHeight / PlayGroundHeight, 1982 * NowWindowHeight / PlayGroundHeight, 433 * NowWindowHeight * CardWidthNormal / CardHeightNormal / PlayGroundHeight, 433 * NowWindowHeight / PlayGroundHeight);
    }
    if(sMeLeader)
    {
        if(mMe->HaveUsedLeader)
        {
            sMeLeader->hide();
        }
        else
        {
            sMeLeader->show();
        }
    }
}

void Playing::ShowEnemyLeader(PlayerInfo *mEnemy)
{
    if(sEnemyLeader)
    {
        sEnemyLeader->hide();
        delete sEnemyLeader;
        sEnemyLeader = nullptr;
    }
    if(sEnemyLeader == nullptr && mEnemy->mLeader != nullptr)
    {
        sEnemyLeader = new ShownCard(mEnemy->mLeader, this);
        connect(sEnemyLeader, SIGNAL(PressedCard(Card*)), this, SLOT(ClickedCard(Card*)));
        sEnemyLeader->setGeometry(3600 * NowWindowHeight / PlayGroundHeight, 0, 433 * NowWindowHeight * CardWidthNormal / CardHeightNormal / PlayGroundHeight, 433 * NowWindowHeight / PlayGroundHeight);
    }
    if(sEnemyLeader)
    {
        if(mEnemy->HaveUsedLeader)
        {
            sEnemyLeader->hide();
        }
        else
        {
            sEnemyLeader->show();
        }
    }
}

void Playing::ShowMeScore(PlayerInfo *mMe)
{
    int i = 0;
    for(auto m : mMe->mFirstRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mMe->mSecondRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mMe->mThirdRow)
    {
        i += m->mCardBloodNow;
    }
    ui->label_ScoreMe->setText(QString::number(i));
}

void Playing::ShowEnemyScore(PlayerInfo *mEnemy)
{
    int i = 0;
    for(auto m : mEnemy->mFirstRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mEnemy->mSecondRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mEnemy->mThirdRow)
    {
        i += m->mCardBloodNow;
    }
    ui->label_ScoreEn->setText(QString::number(i));
}

void Playing::ShowMeWeather(PlayerInfo *mMe)
{
    QString labelText = "";
    if(mMe->FirstWeather.Hit > 0)
    {
        switch (mMe->FirstWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mMe->FirstWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w4->setText(labelText);


    if(mMe->SecondWeather.Hit > 0)
    {
        switch (mMe->SecondWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mMe->SecondWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w5->setText(labelText);


    if(mMe->ThirdWeather.Hit > 0)
    {
        switch (mMe->ThirdWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mMe->ThirdWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w6->setText(labelText);
}

void Playing::ShowEnemyWeather(PlayerInfo *mEnemy)
{
    QString labelText = "";
    if(mEnemy->FirstWeather.Hit > 0)
    {
        switch (mEnemy->FirstWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mEnemy->FirstWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w3->setText(labelText);


    if(mEnemy->SecondWeather.Hit > 0)
    {
        switch (mEnemy->SecondWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mEnemy->SecondWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w2->setText(labelText);


    if(mEnemy->ThirdWeather.Hit > 0)
    {
        switch (mEnemy->ThirdWeather.kind) {
        case 1:
            labelText = "蔽日浓雾";
            break;
        case 2:
            labelText = "倾盆大雨";
            break;
        default:
            labelText = "刺骨冰霜";
            break;
        }
        labelText += "(" + QString::number(mEnemy->ThirdWeather.Hit) + ")";
    }
    else
    {
        labelText = "无天气效果";
    }
    ui->label_w1->setText(labelText);
}

void Playing::on_pushButton_clicked()
{
    emit(PassPressed());
}

int Playing::GetMeScore(PlayerInfo *mMe)
{
    int i = 0;
    for(auto m : mMe->mFirstRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mMe->mSecondRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mMe->mThirdRow)
    {
        i += m->mCardBloodNow;
    }
    return i;
}

int Playing::GetEnemyScore(PlayerInfo *mEnemy)
{
    int i = 0;
    for(auto m : mEnemy->mFirstRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mEnemy->mSecondRow)
    {
        i += m->mCardBloodNow;
    }
    for(auto m : mEnemy->mThirdRow)
    {
        i += m->mCardBloodNow;
    }
    return i;
}

void Playing::ShowTimer(int CountDown)
{
    ui->label_time->setText(QString::number(CountDown));
}
