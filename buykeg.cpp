#include "buykeg.h"
#include "ui_buykeg.h"

BuyKeg::BuyKeg(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BuyKeg),
    mMe(m)
{
    ui->setupUi(this);

    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton_buy->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                      "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                      "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->label->setStyleSheet("color: white");

    ShowInfo();

    ChangeSize(parent->width(), parent->height());
}

BuyKeg::~BuyKeg()
{
    delete ui;
}

void BuyKeg::on_pushButton_back_clicked()
{
    emit PressedBack();
}

void BuyKeg::on_pushButton_buy_clicked()
{
    if(mMe->mMoney < 400)
    {
        QMessageBox box1(QMessageBox::Warning, "提示", "您的钱不足，多玩游戏来赚钱吧！");
        box1.setStandardButtons (QMessageBox::Ok);
        box1.setButtonText (QMessageBox::Ok,QString("确 定"));
        box1.exec();
        return;
    }
    QMessageBox box(QMessageBox::Warning, "提示", "您想花400元购买一个牌桶吗？");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("确 定"));
    box.setButtonText (QMessageBox::Cancel,QString("取 消"));
    if(box.exec() == QMessageBox::Ok)
    {
        mMe->mMoney -= 400;
        mMe->mKegsnum += 1;
        ShowInfo();
        emit PressedBuy();
    }
}

void BuyKeg::ShowInfo()
{
    ui->label->setText("当前牌桶数：" + QString::number(mMe->mKegsnum) + " 当前剩余金钱：" + QString::number(mMe->mMoney));
}

void BuyKeg::ChangeSize(int a, int b)
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
