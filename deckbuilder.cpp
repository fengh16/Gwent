#include "deckbuilder.h"
#include "ui_deckbuilder.h"

DeckBuilder::DeckBuilder(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeckBuilder),
    m_page(1),
    pageMax(INT_MAX),
    mMe(m),
    HavePressed(false)
{
    for(int i = 0; i < 3; i++)
    {
        s[i] = nullptr;
        label[i] = nullptr;
    }

    mBuilderDecks = mMe->mDecks;

    ui->setupUi(this);

    ui->pushButton_l->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed.png)}");
    ui->pushButton_r->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow_right.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move_right.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed_right.png)}");
    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_buy->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/buy.png)}"
                                      "QPushButton:hover{border-image: url(:/pics/arrow/buy_move.png)}"
                                      "QPushButton:pressed{border-image: url(:/pics/arrow/buy_click.png)}");

    ui->label->setStyleSheet("color : white");
    ui->label_2->setStyleSheet("color : white");
    ui->label_3->setStyleSheet("color : white");
    ui->label_4->setStyleSheet("color : white");

    SetDeckList();

    LoadPicture();

    ChangeSize(parent->width(), parent->height());
}

#include <QDebug>
DeckBuilder::~DeckBuilder()
{
    qDebug("a");
    unLoadPicture();
    qDebug("b");
    delete ui;
    qDebug("DeckBuilderc");
}

void DeckBuilder::LoadPicture()
{
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;

    label[0] = ui->label_2;
    label[1] = ui->label_3;
    label[2] = ui->label_4;

    ui->label->setText(QString::number(m_page));
    int addid = m_page * 3 - 3;
    Card *temp;
    for (int i = 0; i < 3; i++)
    {
        try
        {
            temp = GetNewCard(getint(mShownDecks[i + addid].mLeadCard));
            s[i] = new ShownCard(temp, widget[i]);
            s[i]->setFixedSize(WindowWidth / 3, WindowWidth * CardHeightNormal / CardWidthNormal / 3);
            s[i]->show();

            // Use fromLocal8Bit and .data() to deal with Chienese Showing problems
            // 如果直接用fromStdString中文会乱码……所以现在直接取运行平台的8bit编码
            label[i]->setText(QString::fromLocal8Bit(mShownDecks[i + addid].mDeckIntro.data()));
        }
        catch(...)
        {
            qDebug("bug");
        }
    }
    if(s[0] != nullptr)
    {
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
}

void DeckBuilder::unLoadPicture()
{
    for (int i = 0; i < 3; i++)
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

void DeckBuilder::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void DeckBuilder::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void DeckBuilder::on_pushButton_back_clicked()
{
    emit PressedBack();
}

void DeckBuilder::on_pushButton_buy_clicked()
{
    emit PressedBuy();
}

void DeckBuilder::SetDeckList()
{
    mShownDecks.clear();
    mShownDecks.push_back(DeckInfo());
    for (auto t : mMe->mDecks)
    {
        mShownDecks.push_back(t);
    }
    pageMax = mShownDecks.size() / 3 + 1;
    for (int i = mShownDecks.size(); i < pageMax * 3; i++)
    {
        mShownDecks.push_back(DeckInfo());
    }
}

void DeckBuilder::Pressed1()
{
    Pressed(m_page * 3 - 3);
}

void DeckBuilder::Pressed2()
{
    Pressed(m_page * 3 - 2);
}

void DeckBuilder::Pressed3()
{
    Pressed(m_page * 3 - 1);
}

void DeckBuilder::Pressed(int indexPressed)
{
    if(HavePressed)
    {
        qDebug("PressedAgain!!!!!!!!!!!!!!!!!!!");
        return;
    }
    HavePressed = true;

    // if it's a new deck
    // 如果是一个新的牌堆，先选择领袖
    if(mShownDecks[indexPressed].mLeadCard == "0")
    {
        emit ChooseLeader();
        return;
    }

    QMessageBox box(QMessageBox::Warning, "提示", "您想对这个牌组做什么？");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("删 除"));
    box.setButtonText (QMessageBox::Cancel,QString("查看/修改"));
    if(box.exec() == QMessageBox::Ok)
    {
        mMe->mDecks.erase(mMe->mDecks.begin() + indexPressed - 1);
        emit HaveDeleted();
        return;
    }
    emit ChangeDeck(indexPressed - 1);
}

void DeckBuilder::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    WindowWidth = a;
    WindowHeight = b;

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/deckbuilder.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.

    for (auto i = 0; i < 3; i++)
    {
        if(s[i])
        {
            s[i]->setFixedSize(WindowWidth / 3, WindowWidth * CardHeightNormal / CardWidthNormal / 3);
        }
    }
}
