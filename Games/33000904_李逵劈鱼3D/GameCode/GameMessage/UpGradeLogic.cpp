#include"stdafx.h"
//#include "writelog.h"
#include "UpGradeLogic.h"
//���캯��
CUpGradeGameLogic::CUpGradeGameLogic(void)
{
	m_bSortCardStyle = 0; //0 �ư���С����;1 ����������
	m_iCondition=0;			//�޳�����������
	m_bKingCanReplace=false;
	m_iCardShapeRestrict1=0xFFFFFFFF;
	m_iCardShapeRestrict2=0xFFFFFFFF;
	m_iStation[4] = 500;
	for (int i = 0; i < 4; i++)
		m_iStation[i] = 100*i;
	m_iNTNum=0;
	//�����ƶ���
	::memset(m_iReplaceCardArray,0,sizeof(m_iReplaceCardArray));
	//�����ƶ�������
	m_iReplaceCardCount = 0;
}
//���캯��
CUpGradeGameLogic::~CUpGradeGameLogic()
{
}
//�O�ô��������
BOOL CUpGradeGameLogic::SetReplaceCard(BYTE iCardList[],int iCardCount)
{
	::CopyMemory(m_iReplaceCardArray,iCardList,iCardCount);
	m_iReplaceCardCount = iCardCount;
	return TRUE;
}

//��ȡ�����пɴ���������
int CUpGradeGameLogic::GetReplaceCardCount(BYTE iCardList[],int iCardCount)
{
	int iResultCount = 0;
	for(int i = 0;i < m_iReplaceCardCount;i ++)
	{	
		iResultCount += GetCountBySpecifyCard(iCardList,iCardCount,m_iReplaceCardArray[i]);
	}
	return iResultCount;
}

//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind(BYTE iCard, BOOL bTrueHua)
{ 
	int iHuaKind=(iCard&UG_HUA_MASK);
	if (!bTrueHua)
	{
		return iHuaKind=UG_NT_CARD;
	}
	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������	bBulkOrNum false:��ʾ�����ơ�true:��ʾ��������
int CUpGradeGameLogic::GetCardBulk(BYTE iCard,BOOL bSortOrCmp,bool bBulkOrNum)
{
	if ((iCard == 0x4E) || (iCard == 0x4F))
	{
		return bSortOrCmp ? (iCard-14) : (iCard-62); //��С��64+14-62=16	ֻ���ش�Сè��ֵ
	}

	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard, TRUE);

	if (false == bBulkOrNum && iCardNum == m_iNTNum) //2��
	{
		if(bSortOrCmp) 
		{
			return ((iHuaKind>>4)+(15*4));
		}
		else //û�й�����2��
		{
			return 15;
		}
	}

	return ((bSortOrCmp) ? ((iHuaKind>>4)+(iCardNum*4)) : (iCardNum));
}

//��ֵ�õ���
BYTE CUpGradeGameLogic::GetCardByValue(int Value)
{
	BYTE CardArray[55]={
		0x00,
		0x01,0x11, 0x21,0x31,
		0x02, 0x12 ,0x22 , 0x32 ,
		0x03,0x13, 0x23,0x33,
		0x04,0x14, 0x24, 0x34,
		0x05, 0x15,0x25, 0x35,
		0x06, 0x16, 0x26,0x36,
		0x07, 0x17,0x27, 0x37,
		0x08, 0x18, 0x28,  0x38,
		0x09, 0x19, 0x29,0x39,
		0x0A,  0x1A, 0x2A, 0x3A,
		0x0B,0x1B,  0x2B,0x3B,
		0x0C, 0x1C,0x2C, 0x3C,
		0x0D,  0x1D, 0x2D, 0x3D,
		0x4E, 0x4F};

		return CardArray[Value];
}


//���������ִӴ�С�����˿�
BOOL CUpGradeGameLogic::SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount,BOOL bSysSort)
{
	BOOL bSorted=TRUE,bTempUp;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i], true);
	}

	//�������(���Ӵ�С����)
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i]<iStationVol[i+1])
			{	
				//����λ��				//==ð������
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;

				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;

				if (bUp!=NULL)
				{
					bTempUp=bUp[i];
					bUp[i]=bUp[i+1];
					bUp[i+1]=bTempUp;
				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);

	//ϵͳ���в����ǻ�ɫ��������
	if(bSysSort)
	{
		ReverseCard(iCardList,bUp,iCardCount);
		return TRUE;
	}
	if(GetSortCardStyle() == 1) //����������
		SortCardByStyle(iCardList,iCardCount);

	if(GetSortCardStyle() == 2)
		SortCardByKind(iCardList,iCardCount);


	return TRUE;
}

BOOL CUpGradeGameLogic::ReverseCard(BYTE iCardList[], BYTE bUp[], int iCardCount)
{
	BYTE iTemp;
	for(int i=0;i< iCardCount /2 ;i++)
	{
		iTemp = iCardList[i];
		iCardList[i] = iCardList[iCardCount - 1 -i];
		iCardList[iCardCount - 1 -i] = iTemp;
	}
	return TRUE;
}

//����������
BOOL CUpGradeGameLogic::SortCardByStyle(BYTE iCardList[], int iCardCount)
{
	//�������������Ҫ�󰴴�С����
	if(m_bSortCardStyle == 0)
	{
		SortCard(iCardList, NULL, iCardCount);
		return TRUE;
	}

	//����Ĵ���==�������Ŵ�С
	int iStationVol[45];
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i]=GetCardBulk(iCardList[i],false);
	}

	int Start=0;
	int j,step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for(int i=8;i>1;i--)				//����������һ������i����ͬ��ֵ
	{
		for(j=Start;j<iCardCount;j++)
		{
			CardTemp[0]=iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0]=iStationVol[j];
			for(step=1;step<i&&j+step<iCardCount;)			//��һ������i��ֵ��ȵ�����(����������ʱ������)
			{
				if(iStationVol[j]==iStationVol[j+step])
				{
					CardTemp[step]=iCardList[j+step];			//������������
					CardTempVal[step]=iStationVol[j+step];		//��ֵ
					step++;
				}
				else
					break;
			}

			if(step>=i)	//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj,����λ��Ϊj+setp-1
			{			//����Start��ʼ��j���������setp��
				if(j!=Start) //�ų���ʼ��������
				{
					for(;j>=Start;j--) //��Start����j�ź��ƶ�i��
					{
						iCardList[j+i-1]=iCardList[j-1];
						iStationVol[j+i-1]=iStationVol[j-1];
					}
					for(int k=0;k<i;k++)				
					{
						iCardList[Start+k]=CardTemp[k];	//��Start��ʼ���ó�CardSave
						iStationVol[Start+k]=CardTempVal[k];
					}
				}
				Start=Start+i;
			}
			j=j+step-1;
		}
	}
	return TRUE;
}
//void WriteStr(TCHAR *str,int RoomID,int bDesk)
//{
//	CFile file;
//	TCHAR szFileName	[50];
//	TCHAR szNewLine[]="\r\n";
//	wsprintf(szFileName,"%d-%d.txt",RoomID,bDesk+1);
//	file.Open(szFileName,CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);//�������ļ�
//	file.SeekToEnd();
//	file.Write(szNewLine,strlen(szNewLine));			//����	
//	file.Write(str,strlen(str));						//д����Ϸ��Ϣ
//	file.Close();
//}
//����ɫ����
BOOL CUpGradeGameLogic::SortCardByKind(BYTE iCardList[],int iCardCount)
{
	BOOL bSorted=TRUE;
	int iTemp,iLast=iCardCount-1,iStationVol[45];

	//��ȡλ����ֵ
	for (int i=0;i<iCardCount;i++)
	{
		iStationVol[i] = GetCardBulk(iCardList[i], true) +m_iStation[GetCardHuaKind(iCardList[i],TRUE)>>4];
		//if (iStationVol[i]>=15) iStationVol[i]+=m_iStation[4];
		//else iStationVol[i]+=m_iStation[GetCardHuaKind(iCardList[i],FALSE)>>4];
	}

	//�������(���Ӵ�С����)
	do
	{
		bSorted=TRUE;
		for (int i=0;i<iLast;i++)
		{
			if (iStationVol[i] < iStationVol[i+1])
			{	
				//����λ��				//==ð������
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;

				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;

//				if (bUp!=NULL)
//				{
//					bTempUp=bUp[i];
//					bUp[i]=bUp[i+1];
//					bUp[i+1]=bTempUp;
//				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while(!bSorted);
	return TRUE;
}

//�����˿�,������ʹ��
BYTE CUpGradeGameLogic::RandCard(BYTE iCard[], int iCardCount,bool bHaveKing)
{
	static const BYTE m_CardArray[54] =
	{
		0x01, 0x02 ,0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, //���� 2 - A
		0x11, 0x12 ,0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, //÷�� 2 - A
		0x21, 0x22 ,0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, //���� 2 - A
		0x31, 0x32 ,0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, //���� 2 - A
		0x4E, 0x4F //С�����
	};

	BYTE iSend=0,iStation=0,iCardList[216],step=(bHaveKing?54:52);
	srand((unsigned)time(NULL));

	for (int i=0;i<iCardCount;i+=step)
		::CopyMemory(&iCardList[i],m_CardArray,sizeof(m_CardArray));

	do
	{
		iStation=rand()%(iCardCount-iSend);
		iCard[iSend]=iCardList[iStation];
		iSend++;
		iCardList[iStation]=iCardList[iCardCount-iSend];
	} while (iSend<iCardCount);

	return iCardCount;
}

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard(BYTE iRemoveCard[],   //Ҫɾ��������
								  int iRemoveCount,		//Ҫɾ����������
								  BYTE iCardList[],		//Ҫ���������
								  int iCardCount)		//�������������
{
	//��������
	if(iRemoveCount > iCardCount) return 0;
	if (iCardCount > 500) return 0; //ֻ֧��500���ڵ�����

	int iRecount;
	int iDeleteCount = 0; //��Ҫɾ����������

	BYTE BufCard[500]={0};

	for (int i = 0; i<iCardCount; i++)
	{
		BufCard[i] = iCardList[i];
	}
	
	for (int i = 0; i < iRemoveCount; i++)
	{
		for (int j = 0; j < iCardCount; j++)
		{
			if (iRemoveCard[i] == BufCard[j])
			{
				iDeleteCount++;
				BufCard[j] = 0;
				break;
			}
		}
	}
	///2011-5-6 û��ȫ���ҵ�Ҫɾ���ƾ���ɾ�����ɹ��������� zht xyh
	if (iDeleteCount != iRemoveCount)
		return 0;
	iRecount = RemoveNummCard(BufCard, iCardCount); //ɾ�����˱�ǵ���

	if (iDeleteCount!=iRecount)
		return 0;

	for (int i = 0; i<iCardCount; i++)
	{
		iCardList[i] = BufCard[i];
	}

	return iDeleteCount;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard(BYTE iCardList[], int iCardCount)
{
	int iRemoveCount=0;
	for (int i=0;i<iCardCount;i++)
	{
		if (iCardList[i]!=0)
			iCardList[i-iRemoveCount]=iCardList[i];
		else 
			iRemoveCount++;
	}

	return iRemoveCount;
}


//��������

//�Ƚϵ���
BOOL CUpGradeGameLogic::CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard)
{
	//��һ����ʾ�����������, �ڶ�����ʾҪ������
	return GetCardBulk(iFirstCard) < GetCardBulk(iNextCard);
}

//��������ķ���(5,10,K)
int CUpGradeGameLogic::FindPoint(BYTE iCardList[], int iCardCount)
{
	int iPoint = 0; //����
	for (int i=0; i<iCardCount; i++)
	{
		int iNum = GetCardNum(iCardList[i]); //�������
		switch(iNum)
		{
		case 5:
			iPoint += 5;
			break;
		case 10:
		case 13:
			iPoint += 10;
			break;
		}
	}
	return iPoint;
}

//�������Ƿ�����ͬ����
BOOL CUpGradeGameLogic::IsSameNumCard(BYTE iCardList[],int iCardCount)
{
	if(iCardCount <= 0 )
		return FALSE;
	//���ڴ����Ƶ���logic
	if(GetReplaceCardCount(iCardList,iCardCount))
		return IsSameNumCardAndKing(iCardList,iCardCount);
	
	BYTE bFistNum = GetCardNum(iCardList[0]);
	for(int i = 1; i < iCardCount; i++)
	{
		if(bFistNum != GetCardNum(iCardList[i]))
			return FALSE;
	}
	return TRUE;
}

//�Ƿ�Ϊͬ��
BOOL CUpGradeGameLogic::IsSameHuaKind(BYTE iCardList[], int iCardCount)
{
	if (iCardCount <= 0) return FALSE;

	//���ڴ����Ƶ���logic
	if(GetReplaceCardCount(iCardList,iCardCount))
		return IsSameHuaKindAndKing(iCardList,iCardCount);
	
	BYTE iFirstHua = GetCardHuaKind(iCardList[0], TRUE); //ȡ�õ�һ���ƵĻ�ɫ

	for (int i = 1; i < iCardCount; i++) //����Ķ��͵�һ�ŵĻ�ɫ��
	{
		if(GetCardHuaKind(iCardList[i], TRUE) != iFirstHua) 
		{
			return FALSE;
		}
	}
	return TRUE;
}

//�����û�����ը����
BYTE CUpGradeGameLogic::GetBombCount(BYTE iCardList[],int iCardCount,int iNumCount)
{
	int iCount = 0,
		temp[18] = {0};
	for(int i=0; i<iCardCount; i++)
	{
		temp[ GetCardBulk(iCardList[i])]++;
	}
	for(int i=0;i<16;i++)
	{
		if(temp[i] >= iNumCount)
			iCount++;
	}
	return iCount;	
}

//��ȡָ�������Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyNumCount(BYTE iCardList[],int iCardCount,int Num)
{
	BYTE temp[18] = {0};
	int count = 0;
	for(int i = 0;i < iCardCount;i ++)
		if(m_iReplaceCardArray[0] != iCardList[i])
		{
			temp[GetCardBulk(iCardList[i])]++;
		}

	for(int i = 0;i< 18;i++)
		if(temp[i] == Num)
			count++;
	return count;
}

//��ȡָ���Ƹ���
BYTE CUpGradeGameLogic::GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard)
{
	int count = 0;
	for(int i = 0;i < iCardCount;i ++)
		if(iCardList[i] == bCard) 
			count++;

	return count;
}
//��ȡָ���������ƴ�С(��Ҫ�����Ƿ��д�����)
BYTE CUpGradeGameLogic::GetBulkBySpecifyCardCount(BYTE iCardList[],int iCardCount,int iCount)
{
	//���ڴ����Ƶ���logic
	if(GetReplaceCardCount(iCardList,iCardCount))
		return GetBulkBySpecifyCardCountAndKing(iCardList,iCardCount,iCount);
	BYTE temp[18] = {0};
	for(int i = 0;i < iCardCount;i ++)
		temp[GetCardBulk(iCardList[i])]++;

	for(int i = 17;i> 0;i--)
		if(temp[i] == iCount)
			return i;

	return 0;
}
//���ô������߼�
BYTE CUpGradeGameLogic::GetBulkBySpecifyCardCountAndKing(BYTE iCardList[],int iCardCount,int iCount)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);
	//ɾ����
	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;

	if(kingcount <= 0 )
		return 0;

	BYTE temp[18]={0};
	for(int i = 0; i < iTmpCount; i ++)						//�ƶ���
	{
		temp[GetCardBulk(iTmp[i])]++;
	}
	//����ߴ�����(���ܹ������Сè)
	for(int i = 15; i > 0;i --)
	{
	if((temp[i] + kingcount) == iCount ||(temp[i] == iCount ))
			return i;
	}
	return 0;
}
//�Ƿ�Ϊ����˳��
BOOL CUpGradeGameLogic::IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	int iValue = iCardCount / iCount;
	if (iCardCount != iCount *iValue)						 //����������
		return FALSE;

	//���ڴ����Ƶ���logic
	if(GetReplaceCardCount(iCardList,iCardCount))
		return IsVariationSequenceAndKing(iCardList,iCardCount,iCount);

	BYTE temp[18]={0};
	for(int i = 0;i < iCardCount;i ++)						//�ƶ���
	{
		if(iCardList[i] == 0x4f || iCardList[i] == 0x4e)		//ȥ����Сè
			return FALSE;
		temp[GetCardNum(iCardList[i])]++;
	}
	int iFirstMax = 0, iSecondMax = 0,iThirdMax = 0,iMin = 999;//�ҳ���һ��,�ڶ���,���������,����С��

	for(int i=0;i<18;i++)	
	{
		if(temp[i] == 0 )
			continue;
		if(temp[i] !=0 && temp[i] != iCount)	//���Ҷ�˳��
			return false;
		//�������ֵ,��Сֵ
		if(iFirstMax == 0 || iFirstMax < i)
		{
			iThirdMax = iSecondMax;
			iSecondMax = iFirstMax;
			iFirstMax = i;
		}
		//��ǰ������ֵ
		if(iThirdMax == 0 )
			iThirdMax = i;
		//��ǰ�ڶ���ֵ
		if(iSecondMax == 0 )
			iSecondMax = i;
		//��ǰ��Сֵ
		if(iMin == 999 || iMin > i)
			iMin = i;
	}
	
	switch(iCount)
	{
	case 0:
		break;
	case 1://���ֵ�˳
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == 3 && iThirdMax != 13)	//������A��2��3����û��K�����
			return (iThirdMax  - 1) == (iCardCount - 1);
		
		//����23456���ֱ���
		if(iFirstMax == 15 && iMin == 3)//������2��3�����
			return (iSecondMax - 2)  == ( iCardCount - 1);

		//����2Ϊ��С��A�߼�
		if(iFirstMax == 14 && iMin == 2)
		{
			return (iSecondMax - 1) == (iCardCount - 1);
		}
		break;
	case 2://����˫˳
		//����AA22�����������
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == iSecondMax && iThirdMax == iSecondMax)	//������A��2��3����û��K�����
			return true;

		//����AA2233�����������
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == 3 && iThirdMax == iMin)	//������A��2��3����û��K�����
			return true;
		//2233���
		if(iFirstMax == 15 && iSecondMax == 3 )
			return true;
		//����223344
		if(iFirstMax == 15 && iSecondMax == 4 && iThirdMax == 3 )
			return true;
		
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == 3 && iThirdMax != 13)
			return (iThirdMax  - 1) == (iCardCount/2 - 1);
		
		//����23456���ֱ���
		if(iFirstMax == 15 && iMin == 3)//������2��3û��A����������
			return (iSecondMax - 2)  == ( iCardCount/2 - 1);

		//����2Ϊ��С��A�߼�
		if(iFirstMax == 14 && iMin == 2)
		{
			return (iSecondMax - 1) == (iCardCount/2 - 1);
		}

		break;
	case 3://������˳
		//����AAA222�����������
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == iSecondMax && iThirdMax == iSecondMax)	//������A��2��3����û��K�����
			return true;

		//����AAA222333�����������
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == 3 && iThirdMax == iMin)	//������A��2��3����û��K�����
			return true;
		//222333���
		if(iFirstMax == 15 && iSecondMax == 3 )
			return true;
		//����222333444
		if(iFirstMax == 15 && iSecondMax == 4 && iThirdMax == 3 )
			return true;
		
		if(iFirstMax == 15 && iSecondMax == 14 && iMin == 3 && iThirdMax != 13)
			return (iThirdMax  - 1) == (iCardCount/3 - 1);
		
		//����23456���ֱ���
		if(iFirstMax == 15 && iMin == 3)//������2��3û��A����������
			return (iSecondMax - 2)  == ( iCardCount/3 - 1);

		//����2Ϊ��С��A�߼�
		if(iFirstMax == 14 && iMin == 2)
			return (iSecondMax - 1) == (iCardCount/3 - 1);

		break;
	}
	return false;
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsSequence(BYTE iCardList[], int iCardCount, int iCount)
{
	if(GetReplaceCardCount(iCardList,iCardCount))
		return IsSequenceAndKing(iCardList,iCardCount,iCount);

	BYTE temp[18]={0};
	int iFirstMax = 0,iMin = 999;//�ҳ���һ��,�ڶ���,���������,����С��
	for(int i= 0;i < iCardCount;i++)
	{

		if(iCardList[i] == 0x4e || iCardList[i] == 0x4f)		//���ܹ�����
			return FALSE;
		temp[GetCardNum(iCardList[i])]++;
	}

	for(int i=0;i<18;i++)	
	{
		if(temp[i] == 0 )
			continue;
		if(temp[i] != iCount)	//���Ҷ�˳��
			return false;
		//�������ֵ,��Сֵ
		if(iFirstMax == 0 || iFirstMax < i)
		{
			iFirstMax = i;
		}
		//��ǰ��Сֵ
		if(iMin == 999 || iMin > i)
			iMin = i;
	}

	switch(iCount)
	{
	case 0:
		break;
	case 1://���ֵ�˳
			return iFirstMax - iMin == (iCardCount - 1);
		break;
	case 2://����˫˳
			return iFirstMax - iMin == (iCardCount/2 - 1);
		break;
	case 3://������˳
			return iFirstMax - iMin == (iCardCount/3 - 1);
		break;
	}
	
	return false;
}


