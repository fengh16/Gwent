#include "choosing.h"
#include "ui_choosing.h"

Choosing::Choosing(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Choosing)
{
    ui->setupUi(this);

    ui->pushButton_BuyKeg->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_Collection->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_DeckBuilder->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_Friends->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_OpenKeg->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_Playing->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_Reward->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->pushButton_Settings->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ChangeSize(parent->width(), parent->height());
}

Choosing::~Choosing()
{
    delete ui;
}

void Choosing::on_pushButton_Friends_clicked()
{
    emit(pushButtFriends_clicked());
}

void Choosing::on_pushButton_DeckBuilder_clicked()
{
    emit(pushButtDeckBuilder_clicked());
}

void Choosing::on_pushButton_Playing_clicked()
{
    emit(pushButtPlaying_clicked());
}

void Choosing::on_pushButton_OpenKeg_clicked()
{
    emit(pushButtOpenKeg_clicked());
}

void Choosing::on_pushButton_BuyKeg_clicked()
{
    emit(pushButtBuyKeg_clicked());
}

void Choosing::on_pushButton_Settings_clicked()
{
    emit(pushButtSettings_clicked());
}

void Choosing::on_pushButton_Reward_clicked()
{
    emit(pushButtReward_clicked());
}

void Choosing::on_pushButton_Collection_clicked()
{
    emit(pushButtCollection_clicked());
}

void Choosing::ChangeSize(int a, int b)
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
