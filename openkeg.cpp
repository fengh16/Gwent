#include "openkeg.h"
#include "ui_openkeg.h"

OpenKeg::OpenKeg(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenKeg),
    mMe(m)
{
    ui->setupUi(this);

    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_open->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                       "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                       "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->label->setStyleSheet("color: white");

    ShowInfo();

    ChangeSize(parent->width(), parent->height());
}

OpenKeg::~OpenKeg()
{
    delete ui;
}

void OpenKeg::on_pushButton_back_clicked()
{
    emit PressedBack();
}

#include <QDebug>
void OpenKeg::on_pushButton_open_clicked()
{
    if(mMe->mKegsnum <= 0)
    {
        QMessageBox box1(QMessageBox::Warning, "提示", "您的牌桶不足，返回去开牌桶吧！");
        box1.setStandardButtons (QMessageBox::Ok);
        box1.setButtonText (QMessageBox::Ok,QString("确 定"));
        box1.exec();
        return;
    }
    QMessageBox box(QMessageBox::Warning, "提示", "您想开一个牌桶吗？");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    box.setButtonText (QMessageBox::Cancel,QString("取 消"));
    if(box.exec() == QMessageBox::Ok)
    {
        mMe->mKegsnum -= 1;
        vector<int> ChosenCards;
        for (int i = 0; i < 5; i++)
        {
            int ChosenCardNum = IOGetRandomBetweenZeroAndNum(cCard_Num);
            mMe->mCardList.push_back(getstring(ChosenCardNum));
            mMe->mCardMap[getstring(ChosenCardNum)]++;
            ChosenCards.push_back(ChosenCardNum);
            qDebug() << ChosenCardNum;
        }
        // 如果有时间的话，在这里加上显示出来这些抽出的卡牌的部分
        ShowInfo();
        emit PressedOpen();
    }
    QMessageBox box2(QMessageBox::Warning, "提示", "您已经开了一个牌桶，获得了五张牌，去“收藏”里面看看吧！");
    box2.setStandardButtons (QMessageBox::Ok);
    box2.setButtonText (QMessageBox::Ok,QString("确 定"));
    box2.exec();
}

void OpenKeg::ShowInfo()
{
    ui->label->setText("当前牌桶数：" + QString::number(mMe->mKegsnum));
}

void OpenKeg::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/Buy.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.
}
