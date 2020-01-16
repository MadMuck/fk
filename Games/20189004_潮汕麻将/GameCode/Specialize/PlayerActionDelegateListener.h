#ifndef PLAYER_ACTION_DELEGATE_LISTENER_H
#define PLAYER_ACTION_DELEGATE_LISTENER_H

#include "PlayerActionDelegate.h"

// ����й���Ϊ������, ������Ҵ�/�ر��йܵ����

template<typename T>
class PlayerActionDelegateListener : public PlayerActionDelegateEnabledListener
{

	T *m_ActionHandler;
	PlayerActionDelegateCallback m_Callback;
	int m_Type;
	float m_Delay;

public:
	PlayerActionDelegateListener(T *handler, PlayerActionDelegateCallback callback, float delay, int type):
	  m_ActionHandler(handler), m_Callback(callback), m_Delay(delay), m_Type(type)
	{}

	bool OnEnable(GameContext *context, PlayerPos::Type playerPos, bool b)
	{
		if(b)
		{
			// ���й��ˣ�����һ������ִ�е���Ϊ����
			AddActionDelegate(context, playerPos, m_Callback, 1, m_Type, m_ActionHandler,true);
		}
		else
		{
			// �ر��й��ˣ�����һ���ӳ�ִ�е���Ϊ����
			AddActionDelegate(context, playerPos, m_Callback, m_Delay, m_Type, m_ActionHandler,true);
		}

		// ����false, �Զ�ɾ��
		return false;
	}

	static void AddActionDelegate(GameContext *context, PlayerPos::Type playerPos, PlayerActionDelegateCallback callback, 
		float delay, int type, T *handler, bool replace = false)
	{
		// ���ó�ʱ�Զ���������ѡ��
		OBJ_GET_EXT(context, ExtensionPlayerActionDelegate, extPlayerActDelegate);
		extPlayerActDelegate->AddAction(playerPos, type, 
			delay,
			callback, handler, replace);
	}

	static RefCntObj<PlayerActionDelegateEnabledListener> Create(T *handler, PlayerActionDelegateCallback callback, float delay, int type)
	{
		return RefCntObj<PlayerActionDelegateEnabledListener>(new PlayerActionDelegateListener(handler, callback, delay, type));
	}
};

#endif