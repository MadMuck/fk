#pragma once

//#include "StdAfx.h"

#include <vector>


//��������
#define	UG_HUA_MASK					0xF0			//1111 0000
#define	UG_VALUE_MASK				0x0F			//0000 1111

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//����	0100 0000
#define UG_ERROR_HUA				0xF0			//����  1111 0000

//�˿˳�������
#define UG_ERROR_KIND							0				//����

#define UG_ONLY_ONE								1				//����
#define UG_DOUBLE								2				//����

#define UG_VARIATION_STRAIGHT					3				//����˳��(A2345)˳������С
#define UG_STRAIGHT								4               //˳��,5+��������
#define UG_FLUSH								5				//ͬ��(����)

#define UG_STRAIGHT_FLUSH						47               //ͬ��˳,��ɫ��ͬ��˳��
#define UG_VARIATION_STRAIGHT_FLUSH				46               //ͬ��˳,��ɫ��ͬ��˳��

#define UG_THREE								8				//����
#define UG_THREE_ONE							9               //3 �� 1
#define UG_THREE_TWO							10               //3 �� 2
#define UG_THREE_DOUBLE							11				//3 ��1��

#define UG_VARIATION_DOUBLE_SEQUENCE			12				//����˫˳(AA22)��С
#define UG_DOUBLE_SEQUENCE						13				//����,2+�������Ķ���

#define UG_VARIATION_THREE_SEQUENCE				14				//������˳(AAA222��С)
#define UG_THREE_SEQUENCE						15				//�����ţ�2+������������

#define UG_VARIATION_THREE_ONE_SEQUENCE			16				//������˳��һ
#define UG_THREE_ONE_SEQUENCE					17              //2+������������һ

#define UG_VARIATION_THREE_TWO_SEQUENCE			18				//������˳����
#define UG_THREE_TWO_SEQUENCE					19				//2+��������������

#define UG_VARIATION_THREE_DOUBLE_SEQUENCE		20				//���������Ŵ���
#define UG_THREE_DOUBLE_SEQUENCE				21				//�����Ŵ���

#define UG_VARIATION_THREE_SEQUENCE_DOUBLE_SEQUENCE		22		//���ֺ���(��˳����˳)
#define UG_THREE_SEQUENCE_DOUBLE_SEQUENCE		23				//����(��˳����˳)

#define UG_FOUR_ONE								24				//�Ĵ�һ
#define UG_FOUR_TWO								25				//�Ĵ�����
#define UG_FOUR_ONE_DOUBLE						26				//�Ĵ�һ��
#define UG_FOUR_TWO_DOUBLE						27				//�Ĵ�����

#define UG_VARIATION_FOUR_SEQUENCE				28				//������˳
#define UG_FOUR_SEQUENCE						29				//��˳

#define UG_VARIATION_FOUR_ONE_SEQUENCE			30				//�Ĵ�һ˳
#define UG_FOUR_ONE_SEQUENCE					31				//�Ĵ�һ˳

#define UG_VARIATION_FOUR_TWO_SEQUENCE			32				//�Ĵ���˳
#define UG_FOUR_TWO_SEQUENCE					33				//�Ĵ���˳

#define UG_VARIATION_FOUR_ONE_DOUBLE_SEQUENCE	34				//�Ĵ���˳
#define UG_FOUR_ONE_DOUBLE_SEQUENCE				35				//�Ĵ���˳

#define UG_VARIATION_FOUR_TWO_DOUBLE_SEQUENCE	36				//�Ĵ�����˳
#define UG_FOUR_TWO_DOUBLE_SEQUENCE				37				//�Ĵ�����˳


#define UG_SLAVE_510K							38              //510Kը��,��ɫ��ͬ
#define UG_MASTER_510K							39              //510Kͬ��ը��

#define UG_BOMB									45              //ը��>=4��
#define UG_KING_BOMB							50				//��ը(���ը��)


