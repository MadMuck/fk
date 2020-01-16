#ifndef EXTENSION_INVALID_INTERCEPTION_H
#define EXTENSION_INVALID_INTERCEPTION_H

#include "GameContext.h"
#include "Interception.h"
#include <map>
#include <set>

class ExtensionInvalidInterception : public GameContextExtension
{
	std::map<PlayerPos::Type, std::set<int> > m_InvalidInterception;
public:

	void OnGameStart()
	{
		m_InvalidInterception.clear();
	}

	void Add(PlayerPos::Type playerPos, int idx)
	{
		m_InvalidInterception[playerPos].insert(idx);
	}

	bool Contains(PlayerPos::Type playerPos, int idx) const
	{
		bool ret = false;
		std::map<PlayerPos::Type, std::set<int> >::const_iterator itr = m_InvalidInterception.find(playerPos);
		if(itr != m_InvalidInterception.end())
		{
			ret = itr->second.find(idx) != itr->second.end();
		}
		return ret;
	}
};

#endif