/////�������߼���ʼ
//Ŀǰֻ�����Ӷ����������߼�

BOOL CUpGradeGameLogic::IsSameNumCardAndKing(BYTE iCardList[],int iCardCount)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);

	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;

	if(kingcount <= 0 )
		return FALSE;
	if(kingcount == iCardCount )									//ȫ���Ǵ�����
		return true;
	BYTE iFirstNum = GetCardNum(iTmp[0]);							//ȡ�õ�һ���Ƶ�����

	for (int i = 0; i < iTmpCount; i++)								//����Ķ��͵�һ������
	{
		if(GetCardNum(iTmp[i]) != iFirstNum || iTmp[i] == 0x4f || iTmp[i] == 0x4e) //ȥ��Сè
		{
			return FALSE;
		}
	}
	return TRUE;
}
//ͬ��
BOOL CUpGradeGameLogic::IsSameHuaKindAndKing(BYTE iCardList[],int iCardCount)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);

	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;

	if(kingcount <= 0 )
		return 0;
	BYTE iFirstHua = GetCardHuaKind(iTmp[0], TRUE); //ȡ�õ�һ���ƵĻ�ɫ

	for (int i = 0; i < iTmpCount; i++) //����Ķ��͵�һ�ŵĻ�ɫ��
	{
		if(GetCardHuaKind(iTmp[i], TRUE) != iFirstHua) 
		{
			return FALSE;
		}
	}
	
	return true;//IsSameHuaKind(iTmp,iTmpCount);
}

//�������߼�
BOOL CUpGradeGameLogic::IsThreeXAndKing(BYTE iCardList[],int iCardCount,int iX)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);

	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;

	if(kingcount <= 0 )
		return 0;
	//ÿ���Ƶĸ���
	BYTE temp[18]={0};
	for(int i= 0;i < iTmpCount;i++)
	{
		temp[GetCardBulk(iTmp[i])]++;
	}
	
	//��ǰ���������С��ȱ������(����,��,������ĸ���)
	//int min = 999,max = 0,lack=0,lack1 = 0,lack2 = 0,lack3 = 0 ;
	switch(kingcount)
	{
		case 0://�޴�����
			return false;
			break;
		case 1://һ�Ŵ�����
			switch(iX)
			{
			case 0://��������
				return IsSameNumCard(iCardList,iCardCount);
				break;
			case 1:/*//������(ֻҪ�ж������)
				return (iCardCount == 4 
					&&(1 == GetCountBySpecifyNumCount(iCardList,iCardCount,2) 
					||1 == GetCountBySpecifyNumCount(iCardList,iCardCount,3)));
				break;*/
			case 2:/*//��������
					return (iCardCount == 5 
						&&(1 == GetCountBySpecifyNumCount(iCardList,iCardCount,2) 
						|| 1 == GetCountBySpecifyNumCount(iCardList,iCardCount,3)));
				break;*/
			case 3://������(�Ӵ��������ٵö��Ի���һ������)
				return (iCardCount == 5 
					&&(2 == GetCountBySpecifyNumCount(iCardList,iCardCount,2) 
					|| 1 == GetCountBySpecifyNumCount(iCardList,iCardCount,3)));
				break;
			}
			break;
		case 2://���Ŵ�����
			switch(iX)
			{
			case 0://��������
				return IsSameNumCard(iCardList,iCardCount);
				break;
			case 1:/*//������(ֻҪ�ж������)
				return (iCardCount == 5 
					&&(GetCountBySpecifyNumCount(iCardList,iCardCount,1) 
					||  GetCountBySpecifyNumCount(iCardList,iCardCount,2)));
				break;*/
			case 2:/*//��������
					return (iCardCount == 5 
						&&(GetCountBySpecifyNumCount(iCardList,iCardCount,1) 
						||  GetCountBySpecifyNumCount(iCardList,iCardCount,2)
						||GetCountBySpecifyNumCount(iCardList,iCardCount,3)));
				break;*/
			case 3://������(�Ӵ��������ٵ�һ��)
				return (iCardCount == 5 
					&&(1 == GetCountBySpecifyNumCount(iCardList,iCardCount,2) 
					|| 1 == GetCountBySpecifyNumCount(iCardList,iCardCount,3)));
				break;
			}
			break;
	}
	return FALSE;
}

//�Ƿ�Ϊ����˳��
BOOL CUpGradeGameLogic::IsSequenceAndKing(BYTE iCardList[], int iCardCount, int iCount)
{
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);

	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;
	if(kingcount <= 0 )
		return 0;
	//ÿ���Ƶĸ���
	BYTE temp[18]={0};
	for(int i= 0;i < iTmpCount;i++)
	{
		if(iTmp[i] == 0x4e || iTmp[i] == 0x4f)		//���ܹ�����
			return FALSE;
		temp[GetCardNum(iTmp[i])]++;
	}
	
	//��ǰ���������С��ȱ������(����,��,������ĸ���)
	int min = 999,max = 0,lack=0,lack1 = 0,lack2 = 0,lack3 = 0 ;
	switch(iCount)
	{
	case 1://��˳
		for(int i = 2;i< 18;i++)
		{
			if(temp[i] == 0 )
				continue;
			if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
				return false;
			if(min == 999  || min > i)
				min = i;
			if(max < i || max == 0 )
				max = i;
		}
		//ȥ����С��
		if(max >= 15)
			return false;

		switch(kingcount)//�ɴ���������
		{
		case 0:
			return 0;
		case 1:
			if(max - min == 4//ȱ�м�һ��(��4568)
				||max - min == 3)//ȱ��ͷһ��
				return true;
			break;
		case 2:
			if(max - min == 4//ȱ�м����
				|| max - min == 3	//ȱ�м�һ�ű���һ��
				|| max - min == 2)	//ȱ���϶���
				return true;
			break;
		case 3:
			break;
		case 4:
			break;
		}
		break;
	case 2://˫˳
		for(int i = 0;i< 18;i++)
		{
			if(temp[i] == 0 )
				continue;

			if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
				return false;
			if(min == 999  || min > i)
				min = i;
			if(temp[i] != 0 ||  max == 0 )
				max = i;
			//������˳������,����ȱ�Ƹ���
			if(temp[i] != 2)
				lack += 2 - temp[i];
		}
		//ȱ�� > ������
		if(lack != kingcount)
			return false;

		//ȥ����С��
		if(max >= 15)
			return false;
		//�����Ƹ���
		switch(kingcount)
		{
		case 0:
			return 0;
		case 1:
			if(max - min == 2)//ȱһ��(��55667)
				return true;
			break;
		case 2:
			if(max - min == 2	//ȱ�м����
				|| max - min == 1)	//ȱ���϶���
				return true;
			break;
		case 3:
		case 4:
			break;
		}
		break;
	case 3://��˳
		for(int i = 0;i< 18;i++)
		{
			if(temp[i] == 0 )
				continue;

			if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
				return false;
			if(min == 999  || min > i)
				min = i;
			if(max < i || max == 0 )
				max = i;
			//������˳������,����ȱ�Ƹ���
			if(temp[i] != 3)
				lack += 3 - temp[i];
		}
		//ȱ�� > ������
		if(lack != kingcount)
			return false;

		//ȥ����С��
		if(max >= 15)
			return false;
		//�����Ƹ���
		switch(kingcount)
		{
		case 0:
			return 0;
		case 1:
			if(max - min == 1)//ȱһ��(��55667)
				return true;
			break;
		case 2:
			if(max - min == 1)	//ȱ����
				return true;
			break;
		case 3:
			break;
		case 4:
			break;
		}
		break;
	}
	return FALSE;
}

