#include "ExtensionHuAction.h"

void ExtensionHuAction::AddHuAction(PlayerPos::Type playerPos, const InterceptionList &actions)
{
    m_HuActions[playerPos] = actions;
}

const HuActions &ExtensionHuAction::GetHuActions() const
{
    return m_HuActions;
}

void ExtensionHuAction::OnGameReset()
{
    m_HuActions.clear();
}
