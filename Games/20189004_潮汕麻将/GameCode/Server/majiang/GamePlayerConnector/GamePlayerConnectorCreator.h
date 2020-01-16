#ifndef GAME_PLAYER_CONNECTOR_CREATOR_H
#define GAME_PLAYER_CONNECTOR_CREATOR_H

#include "GamePlayerConnector.h"

class GamePlayerConnectorCreator
{
public:
    static GamePlayerConnector *Create();
    static void Destroy(GamePlayerConnector *connector);
};

#endif
