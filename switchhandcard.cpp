#include "switchhandcard.h"
#include "ui_switchhandcard.h"

SwitchHandCard::SwitchHandCard(PlayerInfo *m, int CardsNum, QWidget *parent, int nowPage) :
    QWidget(parent),
    ui(new Ui::SwitchHandCard),
    mMe(m),
    m_page(nowPage),
    pageMax((m->mHand.size() + 2) / 3),
    totalNum(m->mHand.size()),
    HavePressed(false),
    SwitchCardNum(CardsNum)
{
    for(int i = 0; i < 3; i++)
    {
        s[i] = nullptr;
        labels[i] = nullptr;
    }

    ui->setupUi(this);

    ui->pushButton_l->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed.png)}");
    ui->pushButton_r->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow_right.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move_right.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed_right.png)}");
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");

    ChangeSize(parent->width(), parent->height());

    ui->label->setStyleSheet("color:white");
    ui->label_5->setStyleSheet("color:white");

    ui->label_5->setText("您还可以换" + QString::number(SwitchCardNum) + "张牌");

    LoadPicture();
}

SwitchHandCard::~SwitchHandCard()
{
    unLoadPicture();
    delete ui;
}

void SwitchHandCard::LoadPicture()
{
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;

    labels[0] = ui->label_2;
    labels[1] = ui->label_3;
    labels[2] = ui->label_4;

    ui->label->setText(QString::number(m_page));
    int addid = m_page * 3 - 3;
    for (int i = 0; i < 3; i++)
    {
        try
        {
            if(i + addid >= totalNum)
            {
                break;
            }
            s[i] = new ShownCard(mMe->mHand[i + addid], widget[i]);
            s[i]->setFixedSize(WindowWidth / 3, WindowWidth * CardHeightNormal / CardWidthNormal / 3);
            s[i]->show();
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

void SwitchHandCard::unLoadPicture()
{
    labels[0] = ui->label_2;
    labels[1] = ui->label_3;
    labels[2] = ui->label_4;
    for (int i = 0; i < 3; i++)
    {
        if(s[i] != nullptr)
        {
            s[i]->hide();

            delete s[i];
            s[i] = nullptr;
        }
    }
}

void SwitchHandCard::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void SwitchHandCard::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void SwitchHandCard::Pressed1()
{
    Pressed(m_page * 3 - 3);
}

void SwitchHandCard::Pressed2()
{
    Pressed(m_page * 3 - 2);
}

void SwitchHandCard::Pressed3()
{
    Pressed(m_page * 3 - 1);
}

void SwitchHandCard::Pressed(int pressedInt)
{
    if(HavePressed)
    {
        qDebug("PressedAgain!!!!!!!!!!!!!!!!!!!");
        return;
    }
    HavePressed = true;

    emit PressedInt(pressedInt, SwitchCardNum, mMe, m_page);
}

void SwitchHandCard::ChangeSize(int a, int b)
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

    for (auto i = 0; i < 3; i++)
    {
        if(s[i])
        {
            s[i]->setFixedSize(WindowWidth / 3, WindowWidth * CardHeightNormal / CardWidthNormal / 3);
        }
    }
}

void SwitchHandCard::on_pushButton_clicked()
{
    Pressed(-1);
}
