#ifndef ACTION_VALIDATOR_H
#define ACTION_VALIDATOR_H

#include "Interception.h"
#include "Card.h"
#include "GameContext.h"
#include "RefCntObj.h"
#include "CardStatistic.h"
#include <map>

class InterceptionValidatorImpl
{
public:
	virtual ~InterceptionValidatorImpl(){}

	virtual bool IsValid(GameContext *context, PlayerPos::Type player, Interception::Type type, const CardInfo &cardInfo) const;
};

class InterceptionHandlerImpl
{
public:
	virtual ~InterceptionHandlerImpl(){}

	virtual InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const
	{
		return InterceptionList();
	}
};

typedef RefCntObj<InterceptionValidatorImpl> InterceptionValidator;

typedef RefCntObj<InterceptionHandlerImpl> InterceptionHandler;

////////////////////////////////////////////////////////////////////////////////////
class DefaultChiHandlerImpl : public InterceptionHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;
};

class DefaultPengHandlerImpl : public InterceptionHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;
};

class DefaultGangHandlerImpl : public InterceptionHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;
};

class DefaultCollectionHandlerImpl : public InterceptionHandlerImpl
{
protected:
	CardList m_Collection;

public:

	DefaultCollectionHandlerImpl(const CardList &collection);

	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cardList,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;
};


class DefaultHuHandlerImpl : public InterceptionHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;
};

class DefaultTingHandlerImpl : public InterceptionHandlerImpl
{
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;

	virtual bool CardToWin(GameContext *context, CardStatistic &stat, const CardStat &revealedCards, size_t &tryIdx, Card::Type &card, int &remaining) const;

	virtual CardStat GetRevealedCards(GameContext *context, PlayerPos::Type playerPos) const;

	bool IsReadyHand(GameContext *context, PlayerPos::Type playerPos, CardStatistic &stat) const;

	CardList AllCardsToWin(GameContext *context, PlayerPos::Type playerPos, CardStatistic &stat) const;
};

class InterceptionHandlerListImpl : public InterceptionHandlerImpl
{
	std::map<Interception::Type, InterceptionHandler> m_Handlers;
public:
	InterceptionList ToDo(GameContext *context, PlayerPos::Type player, PlayerPos::Type srcPlayer, 
		const CardList &cards,
		const InterceptionList &interceptions,
		Interception::Type type, Card::Type card) const;

	void Add(Interception::Type type, const InterceptionHandler &handler);

	void Remove(Interception::Type type);

	InterceptionHandler Get(Interception::Type type) const;
};

typedef RefCntObj<InterceptionHandlerListImpl> InterceptionHandlerList;

InterceptionHandlerList GetDefaultInterceptionHandlers();

#endif
