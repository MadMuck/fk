#ifndef EXTENSION_BANKER_SELECTOR_H
#define EXTENSION_BANKER_SELECTOR_H

#include "GameContext.h"
#include "BankerSelector.h"

class ExtensionBankerSelector : public GameContextExtension
{
protected:
    BankerSelector *m_BankerSelector;

public:
    ExtensionBankerSelector();
    ~ExtensionBankerSelector();
    BankerSelector *GetBankerSelector();
	PlayerPos::Type GetBankerPos() const;
	void SetBankerPos(PlayerPos::Type playerPos);
    void OnGameOwnerChanged(PlayerPos::Type playerPos);
};

#endif
