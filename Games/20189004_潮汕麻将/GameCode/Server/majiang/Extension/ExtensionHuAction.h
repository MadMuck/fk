#ifndef EXTENSION_HU_ACTION_H
#define EXTENSION_HU_ACTION_H

#include "GameContext.h"
#include "Interception.h"
#include "PlayerPos.h"
#include <map>

class ExtensionHuAction : public GameContextExtension
{
protected:
    HuActions m_HuActions;
public:
    virtual void AddHuAction(PlayerPos::Type playerPos, const InterceptionList &actions);

    virtual const HuActions &GetHuActions() const;

    void OnGameReset();
};

#endif
