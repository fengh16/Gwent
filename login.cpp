#include "login.h"
#include "ui_login.h"
#include "base/GameController.h"
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    QFont f("Calibri", 20);
    ui->pushButton->setFont(f);
    ui->pushButton->setText(tr("START"));
    ui->pushButton->setStyleSheet("QPushButton{border-image: url(:/pics/button/button.png); color: white}"
                                  "QPushButton:hover{border-image: url(:/pics/button/button_move.png); color: white}"
                                  "QPushButton:pressed{border-image: url(:/pics/button/button_click.png); color: yellow}");

    ui->label->setStyleSheet("color: white");
    ui->label_2->setStyleSheet("color: white");

    ChangeSize(parent->width(), parent->height());
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    if(IOCheckPassword(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString()))
    {
        emit(Passed(ui->lineEdit->text().toStdString(), ui->lineEdit_2->text().toStdString()));
    }
    else
    {
        QMessageBox box(QMessageBox::Critical, "提示", "用户名密码错误！");
        box.setStandardButtons (QMessageBox::Ok);
        box.setButtonText (QMessageBox::Ok,QString("确 定"));
        box.exec();
//        QMessageBox::critical(NULL, "Error", "用户名密码错误！", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
}

void Login::ChangeSize(int a, int b)
{
    this->setGeometry(0, 0, a, b);

    // 设置背景
    setAutoFillBackground(true); // 这句要加上, 否则可能显示不出背景图.
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window,
                     QBrush(QPixmap(":/pics/back/Back.png").scaled( // 缩放背景图.
                             size(),
                             Qt::IgnoreAspectRatio,
                             Qt::SmoothTransformation))); // 使用平滑的缩放方式
    setPalette(palette); // 至此, 已给widget加上了背景图.
}
