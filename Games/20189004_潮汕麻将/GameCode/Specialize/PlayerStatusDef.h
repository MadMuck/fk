#ifndef PLAYER_STATUS_DEF_H
#define PLAYER_STATUS_DEF_H

enum PlayerStatusType
{
	PlayerStatus_ReadyHand,		// ����
	PlayerStatus_ShowCard,		// ����
	PlayerStatus_IsLeft,		// ���� �뿪
	PlayerStatus_ActionDelegate,// �й�
	PlayerStatus_TimeOutCount,	// ��ʱ����
};

#endif