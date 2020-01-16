#ifndef EXTENSION_GAME_DIRECTION_H
#define EXTENSION_GAME_DIRECTION_H

#include "GameContext.h"
#include "GameDirection.h"
#include "RefCntObj.h"

class ExtensionGameDirection : public GameContextExtension
{
protected:
    RefCntObj<GameDirection> m_GameDirection;

	int m_Dices1[2];
	int m_StartCardBlockPos1;

	int m_Dices2[2];
	int m_StartCardBlockPos2;

	void Reset();

public:
    ExtensionGameDirection();

	void SetGameDirection(const RefCntObj<GameDirection> &dir);

	PlayerPos::Type GetNextPos(PlayerPos::Type activePlayerPos) const;

	bool IsPreceding(PlayerPos::Type playerPos1, PlayerPos::Type playerPos2, PlayerPos::Type origin) const;

	const int *GetDices1() const;
	int GenerateStartCardBlockPos1(PlayerPos::Type bankerPos, int playerCnt, int diceA = 0, int diceB = 0);
	int GetStartCardBlockPos1() const;

	const int *GetDices2() const;
	int GenerateStartCardBlockPos2();
	int GetStartCardBlockPos2() const;

	int GetStartDrawPos() const;

	void OnGameStart();
	void OnCreate();
};

#endif
