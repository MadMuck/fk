#ifndef BANKER_SELECTOR_H
#define BANKER_SELECTOR_H

#include "PlayerPos.h"

class BankerSelector
{
    PlayerPos::Type m_Banker;
public:
	BankerSelector();

    void SetGameOwner(PlayerPos::Type owner);

    PlayerPos::Type GetBanker() const;

	void SetBanker(PlayerPos::Type playerPos);
};

#endif
