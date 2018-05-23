#include "ansshow.h"
#include "ui_ansshow.h"

AnsShow::AnsShow(bool misB, int mAint, int mBint, int mA1, int mA2, int mA3, int mB1, int mB2, int mB3, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AnsShow),
    isB(misB),
    Aint(mAint),
    Bint(mBint),
    A1(mA1),
    A2(mA2),
    A3(mA3),
    B1(mB1),
    B2(mB2),
    B3(mB3)
{
    ui->setupUi(this);

    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");

    ui->label_A->setStyleSheet("color:white");
    ui->label_A1->setStyleSheet("color:white");
    ui->label_A2->setStyleSheet("color:white");
    ui->label_A3->setStyleSheet("color:white");
    ui->label_AInfo->setStyleSheet("color:white");
    ui->label_B->setStyleSheet("color:white");
    ui->label_B1->setStyleSheet("color:white");
    ui->label_B2->setStyleSheet("color:white");
    ui->label_B3->setStyleSheet("color:white");
    ui->label_BInfo->setStyleSheet("color:white");
    ui->label_top->setStyleSheet("color:white");

    if(isB)
    {
        ui->label_A1->setText(QString::number(B1));
        ui->label_A2->setText(QString::number(B2));
        ui->label_A3->setText(QString::number(B3));
        ui->label_B1->setText(QString::number(A1));
        ui->label_B2->setText(QString::number(A2));
        ui->label_B3->setText(QString::number(A3));
        if(Aint < Bint)
        {
            ui->label_AInfo->setText("胜利");
            ui->label_BInfo->setText("失败");
        }
        else if(Aint > Bint)
        {
            ui->label_AInfo->setText("失败");
            ui->label_BInfo->setText("胜利");
        }
        else
        {
            ui->label_AInfo->setText("平局");
            ui->label_BInfo->setText("平局");
        }
    }
    else
    {
        ui->label_B1->setText(QString::number(B1));
        ui->label_B2->setText(QString::number(B2));
        ui->label_B3->setText(QString::number(B3));
        ui->label_A1->setText(QString::number(A1));
        ui->label_A2->setText(QString::number(A2));
        ui->label_A3->setText(QString::number(A3));
        if(Aint > Bint)
        {
            ui->label_AInfo->setText("胜利");
            ui->label_BInfo->setText("失败");
        }
        else if(Aint < Bint)
        {
            ui->label_AInfo->setText("失败");
            ui->label_BInfo->setText("胜利");
        }
        else
        {
            ui->label_AInfo->setText("平局");
            ui->label_BInfo->setText("平局");
        }
    }

    ChangeSize(parent->width(), parent->height());
}

AnsShow::~AnsShow()
{
    delete ui;
}

void AnsShow::ChangeSize(int a, int b)
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
}

void AnsShow::on_pushButton_clicked()
{
    emit(ClickedClose());
}
