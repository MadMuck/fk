#include "PatternJokerList.h"

PatternJokerList::PatternJokerList(PatternJokerListImpl *impl):m_Impl(impl){}

const CardList &PatternJokerList::GetJokers() const
{
    return m_Impl->GetJokers();
}

void PatternJokerList::AddJoker(Card::Type card)
{
	m_Impl->AddJoker(card);
}

void PatternJokerList::Clear()
{
	m_Impl->Clear();
}

int PatternJokerList::JokersLimitCount() const
{
    return m_Impl->JokersLimitCount();
}

void PatternJokerList::SetJokersLimitCount(int limit)
{
	m_Impl->SetJokersLimitCount(limit);
}
