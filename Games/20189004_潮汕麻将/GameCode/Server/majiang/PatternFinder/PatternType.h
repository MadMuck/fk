#ifndef PATTERN_TYPE_H
#define PATTERN_TYPE_H

enum PatternType
{
	PatternType_Invalid = 0,
	PatternType_Set,
	PatternType_Run,
	PatternType_Const,
	PatternType_IntcSet,
	PatternType_IntcRun,
	PatternType_JokerSet,
	PatternType_JokerRun,
	PatternType_JokerConst,
	PatternType_Pairs,
	PatternType_JokerPairs,
	PatternType_Combination,
	PatternType_NormalWin,
	PatternType_ThreeJoker,
	PatternType_ShiSanYao
};

#endif
