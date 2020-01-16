#ifndef EXTENSION_MESSAGE_DELEGATE_H
#define EXTENSION_MESSAGE_DELEGATE_H

#include "GameContext.h"
#include "MessageDelegate.h"
#include <string>
#include <map>

class ExtensionMessageDelegate : public GameContextExtension
{
	std::map<std::string, MessageDelegate> m_Delegates;
public:
	virtual void SetDelegate(const std::string &name, const MessageDelegate &delegate);

	virtual MessageDelegate GetDelegate(const std::string &name);
};

#endif