#include "ExtensionGamePlayerConnector.h"
#include "GamePlayerConnectorCreator.h"
ExtensionGamePlayerConnector::ExtensionGamePlayerConnector()
{
    m_GamePlayerConnector = GamePlayerConnectorCreator::Create();
}

ExtensionGamePlayerConnector::~ExtensionGamePlayerConnector()
{
    GamePlayerConnectorCreator::Destroy(m_GamePlayerConnector);
}

void ExtensionGamePlayerConnector::OnGameFinish()
{
    Clear();
}

GamePlayerConnector *ExtensionGamePlayerConnector::GetGamePlayerConnector()
{
    return m_GamePlayerConnector;
}

void ExtensionGamePlayerConnector::SendQuestion(const Question &question)
{
    m_GamePlayerConnector->SendQuestion(m_Context, question);
}

void ExtensionGamePlayerConnector::SendNotification(const Notification &notification)
{
    m_GamePlayerConnector->SendNotification(m_Context, notification);
}

void ExtensionGamePlayerConnector::ResendQuestion(PlayerPos::Type playerPos, int tag)
{
	m_GamePlayerConnector->ResendQuestion(m_Context, playerPos, tag);
}

void ExtensionGamePlayerConnector::ResendQuestion()
{
	m_GamePlayerConnector->ResendQuestion(m_Context);
}

bool ExtensionGamePlayerConnector::IsPlayerBlocking(PlayerPos::Type playerPos, int tag)
{
	return m_GamePlayerConnector->IsPlayerBlocking(playerPos,tag);
}

void ExtensionGamePlayerConnector::Clear()
{
    m_GamePlayerConnector->Clear();
}

void ExtensionGamePlayerConnector::OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx)
{
    m_GamePlayerConnector->OnResponse(playerPos, tag, selectionIdx);
}
