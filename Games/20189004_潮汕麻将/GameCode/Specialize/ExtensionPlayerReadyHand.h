#ifndef EXTENSION_PLAYER_READY_HAND_H
#define EXTENSION_PLAYER_READY_HAND_H

#include "GameContext.h"
#include "ExtensionPlayer.h"
#include <map>
#include "Card.h"
#include "PlayerPos.h"
#include "ActionValidator.h"
#include "PlayerStatusDef.h"

class ExtensionPlayerReadyHand : public GameContextExtension
{
	std::map<PlayerPos::Type, CardStat> m_PlayerCardStat;
public:
	void OnGameStart()
	{
		m_PlayerCardStat.clear();
	}

	void OnDiscard(PlayerPos::Type playerPos)
	{
		OBJ_GET_EXT(m_Context, ExtensionPlayer, extPlayer);
		Player *player = extPlayer->GetPlayer(playerPos);
		CardStat stat(player->GetCards());
		if(m_PlayerCardStat[playerPos] != stat)
		{
			m_PlayerCardStat[playerPos] = stat;

			// ¸üÐÂÌýÅÆ×´Ì¬
			CardStatistic cardStat(player->GetCards(), player->GetInterceptions());
			bool readyHand = DefaultTingHandlerImpl().IsReadyHand(m_Context, playerPos, cardStat);
			extPlayer->GetPlayerStatus(playerPos).SetBool(PlayerStatus_ReadyHand, readyHand);
		}
	}
};

#endif