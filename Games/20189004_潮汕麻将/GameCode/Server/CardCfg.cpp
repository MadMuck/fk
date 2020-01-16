
#include <map>
#include <string>
#include "UpgradeMessageEx.h"
#include "../Server/ServerManage.h"

// ����Ѿ����˶����ſ�
std::map<long int, int> UserDrawnCardCount;

unsigned char GetCardValue(const CString &cardListStr, int cnt, unsigned char oriCard)
{
	unsigned char ret = oriCard;
	if(cardListStr.GetLength() > 0)
	{
		int foundPos = 0;
		int lastPos = 0;
		int i = 0;
		while(i <= cnt && foundPos >= 0)
		{
			++i;
			foundPos = cardListStr.Find(',', foundPos);
			if(i <= cnt && foundPos >= 0)
			{
				foundPos += 1;
				lastPos = foundPos;
			}
		}

		if(i > cnt)
		{
			// ���ڵ�i����
			if(foundPos < 0)
			{
				// ��i���������һ��(û���Զ��Ž�����)
				foundPos = cardListStr.GetLength();
			}
			ret = _ttoi(cardListStr.Mid(lastPos, foundPos - lastPos));
		}
	}
	return ret;
}

// �޸�ָ����ҵ���
unsigned char GetSpecifiedCard(long int userID, unsigned char oriCard, bool reset, bool enabled)
{
	if(!enabled) return oriCard;

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s + SKIN_FOLDER  + _T("_s.ini"));

	CString key;
	key = TEXT("SuperSet");
	char nameBuf[256];
	sprintf(nameBuf, "Card_%d", userID);
	CString cardListStr = f.GetKeyVal(key,nameBuf,"");

	unsigned char ret = oriCard;
	if(cardListStr.GetLength() > 0)
	{
		if(reset)
		{
			UserDrawnCardCount[userID] = 0;
		}
		int cnt = UserDrawnCardCount[userID];
		UserDrawnCardCount[userID] = cnt + 1;

		ret = GetCardValue(cardListStr, cnt, oriCard);
// 		if(ret != oriCard)
// 		{
// 			for(int i = 0; i < length; ++i)
// 			{
// 				if(toBeReplace[i] == ret)
// 				{
// 					toBeReplace[i] = oriCard;
// 					break;
// 				}
// 			}
// 		}
	}
	
	return ret;
}

// �޸���/����
unsigned char GetSpecifiedBirdCard(unsigned char oriCard, int cnt, bool enabled)
{
	if(!enabled) return oriCard;

	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s + SKIN_FOLDER  + _T("_s.ini"));

	CString key;
	key = TEXT("SuperSet");
	CString cardListStr = f.GetKeyVal(key,"Bird","");
	return GetCardValue(cardListStr, cnt, oriCard);
}

unsigned char GetSpecifiedCard(char oriCard, int idx)
{
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f(s + SKIN_FOLDER  + _T("_s.ini"));

	CString key;
	key = TEXT("SuperSet");
	CString cardListStr = f.GetKeyVal(key,"SpecialCard","");
	return GetCardValue(cardListStr, idx, oriCard);
}
