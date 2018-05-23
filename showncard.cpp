#include "showncard.h"
#include <QDebug>

ShownCard::ShownCard(Card* c, QWidget *parent) : QPushButton(parent), m_Card(c)
{
    setStyleSheet("QPushButton{border-image: url(:/pics/card/Card_None.png); color: black}"
                  "QPushButton:hover{border-image: url(:/pics/card_move/Card_None_move.png); color: white}"
                  "QPushButton:pressed{border-image: url(:/pics/card_click/Card_None_click.png)}; color: yellow");
    if(c != nullptr)
    {
        SetPic(c->GetCardClassName());
    }
    setFont(QFont("Myanmar Text", 18, QFont::Bold));
    connect(this, SIGNAL(clicked()), this, SLOT(EmitThings()));
}

void ShownCard::SetPic(string Pic)
{
    setStyleSheet("QPushButton{border-image: url(:/pics/card/" + QString::fromStdString(Pic) + ".png); color: black}"
                  "QPushButton:hover{border-image: url(:/pics/card_move/" + QString::fromStdString(Pic) + "_move.png); color: white}"
                  "QPushButton:pressed{border-image: url(:/pics/card_click/" + QString::fromStdString(Pic) + "_click.png)}; color: yellow");
}

void ShownCard::SetPic(QString Pic)
{
    setStyleSheet("QPushButton{border-image: url(:/pics/card/" + Pic + ".png); color: black}"
                  "QPushButton:hover{border-image: url(:/pics/card_move/" + Pic + "_move.png); color: white}"
                  "QPushButton:pressed{border-image: url(:/pics/card_click/" + Pic + "_click.png); color: yellow}");
}

Card* ShownCard::GetCard() const
{
    return m_Card;
}

void ShownCard::EmitThings()
{
    emit(PressedCard(m_Card));
}

void ShownCard::EmitMoveThings()
{
    emit(MovedCard(m_Card));
}
