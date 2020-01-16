#ifndef GAME_DIRECTION_CREATOR_H
#define GAME_DIRECTION_CREATOR_H

#include "GameDirection.h"

class GameDirectionCreator
{
public:
    static GameDirection *Create();
    static void Destroy(GameDirection *gd);
};

#endif
