#include "GamePlayerConnectorCreator.h"

GamePlayerConnector *GamePlayerConnectorCreator::Create()
{
    return new GamePlayerConnector();
}

void GamePlayerConnectorCreator::Destroy(GamePlayerConnector *connector)
{
    delete connector;
}
