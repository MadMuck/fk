#include "ExtensionActionValidator.h"
#include "ActionValidatorCreator.h"
#include "ExtensionLogger.h"


void InterceptionCheckType::Add(Interception::Type type)
{
	bool found = false;
	for(iterator itr = begin(); itr != end(); ++itr)
	{
		if(*itr == type)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		push_back(type);
	}
}


ExtensionActionValidator::ExtensionActionValidator():m_InterceptionValidator(new InterceptionValidatorImpl()),
	m_InterceptionHandlers(GetDefaultInterceptionHandlers())
{
}



bool ExtensionActionValidator::IsInterceptionValid(PlayerPos::Type player, Interception::Type type, const CardInfo &card) const
{
	return m_InterceptionValidator->IsValid(m_Context, player, type, card);

}

void ExtensionActionValidator::SetInterceptionValidator(const InterceptionValidator &validator)//在哪里被调用？
{
	m_InterceptionValidator = validator;
}

InterceptionList ExtensionActionValidator::HandlerInterception(PlayerPos::Type player, PlayerPos::Type srcPlayer, 
	const CardList &cards,
	const InterceptionList &interceptions,
	Interception::Type type, Card::Type card)
{
	return m_InterceptionHandlers->ToDo(m_Context, player, srcPlayer, cards, interceptions, type, card);
}

void ExtensionActionValidator::SetInterceptionHandler(Interception::Type type, const InterceptionHandler &handler)
{
	m_InterceptionHandlers->Add(type, handler);
}

InterceptionHandler ExtensionActionValidator::GetInterceptionHandler(Interception::Type type) const
{
	return m_InterceptionHandlers->Get(type);
}

InterceptionCheckType ExtensionActionValidator::GetCheckType(int type) const
{
	std::map<int, InterceptionCheckType>::const_iterator itr = m_CheckTypes.find(type);
	if(itr != m_CheckTypes.end()) return itr->second;
	
	LOGGER_FILE(m_Context, "Trying to get interception check list with type "<<type<<", but nothing found");
	return InterceptionCheckType();
}

void ExtensionActionValidator::SetCheckType(int type, const InterceptionCheckType &checkType)
{
	m_CheckTypes[type] = checkType;
}

void ExtensionActionValidator::OnGameStart()
{
	m_InterceptionValidator = InterceptionValidator(new InterceptionValidatorImpl());
	m_InterceptionHandlers = GetDefaultInterceptionHandlers();
}
