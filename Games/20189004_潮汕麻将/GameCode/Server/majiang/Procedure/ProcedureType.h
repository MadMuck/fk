/*
游戏过程类型定义
*/

#ifndef PROCEDURE_TYPE_H
#define PROCEDURE_TYPE_H

namespace ProcedureType
{
    enum Type
    {
        Idle,
        GameStart,
        DistributeCard,
        NormalDrawCard,
		AfterDrawCard,
		DrawCardAfterKong,
        InterceptCard,
		DetermineIntercept,
		AfterIntercept,
        WaitSendCard,
		ReadyHandDiscard,
        Liquidation,
        GameFinish
    };
}

#endif
