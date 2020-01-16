#ifndef PLAYER_CREATOR_H
#define PLAYER_CREATOR_H

#include "Player.h"

class PlayerCreator
{
public:
    static Player *Create(PlayerPos::Type pos);
    static void Destroy(Player *);
};

#endif
