#include "collection.h"
#include "ui_collection.h"

Collection::Collection(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Collection),
    m_page(1),
    pageMax(666),
    mMe(m)
{
    for(int i = 0; i < 10; i++)
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
    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_buy->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/buy.png)}"
                                      "QPushButton:hover{border-image: url(:/pics/arrow/buy_move.png)}"
                                      "QPushButton:pressed{border-image: url(:/pics/arrow/buy_click.png)}");

    LoadPicture();

    ChangeSize(parent->width(), parent->height());
}

Collection::~Collection()
{
    unLoadPicture();
    delete ui;
}

void Collection::LoadPicture()
{
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;
    widget[3] = ui->widget_4;
    widget[4] = ui->widget_5;
    widget[5] = ui->widget_6;
    widget[6] = ui->widget_7;
    widget[7] = ui->widget_8;
    widget[8] = ui->widget_9;
    widget[9] = ui->widget_10;

    ui->label->setText(QString::number(m_page));
    int addid = m_page * 10 - 9;
    Card *temp;
    for (int i = 0; i < 10; i++)
    {
        try
        {
            temp = GetNewCard(i + addid);
            if(temp == nullptr)
            {
                pageMax = m_page;
                return;
            }
            s[i] = new ShownCard(temp, widget[i]);
            s[i]->setFont(QFont("Myanmar Text", 50, QFont::Bold));
            s[i]->setText(QString::fromStdString(getstring(mMe->mCardMap[getstring(i + addid)])));
            s[i]->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
            s[i]->show();
        }
        catch(...)
        {
            qDebug("bug");
        }
    }
    Card *c = GetNewCard(m_page * 10 + 1);
    if(c == nullptr)
    {
        pageMax = m_page;
        return;
    }
    delete c;
}

void Collection::unLoadPicture()
{
    for (int i = 0; i < 10; i++)
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

void Collection::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void Collection::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void Collection::on_pushButton_back_clicked()
{
    emit PressedBack();
}

void Collection::on_pushButton_buy_clicked()
{
    emit PressedBuy();
}

void Collection::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    WindowWidth = a;
    WindowHeight = b;

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/collection.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.

    for (auto i = 0; i < 10; i++)
    {
        if(s[i])
        {
            s[i]->setFixedSize(WindowWidth / 5, WindowWidth * CardHeightNormal / CardWidthNormal / 5);
        }
    }
}
