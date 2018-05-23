#include "cardshowclass.h"
#include "ui_cardshowclass.h"

CardShowClass::CardShowClass(vector<string> list, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CardShowClass),
    mList(list),
    m_page(1),
    pageMax((list.size() + 2) / 3),
    totalNum(list.size())
{
    ui->setupUi(this);
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
    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");

    ui->label->setStyleSheet("color : white");

    LoadPicture();

    ChangeSize(parent->width(), parent->height());
}

CardShowClass::~CardShowClass()
{
    unLoadPicture();
    delete ui;
}

void CardShowClass::LoadPicture()
{
    widget[0] = ui->widget;
    widget[1] = ui->widget_2;
    widget[2] = ui->widget_3;

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
            temp = GetNewCard(getint(mList[i + addid]));
            s[i] = new ShownCard(temp, widget[i]);
            s[i]->setFixedSize(257, 336);
            s[i]->show();
        }
        catch(...)
        {
            qDebug("bug");
        }
    }
}

void CardShowClass::unLoadPicture()
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

void CardShowClass::on_pushButton_l_clicked()
{
    if(m_page > 1)
    {
        m_page -= 1;
        unLoadPicture();
        LoadPicture();
    }
}

void CardShowClass::on_pushButton_r_clicked()
{
    if(m_page < pageMax)
    {
        m_page += 1;
        unLoadPicture();
        LoadPicture();
    }
}

void CardShowClass::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/deckbuilder.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.
}

void CardShowClass::on_pushButton_back_clicked()
{
    emit PressedBack();
}
