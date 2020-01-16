#ifndef QUESTION_H
#define QUESTION_H

#include "PlayerPos.h"
#include "RefCntObj.h"
#include "GameContext.h"

class QuestionImpl
{
public:
    virtual ~QuestionImpl(){}

    virtual void OnSend(GameContext *context) const = 0;

    virtual bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const = 0;

    virtual bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx) = 0;

	virtual bool IsInterception();
};

// 询问
class Question
{
    RefCntObj<QuestionImpl> m_Impl;
public:
    Question(QuestionImpl *impl);

    // 通知发送
    void OnSend(GameContext *context) const;

    // 是否要处理某个玩家的反馈
    bool IsInterestedIn(PlayerPos::Type playerPos, int tag) const;

    // 处理玩家的反馈
    // @return true:输入有效。false:输入无效，需要新输入。
    bool OnResponse(PlayerPos::Type playerPos, int tag, int selectionIdx);

	bool IsInterception();
};

#endif
