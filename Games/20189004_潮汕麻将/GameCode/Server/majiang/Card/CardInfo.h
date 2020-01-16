#ifndef CARD_INFO_H
#define CARD_INFO_H

#include "Card.h"
#include "PlayerPos.h"
#include "ProcedureType.h"

struct CardInfo
{
    CardInfo():playerPos(PlayerPos::P0), card(Card::Invalid){}
    CardInfo(PlayerPos::Type pos, Card::Type c):playerPos(pos), card(c){}

    PlayerPos::Type playerPos;
    Card::Type card;
};

#endif