#define KING_COUNT								4				//�������ĸ���
//�˿˷����ṹ	��ֻ���ڷ������Ա��˿ˣ�
struct CardAnalyseStruct
{
	int		iSingleCount;	  //������Ŀ
	int		iDoubleCount;	  //������Ŀ
	int		iThreeCount;	  //������Ŀ
	int		iBombCount;		  //ը����
	int		i510KCount;		  //510K��Ŀ
	int		i510KMastCount;	  //��510K��Ŀ
//	BYTE	iBigsingleCard;	  //�����
//	BYTE	iLesssingleCard;  //��С����

	BYTE	iSingleArray[54]; //����
	BYTE	iDoubleArray[54]; //��������
	BYTE	iThreeArray[54];  //��������
	BYTE	iBombArray[54];	  //�������������� ��0 �ָ���ȣ����������ݣ�
	BYTE	i510KArray[54];	  //�������������� ��0 �ָ���ȣ����������ݣ�
};

//510K�߼��� ֧�� 2 ���˿ˣ�
class CUpGradeGameLogic
{
	//��������
private:
	BYTE			m_iNTNum;		        //��������
	int				m_iStation[5];	        //���λ�ã����飬÷�������ң����ң����ƣ�
	BYTE			m_bSortCardStyle;       //����ʽ
	int				m_iCondition;		    //��������
	BOOL			m_bKingCanReplace;		//���Ƿ�ɵ�
	DWORD			m_iCardShapeRestrict1;	//�����б�
	DWORD			m_iCardShapeRestrict2;	//�����б�

	BYTE			m_iReplaceCardArray[54];//���������
	BYTE			m_iReplaceCardCount;	//����������L��
	//��������
public:
	//���캯��		
	CUpGradeGameLogic(void);
	//��������
	virtual ~CUpGradeGameLogic();

	//���ܺ���������������
public:
	//[�������]
	//������������
	inline void SetNTCardNum(BYTE iNTNum) { m_iNTNum=iNTNum;}
	//��ȡ��������
	inline BYTE GetNTCardNum() { return m_iNTNum; }
	//�O�ô��������
	BOOL SetReplaceCard(BYTE iCardList[],int iCardCount);
	//��ȡ�����пɴ���������
	int GetReplaceCardCount(BYTE iCardList[],int iCardCount);
	//��ȡ�˿�����
	inline int GetCardNum(BYTE iCard) { return (iCard&UG_VALUE_MASK)+1; }
	//��ȡ�˿˻�ɫ(Ĭ��Ϊ��ʵ��ɫ)
	BYTE GetCardHuaKind(BYTE iCard, BOOL bTrueHua = TRUE);
	//��ȡ�˿���Դ�С(Ĭ��Ϊ�ƴ�С,�������С)	bBulkOrNum false:��ʾ�����ơ�true:��ʾ��������
	int GetCardBulk(BYTE iCard,BOOL bSortOrCmp = FALSE,bool bBulkOrNum = false);
	//��ȡ�˿���ͨ����Դ�С
	BYTE GetCardByValue(int iCardValue);
	//�O�������Ԯ���
	void SetKingCanReplace(BOOL bKingCanReplace=false){m_bKingCanReplace=bKingCanReplace;}
	//��������
	void SetCardShapeRestrict(DWORD iCardShape1,DWORD iCardShape2 )
	{
		m_iCardShapeRestrict1 = iCardShape1;
		m_iCardShapeRestrict2 = iCardShape2;
	}
	//�Cȡ���Ƿ���Ԯ���
	BOOL GetKingCanReplace(){return m_bKingCanReplace;}
	//��������ʽ
	void SetSortCardStyle(BYTE SortCardStyle){m_bSortCardStyle = SortCardStyle;}
	//��ȡ����ʽ
	BYTE GetSortCardStyle(){return m_bSortCardStyle;}
	//��ȡ�˿�(���ݻ�ɫ,����ȷ����)
	BYTE GetCardFromHua(int iHuaKind, int iNum)
	{
		if (iHuaKind!=UG_ERROR_HUA) return (iHuaKind+iNum-1);
		return 0;
	}
	//[����]
public:
	//�����˿�,����С(����ϵͳ����)
	BOOL SortCard(BYTE iCardList[], BYTE bUp[], int iCardCount,BOOL bSysSort = FALSE);
	//��ת��˳(�ӵ�->��)
	BOOL ReverseCard(BYTE iCardList[], BYTE bUp[], int iCardCount);
	//����������
	BOOL SortCardByStyle(BYTE iCardList[],int iCardCount);
	//����ɫ����
	BOOL SortCardByKind(BYTE iCardList[],int iCardCount);
public:
	//�����˿�
	BYTE RandCard(BYTE iCard[], int iCardCount,bool bHaveKing=false);		//bHaveKing��ʾ�Ƿ��д�Сè,false��,ture��
	//ɾ���˿�
	int RemoveCard(BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount);
private:
	//��� 0 λ�˿�
	int RemoveNummCard(BYTE iCardList[], int iCardCount);

public://[��������]
	//�Աȵ���
	BOOL CompareOnlyOne(BYTE iFirstCard, BYTE iNextCard);
	//���ҷ���
	int FindPoint(BYTE iCardList[], int iCardCount);
	//�Ƿ�Ϊͬһ������
	BOOL IsSameNumCard(BYTE iCardList[],int iCardCount);
	//�Ƿ�Ϊͬһ������(����������߼�)
	BOOL IsSameNumCardAndKing(BYTE iCardList[],int iCardCount);
	//�Ƿ�Ϊͬһ��ɫ
	BOOL IsSameHuaKind(BYTE iCardList[],int iCardCount);
	//�Ƿ�Ϊͬһ��ɫ(�����������߼�)
	BOOL IsSameHuaKindAndKing(BYTE iCardList[],int iCardCount);
	//���� >=4 ը��������ը������
	BYTE GetBombCount(BYTE iCardList[], int iCardCount,int iNumCount = 4);
	//��ȡָ����С�Ƹ���
	BYTE GetCountBySpecifyNumCount(BYTE iCardList[],int iCardCount,int Num);
	//��ȡָ���Ƹ���
	BYTE GetCountBySpecifyCard(BYTE iCardList[],int iCardCount,BYTE bCard);
	//��ȡָ���������ƴ�С(������ֻ�ܹ���һ���Ƶ�����ΪiCount,��Ȼ����ȥ�Ľ��ǵ�һ��ָ��������ֵ)
	BYTE GetBulkBySpecifyCardCount(BYTE iCardList[],int iCardCount,int iCount);
	//���ô������߼�(��ָ��������)
	BYTE GetBulkBySpecifyCardCountAndKing(BYTE iCardList[],int iCardCount,int iCount);

