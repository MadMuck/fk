#ifndef EXTENSION_HAI_DI_H
#define EXTENSION_HAI_DI_H

#include "GameContext.h"
#include "PlayerPos.h"

class ExtensionHaiDi : public GameContextExtension
{
public:

	std::set<PlayerPos::Type> playerlist;

	void OnGameStart()
	{
		playerlist.clear();
	}
};

#endif
