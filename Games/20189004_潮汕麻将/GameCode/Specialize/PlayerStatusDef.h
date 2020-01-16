#ifndef PLAYER_STATUS_DEF_H
#define PLAYER_STATUS_DEF_H

enum PlayerStatusType
{
	PlayerStatus_ReadyHand,		// 听牌
	PlayerStatus_ShowCard,		// 明牌
	PlayerStatus_IsLeft,		// 掉线 离开
	PlayerStatus_ActionDelegate,// 托管
	PlayerStatus_TimeOutCount,	// 超时次数
};

#endif