	//�Ƿ�Ϊĳָ����˳��(����˳��)
	BOOL IsVariationSequence(BYTE iCardList[], int iCardCount, int iCount);
	//�Ƿ�Ϊĳָ����˳��(����˳��)(����������߼�)
	BOOL IsVariationSequenceAndKing(BYTE iCardList[], int iCardCount, int iCount);
	//�Ƿ�Ϊĳָ����˳��
	BOOL IsSequence(BYTE iCardList[], int iCardCount, int iCount);
	//�Ƿ����˳��
	BOOL IsSequenceAndKing(BYTE iCardList[],int iCardCount,int iCount);
	//��ȡָ������
	BYTE TackOutBySpecifyCard(BYTE iCardList[], int iCardCount,BYTE bCardBuffer[],int &iResultCardCount,BYTE bCard);
	//��ȡĳ��ָ�����ֵ���
	BOOL TackOutCardBySpecifyCardNum(BYTE iCardList[],int iCardCount,BYTE iBuffer[],int &iBufferCardCount,BYTE iCard);
	//��ȡ���з�����������,����,����,����,4ը������
	int TackOutBySepcifyCardNumCount(BYTE iCardList[], int iCardCount, BYTE iDoubleBuffer[],BYTE bCardNum);
	//��ȡָ����ɫ��
	int TackOutByCardKind(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],BYTE iCardKind);
	//���(�������ƶ�Ĳ���ٵ�)
	int TackOutMuchToFew(BYTE iCardList[],int iCardCount,BYTE iDoubleBuffer[],int &iBufferCardCount,BYTE iCardMuch,BYTE iCardFew);
	//���Ҵ���iCard�ĵ�������iCardList�е����
	BYTE GetSerialByMoreThanSpecifyCard(BYTE iCardList[],int iCardCount,int iCard,BYTE iBaseCardCount, bool bExtValue=false);
	//����==iCard�ĵ�������iCardList�е����(��ʼλ��,���K�cλ��)
	int GetSerialBySpecifyCard(BYTE iCardList[],int iStart,int iCardCount,BYTE iCard,int temp = -1);
	//��ȡָ��˳�����Ƶ���Сֵ(iSequence ����˳�ӵ��������Ϊ
	BYTE GetBulkBySpecifySequence(BYTE iCardList[],int iCardCount ,int iSequence = 3);
	//��ȡָ��˳�����Ƶ����ֵ����˳��
	BYTE GetBulkBySpecifyVariationSequence(BYTE iCardList[],int iCardCount ,int iSequence = 3);
	//������С (1) or ��� (255) �� ,bBulkOrNum false:��ʾ�����ơ�true:��ʾ��������
	int	GetBulkBySepcifyMinOrMax(BYTE iCardList[], int iCardCount, int MinOrMax,bool bBulkOrNum = false);
	//�õ����������
	BYTE GetMaxCard(BYTE iCardList[],int iCardCount);
	//�õ�������С��
	BYTE GetMinCard(BYTE iCardList[],int iCardCount);
	////////////////////////////////////////////////////////////////////////////////////////////////////
	//[�������]
