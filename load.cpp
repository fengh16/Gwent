#include "load.h"
#include "ui_load.h"
#include <QtGui>
#include <QPropertyAnimation>
#include <QRect>
#include <QParallelAnimationGroup>

Load::Load(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Load)
{
    ui->setupUi(this);

    QFont f("Calibri", 20);
    ui->pushButton->setFont(f);
    ui->pushButton->setText("START");

    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_2->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");

    ChangeSize(parent->width(), parent->height());
    GetAnimation();
}

void Load::GetAnimation()
{
    QPropertyAnimation *animation1 = new QPropertyAnimation(ui->pushButton_2, "geometry");
    animation1->setDuration(10000);
    animation1->setStartValue(QRect(0, 0, 5, 100));
    animation1->setEndValue(QRect(0, 0, 700, 100));
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

Load::~Load()
{
    delete ui;
}

void Load::on_pushButton_clicked()
{
    emit(Pressed());
}

void Load::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/LoadBack.jpg").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    this->setPalette(palette); // 至此, 已给widget加上了背景图.
}
