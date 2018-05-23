#ifndef LOAD_H
#define LOAD_H

#include <QWidget>

namespace Ui {
class Load;
}

class Load : public QWidget
{
    Q_OBJECT

public:
    explicit Load(QWidget *parent = 0);
    ~Load();
    void GetAnimation();

private:
    Ui::Load *ui;

public slots:
    void ChangeSize(int, int);

signals:
    void Pressed();

private slots:
    void on_pushButton_clicked();
};

#endif // LOAD_H
