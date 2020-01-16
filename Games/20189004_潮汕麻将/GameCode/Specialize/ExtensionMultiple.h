#ifndef EXTENSION_MULTIPLE_H
#define EXTENSION_MULTIPLE_H

#include "GameContext.h"
#include "PlayerPos.h"

class ExtensionMultiple : public GameContextExtension
{
public:

	std::map<PlayerPos::Type, int> playersMultiple;

	void OnGameStart()
	{
		playersMultiple.clear();
	}
};

#endif

