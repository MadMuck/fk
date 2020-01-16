#include "NotificationLiquidation.h"
#include "Logger.h"
#include "ExtensionMessageDelegate.h"

NotificationLiquidation::NotificationLiquidation(const Liquidation &liquidation):m_Liquidation(liquidation)
{
}

void NotificationLiquidation::OnSend(GameContext *context) const
{
	OBJ_GET_EXT(context, ExtensionMessageDelegate, extMsgDelegate);
	extMsgDelegate->GetDelegate(MSG_DELEGATE_LIQUIDATION).OnSend(context, (void *)&m_Liquidation);
}
