#ifndef ANSSHOW_H
#define ANSSHOW_H

#include <QWidget>

namespace Ui {
class AnsShow;
}

class AnsShow : public QWidget
{
    Q_OBJECT

public:
    explicit AnsShow(bool misB, int mAint, int mBint, int mA1, int mA2, int mA3, int mB1, int mB2, int mB3, QWidget *parent = 0);
    ~AnsShow();

private:
    Ui::AnsShow *ui;
    bool isB;
    int Aint;
    int Bint;
    int A1;
    int A2;
    int A3;
    int B1;
    int B2;
    int B3;
    int WindowWidth;
    int WindowHeight;

public slots:
    void ChangeSize(int, int);

signals:
    void ClickedClose();
private slots:
    void on_pushButton_clicked();
};

#endif // ANSSHOW_H
