#ifndef SHOWNCARD_H
#define SHOWNCARD_H

#include <QPushButton>
#include "base/Cards.h"

class ShownCard : public QPushButton
{
    Q_OBJECT

public:
    explicit ShownCard(Card* c = nullptr, QWidget *parent = 0);
    void SetPic(string Pic);
    void SetPic(QString Pic);
    Card* GetCard() const;

private:
    Card* m_Card;

signals:
    void PressedCard(Card*);
    void MovedCard(Card*);

public slots:
    void EmitThings();
    void EmitMoveThings();
};

#endif // SHOWNCARD_H