//���ִ���˳��
BOOL CUpGradeGameLogic::IsVariationSequenceAndKing(BYTE iCardList[],int iCardCount,int iCount)
{
	//ȥ�����пɴ�����
	//���浱ǰ�����пɵ��ƶ���
	BYTE iBuffer[54];
	//����һ��
	BYTE iTmp[54];
	int iTmpCount = iCardCount;
	::CopyMemory(iTmp,iCardList,sizeof(BYTE)*iCardCount);

	int kingcount = 0;// = GetReplaceCardCount(iCardList,iCardCount);
	TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	int iRev = RemoveCard(iBuffer,kingcount,iTmp,iCardCount);
	iTmpCount -= iRev;

	if(kingcount <= 0 )
		return 0;
	//ÿ���Ƶĸ���
	BYTE temp[18]={0};
	for(int i= 0;i < iTmpCount;i++)
	{
		if(iTmp[i] == 0x4f || iTmp[i] == 0x4e)		//ȥ����Сè
			return FALSE;
		temp[GetCardNum(iTmp[i])]++;
	}
	//��ǰ���������С��ȱ������(����,��,������ĸ���)���ӵڶ������б�
	int min = 999,max = 0,maxtwo = 0,lack=0,lack1 = 0,lack2 = 0,lack3 = 0 ;

	switch(iCount)
	{
		case 0:
			break;
		case 1://���ֵ�˳
			for(int i = 2;i< 18;i++)
			{
				if(temp[i] == 0 )
					continue;
				if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
					return false;
				if(min == 999 || min > i)
					min = i;
				if(max < i || max == 0 )
				{
					maxtwo = max;//�ڶ���
					max = i;
				}
				if(temp[i] != 0 && (maxtwo == 0))
				{
					maxtwo = i;
				}
			if(temp[i] != iCount)
				lack += iCount - temp[i];
			}
			//ȥ����С��
			if(max >= 15)
				return false;
			if(lack > kingcount)
				return false;
			if(max == 14  && maxtwo <= 5)	//��������A2345
			{
				min = 1;
				switch(kingcount)
				{
				case 0:
					break;
				case 1://һ�Ŵ�����
					return (maxtwo - min == 4 || maxtwo  - min == 3);	//�����м�һ�Ż������һ��5
					break;
				case 2:
					return (maxtwo - min == 4 || maxtwo - min == 2);	//�����м���Ż�����϶��ż�45
					break;
				}
			}
			break;
		case 2://���ֶ�˳
			for(int i = 2;i< 18;i++)
			{
				if(temp[i] == 0 )
					continue;
				if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
					return false;
				if(min == 999  || min > i)
					min = i;
				if(max < i || max == 0 )
				{
					maxtwo = max;//�ڶ���
					max = i;
				}
				if(maxtwo == 0)
				{
					maxtwo = i;
				}
			if(temp[i] != iCount)
				lack += iCount - temp[i];

			}
			//ȥ����С��
			if(max >= 15)
				return false;
			if(lack > kingcount)
				return false;
			if(max == 14  && maxtwo <= 5)	//��������A2345
			{
				min = 1;
				switch(kingcount)
				{
				case 0:
					break;
				case 1://һ�Ŵ�����
					return (maxtwo - min == 2);	//�����м�һ�Ż������һ��
					break;
				case 2:
					return (maxtwo - min == 2 || maxtwo - min == 1);	//�����м���Ż�����϶��ż�45
					break;
				}
			}

			break;
		case 3://������˳
			for(int i = 2;i< 18;i++)
			{
				if(temp[i] == 0 )
					continue;
				if(temp[i] > iCount)	//ȥ�����ƺ�,����Ҫһ��С�����
					return false;
				if(min == 999  || min > i)
					min = i;
				if(max < i || max == 0 )
				{
					maxtwo = max;//�ڶ���
					max = i;
				}
				if(maxtwo == 0)
				{
					maxtwo = i;
				}
				if(temp[i] != iCount)
					lack += iCount - temp[i];

			}
			//ȥ����С��
			if(max >= 15)
				return false;
			if(lack > kingcount)
				return false;

			if(max == 14  && maxtwo <= 5)	//��������A2345
			{
				min = 1;
				switch(kingcount)
				{
				case 0:
					break;
				case 1://һ�Ŵ�����
					return (maxtwo - min == 1);	//�����м�һ�Ż������һ��
					break;
				case 2:
					return (maxtwo - min == 1);	//�����м���Ż�����϶��ż�45
					break;
				}
			}
			break;
	}
	return FALSE;
}

/////�������߼�����

//��ȡָ���Ʒ����ҵ��Ƃ���
BYTE  CUpGradeGameLogic::TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard)
{
	iResultCardCount = 0;
	for(int i = 0;i < iCardCount;i++)
	{
		if(iCardList[i] == bCard)
			bCardBuffer[iResultCardCount ++] = iCardList[i];
	}
	return iResultCardCount;
}


//��ȡ1,2,3 or 4����ͬ���ֵ���
int CUpGradeGameLogic::TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount, 
													BYTE iDoubleBuffer[], BYTE bCardNum)
{
	memset(iDoubleBuffer,0,sizeof(iDoubleBuffer));
	int iCount = 0, temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=0; i<18; i++)
	{
		if(temp[i] == bCardNum) //����Ҫ���ҵ�����:one?double?three?four_bomb?
		{
			for(int j = 0; j < iCardCount; j++)
			{
				if(i == GetCardBulk(iCardList[j]))
					iDoubleBuffer[iCount++] = iCardList[j];
			}
		}
	}
	return iCount;
}

//��ȡָ����ɫ��
int CUpGradeGameLogic::TackOutByCardKind(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],BYTE iCardKind)
{
	int count =0;

	for(int i = 0;i < iCardCount; i++)
	{
		if( GetCardHuaKind(iCardList[i]) == iCardKind)
		{
			iDoubleBuffer[count++] = iCardList[i];
		}
	}
	return count;
}

//���(�������ƶ�Ĳ���ٵ�)
int CUpGradeGameLogic::TackOutMuchToFew(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],int &iBufferCardCount,BYTE iCardMuch,BYTE iCardFew)
{
	iBufferCardCount=0;
	int count =0;
	BYTE iBuffer[54];
	int iCount = TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,iCardMuch);
	if(iCount <=0)
		return count;
	for(int i = 0;i < iCount; i += iCardMuch)
	{
		::CopyMemory(&iDoubleBuffer[iBufferCardCount],&iBuffer[i],sizeof(BYTE)*iCardFew);
		iBufferCardCount += iCardFew;
		count++;
	}
	return count;
}

//��ȡĳ��ָ����С����
BOOL CUpGradeGameLogic::TackOutCardBySpecifyCardNum(BYTE iCardList[],int iCardCount,BYTE iBuffer[],int &iBufferCardCount,BYTE iCard)
{
	iBufferCardCount = 0;
	BYTE iCardNum = GetCardBulk(iCard); //�õ��������
	for(int i = 0; i < iCardCount; i++)
	{
		if(GetCardBulk(iCardList[i]) == iCardNum) //����Ҫ���ҵ��Ƶ�����
		{
			iBuffer[iBufferCardCount++] = iCardList[i];
		}
	}

	return iBufferCardCount;
}

//���Ҵ���iCard�ĵ�������iCardList�е����
BYTE  CUpGradeGameLogic::GetSerialByMoreThanSpecifyCard(BYTE iCardList[], int iCardCount,
														int iCard, BYTE iBaseCardCount,
														bool bExtValue)
{
	BYTE MaxCard=0;
	BYTE Serial=0;
	BYTE MaxCardNum=255;
	int BaseCardNum = iCard;  //GetCardBulk(iCard);	//��ǰ�Ƚ�ֵ

	for(BYTE i=0; i<iCardCount; i+=iBaseCardCount)	
	{
		int temp = GetCardBulk(iCardList[i]);

		
		if(temp<MaxCardNum && temp>BaseCardNum)
		{
			MaxCardNum = temp;
			Serial = i; //�õ����		
			break;
		}
	}
	return Serial;
}


//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard,int temp)
{
	for(int i = iStart;i < iCardCount;i ++)
	{
		if (i == temp)
			continue;
		if(iCardList[i] == iCard)
			return i;
	}
	return -1;
}

//����˳��������
BYTE CUpGradeGameLogic::GetBulkBySpecifyVariationSequence(BYTE iCardList[],int iCardCount ,int iSequence )
{
	int iFirstMax = 0, iSecondMax = 0,iThirdMax = 0;//�ҳ���һ��,�ڶ���,���������,����С��
	BYTE temp[18]={0};
	for(int i = 0;i < iCardCount;i ++)						//�ƶ���
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i=17;i>0;i++)
	{
		if(temp[i] == iSequence)
		{
			iFirstMax=i;						//������2Ҳ������A
			for(int j=i-1;j>0;j--)
			{
				if(temp[j] == iSequence)//�ҵ��ڶ�����˳�ѭ��(�޵������)//������AҲ���Է�A
				{
					iSecondMax = j;
					for(int k=j-1;j>0;j--)
					{
						if(temp[k] == iSequence)//��ڵ�������˳�ѭ��	//���Ǵ���Ҳ���Բ�����
						{
							iThirdMax =k;
							break;
						}
					}
					break;
				}
			}
			break;
		}
	}

	if(iFirstMax == 15)	            //����2,�ٿ��Ƿ����A
	{
		if(iSecondMax == 14)		//����A
		{
			if(iThirdMax == 0)		//�����ڵ�����,Ҳֻ��A2������
				return 2;

			return iThirdMax;		//���� A2�����������AA2233
		}
		return iSecondMax;
	}
	return 0;
}

//��ȡָ��˳�����Ƶ���Сֵ(iSequence ����˳�ӵ��������Ϊ
BYTE  CUpGradeGameLogic::GetBulkBySpecifySequence(BYTE iCardList[],int iCardCount ,int iSequence )
{
	int temp[18] = {0};
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardNum(iCardList[i])]++;
	}

	for(int k = 0; k < 15; k++)
	{
		if(temp[k] == iSequence)
		{
			return k;
		}
	}
	return 0;
}

//������С (1) or ��� (255) �� ,bBulkOrNum false:��ʾ�����ơ�true:��ʾ��������
int  CUpGradeGameLogic::GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax/*1 or 255*/,bool bBulkOrNum)
{
	int CardNum = GetCardBulk(iCardList[0], false,bBulkOrNum);

	if(MinOrMax == 1) //����С��
	{
		for(int i = 1; i < iCardCount; i++)
		{
			if(GetCardBulk(iCardList[i], false,bBulkOrNum) < CardNum)
				CardNum = GetCardBulk(iCardList[i], false,bBulkOrNum);
		}
	}
	else if(MinOrMax == 255)
	{
		for(int i = 1; i < iCardCount; i++)
		{
			if(GetCardBulk(iCardList[i], false,bBulkOrNum) > CardNum)
				CardNum = GetCardBulk(iCardList[i], false,bBulkOrNum);
		}
	}


	//���ص��� GetCardBulk() �õ���ֵ
	return CardNum;
}

