#ifndef GANG_HANDLER_H
#define GANG_HANDLER_H

#include "ActionValidator.h"
#include "ExtensionQueMen.h"

class GangHandlerImpl : public DefaultGangHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards, const InterceptionList &interceptions, Interception::Type type, Card::Type card) const
	{
		OBJ_GET_EXT(context, ExtensionQueMen, extQueMen);

		CardList newCards = cards - extQueMen->FilterCards(player, cards);

		return DefaultGangHandlerImpl::ToDo(context, player, srcPlayer, newCards, interceptions, type, card);
	}
};

class GangHandler : public InterceptionHandler
{
public:
	GangHandler():InterceptionHandler(new GangHandlerImpl()){}
};

#endif