#ifndef ADAPTER_UTIL_H
#define ADAPTER_UTIL_H

#include "UpgradeMessageEx.h"
#include "Card.h"
#include "CardBlocks.h"
#include "ExtensionPlayer.h"
#include "Interception.h"
#include "PlayerStatusDef.h"
#include "ExtensionGameRunningData.h"
#include "ExtensionLiquidation.h"
#include "WinType.h"
#include <set>

typedef std::set<PlayerPos::Type> PlayerPosList;

unsigned char *CopyCards(unsigned char buf[], int bufSize, const CardList &cards, bool invisible);

unsigned char *CopyCards(unsigned char buf[], int bufSize, const std::vector<TwoCards> &cards, bool invisible);

unsigned char *CopyAllCardsInHand(unsigned char *buf, int rows, int bufSize, const PlayerList &players, const PlayerPosList &visiblePos);

unsigned char *CopyCardBlocks(unsigned char *buf, int rows, int bufSize, const CardBlocks &cardBlocks, bool invisible = true);

int GetCpgType(Interception::Type type);

Interception::Type GetInterceptionType(int type);

PlayerPosList ShownCardPlayerPosList(ExtensionPlayer *extPlayer);

void CopyInterceptionData(GCPStructEx *out, int len, PlayerPos::Type playerPos, const InterceptionList &interceptions);

int GetActionFlags(PlayerPos::Type playerPos, const InterceptionList &interceptions);

void FillInterceptionData(PlayerPos::Type playerPos,  const InterceptionList &interceptions, tagCPGNotify &out);

void FillWinningInfo(tagHuPaiEx &info, GameContext *context, PlayerPos::Type playerPos);

WinTypeList TranslateWinTypeList(const WinTypeList &innerWinTypes);

#endif