//��������������߻���С�ߵõ�������
BYTE CUpGradeGameLogic::GetMaxCard(BYTE iCardList[],int iCardCount)
{
	if(iCardCount <= 0)
		return 0;
	BYTE bCard  = iCardList[0];
	for(int i = 1; i < iCardCount ; i ++)
	{
		if (iCardList[i] == m_iReplaceCardArray[0])
			continue;
		if(GetCardBulk(bCard) < GetCardBulk(iCardList[i]) && iCardList[i] != m_iReplaceCardArray[0]//�ȵ�ǰ�ƴ��Ҳ��Ǵ�����
		||bCard == m_iReplaceCardArray[0])//��ǰ��Ϊ�����Ʊ㻻
		{
			bCard = iCardList[i];
		}
	}
	return bCard;
}
//�ж��ǲ��Ƕ๦���ƣ�רΪ��ʱ����
//����TURE ��ʾ�� ��False ��ʾ����
BOOL CUpGradeGameLogic::JudgeCard(BYTE iCard)
{

	if(iCard == m_iReplaceCardArray[0])
	{
		return TRUE;
	}
	return FALSE;
}
//������������С�ߵõ�������
BYTE CUpGradeGameLogic::GetMinCard(BYTE iCardList[],int iCardCount)
{
	if(iCardCount <= 0)
		return 0;
	BYTE bCard  = iCardList[0];
	for(int i = 1; i < iCardCount ; i ++)
	{
		if(GetCardBulk(bCard) > GetCardBulk(iCardList[i]))
			bCard = iCardList[i];
	}
	return bCard;
}
/////////////////////////////////////////////////////////////////////////
/**
* @info ��ȡ����
* @param iCardList[] ��
* @param iCardCount �Ƶ�����
* @param bExlVol ?
*/
BYTE CUpGradeGameLogic::GetCardShape(BYTE iCardList[], int iCardCount)
{
	if (IsOnlyOne(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<1))) return UG_ONLY_ONE; //����
	if (IsDouble(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<2))) return UG_DOUBLE;	 //����
	if (IsThree(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<8))) return UG_THREE;	 //����

	if (IsBomb(iCardList, iCardCount)&&(m_iCardShapeRestrict2&(0x01<<15))) return UG_BOMB; //4������ͬ���ƣ�ը��
	if (IsKingBomb(iCardList,iCardCount)&&(m_iCardShapeRestrict2&(0x01<<20))) return UG_KING_BOMB;//��ը
	

	if (IsThreeX(iCardList,iCardCount,3)&&(m_iCardShapeRestrict1&(0x01<<11)))	return UG_THREE_DOUBLE;	//������
	if (IsThreeX(iCardList, iCardCount, 2)&&(m_iCardShapeRestrict1&(0x01<<10))) return UG_THREE_TWO; //������
	if (IsThreeX(iCardList, iCardCount, 1)&&(m_iCardShapeRestrict1&(0x01<<9))) return UG_THREE_ONE; //����һ

	if  (IsFourX(iCardList,iCardCount,4)&&(m_iCardShapeRestrict1&(0x00000001<<27))) return UG_FOUR_TWO_DOUBLE;		//�Ĵ�����(Ҫ���Ƕ���)
	if  (IsFourX(iCardList,iCardCount,3)&&(m_iCardShapeRestrict1&(0x00000001<<26))) return UG_FOUR_ONE_DOUBLE;		//�Ĵ�һ��(Ҫ��ɶ�)
	if	(IsFourX(iCardList,iCardCount,2)&&(m_iCardShapeRestrict1&(0x00000001<<25))) return UG_FOUR_TWO;			//�Ĵ���(��Ҫ��ɶ�)
	if	(IsFourX(iCardList,iCardCount,1)&&(m_iCardShapeRestrict1&(0x00000001<<24))) return UG_FOUR_ONE;			//�Ĵ�һ

	if (IsFourXSequence(iCardList,iCardCount,4)&&(m_iCardShapeRestrict2&(0x01<<7))) return UG_FOUR_TWO_DOUBLE_SEQUENCE;	//��˳������
	if (IsFourXSequence(iCardList,iCardCount,3)&&(m_iCardShapeRestrict2&(0x01<<5))) return UG_FOUR_ONE_DOUBLE_SEQUENCE;	//��˳��һ��
	if (IsFourXSequence(iCardList,iCardCount,2)&&(m_iCardShapeRestrict2&(0x01<<3))) return UG_FOUR_TWO_SEQUENCE;	//��˳��������
	if (IsFourXSequence(iCardList,iCardCount,1)&&(m_iCardShapeRestrict2&(0x01<<1))) return UG_FOUR_ONE_SEQUENCE;	//��˳������
	if (IsFourXSequence(iCardList,iCardCount,0)&&(m_iCardShapeRestrict1&(0x01<<29))) return UG_FOUR_SEQUENCE;		//��˳

	if (IsVariationFourXSequence(iCardList,iCardCount,0)&&(m_iCardShapeRestrict1&(0x01<<28))) return UG_VARIATION_FOUR_SEQUENCE;	//������˳
	if (IsVariationFourXSequence(iCardList,iCardCount,1)&&(m_iCardShapeRestrict2&(0x01<<0))) return UG_VARIATION_FOUR_ONE_SEQUENCE;	//������˳������
	if (IsVariationFourXSequence(iCardList,iCardCount,2)&&(m_iCardShapeRestrict2&(0x01<<2))) return UG_VARIATION_FOUR_TWO_SEQUENCE;	//������˳��������
	if (IsVariationFourXSequence(iCardList,iCardCount,3)&&(m_iCardShapeRestrict2&(0x01<<4))) return UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE;	//������˳��һ��
	if (IsVariationFourXSequence(iCardList,iCardCount,4)&&(m_iCardShapeRestrict2&(0x01<<6))) return UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE;	//������˳������

	// ˳���а��� ͬ��˳,�������ж��Ƿ�ͬ��˳,������ǣ����ж��Ƿ���˳�ӣ������˳�ӣ�����һ���˳����
	if (IsStraightFlush(iCardList, iCardCount)&&(m_iCardShapeRestrict2&(0x01<<16))) return UG_STRAIGHT_FLUSH; //ͬ��˳
	if	(IsVariationStraightFlush(iCardList,iCardCount)&&(m_iCardShapeRestrict2&(0x01<<17))) return UG_VARIATION_STRAIGHT_FLUSH;				//����ͬ��˳
	
	if	(IsFlush(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<5))) return UG_FLUSH;					//ͬ��(��˳��)
	
	if  (IsStraight(iCardList, iCardCount)&&(m_iCardShapeRestrict1&(0x01<<4))) return UG_STRAIGHT;            //˳��	
	if	(IsVariationStraight(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<3))) return UG_VARIATION_STRAIGHT;				//���ֵ�˳

	if (IsDoubleSequence(iCardList, iCardCount)&&(m_iCardShapeRestrict1&(0x01<<13))) return UG_DOUBLE_SEQUENCE;  //����
	if	(IsVariationDoubleSequence(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<12))) return UG_VARIATION_DOUBLE_SEQUENCE;		//����˫˳

	if (IsThreeSequenceDoubleSequence(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<23))) return UG_THREE_SEQUENCE_DOUBLE_SEQUENCE;//����
	if (IsVariationThreeSequenceDoubleSequence(iCardList,iCardCount)&&(m_iCardShapeRestrict1&(0x01<<22))) return UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE;//���ֺ���

	if (IsThreeXSequence(iCardList, iCardCount, 3)&&(m_iCardShapeRestrict1&(0x01<<21))) return UG_THREE_DOUBLE_SEQUENCE; //����������
	if (IsThreeXSequence(iCardList, iCardCount, 2)&&(m_iCardShapeRestrict1&(0x01<<19))) return UG_THREE_TWO_SEQUENCE; //����������
	if (IsThreeXSequence(iCardList, iCardCount, 1)&&(m_iCardShapeRestrict1&(0x01<<17))) return UG_THREE_ONE_SEQUENCE; //��������һ
	if (IsThreeXSequence(iCardList, iCardCount, 0)&&(m_iCardShapeRestrict1&(0x01<<15))) return UG_THREE_SEQUENCE; //����

	if (IsVariationThreeXSequence(iCardList, iCardCount, 3)&&(m_iCardShapeRestrict1&(0x01<<20))) return UG_VARIATION_THREE_DOUBLE_SEQUENCE; //��������������
	if (IsVariationThreeXSequence(iCardList, iCardCount, 2)&&(m_iCardShapeRestrict1&(0x01<<18))) return UG_VARIATION_THREE_TWO_SEQUENCE; //��������������
	if (IsVariationThreeXSequence(iCardList, iCardCount, 1)&&(m_iCardShapeRestrict1&(0x01<<16))) return UG_VARIATION_THREE_ONE_SEQUENCE; //������������һ
	if (IsVariationThreeXSequence(iCardList, iCardCount, 0)&&(m_iCardShapeRestrict1&(0x01<<14))) return UG_VARIATION_THREE_SEQUENCE; //��������

	if (IsMaster510K(iCardList, iCardCount)&&(m_iCardShapeRestrict2&(0x01<<9))) return UG_MASTER_510K; //510Kͬ��ը��
	if (IsSlave510K(iCardList, iCardCount)&&(m_iCardShapeRestrict2&(0x01<<8))) return UG_SLAVE_510K;            //510Kը��

	return UG_ERROR_KIND;
}

//����
BOOL CUpGradeGameLogic::IsDouble(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 2) 
		return FALSE;
	return IsSameNumCard(iCardList, iCardCount);
}
//����
BOOL CUpGradeGameLogic::IsThree(BYTE iCardList[], int iCardCount)
{
	if (iCardCount != 3) 
		return FALSE;
	return IsSameNumCard(iCardList, iCardCount);
}
//3 �� 0,1or2,or3
BOOL CUpGradeGameLogic::IsThreeX(BYTE iCardList[], int iCardCount, int iX)
{
	if(iCardCount > 5 || iCardCount < 3) 
		return FALSE;

	//���ڴ����Ƶ���logic
	if(GetReplaceCardCount(iCardList,iCardCount))
		return IsThreeXAndKing(iCardList,iCardCount,iX);

	if(GetCountBySpecifyNumCount(iCardList, iCardCount,3) != 1)//�Ƿ��������
		return false;
	switch(iX)
	{
	case 0:	
		return iCardCount == 3;//IsSameNumCard(iCardList, iCardCount);//����
		break;
	case 1:
		return iCardCount == 4;//������
		break;
	case 2:
		return iCardCount == 5;//�����ţ����ԷǶ��ӣ�
		break;
	case 3:					//��һ��
		return GetCountBySpecifyNumCount(iCardList,iCardCount,2)==1;//�Ƿ���ڶ���
		break;
	default:
		break;
	}
	return false;
}

//�Ĵ�1or2
BOOL CUpGradeGameLogic::IsFourX(BYTE iCardList[],int iCardCount,int iX)
{
	if(iCardCount >8 || iCardCount < 4)
		return false;

	if(GetCountBySpecifyNumCount(iCardList,iCardCount,4) != 1)//�Ƿ����ĸ�����
		return false;

	switch(iX)
	{
	case 0:
		return iCardCount == 4;//����
	case 1:						
		return iCardCount == 5;//�Ĵ�1��
	case 2:
		return iCardCount ==6;//�Ĵ�2(��Ҫ��ɶ�)
	case 3:
		return (iCardCount == 6 && 1 == GetCountBySpecifyNumCount(iCardList,iCardCount,2));//Ҫ��ɶ�
	case 4:
		return (iCardCount == 8 && 2 == GetCountBySpecifyNumCount(iCardList,iCardCount,2));	//�Ĵ�2��
	}

	return FALSE;
}

//��ը
BOOL CUpGradeGameLogic::IsKingBomb(BYTE iCardList[],int iCardCount)			//�Ƿ�Ϊ��ը(ץ��������)
{
	if(iCardCount != KING_COUNT)
	{
		return false;
	}

	//����������
	for(int i=0;i<iCardCount;i++)
	{
		if(iCardList[i]!=0x4e && iCardList[i]!=0x4f)
		{
			return false;
		}
	}
	return true;
}

//4+���� ը��
BOOL CUpGradeGameLogic::IsBomb(BYTE iCardList[], int iCardCount)
{
	if(iCardCount < 4)
		return FALSE;

	return IsSameNumCard(iCardList, iCardCount); //�Ƿ�����ͬ����
}

//ͬ��ը��
BOOL CUpGradeGameLogic::IsBombSameHua(BYTE iCardList[],int iCardCount)
{
	if(!IsBomb(iCardList,iCardCount)) return false;
	if(!IsSameHuaKind(iCardList,iCardCount)) return false;
	return TRUE;
}

//ͬ��(��ͬ��)
BOOL CUpGradeGameLogic::IsFlush(BYTE iCardList[],int iCardCount)
{
	return IsSameHuaKind(iCardList, iCardCount);
}

//ͬ��˳ 5��ͬ��������
BOOL CUpGradeGameLogic::IsStraightFlush(BYTE iCardList[], int iCardCount)
{
	if(!IsSameHuaKind(iCardList, iCardCount)) return FALSE; //ͬ����

	if( !IsStraight(iCardList, iCardCount) ) return FALSE; //˳�ӣ�
	return TRUE;
}

//����ͬ��˳ 5��ͬ��������
BOOL CUpGradeGameLogic::IsVariationStraightFlush(BYTE iCardList[], int iCardCount)
{
	if(!IsSameHuaKind(iCardList, iCardCount)) return FALSE; //ͬ����

	if( !IsVariationStraight(iCardList, iCardCount) ) return FALSE; //˳�ӣ�
	return TRUE;
}
//�Ƿ��Ǳ���˳��(A2345)��23456
BOOL CUpGradeGameLogic::IsVariationStraight(BYTE iCardList[],int iCardCount)
{
	//ֻ�����又(������Ϸֻ������)
	if(iCardCount != 5 )
		return false;
	return IsVariationSequence(iCardList,iCardCount,1);
}

//�Ƿ���˳��ָ������
BOOL CUpGradeGameLogic::IsStraight(BYTE iCardList[],int iCardCount)
{
	//ֻ�����又(������Ϸֻ������)
	if(iCardCount != 5 )
		return false;
	return IsSequence(iCardList,iCardCount,1);
}

//�Ƿ��Ǳ�������AA22��2233��
BOOL CUpGradeGameLogic::IsVariationDoubleSequence(BYTE iCardList[],int iCardCount)
{
	//ֻ�����又(������Ϸֻ������)
	if(iCardCount%2 != 0 || iCardCount != 6)
		return false;

	return IsVariationSequence(iCardList,iCardCount,2);
}

//�Ƿ�������
BOOL CUpGradeGameLogic::IsDoubleSequence(BYTE iCardList[],int iCardCount)
{
	//ֻ����6��(������Ϸֻ������)
	if(iCardCount%2 != 0 || iCardCount != 6)
		return false;

	return IsSequence(iCardList,iCardCount,2);
}

