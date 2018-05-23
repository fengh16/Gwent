#ifndef DECKBUILDER_H
#define DECKBUILDER_H

#include <QWidget>
#include "showncard.h"
#include "base/PlayerInfo.h"

namespace Ui {
class DeckBuilder;
}

class DeckBuilder : public QWidget
{
    Q_OBJECT

public:
    explicit DeckBuilder(PlayerInfo *m, QWidget *parent = 0);
    ~DeckBuilder();

private slots:
    void on_pushButton_l_clicked();
    void on_pushButton_r_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_buy_clicked();

private:
    Ui::DeckBuilder *ui;
    int m_page;
    ShownCard *s[3];
    QWidget *widget[3];
    QLabel *label[3];
    int pageMax;
    PlayerInfo *mMe;
    vector<DeckInfo> mBuilderDecks;
    vector<DeckInfo> mShownDecks;
    int WindowHeight;
    int WindowWidth;

    void LoadPicture();
    void unLoadPicture();
    void SetDeckList();

    bool HavePressed;

    void Pressed(int);

signals:
    void PressedBack();
    void PressedBuy();
    void ChooseLeader();
    void HaveDeleted();
    void ChangeDeck(int);

public slots:
    void Pressed1();
    void Pressed2();
    void Pressed3();
    void ChangeSize(int, int);
};

#endif // DECKBUILDER_H
