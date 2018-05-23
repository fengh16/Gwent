#include "reward.h"
#include "ui_reward.h"

Reward::Reward(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Reward),
    mMe(m)
{
    ui->setupUi(this);

    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->label->setStyleSheet("color: white");

    ChangeSize(parent->width(), parent->height());
}

Reward::~Reward()
{
    delete ui;
}

void Reward::on_pushButton_back_clicked()
{
    emit PressedBack();
}

void Reward::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/Wood_Background.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.
}