//������˳����˳
BOOL CUpGradeGameLogic::IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount)
{
	if(iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[54],iBuffer2[54];
	BOOL bValue3 = false,bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0,TackOutCount2 = 0;

	TackOutCount3=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer3,3);//����
	TackOutCount2=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer2,2);//����

	if(TackOutCount3 <=0 || TackOutCount2 <=0 || TackOutCount3 + TackOutCount2 !=iCardCount || TackOutCount3/3 != TackOutCount2/2 )
		return FALSE;

	bValue3 =IsVariationSequence(iBuffer3,TackOutCount3,3);
	bValue2 =(IsVariationSequence(iBuffer2,TackOutCount2,2)||IsSequence(iBuffer2,TackOutCount2,2));
	return bValue3&&bValue2;
}

//��˳����˳
BOOL CUpGradeGameLogic::IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount)
{
	if(iCardCount < 10)		//��˳����2��˳Ҳ���ٶ�
		return false;

	BYTE iBuffer3[54],iBuffer2[54];
	BOOL bValue3 = false,bValue2 = false;	//��˳,��˳�Ƿ�Ϊ˳,
	int TackOutCount3 = 0,TackOutCount2 = 0;

	TackOutCount3=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer3,3);//����
	TackOutCount2=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer2,2);//����
	if(TackOutCount3 <=0 || TackOutCount2 <=0 || TackOutCount3 + TackOutCount2 !=iCardCount || TackOutCount3/3 != TackOutCount2/2 )
		return FALSE;
	bValue3 =IsSequence(iBuffer3,TackOutCount3,3);

	bValue2 =(IsVariationSequence(iBuffer2,TackOutCount2,2)||IsSequence(iBuffer2,TackOutCount2,2));

	return bValue3&&bValue2;
}


//����������x
BOOL CUpGradeGameLogic::IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX)
{
	//������Ϸֻ�޶�������)
	if(iCardCount%2 != 0 || iCardCount != 6)
		return false;

//	if(iCardCount < 6)		//��˳����2
//		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%3 != 0)		
			return false;
		return IsVariationSequence(iCardList,iCardCount,3);
		break;
	case 1://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 && TackOutCount/3*4 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,3);
		break;
	case 2://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,3);
	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,3);

		break;
	}
	return false;
}

//�������� 0,1 or 2
BOOL CUpGradeGameLogic::IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX)
{
	//(������Ϸֻ�޶�������)
	if(iCardCount%2 != 0 || iCardCount != 6)
		return false;

//	if(iCardCount < 6)		//��˳����2
//		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%3 != 0)		
			return false;
		return IsSequence(iCardList,iCardCount,3);
		break;
	case 1://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 && TackOutCount/3*4 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,3);
		break;
	case 2://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,3);
	case 3://����
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,3);
		if(TackOutCount>0 &&TackOutCount/3*5 == iCardCount
			&&GetCountBySpecifyNumCount(iCardList,iCardCount,2) == TackOutCount/3)
			return IsSequence(iBuffer,TackOutCount,3);

		break;
	}
	return false;
}

//������˳����
BOOL CUpGradeGameLogic::IsVariationFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX)
{
	if(iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%4 != 0)		
			return false;
		return IsVariationSequence(iCardList,iCardCount,4);
		break;

	case 1://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 && TackOutCount/4*5 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount)
			return IsVariationSequence(iBuffer,TackOutCount,4);

	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount 
			&&TackOutCount/4 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,4);

	case 4://(�����ԣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount
			&&TackOutCount/2 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsVariationSequence(iBuffer,TackOutCount,4);
		break;
	}	
	return FALSE;
}

//��˳����
BOOL CUpGradeGameLogic::IsFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX)
{
	if(iCardCount < 8)		//��˳����2
		return false;

	BYTE iBuffer[54];
	int TackOutCount=0;
	switch(iSeqX)
	{
	case 0:
		if( iCardCount%4 != 0)		
			return false;
		return IsSequence(iCardList,iCardCount,4);
		break;

	case 1://������
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 && TackOutCount/4*5 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,4);
		break;

	case 2://������(���ԷǶ��ӣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount)
			return IsSequence(iBuffer,TackOutCount,4);

	case 3://��һ��
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount 
			&&TackOutCount/4 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsSequence(iBuffer,TackOutCount,4);

	case 4://(�����ԣ�
		TackOutCount=TackOutBySepcifyCardNumCount(iCardList,iCardCount,iBuffer,4);
		if(TackOutCount>0 &&TackOutCount/4*6 == iCardCount
			&&TackOutCount/2 == GetBulkBySpecifyCardCount(iCardList,iCardCount,2))
			return IsSequence(iBuffer,TackOutCount,4);
		break;
	}	
	return FALSE;
}

//�ж��Ƿ���510K ը��
BOOL CUpGradeGameLogic::IsSlave510K(BYTE iCardList[], int iCardCount)
{
	if(iCardCount != 3) return false;
	BYTE Test[18]={0};
	for(int i = 0; i < iCardCount; i++)
	{
		Test[GetCardNum(i)] ++;
	}

	return (Test[5]==Test[10]==Test[13]==1);
}

//�ж��Ƿ���510K ͬ��
BOOL CUpGradeGameLogic::IsMaster510K(BYTE iCardList[], int iCardCount)
{
	if(iCardCount != 3) return false; //��������

	if( !IsSameHuaKind(iCardList, iCardCount) ) return false; //ͬ�� ��

	if( !IsSlave510K(iCardList, iCardCount) ) return false; //510K ��	

	return true;
}
////////////////////////////////////////////////////////////////////////////////////


//�Զ��ҳ����Գ�����
BOOL CUpGradeGameLogic::AutoOutCard(BYTE iHandCard[], int iHandCardCount, //��ǰ����������е�������
									BYTE iBaseCard[], int iBaseCardCount, //ǰһ�����Ƶ��˳���������
									BYTE iResultCard[], int & iResultCardCount, //�ҵ��Ľ��
									BOOL bFirstOut,BOOL bIsCue) //��ǰ����Ƿ�����
{
	iResultCardCount=0;
	if(bFirstOut) //���ֳ����ұ�һ����
	{
		TackOutCardBySpecifyCardNum(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iHandCard[iHandCardCount-1]);
	}
	else //����
	{
		//�����е������ҳ��������ϴ����
		TackOutCardMoreThanLast(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,false,bIsCue);

		if(!CanOutCard(iResultCard, iResultCardCount, iBaseCard, iBaseCardCount, iHandCard, iHandCardCount))
		{
			iResultCardCount = 0;
		}
	}
	return TRUE;
}

//��ȡ����������0, 1 or 2 ���״����Ǽ�,�� iBaseCount-3 ������
BYTE CUpGradeGameLogic::TackOutThreeX(BYTE iCardList[], int iCardCount, 
									  BYTE iBaseCard[], int iBaseCount, 
									  BYTE iResultCard[], int &iResultCount, int iValue)
{
	iResultCount = 0;

	//û���㹻����ֱ�ӷ���
	if(iCardCount<iBaseCount)
		return FALSE;

	BYTE iTempCard[54];
	int threecard = GetBulkBySpecifyCardCount(iBaseCard,iBaseCount,3);//���������ŵĵ���
	CString str;
	//3�����ܸ���
	BYTE iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iTempCard, 3);

	if(iCount > 0)//��ȡ�������������
	{
		BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, threecard, 3, true);//����ֵ��ȥ
		CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*3);

		if(threecard >= GetBulkBySpecifyCardCount(iResultCard,3,3))
			return FALSE;
	}else 
		return FALSE;
	//��ԭֵ����
	BYTE Tmp[54];
	int iTempCount=iCardCount;
	::CopyMemory(Tmp,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,3,Tmp,iTempCount);
	iTempCount-=3;
	int destCount = iBaseCount - 3;

	switch(iValue)
	{
	case 1:
	case 2:
		{	
			iCount=TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,1);
			if(iCount >= destCount)//���ҵ�����
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//�����������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount >= destCount)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;
		}
	case 3:
		{
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount > 0)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;

		}
	default:
		iResultCount = 0;
		break;
	}

	if(iResultCount == iBaseCount )
		return  TRUE;
	iResultCount =0;
	return FALSE;
}
//��ȡ����
BOOL  CUpGradeGameLogic::TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount = 0;
	if(iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[54];
	int tmpbaseCardCount =0,destCardCount =0;
	//�������Ƶ������������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,3);
	if(tmpbaseCardCount < 6)	//������������
		return FALSE;
	//����ȡ����������˳
	if(!TackOutSequence(iCardList,iCardCount,tmpBaseCard,tmpbaseCardCount,iResultCard,iResultCardCount,3))
		return FALSE;
	//�����Ƹ���һ��(�Ƴ���˳��)
	BYTE TMP[54];
	int TmpCount = iCardCount ;
	::CopyMemory(TMP,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,iResultCardCount,TMP,TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	BYTE twoList[54];
	int twoCount;
	//�������ƵĶ�˳�������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,2);
	if(!TackOutSequence(TMP,TmpCount,tmpBaseCard,tmpbaseCardCount,twoList,twoCount,2,true))
		return false;
	::CopyMemory(&iResultCard[iResultCardCount],twoList,sizeof(BYTE)*twoCount);
	iResultCardCount +=twoCount;
	return true;
}
//��ȡָ��������˳
BOOL  CUpGradeGameLogic::TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue)
{
	iResultCardCount=0;
	if(iCardCount < iBaseCount)	//��������
		return false;
	BYTE tmpBaseCard[54];
	int tmpbaseCardCount =0,destCardCount =0;
	//�������Ƶ������������
	tmpbaseCardCount =TackOutBySepcifyCardNumCount(iBaseCard,iBaseCount,tmpBaseCard,3);
	if(tmpbaseCardCount < 6)	//������������
		return FALSE;

	//����ȡ����������˳
	if(!TackOutSequence(iCardList,iCardCount,tmpBaseCard,tmpbaseCardCount,iResultCard,iResultCardCount,3))
		return FALSE;

	//�����Ƹ���һ��
	BYTE TMP[54];
	int TmpCount = iCardCount ;
	::CopyMemory(TMP,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,iResultCardCount,TMP,TmpCount);
	TmpCount -= iResultCardCount;
	destCardCount = iBaseCount - iResultCardCount;	//��������

	switch(xValue)
	{
	case 1:
	case 2:
		{
			tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,1);//�յ���
			if(tmpbaseCardCount >= destCardCount)
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);//����
				iResultCardCount += destCardCount;
			}
			else
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
				iResultCardCount += tmpbaseCardCount;
				destCardCount -= tmpbaseCardCount;
				tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,2);//�ö��Ʋ�
				if(tmpbaseCardCount>=destCardCount)
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
					iResultCardCount += destCardCount;
				}
				else
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
					iResultCardCount += tmpbaseCardCount;
					destCardCount -= tmpbaseCardCount;
					tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);//��������
					//
					if(tmpbaseCardCount>=destCardCount)
					{
						::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
						iResultCardCount += destCardCount;
					}
				}
			}
			break;
		}
	case 3:
		{
			tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,2);//�ն���
			if(tmpbaseCardCount>=destCardCount)
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
				iResultCardCount += destCardCount;
			}
			else
			{
				::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*tmpbaseCardCount);
				iResultCardCount += tmpbaseCardCount;
				destCardCount -= tmpbaseCardCount;
				//tmpbaseCardCount =TackOutBySepcifyCardNumCount(TMP,TmpCount,tmpBaseCard,3);//����������
				TackOutMuchToFew(TMP,TmpCount,tmpBaseCard,tmpbaseCardCount,3,2);	//������������ɶ�����
				if(tmpbaseCardCount >=destCardCount)//������Թ���
				{
					::CopyMemory(&iResultCard[iResultCardCount],tmpBaseCard,sizeof(BYTE)*destCardCount);
					iResultCardCount += destCardCount;
				}
			}
			break;
		}
	default:
		break;
	}

	if(iResultCardCount == iBaseCount )
		return true;
	iResultCardCount =0;
	return FALSE;
	return (iResultCardCount == iBaseCount);
	//	return FALSE;
}

//��д��ȡ���ŵ�˳��,���� or ����
BOOL CUpGradeGameLogic::TackOutSequence(BYTE iCardList[], int iCardCount,      //���е���
										BYTE iBaseCard[], int iBaseCount,      //������������, �Ƶĸ���
										BYTE iResultCard[], int &iResultCount, //�ҵ�����
										int xSequence,BOOL bNoComp)			   //˳�ӵĸ���
{
	iResultCount=0;
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iBaseStart,iDestStart=0,iDestEnd=0;
	int iSequenceLen = iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;
	//����һ��ϵͳ����������(����������С->�����
	SortCard(iTack,NULL,iTackCount,TRUE);
	//�û����������
	for(int i=0;i<iTackCount;i++)
	{
		temp[GetCardBulk(iTack[i])]++;
	}

	switch(xSequence)
	{
		//��˳
	case 1:
		iSequenceLen = iBaseCount;
		if(!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard,iBaseCount,1);
		else
			iBaseStart = 2;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i]>=1)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)//�ҵ�һ����
			{
				iResultCard[iResultCount++] = iTack[j];
				iDestStart++;
				iDestEnd--;
			}
			//�Ѿ���ȫ
			if(iDestEnd == 0)
				return true;
		}
		break;
		//˫˳
	case 2:       
		iSequenceLen = iBaseCount/2;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,2);     //�õ�˳�ӵ���ʼλ��
		else
			iBaseStart =3;

		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 2)          //������ֵ������2������
			{
				if(iDestStart == 0)
					iDestStart = i;

				iDestEnd++;

				if(iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd == 0)
		{
			return false;
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}

			if(num ==2)//һ���Ѿ��ҵ�
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}
		}
		break;
	case 3:
		iSequenceLen = iBaseCount/3;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,3);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 3)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				iDestStart = 0;
				iDestEnd = 0;
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;

				if(num == 3)//�ҵ�����
				{
					num=0;
					iDestStart++;
					iDestEnd--;
					//�Ѿ���ȫ
					if(iDestEnd == 0)
						return true;
				}
			}
		}

		break;
	}
	return FALSE;
}

