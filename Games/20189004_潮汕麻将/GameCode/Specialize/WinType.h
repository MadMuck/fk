#ifndef RQMJ_WIN_TYPE_H
#define RQMJ_WIN_TYPE_H

enum WIN_TYPE//胡牌类型枚举结构
{
	WIN_TYPE_JI_HU						,		
	WIN_TYPE_DUI_DUI_HU			,		//对对胡 2倍
	WIN_TYPE_QING_YI_SE			,		//清一色 3倍
	WIN_TYPE_HUN_YI_SE				,		//混一色 2倍
	WIN_TYPE_YAO_JIU					,		//幺九 2倍
	WIN_TYPE_SHI_SAN_YAO		,		//十三幺 10倍

	WIN_TYPE_QIANG_GANG		,		//抢杠 2倍
	WIN_TYPE_QIXIAODUI				,		//七小对 3倍
	WIN_TYPE_SUPER_QIDUI			,		//豪华七对 4倍
	WIN_TYPE_DOUBLE_S_QIDUI	,		//双豪华七对 6倍
	WIN_TYPE_TRIPLE_S_QIDUI		,		//三豪华七对 8倍

	WIN_TYPE_18LUOHAN			,		//18罗汉 10倍
	WIN_TYPE_XIAO_SIXI				,		//小四喜 4倍
	WIN_TYPE_DA_SIXI					,		//大四喜 6倍
	WIN_TYPE_XIAO_SANYUAN		,		//小三元 4倍
	WIN_TYPE_DA_SANYUAN		,		//大三元 6倍

	WIN_TYPE_GANG_BAO			,		//杠爆--杠上开花 2倍
	WIN_TYPE_TIAN_HU					,		//天胡 10倍
	WIN_TYPE_DI_HU						,		//地胡 10倍
	WIN_TYPE_HAIDILAO				,		//海底捞 2倍
};

#endif