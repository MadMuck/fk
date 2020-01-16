#include "PlayerCreator.h"

Player *PlayerCreator::Create(PlayerPos::Type pos)
{
    return new Player(pos);
}

void PlayerCreator::Destroy(Player *player)
{
    delete player;
}
