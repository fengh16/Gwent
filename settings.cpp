#include "settings.h"
#include "ui_settings.h"

Settings::Settings(PlayerInfo *m, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings),
    mMe(m)
{
    ui->setupUi(this);

    ui->pushButton_back->setStyleSheet("QPushButton{border-image: url(:/pics/arrow/back.png)}"
                                       "QPushButton:hover{border-image: url(:/pics/arrow/back_move.png)}"
                                       "QPushButton:pressed{border-image: url(:/pics/arrow/back_click.png)}");
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");
    ui->label->setStyleSheet("color: white");

    ChangeSize(parent->width(), parent->height());
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_clicked()
{
    if(ui->lineEdit->text() == "")
    {
        QMessageBox box(QMessageBox::Critical, "提示", "请输入修改后的密码！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
    }
    string old = mMe->mPassword;
    mMe->mPassword = ui->lineEdit->text().toStdString();
    emit PressedSave(old);
    QMessageBox box1(QMessageBox::Warning, "提示", "修改密码成功！");
    box1.setStandardButtons (QMessageBox::Ok);
    box1.setButtonText (QMessageBox::Ok,QString("确 定"));
    box1.exec();
}

void Settings::on_pushButton_back_clicked()
{
    emit PressedBack();
}

void Settings::ChangeSize(int a, int b)
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
