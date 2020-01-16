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

// 托管开启/关闭的监听器
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

	// 设定玩家超时行为代理
	void AddAction(PlayerPos::Type playerPos, int type, float time, PlayerActionDelegateCallback callback, void *data = NULL, bool replace = false);

	// 通知玩家行为代理，有玩家作出相应了
	void OnPlayerResponse(PlayerPos::Type playerPos, int type);

	// 删除玩家超时行为代理
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