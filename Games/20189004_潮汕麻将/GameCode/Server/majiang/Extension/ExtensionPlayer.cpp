#include "ExtensionPlayer.h"
#include "PlayerCreator.h"
#include "GameContext.h"


void PlayerStatus::SetFlag(int type, int flag)
{
	m_Flag[type] = flag;
}

int PlayerStatus::GetFlag(int type, int defaultValue) const
{
	int ret = defaultValue;
	if(m_Flag.find(type) != m_Flag.end())
	{
		ret = m_Flag.find(type)->second;
	}
	return ret;
}

void PlayerStatus::SetBool(int type, bool val)
{
	SetFlag(type, val ? 1 : 0);
}

bool PlayerStatus::GetBool(int type, bool defaulVal) const
{
	return GetFlag(type, defaulVal) > 0;
}

void PlayerStatus::Reset()
{
	m_Flag.clear();
}


ExtensionPlayer::ExtensionPlayer():m_HandSortingDelegate(new HandSortingDelegate()){}

void ExtensionPlayer::Reset()
{
	for(PlayerList::iterator itr = m_Players.begin();
		itr != m_Players.end();
		++itr)
	{
		itr->second->Clear();
	}

	for(std::map<PlayerPos::Type, PlayerStatus>::iterator itr = m_PlayerStatus.begin();
		itr != m_PlayerStatus.end();
		++itr)
	{
		itr->second.Reset();
	}

	m_ActivePlayer = PlayerPos::Max;
	m_LastActivePlayer = PlayerPos::Max;
	m_GangBao.clear();
}

void ExtensionPlayer::OnCreate()
{
    for(int i = 0; i < m_Context->GetPlayerCount(); ++i)
    {
        PlayerPos::Type pos = PlayerPos::P0 + i;
        m_Players[pos] = PlayerCreator::Create(pos);
		m_PlayerStatus[i] = PlayerStatus();
    }

	SetDes(m_Context->GetPlayerCount(), 0);
}

void ExtensionPlayer::OnDestroy()
{
	for(int i = 0; i < m_Players.size(); ++i)
	{
		PlayerPos::Type pos = PlayerPos::P0 + i;
		if(m_Players[pos])
		{
			PlayerCreator::Destroy(m_Players[pos]);
		}
	}
	m_Players.clear();
}

void ExtensionPlayer::SetActivePlayerPos(PlayerPos::Type player)
{
	m_LastActivePlayer = m_ActivePlayer;
    m_ActivePlayer = player;
}

PlayerPos::Type ExtensionPlayer::GetActivePlayerPos() const
{
    return m_ActivePlayer;
}

PlayerPos::Type ExtensionPlayer::GetLastActivePlayerPos() const
{
	return m_LastActivePlayer;
}

Player *ExtensionPlayer::GetActivePlayer()
{
    return GetPlayer(m_ActivePlayer);
}

Player *ExtensionPlayer::GetPlayer(PlayerPos::Type pos)
{
	Player *ret = NULL;
	PlayerList::iterator itr = m_Players.find(pos);
	if(m_Players.end() != itr)
	{
		ret = itr->second;
	}
    return ret;
}

PlayerList &ExtensionPlayer::GetPlayerList()
{
    return m_Players;
}

PlayerStatus &ExtensionPlayer::GetPlayerStatus(PlayerPos::Type playerPos)
{
	return m_PlayerStatus.find(playerPos)->second;
}

void ExtensionPlayer::SetHandSortingDelegate( const RefCntObj<HandSortingDelegate> &delegate)
{
	m_HandSortingDelegate = delegate;
}

void ExtensionPlayer::SortHand(PlayerPos::Type playerPos)
{
	m_HandSortingDelegate->Sort(playerPos, GetPlayer(playerPos)->GetMutableCards());
}

void ExtensionPlayer::SetGangBaoPos(PlayerPos::Type playerPos, PlayerPos::Type srcPos)
{
	m_GangBao[playerPos] = srcPos;
}

PlayerPos::Type ExtensionPlayer::GetGangBaoPos(PlayerPos::Type playerPos)
{
	PlayerPos::Type srcPos = PlayerPos::Max;
	
	auto itr = m_GangBao.find(playerPos);

	if(itr != m_GangBao.end())
		srcPos = itr->second;

	return srcPos;
}

void ExtensionPlayer::SetDes(int playcount, PlayerPos::Type bankpos)
{
	switch (playcount)
	{
	case 2: 
		{
			m_PlayerDes[bankpos] = 0;
			m_PlayerDes[(bankpos + 1) % playcount] = 2;
			break;
		}
	case 3:
	case 4:
		{
			for (int i = 0; i < playcount; ++i)
			{
				//m_PlayerDes[(bankpos + i) % playcount] = i;
				m_PlayerDes[(bankpos + playcount - i) % playcount] = i;
			}
			break;
		}
	default:
		break;
	}
}

const unsigned char ExtensionPlayer::GetDes(PlayerPos::Type pos) const
{
	if(pos >= m_PlayerDes.size()) return 255;

	return m_PlayerDes.find(pos)->second;
}

void ExtensionPlayer::OnGameReset()
{
    Reset();
}

void ExtensionPlayer::OnGameStart()
{
	Reset();
}

void ExtensionPlayer::OnGameFinish()
{
	Reset();
}