//��ȡͬ���
BOOL CUpGradeGameLogic::TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[], int &iResultCardCount)
{
	iResultCardCount=0;
	if(iCardCount < iBaseCount)
		return false;
	BYTE iBaseMinCard = GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1);//�����˳������С����
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iDestStart=0,iDestEnd=0;
	int iSequenceLen=iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount,6);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;

	SortCard(iTack,NULL,iTackCount,TRUE);

	BYTE iTempKind[54];
	int iTempKindCount =0;

	//�û����������
	for(int kind = 0;kind<=48 ;kind+=16)
	{	//��ȡ����
		iResultCardCount=0;
		iTempKindCount = TackOutByCardKind(iTack,iTackCount,iTempKind,kind);

		if(iTempKindCount >= iBaseCount)					//��������
		{
			for(int i = 0 ;i < iTempKindCount;i++)
			{
				temp[GetCardBulk(iTempKind[i])]++;
			}

			for(int i = iBaseMinCard+1;i<15;i++)//�Զ������б���
			{
				if(temp[i]>=1)		//ĳ��ɫ����
				{
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}else
				{
					iDestStart = 0;
					iDestEnd = 0;
				}
			}


			if(iDestEnd != iBaseCount)	//ĳ�ֻ�ɫ������,������һ�ֻ�ɫ
				continue;
			//��ȡ����
			for(int j=0;j<iTempKindCount;j++)
			{
				if(GetCardBulk(iTempKind[j]) == iDestStart)
				{
					iResultCard[iResultCardCount++] = iTempKind[j];
					iDestStart++;
					iDestEnd--;
				}
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}
		}

	}

	return FALSE;
}

//��ȡ�����е�ͬ��˳
BOOL CUpGradeGameLogic::TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iResultCard[], int &iResultCardCount)
{
	if(iCardCount < 5)
		return FALSE;

	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iDestStart=0,iDestEnd=0;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount,6);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;

	SortCard(iTack,NULL,iTackCount,TRUE);

	BYTE iTempKind[54];
	int iTempKindCount =0;

	for(int kind = 0;kind<=48 ;kind+=16)
	{	//��ȡ����
		iResultCardCount=0;
		//��ȡ��ɫ
		iTempKindCount = TackOutByCardKind(iTack,iTackCount,iTempKind,kind);

		if(iTempKindCount >= 5)					//��������
		{
			for(int i = 0 ;i < iTempKindCount;i++)
			{
				temp[GetCardBulk(iTempKind[i])]++;
			}

			for(int i = GetCardBulk(iTempKind[0]);i<15;i++)//�Զ������б���
			{
				if(temp[i]>=1)		//ĳ��ɫ����
				{
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == 5)
						break;
				}else
				{
					iDestStart = 0;
					iDestEnd = 0;
				}
			}


			if(iDestEnd != 5)	//ĳ�ֻ�ɫ������,������һ�ֻ�ɫ
				continue;
			//��ȡ����
			for(int j=0;j<iTempKindCount;j++)
			{
				if(GetCardBulk(iTempKind[j]) == iDestStart)
				{
					iResultCard[iResultCardCount++] = iTempKind[j];
					iDestStart++;
					iDestEnd--;
				}
				//�Ѿ���ȫ
				if(iDestEnd == 0)
				{
					return TRUE;
				}
			}
		}

	}
	return FALSE;
}

//�õ�˳�ӵ���ʼλ��
int CUpGradeGameLogic::GetSequenceStartPostion(BYTE iCardList[],int iCardCount,int xSequence)
{
	BYTE temp[18]={0};
	int Postion=0;
	int minPos = 999,maxPos = 0,secondPos = 0,thirdPos = 0;
	for(int i=0;i < iCardCount;i++)
	{
		temp[GetCardBulk(iCardList[i])]++;
	}

	for(int i = 0;i < 18;i++)
	{
		if(temp[i] != xSequence)
			continue;
		if(minPos > i)
			minPos = i;

		if(maxPos == 0 || maxPos < i)
		{
			thirdPos = secondPos;
			secondPos = maxPos;
			maxPos = i;
		}
		if(secondPos == 0)
			secondPos =i;

		if(thirdPos == 0)
			thirdPos =i;
	}

	switch(xSequence)
	{
	case 0:
		break;
	case 1://����˳
		break;
	case 2://˫˳
		break;
	case 3://��˳
		break;
	}
	//����A2345�������
	return minPos;
}

//��ȡ510K
BOOL CUpGradeGameLogic::TrackOut510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BOOL bIsMast510K)
{
	iResultCardCount=0;
	BYTE temp[48]={0};
	BYTE huasei[4][16]={0};
	int k = 0, num[4] = {0};
	//�õ�510K����
	for(int i=0; i<iCardCount; i++)
	{
		int n = GetCardNum(iCardList[i]);
		if(n==5 || n==10 || n==13)
		{
			temp[k++] = iCardList[i];
			int kind = GetCardHuaKind(iCardList[i],true) >> 4;

			huasei[kind][num[kind]++] = iCardList[i];
		}
	}
	//5,10,k��Ŀ����3��
	if(num[0]+num[1]+num[2]+num[3] < 3)
		return false;
	//Ҫ����510K��������3��
	if(bIsMast510K && num[0]<3 && num[1]<3 && num[2]<3 && num[3]<3)
		return false;
	for(int i=0;i<4;i++)
	{
		if(Test510K(huasei[i],num[i]))//ĳһ��ɫ�Ƿ�Ϊ��510K
		{
			Copy510K(huasei[i],num[i],iResultCard,iResultCardCount);	

			if(bIsMast510K) //�Ƿ���Ҫ��ȡ��510K
				return true;
			else
			{
				RemoveCard(iResultCard,iResultCardCount,huasei[i],num[i]);//����510K�Ƴ�ѡ����ɫ����
				RemoveCard(iResultCard,iResultCardCount,temp,k);	//����510K�Ƴ�510K����
				num[i]-=iResultCardCount;
				k-=iResultCardCount;	
			}
			//			return true;
		}
	}

	if(bIsMast510K) return false; //��Ҫͬ��510K

	if(Test510K(temp,k))
	{
		Copy510K(temp,k,iResultCard,iResultCardCount);
		return true;
	}
	return FALSE;
}

//����510K
BOOL CUpGradeGameLogic::Copy510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount)
{
	iResultCardCount=0;
	BYTE five,ten,k;
	for(int i=0;i<iCardCount;i++)
	{
		if(GetCardNum(iCardList[i])==5)
			five=iCardList[i];
		else if(GetCardNum(iCardList[i])==10)
			ten=iCardList[i];
		else k=iCardList[i];
	}
	iResultCard[0]=five;
	iResultCard[1]=ten;
	iResultCard[2]=k;
	iResultCardCount=3;
	return  TRUE;
}

//�����Ƿ�Ϊ5,10k
BOOL CUpGradeGameLogic::Test510K(BYTE iCardList[],int iCardCount)
{
	BOOL five=false,ten=false,k=false;
	for(int i=0;i<iCardCount;i++)
	{
		if(GetCardNum(iCardList[i])==5)
			five=true;
		else if (GetCardNum(iCardList[i])==10)
			ten=true;
		else 
			k=true;
	}
	//��5,10,k
	if(five&&ten&&k)
		return true;
	return false;
}

BOOL CUpGradeGameLogic::TackOutAddLevelCard(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount)
{
	BYTE temp[18] = {0};
	int t = GetCardBulk(iBaseCard[0], false); //�õ��������Ǹ��Ƶ�ֵ
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i],false)]++;
	}
	for(int i = 0;i<18;i++)
	{
		if(temp[i] + m_iReplaceCardCount == iBaseCardCount && i>t) 
			for(int j = 0;j < iCardCount;j++)
			{
				if(GetCardBulk(iCardList[j],false) == i)
				{
					iResultCard[iResultCardCount++] = iCardList[j];
					if(iResultCardCount + m_iReplaceCardCount == iBaseCardCount)
						return true ;			
				}
			}
	}
}
//���
BOOL CUpGradeGameLogic::TackOutCardByNoSameShape(BYTE iCardList[],int iCardCount,
												 BYTE iResultCard[],int &iResultCardCount,
												 BYTE iBaseCard[],int iBaseCardCount)
{
	iResultCardCount = 0;
	BYTE temp[18] = {0};
	int tempcard = 0;
	if(iBaseCardCount != GetReplaceCardCount(iBaseCard,iBaseCardCount))
	{
		for(int index = 0;index < iBaseCardCount;index++)
		{
			if(iBaseCard[index] != m_iReplaceCardArray[0])
			{
				tempcard = GetCardBulk(iBaseCard[index]); //�õ��������Ǹ��Ƶ�ֵ
				break;
			}
		}
	}
	else
	{
		tempcard = GetCardBulk(iBaseCard[0]);
	}
	for(int i = 0; i < iCardCount; i++)
	{
		temp[GetCardBulk(iCardList[i],false)]++;
	}

	//��(ը�Ʋ���)
	for(int i = 0; i < 18; i++)
	{
		if( temp[i] > iBaseCardCount &&//�����������ƶ�
			temp[i] < 4 &&              //��ը����
			i > tempcard )              //�����ִ�
		{
			for(int j=0; j < iCardCount; j++)
			{
				if(GetCardBulk(iCardList[j],false) == i)
				{
					iResultCard[iResultCardCount++] = iCardList[j];
					if(iResultCardCount == iBaseCardCount)
						return true ;			
				}
			}
		}
	}
	return false;
}


