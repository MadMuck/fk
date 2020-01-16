#include "ExtensionBankerSelector.h"
#include "BankerSelectorCreator.h"

ExtensionBankerSelector::ExtensionBankerSelector()
{
    m_BankerSelector = BankerSelectorCreator::Create();
}

ExtensionBankerSelector::~ExtensionBankerSelector()
{
    BankerSelectorCreator::Destroy(m_BankerSelector);
}

BankerSelector *ExtensionBankerSelector::GetBankerSelector()
{
    return m_BankerSelector;
}

PlayerPos::Type ExtensionBankerSelector::GetBankerPos() const
{
	return m_BankerSelector->GetBanker();
}

void ExtensionBankerSelector::SetBankerPos(PlayerPos::Type playerPos)
{
	return m_BankerSelector->SetBanker(playerPos);
}

void ExtensionBankerSelector::OnGameOwnerChanged(PlayerPos::Type playerPos)
{
    m_BankerSelector->SetGameOwner(playerPos);
}
