#include "friends.h"
#include "ui_friends.h"

Friends::Friends(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Friends),
    mMe(m)
{
    ui->setupUi(this);

    ChangeSize(parent->width(), parent->height());
}

Friends::~Friends()
{
    delete ui;
}

void Friends::ChangeSize(int a, int b)
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
