#ifndef CHOOSING_H
#define CHOOSING_H

#include <QWidget>

namespace Ui {
class Choosing;
}

class Choosing : public QWidget
{
    Q_OBJECT

public:
    explicit Choosing(QWidget *parent = 0);
    ~Choosing();

private slots:
    void on_pushButton_Friends_clicked();
    void on_pushButton_DeckBuilder_clicked();
    void on_pushButton_Playing_clicked();
    void on_pushButton_OpenKeg_clicked();
    void on_pushButton_BuyKeg_clicked();
    void on_pushButton_Settings_clicked();
    void on_pushButton_Reward_clicked();
    void on_pushButton_Collection_clicked();

signals:
    void pushButtFriends_clicked();
    void pushButtDeckBuilder_clicked();
    void pushButtPlaying_clicked();
    void pushButtOpenKeg_clicked();
    void pushButtBuyKeg_clicked();
    void pushButtSettings_clicked();
    void pushButtReward_clicked();
    void pushButtCollection_clicked();

private:
    Ui::Choosing *ui;

public slots:
    void ChangeSize(int, int);

};

#endif // CHOOSING_H
