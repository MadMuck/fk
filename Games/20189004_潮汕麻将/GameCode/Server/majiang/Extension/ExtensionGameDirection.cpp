#include "ExtensionGameDirection.h"
#include "GameDirectionCreator.h"
#include "Util.h"

void ExtensionGameDirection::Reset()
{
	m_GameDirection->SetPlayerCount(m_Context->GetPlayerCount());
	m_Dices1[0] = 1;
	m_Dices1[1] = 1;
	m_StartCardBlockPos1 = 0;
	m_Dices2[0] = 1;
	m_Dices2[1] = 1;
	m_StartCardBlockPos2 = 0;
}

ExtensionGameDirection::ExtensionGameDirection():m_GameDirection(new GameDirection())
{
}

void ExtensionGameDirection::SetGameDirection(const RefCntObj<GameDirection> &dir)
{
	m_GameDirection = dir;
}

bool ExtensionGameDirection::IsPreceding(PlayerPos::Type playerPos1, PlayerPos::Type playerPos2, PlayerPos::Type origin) const
{
	return m_GameDirection->IsPreceding(playerPos1, playerPos2, origin);
}

const int *ExtensionGameDirection::GetDices1() const
{
	return m_Dices1;
}

int ExtensionGameDirection::GenerateStartCardBlockPos1(PlayerPos::Type bankerPos, int playerCnt, int diceA, int diceB)
{
	m_Dices1[0] = diceA > 0 ? diceA : (Random() % 6 + 1);
	m_Dices1[1] = diceB > 0 ? diceB : (Random() % 6 + 1);

	int offset = (m_Dices1[0] + m_Dices1[1]) - 1;
	// 逆时针数
	//m_StartCardBlockPos1 = (bankerPos + playerCnt - (offset % playerCnt)) % playerCnt;

	// 顺时针数
	m_StartCardBlockPos1 = (bankerPos + playerCnt + offset) % playerCnt;

	return GetStartCardBlockPos1();
}

int ExtensionGameDirection::GetStartCardBlockPos1() const
{
	return m_StartCardBlockPos1;
}

const int *ExtensionGameDirection::GetDices2() const
{
	return m_Dices2;
}

int ExtensionGameDirection::GenerateStartCardBlockPos2()
{
	m_Dices2[0] = Random() % 6 + 1;
	m_Dices2[1] = Random() % 6 + 1;

	m_StartCardBlockPos2 = (m_Dices2[0] + m_Dices2[1]);
	return GetStartCardBlockPos2();
}

int ExtensionGameDirection::GetStartCardBlockPos2() const
{
	return m_StartCardBlockPos2;
}

int ExtensionGameDirection::GetStartDrawPos() const
{
	return std::min(m_Dices1[0], m_Dices1[1]);
}

PlayerPos::Type ExtensionGameDirection::GetNextPos(PlayerPos::Type activePlayerPos) const
{
	return m_GameDirection->GetNextPos(activePlayerPos);
}

void ExtensionGameDirection::OnGameStart()
{
	Reset();
}

void ExtensionGameDirection::OnCreate()
{
	Reset();
}