public:
    //��ȡ����
	BYTE GetCardShape(BYTE iCardList[], int iCardCount);
	//�Ƿ���
	inline BOOL IsOnlyOne(BYTE iCardList[], int iCardCount) { return iCardCount==1; };
	//�Ƿ����
	BOOL IsDouble(BYTE iCardList[], int iCardCount);
	//�Ƿ�����
	BOOL IsThree(BYTE iCardList[],int iCardCount);
	//3 �� 1or2(��һ�Դ������Ż��һ����
	BOOL IsThreeX(BYTE iCardList[], int iCardCount, int iX/*1or2*/);
	//3��1or 2(��һ�Դ��������´�һ����)����������߼�
	BOOL IsThreeXAndKing(BYTE iCardList[],int iCardCount,int iX);
	//��ը
	BOOL IsKingBomb(BYTE iCardList[],int iCardCount);
	//4+���� ը��
	BOOL IsBomb(BYTE iCardList[], int iCardCount);
	//ͬ��ը��
	BOOL IsBombSameHua(BYTE iCardList[],int iCardCount);
	//ͬ��(��˳��)
	BOOL IsFlush(BYTE iCardList[],int iCardCount);
	//�Ƿ���ͬ��˳
	BOOL IsStraightFlush(BYTE iCardList[], int iCardCount);
	//�Ƿ����ͬ��˳
	BOOL IsVariationStraightFlush(BYTE iCardList[], int iCardCount);
	//���ֵ�˦
	BOOL IsVariationStraight(BYTE iCardList[],int iCardCount);
	//��˦
	BOOL IsStraight(BYTE iCardList[],int iCardCount);
	//���ֶ�˦
	BOOL IsVariationDoubleSequence(BYTE iCardList[],int iCardCount);
	//��˦ //����?
	BOOL IsDoubleSequence(BYTE iCardList[],int iCardCount);

	//�Ƿ����������������X(0,1,2,3)
	BOOL IsVariationThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/);

	//�Ƿ�������������X(0,1,2,3)
	BOOL IsThreeXSequence(BYTE iCardList[], int iCardCount, int iSeqX/*0,1or2*/);
	//�Ƿ���˳����˳(����)
	BOOL IsThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount);
	//�Ƿ���˳����˳(����)
	BOOL IsVariationThreeSequenceDoubleSequence(BYTE iCardList[], int iCardCount);
	//510K ը��
	BOOL IsSlave510K(BYTE iCardList[], int iCardCount);
	//510K ͬ��ը��
	BOOL IsMaster510K(BYTE iCardList[], int iCardCount);
	//�Ĵ�һ�����Ĵ���
	BOOL IsFourX(BYTE iCardList[],int iCardCount,int iX/*1or 2*/);//����1,������2,һ����,2��4
	//�Ƿ�����Ĵ�X˳
	BOOL IsVariationFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX);
	//�Ĵ�һ�����Ĵ�����˳��
	BOOL IsFourXSequence(BYTE iCardList[],int iCardCount,int iSeqX);
	//[�������]
