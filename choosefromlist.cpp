#include "choosefromlist.h"
#include "ui_choosefromlist.h"

ChooseFromList::ChooseFromList(ChooseFunctionFrom fromid, vector<string> list, vector<string> info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseFromList),
    mList(list),
    mInfo(info),
    m_page(1),
    pageMax((list.size() + 2) / 3),
    totalNum(list.size()),
    mFromid(fromid),
    HavePressed(false)
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

    ui->label->setStyleSheet("color : white");
    ui->label_2->setStyleSheet("color : white");
    ui->label_3->setStyleSheet("color : white");
    ui->label_4->setStyleSheet("color : white");

    LoadPicture();

    ChangeSize(parent->width(), parent->height());
}


ChooseFromList::ChooseFromList(ChooseFunctionFrom fromid, vector<string> list, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChooseFromList),
    mList(list),
    m_page(1),
    pageMax((list.size() + 2) / 3),
    totalNum(list.size()),
    mFromid(fromid),
    HavePressed(false)
{
    for(int i = 0; i < 3; i++)
    {
        s[i] = nullptr;
    }

    ui->setupUi(this);

    ui->pushButton_l->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed.png)}");
    ui->pushButton_r->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/arrow_right.png)}"
                                    "QPushButton:hover{border-image: url(:/pics/arrow/arrow_move_right.png)}"
                                    "QPushButton:pressed{border-image: url(:/pics/arrow/arrow_pressed_right.png)}");

    ChangeSize(parent->width(), parent->height());

    ui->label->setStyleSheet("color:white");
    ui->label_2->setStyleSheet("color:white");
    ui->label_3->setStyleSheet("color:white");
    ui->label_4->setStyleSheet("color:white");
    LoadPicture();
}

#include <QDebug>
ChooseFromList::~ChooseFromList()
{
    unLoadPicture();
    delete ui;
}

void ChooseFromList::LoadPicture()
{
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;

    labels[0] = ui->label_2;
    labels[1] = ui->label_3;
    labels[2] = ui->label_4;

    ui->label->setText(QString::number(m_page));
    int addid = m_page * 3 - 3;
    Card *temp;
    for (int i = 0; i < 3; i++)
    {
        try
        {
            if(i + addid >= totalNum)
            {
                break;
            }
            if(mFromid == cChooseDeck)
            {
                labels[i]->setText(QString::fromLocal8Bit(mInfo[i + addid].data()));
            }
            temp = GetNewCard(getint(mList[i + addid]));
            s[i] = new ShownCard(temp, widget[i]);
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

void ChooseFromList::unLoadPicture()
{
    labels[0] = ui->label_2;
    labels[1] = ui->label_3;
    labels[2] = ui->label_4;
    for (int i = 0; i < 3; i++)
    {
        if(mFromid == cChooseDeck)
        {
            labels[i]->setText("");
        }
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

void ChooseFromList::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void ChooseFromList::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void ChooseFromList::Pressed1()
{
    Pressed(m_page * 3 - 3);
}

void ChooseFromList::Pressed2()
{
    Pressed(m_page * 3 - 2);
}

void ChooseFromList::Pressed3()
{
    Pressed(m_page * 3 - 1);
}

void ChooseFromList::Pressed(int pressedInt)
{
    if(HavePressed)
    {
        qDebug("PressedAgain!!!!!!!!!!!!!!!!!!!");
        return;
    }
    HavePressed = true;

    emit PressedInt(mFromid, pressedInt);
}

void ChooseFromList::ChangeSize(int a, int b)
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

    for (int i = 0; i < 3; i++)
    {
        if(s[i])
        {
            s[i]->setFixedSize(WindowWidth / 3, WindowWidth * CardHeightNormal / CardWidthNormal / 3);
        }
    }
}
