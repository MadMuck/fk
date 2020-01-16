#include "ExtensionMessageDelegate.h"

void ExtensionMessageDelegate::SetDelegate(const std::string &name, const MessageDelegate &delegate)
{
	m_Delegates.insert(std::make_pair(name, delegate));
}

MessageDelegate ExtensionMessageDelegate::GetDelegate(const std::string &name)
{
	MessageDelegate ret(NULL);
	std::map<std::string, MessageDelegate>::iterator itr = m_Delegates.find(name);
	if(itr != m_Delegates.end())
	{
		ret = itr->second;
	}
	return ret;
}