//�Ƿ���Գ���
BOOL CUpGradeGameLogic::CanOutCard(BYTE iOutCard[], int iOutCount,  //Ҫ������
								   BYTE iBaseCard[], int iBaseCount,//Ҫѹ����
								   BYTE iHandCard[], int iHandCount,//���е���
								   BOOL bFirstOut)
{
	BYTE iOutCardShape = GetCardShape(iOutCard, iOutCount);

	if(iOutCardShape == UG_ERROR_KIND) //���Ͳ���
	{
		return FALSE;
	}

	if(bFirstOut)						//��һ������
	{
		return TRUE;
	}
	BYTE iBaseCardShape = GetCardShape(iBaseCard, iBaseCount); //�����ϵ�����

	//ͬ��˳>С��6ը
	if( iOutCardShape == UG_STRAIGHT_FLUSH || iOutCardShape == UG_VARIATION_STRAIGHT_FLUSH  )
	{
		if(iBaseCardShape < UG_BOMB || (UG_BOMB == iBaseCardShape && iBaseCount < 6))
		{
			return TRUE;
		}
		if(UG_BOMB == iBaseCardShape && iBaseCount >= 6)
		{
			return FALSE;
		}
	}
	//6ը����ͬ��˳
	if(UG_BOMB == iOutCardShape && iOutCount >= 6 &&(iBaseCardShape == UG_STRAIGHT_FLUSH || iBaseCardShape == UG_VARIATION_STRAIGHT_FLUSH ))	
		return TRUE;

	if(iBaseCardShape > iOutCardShape)						//����<
	{
		return FALSE;
	}

	if( iBaseCardShape < iOutCardShape)						//����>
	{
		if(UG_SLAVE_510K <= iOutCardShape)					//ը��
		{
			return TRUE;
		}	
		//����һ��������Ҳ���Դ����˳�Ӻ�˳�Ӵ�С�Ƚ�
		if(iBaseCount != iOutCount) //��������
			return FALSE;

		switch(iBaseCardShape)
		{
		/*case UG_STRAIGHT:									//ͬ��혴������
			{
				if(iOutCardShape == UG_STRAIGHT_FLUSH)
					return true;
			}*/
		case UG_VARIATION_STRAIGHT:							//��С����˳��
			{
				if(iOutCardShape == UG_STRAIGHT)			//����˳������Ч���ֵС������˳���������
					return TRUE;
					//return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,1) <  GetBulkBySpecifySequence(iOutCard, iOutCount,1) ;
				return false;
			}

		case UG_VARIATION_DOUBLE_SEQUENCE://��С����˳��
			{
				if(iOutCardShape == UG_DOUBLE_SEQUENCE)	//����˳������Ч���ֵС������˳���������
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,2) <  GetBulkBySpecifySequence(iOutCard, iOutCount,2) ;
				return false;
			}
		case UG_VARIATION_STRAIGHT_FLUSH: //����ͬ��˳
			{
				if(iOutCardShape == UG_STRAIGHT_FLUSH)			//����˳������Ч���ֵС������˳���������
					return TRUE;
				//return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,1) <  GetBulkBySpecifySequence(iOutCard, iOutCount,1) ;
				return false;
			}

		case UG_VARIATION_THREE_SEQUENCE:		//������˳
		case UG_VARIATION_THREE_ONE_SEQUENCE://������˳
		case UG_VARIATION_THREE_TWO_SEQUENCE://����������˳
		case UG_VARIATION_THREE_DOUBLE_SEQUENCE://����������˳
		case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://������˳����˳
			{
				if(iOutCardShape == iBaseCardShape+1)
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,3) <  GetBulkBySpecifySequence(iOutCard, iOutCount,3) ;
				return false;
			}
		case UG_VARIATION_FOUR_SEQUENCE:		//������˳
		case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
		case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
		case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
		case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
			{
				if(iOutCardShape == iBaseCardShape+1)
					return GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,4) <  GetBulkBySpecifySequence(iOutCard, iOutCount,4) ;
				return false;
			}
		case UG_THREE_TWO://����һ������������
			{
				if(iOutCardShape == UG_THREE_DOUBLE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
				return false;
			}
		case UG_THREE_TWO_SEQUENCE://����һ���(�����)>���������
			{
				if(iOutCardShape == UG_THREE_DOUBLE_SEQUENCE || iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
				return false;
			}
		case UG_FOUR_TWO://�Ď�һ�����Ď�����
			{
				if(iOutCardShape == UG_FOUR_ONE_DOUBLE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
				return false;
			}
		case UG_FOUR_TWO_SEQUENCE://�Ď�һ��혣��Ď������
			{
				if(iOutCardShape == UG_FOUR_ONE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
				return false;
			}
		case UG_THREE_DOUBLE_SEQUENCE:	//����������혎���
			{
				if(iOutCardShape == UG_THREE_SEQUENCE_DOUBLE_SEQUENCE)
					return GetBulkBySpecifySequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifySequence(iOutCard, iOutCount,3);
				return false;
			}
		}
		return false;
	}	

	switch(iBaseCardShape)			//��������һ��
	{
	case UG_ONLY_ONE:  //����
	case UG_DOUBLE:    //����
	case UG_THREE:     //����
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
		}		
	case UG_BOMB: //4+�� ը��
		{
			if(iBaseCount > iOutCount) //�������ը����
				return FALSE;

			if(iBaseCount == iOutCount) //������ͬ,�ȵ���
				return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1);
			return TRUE;
		}

	case UG_FLUSH:			//ͬ��(��˳�ӣ��Ƚ�ͬ����������
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 255,true) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 255,true);
		}
	case UG_STRAIGHT_FLUSH: //ͬ��˳
	case UG_STRAIGHT:		//˳��
	case UG_DOUBLE_SEQUENCE: //����
	case UG_THREE_SEQUENCE:  //����  
	case UG_FOUR_SEQUENCE:	//��˳
		if(iOutCount != iBaseCount)
			return FALSE;
		{
			return GetBulkBySepcifyMinOrMax(iBaseCard, iBaseCount, 1,true) < GetBulkBySepcifyMinOrMax(iOutCard, iOutCount, 1,true);
		}

	case UG_THREE_ONE:		//����һ
	case UG_THREE_TWO:		//������
	case UG_THREE_DOUBLE:	//������
		//��һ�������Ƶ��Ƶ��С������
		//return (SearchThreeCard(iBaseCard, iBaseCount) < SearchThreeCard(iOutCard, iOutCount));
		{
			return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,3)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,3);
		}
	case UG_FOUR_ONE:						//�Ĵ�һ
	case UG_FOUR_TWO:						//�Ĵ���
	case UG_FOUR_ONE_DOUBLE:				//�Ĵ�һ��
	case UG_FOUR_TWO_DOUBLE:				//�Ĵ�����
		{
			return GetBulkBySpecifyCardCount(iBaseCard, iBaseCount,4)<GetBulkBySpecifyCardCount(iOutCard, iOutCount,4);
		}

	case UG_THREE_ONE_SEQUENCE: //2+������һ
	case UG_THREE_TWO_SEQUENCE: //2+��������
	case UG_THREE_DOUBLE_SEQUENCE://������˳
	case UG_THREE_SEQUENCE_DOUBLE_SEQUENCE:		//��˳����˳(����)
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,3));
		}
	case UG_FOUR_ONE_SEQUENCE:					//��˳
	case UG_FOUR_TWO_SEQUENCE:
	case UG_FOUR_ONE_DOUBLE_SEQUENCE:
	case UG_FOUR_TWO_DOUBLE_SEQUENCE:
		{
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,4));
		}
	case UG_MASTER_510K: //ͬ��510K����ɫ:���� > ���� > ÷�� > ��Ƭ
		{
			return (GetCardHuaKind(iBaseCard[0],true) < GetCardHuaKind(iOutCard[0],true)); //�Ȼ�ɫ
		}
	case UG_SLAVE_510K: //��510K��һ����
		{
			return FALSE;
		}
		//�������δ���
	case UG_VARIATION_STRAIGHT://��˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,1) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,1));

			break;
		}
	case UG_VARIATION_DOUBLE_SEQUENCE://��˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,2) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,2));
			break;
		}
	case UG_VARIATION_THREE_SEQUENCE://��˳
	case UG_VARIATION_THREE_ONE_SEQUENCE://����һ˳
	case UG_VARIATION_THREE_TWO_SEQUENCE://������˳
	case UG_VARIATION_THREE_DOUBLE_SEQUENCE://������˳
	case UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE://��˳����˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifyVariationSequence(iBaseCard, iBaseCount,3) < GetBulkBySpecifyVariationSequence(iOutCard, iOutCount,3));
			break;
		}
	case UG_VARIATION_FOUR_SEQUENCE:		//������˳
	case UG_VARIATION_FOUR_ONE_SEQUENCE:	//�����Ĵ�һ˳
	case UG_VARIATION_FOUR_TWO_SEQUENCE:	//�����Ĵ���˳
	case UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE://�����Ĵ�һ��˳
	case UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE://�����Ĵ�����˳
		{
			if(iOutCount != iBaseCount)
				return FALSE;
			return(GetBulkBySpecifySequence(iBaseCard, iBaseCount,4) < GetBulkBySpecifyCardCount(iOutCard, iOutCount,4));

			return false;
		}
	}

	return FALSE;
}

//��ȡ����ը��Ϊ�ᷴ��˳,˫˳,��˳��׼��
BOOL CUpGradeGameLogic::TackOutAllBomb(BYTE iCardList[],int iCardCount,
									   BYTE iResultCard[],int &iResultCardCount,int iNumCount)
{
	iResultCardCount=0;
	BYTE bCardBuffer[54];
	BYTE bombcount=GetBombCount(iCardList,iCardCount,iNumCount);  //��ȡ������iNumCount���Ƶ�ը��
	if(bombcount<0)
		return false;
	for(int i=iNumCount;i < 9;i++)
	{
		int count=TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i);
		if(count > 0)
		{
			::CopyMemory(&iResultCard[iResultCardCount],bCardBuffer,sizeof(BYTE)*count);
			iResultCardCount+=count;
			break;
		}
	}
	return true;
}

//��ȡը��
BOOL CUpGradeGameLogic::TackOutBomb(BYTE iCardList[], int iCardCount,
									BYTE iResultCard[], int &iResultCardCount,int iNumCount)
{
	iResultCardCount=0;
	BYTE bCardBuffer[54];
	memset(bCardBuffer,0,sizeof(bCardBuffer));
	//BYTE bombcount=GetBombCount(iCardList,iCardCount,iNumCount);
	//if(bombcount<= 0 || )
	//	return false;
	for(int i=iNumCount;i<9;i++)
	{
		int count=TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i);
		if(count > 0)
		{
			::CopyMemory(iResultCard,bCardBuffer,sizeof(BYTE)*i);
			iResultCardCount=i;
			return TRUE;
		}
	}
	
	///<���û���ҵ���ը������������ƴճɵ�ը��    2011--5--12 xyh
	if(iResultCardCount == 0 && GetReplaceCardCount(iCardList,iCardCount))
	{
		BYTE iBuffer[54];
		int kingcount;
		TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
		
		for(int ReplaceCount=1;ReplaceCount<=kingcount; ReplaceCount++)
		{
			iNumCount -= ReplaceCount;
			for(int i=iNumCount;i<9;i++)
			{
				int count=TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i);
				if(count > 0)
				{
					::CopyMemory(iResultCard,bCardBuffer,sizeof(BYTE)*i);
					iResultCardCount = i;
					if(GetReplaceCardCount(iResultCard,i))
					{
						//count = TackOutBySepcifyCardNumCount(iCardList,iCardCount,bCardBuffer,i + ReplaceCount);
						//if(count)
						//{
						//	::CopyMemory(iResultCard,bCardBuffer,sizeof(BYTE)*(i + ReplaceCount));
						//	iResultCardCount = i + ReplaceCount;			
						//	return TRUE;
						//}
						iResultCardCount == 0;
					}
					else
					{
						for(int index = 0;index<ReplaceCount;index++)
						{
							iResultCard[i + index] = iBuffer[index];
							iResultCardCount++;
						}
						if(iResultCardCount == i + ReplaceCount)
						{
							return TRUE;
						}
					}
					
				}
			}
		}
	}
	if(iResultCardCount == 0)
		if(TackOutKingBomb(iCardList,iCardCount,iResultCard,iResultCardCount))
		{
			return TRUE;
		}
	return FALSE;
}

//��ȡ��ը
BOOL CUpGradeGameLogic::TackOutKingBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount)
{
	iResultCardCount=0;

	BYTE bCardBuf[8];
	int kingcount=0;
	int SingKing= KING_COUNT/2;
	int count=TackOutBySpecifyCard(iCardList,iCardCount,bCardBuf,kingcount,0x4e);
	if(count != SingKing)
		return false;

	::CopyMemory(iResultCard,bCardBuf,sizeof(BYTE)*count);

	count=TackOutBySpecifyCard(iCardList,iCardCount,bCardBuf,kingcount,0x4f);
	if(count != SingKing)
	{
		return false;
	}	
	::CopyMemory(&(iResultCard[SingKing]),bCardBuf,sizeof(BYTE)*count);
	return iResultCardCount = KING_COUNT;
}

/*
paramenter iCardList[] �����ƶ���
paramenter iCardCount,�����ƶ�������
paramenter iResultCard[],����ƶ���
paramenter iResultCardCount,����Ƴ���
paramenter iConditionCard[] ������
paramenter iConditionCount  ������������
return value true found >0 
			false found =0
*/

//��������ĳ��������
BOOL CUpGradeGameLogic::TackOutByCondition(BYTE iCardList[],int iCardCount, 
										   BYTE iResultCard[], int & iResultCardCount,
										   BYTE iConditionCard[],int iConditionCardCount)
{
	iResultCardCount = 0;
	for(int i = 0 ;i < iConditionCardCount ; i++)
	{
		for(int j = 0 ;j<iCardCount ;j ++)
		{
			if(iConditionCard[i] == iCardList[j])
				iResultCard[iResultCardCount++] = iCardList[j]; 
		}
	}
	return iResultCardCount;
}

//����һ���ȵ�ǰ���
BOOL CUpGradeGameLogic::TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount,
												BYTE iBaseCard[], int iBaseCardCount,
												BYTE iResultCard[], int &iResultCardCount, 
												bool bExtVal, BOOL bIsCue)
{
	BYTE iTempCard[54];
	iResultCardCount = 0;
	int iBaseShape = GetCardShape(iBaseCard, iBaseCardCount); //�������Ƶ�����
	//���ұ������ϴ����ͬ����
	switch(iBaseShape)
	{
	case UG_ONLY_ONE:                        //����
	case UG_DOUBLE:                          //����
	case UG_THREE:                           //����
	case UG_BOMB:	                         //����_ը��
		{
			//����������ͬ����
			BYTE iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iBaseCardCount);
			if(iCount > 0)
			{
				int TempValue = 0;
				BYTE Temp = 0;
				BOOL IsAllReplaceCard = TRUE;    //�ѳ����� �Ƿ� ȫ���ǿ������

				//��ֹ��ʾ�������п�����Ƶ�����
				if(GetReplaceCardCount(iBaseCard,iBaseCardCount) && iBaseShape != UG_ONLY_ONE )
				{
					for(int index = 0;index < iBaseCardCount;index++)
					{
						if(iBaseCard[index]!=m_iReplaceCardArray[0])
						{
							TempValue = GetCardBulk(iBaseCard[index]);
							IsAllReplaceCard = FALSE;
							Temp = iBaseCard[index];
							break;
						}
					}
					if(IsAllReplaceCard)
					{
						TempValue = GetCardBulk(iBaseCard[0]);    //����ֵ
						Temp = iBaseCard[0];                      //ȫֵ
						
					}
				}
				else
				{
					TempValue = GetCardBulk(iBaseCard[0]);
					Temp = iBaseCard[0];
				}

				BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, TempValue, iBaseCardCount, false);  //����ֵ����
				CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iBaseCardCount);				
			
				if(CompareOnlyOne(Temp, iResultCard[0]))           //ʵ��ֵ�Ƚ�
				{
					iResultCardCount = iBaseCardCount;
					return TRUE;
				}
			}
			break;
		}	
	case UG_THREE_DOUBLE:	                 //����һ��
		{
			if(TackOutThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
				return TRUE;
			break;
		}
	case UG_THREE_SEQUENCE:                  //����
		{
			if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
				return TRUE;
			break;
		}
	case UG_DOUBLE_SEQUENCE:                 //����
		{	
			if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
				return TRUE; 
			break;
		}
	case UG_STRAIGHT:                        //˳��
		{
			if(TackOutSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
				return TRUE; 
			break;
		}
	case UG_STRAIGHT_FLUSH:                  //ͬ��˳
		{
			if(TackOutStraightFlush(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount))
				return TRUE;
			break;
		}
	default:
		{
			iResultCardCount = 0;
		}
	}
	//û�ҵ�ͬ���͵Ĵ���,�ͽ�������Ʒ�������
	if(	bIsCue && iResultCardCount == 0) 
	{
		switch(iBaseShape)
		{
		case UG_ONLY_ONE:                     
		case UG_DOUBLE:						   //����
		case UG_THREE:                         //����
		case UG_BOMB:	                       //���� ը��
			{
				int kingcount = 0;
				int iNumCount = iBaseCardCount;
				BYTE iBuffer[54];
				if(GetReplaceCardCount(iHandCard,iHandCardCount))
				{
					TackOutByCondition(iHandCard,iHandCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
				}else
					break;
				int BaseCard = GetCardBulk(iBaseCard[0]);

				if(GetReplaceCardCount(iBaseCard,iBaseCardCount))
				{
					for(int i=0; i<iBaseCardCount; i++)
						if(m_iReplaceCardArray[0]!=iBaseCard[i])
						{
							BaseCard = GetCardBulk(iBaseCard[i]);  
							break;
						}
				}
	
				for(int ReplaceCount=1;ReplaceCount<=kingcount; ReplaceCount++)
				{

					iNumCount = iBaseCardCount - ReplaceCount;
					BYTE iCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iNumCount);   //���ز������͵�����
					if(iCount > 0)
					{
						BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, BaseCard, iNumCount, false);
						if (GetCardNum(iTempCard[Step]) > 14)
							break;
						CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iNumCount);	

						//ѡ���������м�����
						if(GetReplaceCardCount(iResultCard,iNumCount))
						{
							iNumCount += ReplaceCount;
							int iReFindCount = TackOutBySepcifyCardNumCount(iHandCard, iHandCardCount, iTempCard, iNumCount);
							if (iReFindCount > 0 )
							{
								BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, BaseCard, iNumCount, false);
								CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*iNumCount);	
							}

							//2011-5-6ȷ��ѡ�����Ʊ������ƴ� xyh
							if(BaseCard > GetCardBulk(iResultCard[0]))
							{
								iResultCardCount = 0;
								continue;
							}
							else
							{
								iResultCardCount = iNumCount;
								return TRUE;
							}
						}
						else
						{
							if(BaseCard>GetCardBulk(iResultCard[0]))
							{
								iResultCardCount = 0;
								continue;
							}
							else
							{
								for (int i=0; i<ReplaceCount; i++)
								{
									iResultCard[iNumCount+i] = iBuffer[i];
									iNumCount++;
								}

								if(iNumCount == iBaseCardCount)
								{
									iResultCardCount = iBaseCardCount;
									return TRUE;
								}
							}
						}
					}
					else
					{
						iResultCardCount = 0;
						continue;
					}
				}
				break;
			}
		case UG_THREE_DOUBLE:	               //����һ��
			{
				if (TackOuttReplaceThreeX(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
					return TRUE;
				break;
			}
		case UG_THREE_SEQUENCE:                //����
			{
				if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,3))
					return TRUE;
				break;
			}
		case UG_DOUBLE_SEQUENCE:              //����
			{
				if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,2))
					return TRUE;
				break;
			}
		case UG_STRAIGHT:                      //˳��
			{
				if (TackOutReplaceSequence(iHandCard, iHandCardCount, iBaseCard, iBaseCardCount, iResultCard, iResultCardCount,1))
					return TRUE;
				break;
			}
		case UG_STRAIGHT_FLUSH:               //ͬ��˳
			{
				break;
			}
		default:                            //����Ҳ�����Ķ��ӵ��ƾ��Ҵ������,warning�˴�����break
			iResultCardCount = 0;
			break;
		}
	}
	if(iResultCardCount == 0)
		if(TackOutMoreThanLastShape(iHandCard, iHandCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount,iBaseShape))
			return TRUE;
	return FALSE;
}

