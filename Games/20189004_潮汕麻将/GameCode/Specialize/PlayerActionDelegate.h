#ifndef PLAYER_ACTION_DELEGATE_H
#define PLAYER_ACTION_DELEGATE_H

#include "GameContext.h"
#include "ExtensionTimer.h"
#include "ExtensionPlayer.h"
#include <map>

typedef void (*PlayerActionDelegateCallback)(GameContext *context, int type, PlayerPos::Type playerPos, void *data);

struct DelegateInfo
{
	PlayerPos::Type playerPos;
	int actionType;
	TimerData data;
	PlayerActionDelegateCallback callback;
	void *otherData;
};

// �йܿ���/�رյļ�����
class PlayerActionDelegateEnabledListener
{
public:
	virtual ~PlayerActionDelegateEnabledListener(){}

	virtual bool OnEnable(GameContext *context, PlayerPos::Type playerPos, bool b) = 0;
};


class ExtensionPlayerActionDelegate : public GameContextExtension
{
	typedef std::map<int, DelegateInfo*> ActionDelegateInfo;
	typedef std::map<PlayerPos::Type, ActionDelegateInfo> PlayerActionDelegateInfo;
	typedef std::map<PlayerPos::Type, RefCntObj<PlayerActionDelegateEnabledListener> > ListenerList;

	PlayerActionDelegateInfo m_DelegateInfo;

	void EraseDelegateInfo(DelegateInfo *info);

	ListenerList m_EnabledListeners;

public:

	// �趨��ҳ�ʱ��Ϊ����
	void AddAction(PlayerPos::Type playerPos, int type, float time, PlayerActionDelegateCallback callback, void *data = NULL, bool replace = false);

	// ֪ͨ�����Ϊ���������������Ӧ��
	void OnPlayerResponse(PlayerPos::Type playerPos, int type);

	// ɾ����ҳ�ʱ��Ϊ����
	void RemoveAction(PlayerPos::Type playerPos, int type);

	void RemoveAll();

	void AddListener(PlayerPos::Type playerPos, const RefCntObj<PlayerActionDelegateEnabledListener> &listener);

	void RemoveListener(PlayerPos::Type playerPos);

	void OnEnableActionDelegate(PlayerPos::Type playerPos, bool b);

	void OnGameStart();

	void OnGameFinish();

	void OnGameReset();
};

#endif