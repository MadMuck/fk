#include "GameDirectionCreator.h"

GameDirection *GameDirectionCreator::Create()
{
    return new GameDirection();
}

void GameDirectionCreator::Destroy(GameDirection *gd)
{
    delete gd;
}