//ֱ����ȡ�������ϴ������
BOOL  CUpGradeGameLogic::TackOutMoreThanLastShape(BYTE iCardList[], int iCardCount,            //������ϵ���                  
												  BYTE iResultCard[], int &iResultCardCount,   //�������������������ҵ�����
												  BYTE iBaseCard[], int iBaseCardCount,        //����������
												  int ishape)                                  //��������
{
	iResultCardCount = 0;
	switch(ishape)
	{
		case UG_ONLY_ONE:               //���Բ����,��������ѹ���ƻ��߶���
		case UG_DOUBLE:   
			{
				if(TackOutCardByNoSameShape(iCardList, iCardCount, iResultCard, iResultCardCount, iBaseCard, iBaseCardCount))
					return TRUE;
			}
		case UG_THREE:                         //����
		case UG_THREE_DOUBLE:	               //����һ��
		case UG_THREE_SEQUENCE:                //����
		case UG_DOUBLE_SEQUENCE:               //����
		case UG_STRAIGHT:					   //˳��
			{
				//��ը�������û�л���ը���Ҹ����ը��
				if(TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,4))
					return TRUE;
				else 
					if(TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,5))
						return TRUE;
					else 
						if(TackOutStraightFlush(iCardList, iCardCount,iResultCard, iResultCardCount))
							return TRUE;
			}
		case UG_VARIATION_STRAIGHT_FLUSH:
			{
				if(TackOutStraightFlush(iCardList, iCardCount,iResultCard, iResultCardCount))
					return TRUE;
			}
		case UG_STRAIGHT_FLUSH:                //ͬ��˳
			{
				if(TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,6))
					return TRUE;
				else 
					if(TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,7))
						return TRUE;
					else
						if(TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,8))
							return TRUE;
				break;
			}
		case UG_BOMB:                         //ը��
			{
				//����û�ҵ���ͬ�����Ĵ�ը��,���������������
				if((iBaseCardCount+1)<10 &&TackOutBomb(iCardList, iCardCount, iResultCard, iResultCardCount, iBaseCardCount+1))
					return TRUE;
				else 
					if(iBaseCardCount<6 && TackOutStraightFlush(iCardList, iCardCount,iResultCard, iResultCardCount))
						return TRUE;
					else 
						if((iBaseCardCount+2)<10 && TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,iBaseCardCount+2))
							return TRUE;
						else
							if((iBaseCardCount+3)<10 && TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,iBaseCardCount+3))
								return TRUE;
							else 
								if((iBaseCardCount+4)<10 && TackOutBomb(iCardList,iCardCount,iResultCard,iResultCardCount,iBaseCardCount+4))
									return TRUE;
				break;
			}
		default:
			if(TackOutKingBomb(iCardList,iCardCount,iResultCard,iResultCardCount))//����ը��
				return TRUE;
	}
	return FALSE;
}
//��ȡ����������0, 1 or 2 ���״����Ǽ�,�� iBaseCount-3 ������
BYTE CUpGradeGameLogic::TackOuttReplaceThreeX(BYTE iCardList[], int iCardCount, 
									  BYTE iBaseCard[], int iBaseCount, 
									  BYTE iResultCard[], int &iResultCount, int iValue)
{
	iResultCount = 0;
	if(iCardCount<iBaseCount)
		return FALSE;
	int kingcount = 0;
	BYTE iBuffer[54];
	if(GetReplaceCardCount(iCardList,iCardCount))
	{
		TackOutByCondition(iCardList,iCardCount,iBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	}else
		return FALSE;
	int count = 3 - kingcount;
	if (count <= 0)
		return FALSE;

	BYTE iTempCard[54];
	int threecard = GetBulkBySpecifyCardCount(iBaseCard,iBaseCount,3);//���������ŵĵ���
	//3�����ܸ���
	BYTE iCount = TackOutBySepcifyCardNumCount(iCardList, iCardCount, iTempCard, count);

	if(iCount > 0)//��ȡ�������������
	{
		BYTE Step = GetSerialByMoreThanSpecifyCard(iTempCard, iCount, threecard, count, true);  //����ֵ��ȥ
		if (iTempCard[Step] == 0x4e || iTempCard[Step] == 0x4f)
			return FALSE;

		//��ֹ�����Ʊ�
		CopyMemory(iResultCard, &iTempCard[Step], sizeof(BYTE)*count);
		if(GetReplaceCardCount(iResultCard,count))
		{
			return FALSE;           //��������д���ֱ���˳�
		}
		if(threecard >= GetBulkBySpecifyCardCount(iResultCard,count,count))
		{
			return FALSE;
		}
		CopyMemory(&iResultCard[count], iBuffer, sizeof(BYTE)*kingcount);

	}
	else
	{
		return FALSE;
	}
	//��ԭֵ����
	BYTE Tmp[54];
	int iTempCount=iCardCount;
	::CopyMemory(Tmp,iCardList,sizeof(BYTE)*iCardCount);
	RemoveCard(iResultCard,3,Tmp,iTempCount);
	iTempCount-=3;
	int destCount = iBaseCount - 3;

	switch(iValue)
	{
	case 1:
	case 2:
		{	
			iCount=TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,1);
			if(iCount >= destCount)//���ҵ�����
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//�����������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount >= destCount)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;
		}
	case 3:
		{
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,2);
			if(iCount > 0)
			{
				CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
				iResultCount = iBaseCount;
				break;
			}
			//��������������
			iCount = TackOutBySepcifyCardNumCount(Tmp,iTempCount,iTempCard,3);
			if(iCount < 3)//��һ�����޷���
				break;
			CopyMemory(&iResultCard[3],iTempCard,sizeof(BYTE)*destCount);
			iResultCount=iBaseCount;
			break;

		}
	default:
		iResultCount = 0;
		break;
	}
	if(iResultCount == iBaseCount )
		return  TRUE;
	iResultCount =0;
	return FALSE;
}
//��д��ȡ���ŵ�˳��,���� or ����
BOOL CUpGradeGameLogic::TackOutReplaceSequence(BYTE iCardList[], int iCardCount, //���е���
										BYTE iBaseCard[], int iBaseCount,        //������������, �Ƶĸ���
										BYTE iResultCard[], int &iResultCount,   //�ҵ�����
										int xSequence,BOOL bNoComp)				 //˳�ӵĸ���
{
	int ReplaceCount = GetReplaceCardCount(iCardList,iCardCount);
	 if (ReplaceCount==0)
		return FALSE;
	int TempCount = ReplaceCount;
	int kingcount = 0;
	BYTE iReplaceBuffer[54];
	TackOutByCondition(iCardList,iCardCount,iReplaceBuffer,kingcount,m_iReplaceCardArray,m_iReplaceCardCount);
	iResultCount=0;
	BYTE iTack[54];
	int iTackCount=iCardCount;
	//����һ��
	::CopyMemory(iTack,iCardList,sizeof(BYTE)*iCardCount);
	BYTE iBuffer[54];
	int iBufferCount=0;
	int iBaseStart,iDestStart=0,iDestEnd=0;
	int iSequenceLen=iBaseCount;
	int temp[18]={0};
	int num=0;
	//��ȡ����ը��(������ɾ������ը��)
	TackOutAllBomb(iTack,iTackCount,iBuffer,iBufferCount);
	RemoveCard(iBuffer,iBufferCount,iTack,iTackCount);
	iTackCount-=iBufferCount;
	RemoveCard(iReplaceBuffer,kingcount,iTack,iTackCount);
	iTackCount-=kingcount;
	//����һ��ϵͳ����������(����������С->�����
	SortCard(iTack,NULL,iTackCount,TRUE);
	//�û����������
	for(int i=0;i<iTackCount;i++)
	{
		temp[GetCardBulk(iTack[i])]++;
	}

	switch(xSequence)
	{
		//��˳
	case 1:
		iSequenceLen = iBaseCount;
		if(!bNoComp)
			iBaseStart = GetSequenceStartPostion(iBaseCard,iBaseCount,1);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i]>=1)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				if (TempCount > 1)
				{
					TempCount = TempCount - 1;
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{

					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			
			if(GetCardBulk(iTack[j]) == iDestStart)//�ҵ�һ����
			{
				iResultCard[iResultCount++] = iTack[j];
				iDestStart++;
				iDestEnd--;
				//break;
			}
			else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					TempCount++;
					iDestStart++;
					iDestEnd--;
					j--;
				}
			}
			//�Ѿ���ȫ
			if(iDestEnd == 0)
			{
				return true;
			}
		}
		break;
	case 2:
		iSequenceLen = iBaseCount/2;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,2);
		else
			iBaseStart =3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 2)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}
			else
			{
				if (TempCount >= (2 - temp[i]))
				{
					TempCount = (TempCount - (2 - temp[i]));
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{
					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}
			else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					TempCount++;
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					
					num++;
					j--;
				}
			}

			if(num ==2)//һ���Ѿ��ҵ�
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}
		}
		break;
	case 3:
		iSequenceLen = iBaseCount/3;
		if(!bNoComp)
			iBaseStart=GetSequenceStartPostion(iBaseCard,iBaseCount,3);
		else
			iBaseStart = 3;
		for(int i=iBaseStart+1;i<15;i++)
		{
			if(temp[i] >= 3)
			{
				if(iDestStart == 0)
					iDestStart = i;
				iDestEnd++;
				if(iDestEnd == iSequenceLen)
					break;
			}else
			{
				if (TempCount >= (3 - temp[i]))
				{
					TempCount = (TempCount - (3 - temp[i]));
					if(iDestStart == 0)
						iDestStart = i;
					iDestEnd++;
					if(iDestEnd == iSequenceLen)
						break;
				}
				else
				{
					i = i - iDestEnd;
					iDestStart = 0;
					iDestEnd = 0;
					TempCount = ReplaceCount;
				}
			}
		}
		if(iDestEnd != iSequenceLen)
			return false;
		num=0;
		//��ȡ����
		for(int j=0;j<iTackCount;j++)
		{
			if(GetCardBulk(iTack[j]) == iDestStart)
			{
				iResultCard[iResultCount++] = iTack[j];
				num++;
			}else
			{
				if (GetCardBulk(iTack[j]) > iDestStart)
				{
					TempCount++;
					iResultCard[iResultCount++] = iReplaceBuffer[TempCount-1];
					
					num++;
					j--;
				}
			}
			if(num == 3)//�ҵ�����
			{
				num=0;
				iDestStart++;
				iDestEnd--;
				//�Ѿ���ȫ
				if(iDestEnd == 0)
					return true;
			}

		}

		break;
	}
	return FALSE;
}