public:
	//�Զ����ƺ���
	BOOL AutoOutCard(BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut,BOOL bIsCue = FALSE);
	//�Ƿ���Գ���
	BOOL CanOutCard(BYTE iOutCard[], int iOutCount, BYTE iBaseCard[], int iBaseCount, BYTE iHandCard[], int iHandCount,BOOL bFirstOut = FALSE);
	
	//[��ȡ��]
public:	
	//��ȡ����������1 or 2or 3(��,һ��,�������)
	BYTE TackOutThreeX(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount,int  xValue);
	//��ȡ2����������������1,2
	BOOL TrackOut3XSequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount, int xValue);
	//��ȡ2����������������1,2
	BOOL TrackOut3Sequence2Sequence(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount);
	//��ȡ˳������Сλ��ֵ(xSequence��ʾĬ�ϵ�˳)
	int GetSequenceStartPostion(BYTE iCardList[],int iCardCount,int xSequence=1);
	//��ȡ���ŵ�˳��,����˳��,����˳��
	BOOL TackOutSequence(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[],int &iResultCardCount,int xSequence,BOOL bNoComp = false);
	//��ȡͬ��˳
	BOOL TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[], int &iResultCardCount);
	//��ȡͬ��˳
	BOOL TackOutStraightFlush(BYTE iCardList[],int iCardCount,BYTE iResultCard[], int &iResultCardCount);
	//��ȡ����ը��
	BOOL TackOutAllBomb(BYTE iCardList[],int iCardCount,
									   BYTE iResultCard[],int &iResultCardCount,int iNumCount=4);
	//��ȡը��(����Ĭ��Ϊ4)
	BOOL TackOutBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,int iNumCount=4);
	//��ȡ��ը
	BOOL TackOutKingBomb(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount);
	//��ȡ510K
	BOOL TrackOut510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BOOL bIsMast510K = FALSE);
	//����510K
	BOOL Test510K(BYTE iCardList[],int iCardCount);
	//����
	BOOL Copy510K(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount);
	//���������
	BOOL TackOutCardByNoSameShape(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount);
	//�ô�����ƴ������ϵ���
	BOOL TackOutMoreThanLastShape(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount,int ishape);
public:
	//��������������С���Ǹ����ŵ��Ƶ�
	int SearchMinThreeSeq(BYTE iCardList[], int iCardCount);
	//������������
	void ResetCondition();
	//��������ĳ��������
	BOOL TackOutByCondition(BYTE iCardList[],int iCardCount, BYTE iResultCard[],
								int & iResultCardCount,
								BYTE iConditionCard[],int iConditionCardCount);
	BOOL TackOutAddLevelCard(BYTE iCardList[],int iCardCount,BYTE iResultCard[],int &iResultCardCount,BYTE iBaseCard[],int iBaseCardCount);
	
	//���ұȵ�ǰ���ƴ��
	BOOL TackOutCardMoreThanLast(BYTE iHandCard[], int iHandCardCount,BYTE iBaseCard[], int iBaseCardCount,
									BYTE iResultCard[], int & iResultCardCount,bool bExtVal=false, BOOL bIsCue = FALSE);
	//��ȡ���浥��������1 or 2or 3(��,һ��,�������)
	BYTE TackOuttReplaceThreeX(BYTE iCardList[], int iCardCount, BYTE iBaseCard[], int iBaseCount, BYTE iResultCard[], int &iResultCardCount,int  xValue);
	//��ȡ���ŵ�˳��,����˳��,����˳��
	BOOL TackOutReplaceSequence(BYTE iCardList[],int iCardCount,BYTE iBaseCard[],int iBaseCount,BYTE iResultCard[],int &iResultCardCount,int xSequence,BOOL bNoComp = false);
	//�ж��ǲ��Ƕ๦����
	BOOL JudgeCard(BYTE iCard);
};
