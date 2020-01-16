#ifndef EXTENSION_ACTION_VALIDATOR_H
#define EXTENSION_ACTION_VALIDATOR_H

#include "GameContext.h"
#include "ActionValidator.h"
#include <map>
#include <list>

class InterceptionCheckType : public std::list<Interception::Type>
{
public:
	void Add(Interception::Type type);
};

class ExtensionActionValidator : public GameContextExtension
{
protected:

	InterceptionValidator m_InterceptionValidator;

	InterceptionHandlerList m_InterceptionHandlers;

	std::map<int, InterceptionCheckType> m_CheckTypes;

public:
    ExtensionActionValidator();

	bool IsInterceptionValid(PlayerPos::Type player, Interception::Type type, const CardInfo &card) const;

	void SetInterceptionValidator(const InterceptionValidator &validator);

	InterceptionList HandlerInterception(PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card);

	void SetInterceptionHandler(Interception::Type type, const InterceptionHandler &handler);

	InterceptionHandler GetInterceptionHandler(Interception::Type type) const;

	InterceptionCheckType GetCheckType(int type) const;

	void SetCheckType(int type, const InterceptionCheckType &checkType);

	void OnGameStart();
};

#endif
