#include "StdAfx.h"
#include "UpGradeLogic.h"
//���캯��
CUpGradeGameLogic::CUpGradeGameLogic( void )
{
	//m_iNTNum=0;
	//m_iNTHuaKind=UG_ERROR_HUA;
	m_iStation[4]=500;
	for ( int i=0;i<4;i++ ) m_iStation[i]=100*i;

}


//��ȡ�˿˻�ɫ
BYTE CUpGradeGameLogic::GetCardHuaKind( BYTE iCard )
{ 
	int iHuaKind=( iCard&UG_HUA_MASK );

	return iHuaKind; 
}

//��ȡ�˿˴�С ��2 - 18 �� 15 ���������� �� 2 - 21 �� 15 ����������
int CUpGradeGameLogic::GetCardBulk( BYTE iCard, BOOL bExtVol )
{
	if ( ( iCard==0x4E )||( iCard==0x4F ) )
	{
		return bExtVol?( iCard-14 ):( iCard-62 );	//��С��64+14-62=16			ֻ���ش�Сè��ֵ
	}

	int iCardNum=GetCardNum( iCard );
	int iHuaKind=GetCardHuaKind( iCard );

	if ( iCardNum==14 )	// AΪ1��, ţţ������	
	{
		if( bExtVol )		
			return ( ( iHuaKind>>4 )+( 1*4 ) );
		else
			return 1;
	}
	return ( ( bExtVol )?( ( iHuaKind>>4 )+( iCardNum*4 ) ):( iCardNum ) );
}

//ר��Ϊ˳���������أ��߻�˵2������˳�ӣ��ǾͺͶ�����һ����
int CUpGradeGameLogic::GetCardBulkEx(BYTE iCard, BOOL bAToOne)
{
	int iCardNum = GetCardNum(iCard);
	int iHuaKind = GetCardHuaKind(iCard);

	//˳��ţ1��2��3��4��5��ʱ��������ʱ��A���ǵ���1��
	if(bAToOne && iCardNum == 14)
	{
		iCardNum = 1;
	}

	return iCardNum;
}

//�����˿�
BOOL CUpGradeGameLogic::SortCard( BYTE iCardList[], BYTE bUp[], int iCardCount )
{
	BOOL bSorted=TRUE, bTempUp;
	int iTemp, iLast=iCardCount-1, iStationVol[45];

	//��ȡλ����ֵ
	for ( int i=0;i<iCardCount;i++ )
	{
		iStationVol[i]=GetCardBulk( iCardList[i], TRUE );
		///if ( iStationVol[i]>=15 ) iStationVol[i]+=m_iStation[4];
		///else iStationVol[i]+=m_iStation[GetCardHuaKind( iCardList[i], FALSE )>>4];
	}

	//�������
	do
	{
		bSorted=TRUE;
		for ( int i=0;i<iLast;i++ )
		{
			if ( iStationVol[i]<iStationVol[i+1] )
			{	
				//����λ��
				iTemp=iCardList[i];
				iCardList[i]=iCardList[i+1];
				iCardList[i+1]=iTemp;
				iTemp=iStationVol[i];
				iStationVol[i]=iStationVol[i+1];
				iStationVol[i+1]=iTemp;
				if ( bUp!=NULL )
				{
					bTempUp=bUp[i];
					bUp[i]=bUp[i+1];
					bUp[i+1]=bTempUp;
				}
				bSorted=FALSE;
			}	
		}
		iLast--;
	} while( !bSorted );

	return TRUE;
}



//��������
BOOL CUpGradeGameLogic::ReSortCard( BYTE iCardList[], int iCardCount )
{
	SortCard( iCardList, NULL, iCardCount );
	//====�������Ŵ�С
	int iStationVol[45];
	for ( int i=0;i<iCardCount;i++ )
	{
		iStationVol[i]=GetCardBulk( iCardList[i], false );
	}

	int Start=0;
	int j, step;
	BYTE CardTemp[8];					//��������Ҫ��λ������
	int CardTempVal[8];					//����������λ������ֵ
	for( int i=8;i>1;i-- )				//����������һ������i����ͬ��ֵ
	{
		for( j=Start;j<iCardCount;j++ )
		{
			CardTemp[0]=iCardList[j];			//���浱ǰi��������ͬ��ֵ
			CardTempVal[0]=iStationVol[j];
			for( step=1;step<i&&j+step<iCardCount; )			//��һ������i��ֵ��ȵ�����( ����������ʱ������ )
			{
				if( iStationVol[j]==iStationVol[j+step] )							
				{
					CardTemp[step]=iCardList[j+step];			//������������
					CardTempVal[step]=iStationVol[j+step];		//��ֵ
					step++;
				}
				else
					break;
			}

			if( step>=i )			//�ҵ�һ������i����ȵ����鴮��ʼλ��Ϊj, ����λ��Ϊj+setp-1
			{					//����Start��ʼ��j���������setp��
				if( j!=Start )				//�ų���ʼ��������
				{
					for( ;j>=Start;j-- )					//��Start����j�ź��ƶ�i��
					{
						iCardList[j+i-1]=iCardList[j-1];
						iStationVol[j+i-1]=iStationVol[j-1];
					}
					for( int k=0;k<i;k++ )				
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
	return true;
}


//��ȡ�˿�
BYTE CUpGradeGameLogic::GetCardFromHua( int iHuaKind, int iNum )
{
	if ( iHuaKind!=UG_ERROR_HUA ) return ( iHuaKind+iNum-1 );
	return 0;
}

//�Ƿ����
BOOL CUpGradeGameLogic::IsDouble( BYTE iCardList[], int iCardCount )
{
	if ( iCardCount!=3 )
		return FALSE;
	int temp[17]={0}, itwo=0;
	for( int i=0;i<iCardCount;i++ )
		temp[GetCardNum( iCardList[i] )]++;

	for( int i=0;i<17;i++ )
	{
		if( temp[i]==2 )
			itwo++;
	}
	return ( itwo==1 );
}

//�Ƿ�����
BOOL CUpGradeGameLogic::IsThree( BYTE iCardList[], int iCardCount )
{
	if ( iCardCount!=3 )
		return FALSE;
	int temp[17]={0}, ithree=0;;
	for( int i=0;i<iCardCount;i++ )
		temp[GetCardNum( iCardList[i] )]++;

	for( int i=0;i<17;i++ )
	{
		if( temp[i]==3 )
			ithree++;
	}
	return ( ithree==1 );
}
/*
//�Ƿ����ԣ��������ӣ�
BOOL CUpGradeGameLogic::IsCompleDouble( BYTE iCardList[], int iCardCount )
{
if ( iCardCount <4 )
return FALSE;

int temp[17]={0}, itwo=0;;
for( int i=0;i<iCardCount;i++ )
temp[GetCardNum( iCardList[i] )]++;

for( int i=0;i<17;i++ )
{
if( temp[i]==2 )
itwo++;
}
return ( itwo==2 );
}
*/
//�Ƿ�ͬ��( ͬ��Ϊ������Ϊһ�ֻ�ʽ )
BOOL CUpGradeGameLogic::IsSameHua( BYTE iCardList[], int iCardCount )
{
	if ( iCardCount != 3 ) return FALSE;
	int hs = -1;
	SortCard( iCardList, NULL, iCardCount );
	for ( int i = 0;i < iCardCount;i++ )
	{
		int hua = GetCardHuaKind( iCardList[i] );
		if ( hs < 0 )
		{
			hs = hua;
			continue;
		}
		if ( hs != hua ) return FALSE;
	}
	return TRUE;
}

//�Ƿ�Ϊͬ��ţ
bool CUpGradeGameLogic::IsTonghuaNiu( BYTE iCardList[], int iCardCount )
{
	if ( 5 != iCardCount ) return false;
	int iHs = GetCardHuaKind( iCardList[0] );
	for ( int i = 1; i < iCardCount; ++i )
	{
		if ( GetCardHuaKind( iCardList[i] ) != iHs ) return false;
	}
	return true;
}

BOOL CUpGradeGameLogic::IsSpecial( BYTE iCardList[], int iCardCount )
{
	if( iCardCount!=3 )
		return false;
	SortCard( iCardList, NULL, iCardCount );
	if( GetCardNum( iCardList[0] )==5
		&&GetCardNum( iCardList[1] )==3
		&&GetCardNum( iCardList[2] )==2 )
		return true;
	return false;
}

//�Ƿ�Ϊ��«������һ�����ƴ�2��һ�����ƣ�
BOOL CUpGradeGameLogic::IsHuLu( BYTE iCardList[], int iCardCount )
{
	return GetHuLuNum(iCardList, iCardCount) != INVALID;
}

//��ȡ��«��
BYTE CUpGradeGameLogic::GetHuLuNum(BYTE iCardList[],int iCardCount )
{
	if (5 != iCardCount) return INVALID;
	
	BYTE byRetValue = INVALID;

	BYTE temp[18] = {0};

	for(int i = 0;i < iCardCount;i ++)
		temp[GetCardBulk(iCardList[i])]++;

	bool bThreeCard = false;
	bool bTwoCard = false;
	for(int i = 0;i< 18;i++)
	{
		if(temp[i] == 3)
		{
			byRetValue = i;
			bThreeCard = true;
		}
		else if (temp[i] == 2)
		{
			bTwoCard = true;
		}
	}
	return bThreeCard && bTwoCard ? byRetValue : INVALID;
}

/*
//�Ƿ�Ϊ��֧��4��ͬ����С���ƣ�
BOOL CUpGradeGameLogic::IsTieZhi( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 4 ) 
return FALSE;
int pd = -1;
for ( int i = 0;i < iCardCount;i++ )
{
int dian = GetCardNum( iCardList[i] );
if ( pd < 0 )
{
pd =dian;
continue;
}
if ( pd != dian ) 
return FALSE;
}
return TRUE;
}
*/
//�Ƿ�ͬ��˳��5�ţ�
BOOL CUpGradeGameLogic::IsSameHuaContinue( BYTE iCardList[], int iCardCount )
{
	if ( iCardCount != 3 ) 
		return FALSE;

	if ( !IsSameHua( iCardList, iCardCount ) ) 
		return FALSE;
	if ( !IsSingleContinue( iCardList, iCardCount ) ) return FALSE;

	return TRUE;
}
////�Ƿ�ڽܿ�
//BOOL CUpGradeGameLogic::IsBlackJack( BYTE iCardList[], int iCardCount, bool Split )
//{
//	if ( Split ) //�������û�кڽܿ�, 
//		return false;
//	if ( iCardCount != 2 )//�ڽܿ�ֻ������
//		return false;
//	if ( GetCardPoint( iCardList, iCardCount, Split ) != 22 )//�ڽܿ˵ĵ���Ϊ21��
//		return false;
//	return true;
//}

//�Ƿ�˳( ����3�� )
BOOL CUpGradeGameLogic::IsSingleContinue( BYTE iCardList[], int iCardCount )
{
	if ( iCardCount != 3 ) return FALSE;

	//if ( IsSameHua( iCardList, iCardCount ) ) return FALSE;

	SortCard( iCardList, NULL, iCardCount );

	int hs = -1;
	int temp1, temp2, temp3;
	temp1=GetCardNum( iCardList[0] );
	temp2=GetCardNum( iCardList[1] );
	temp3=GetCardNum( iCardList[2] );
	for ( int i = 0;i < iCardCount - 1;i++ )
	{
		//if ( GetCardNum( iCardList[i] ) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F )//���ܺ�2�ʹ�С��
		//	return FALSE;

		if ( GetCardNum( iCardList[i] ) % 15 != ( GetCardNum( iCardList[i+1] ) + 1 ) % 15 )
		{
			if( temp1==14 && temp2==3 &&temp3==2 )//1, 2, 3��˳��
				return true;
			else 
				return FALSE;

		}
	}
	return TRUE;
}

void CUpGradeGameLogic::SortByValue( BYTE iCardList[], int iCardCount, int iShape)
{
	bool bHave2 = false;
	for(int i= 0;i < iCardCount;i++)
	{
		if(GetCardBulkEx(iCardList[i]) == 2)
		{
			bHave2 = true;
			break;
		}
	}

	bool bATo2 = ( (iShape == UG_BULL_ShunZi || iShape == UG_BULL_TongHuaShun) && bHave2 );

	for (int i = 0; i < iCardCount - 1; ++i)
	{
		for (int j = 0; j < iCardCount - i - 1; j++)
		{
			if ( GetCardBulkEx(iCardList[j], bATo2) > GetCardBulkEx(iCardList[j + 1], bATo2) ) 
			{
				BYTE temp = iCardList[j];
				iCardList[j] = iCardList[j + 1];
				iCardList[j + 1] = temp;
			}
		}
	}
}

//�Ƿ���˳��ţ
bool CUpGradeGameLogic::IsShunZiNiu( BYTE iCardList[], int iCardCount )
{
	if ( 5 != iCardCount ) return false;

	bool bHave2 = false;
	for(int i= 0;i < iCardCount;i++)
	{
		if(GetCardBulkEx(iCardList[i]) == 2)
		{
			bHave2 = true;
			break;
		}
	}

	const int iCount = 1;

	BYTE temp[18]={0};
	for(int i= 0;i < iCardCount;i++)
	{
		temp[GetCardBulkEx(iCardList[i], bHave2)]++;
	}

	for(int i = 0; i < 15 ; i ++)
	{
		if(temp [i]!= 0 &&temp[i] !=iCount)	//��ָ��˳
			return false;
	}

	int len = iCardCount / iCount;

	for(int i=0;i<15;i++)
	{
		if(temp[i] != 0)//��ֵ
		{
			for(int j = i;j < i + len  ;j ++)
			{
				if(temp[j] != iCount || j >=15 )
					return false;
			}
			return true;
		}
	}
	return false;
}

//�Ƿ�˫˳( ��������3�� )
/*BOOL CUpGradeGameLogic::IsDoubleContinue( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 6 || iCardCount % 2 != 0 ) return FALSE;

SortCard( iCardList, NULL, iCardCount );

for ( int i = 0;i < iCardCount - 2;i++ )
{
if ( GetCardNum( iCardList[i] ) == 2 || iCardList[i] == 0x4E || iCardList[i] == 0x4F )
return FALSE;
if ( ( GetCardNum( iCardList[i] ) % 15 ) != ( ( GetCardNum( iCardList[i+2] ) + 1 ) % 15 ) )
return FALSE;
}
return TRUE;
}

//�Ƿ���˳������ͬ������3����
BOOL CUpGradeGameLogic::IsThreeContinue( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 9 || iCardCount % 3 != 0 ) return FALSE;

SortCard( iCardList, NULL, iCardCount );

for ( int i = 0;i < iCardCount - 3;i++ )
{
if ( GetCardNum( iCardList[i] ) == 2 || iCardList[i] == 0x4E || iCardList[0] == 0x4F )
return FALSE;
if ( ( GetCardNum( iCardList[i] ) % 15 ) != ( ( GetCardNum( iCardList[i+3] ) + 1 ) % 15 ) )
return FALSE;
}
return TRUE;
}

//�Ƿ�ɻ������( ��˳+ͬ�����ĵ��Ż���� )
BOOL CUpGradeGameLogic::IsPlaneAndWing( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 8 ) return FALSE;

SetCard( iCardList, iCardCount );

bool IsDaiOne = true;
int shunCount = 0, daiCount = 0;
PSTypeNode *first = NULL;

int iTempNum[5];
int iCnt = 0;
for ( POSITION pos = m_ptrPSList.GetHeadPosition( );pos != NULL; )
{
PSTypeNode *pn = ( PSTypeNode * ) m_ptrPSList.GetNext( pos );
if ( !pn ) continue;
if ( !first )
{
first = pn;
shunCount++;
iTempNum[iCnt++] = pn->mps;
continue;
}
if ( first->mcount != 3 ) return FALSE;
if ( pn->mcount == 3 )
{
if ( ( pn->mps + 1 ) % 15 == first->mps % 15 )
{
first = pn;
shunCount++;
iTempNum[iCnt++] = pn->mps;
continue;
}
else return FALSE;
}

if ( pn->mcount == 2 ) 
{
daiCount++;
IsDaiOne = false;
continue;
}
else if ( pn->mcount == 1 )
{
if ( !IsDaiOne ) return FALSE;

daiCount++;
continue;
}

}

if ( shunCount != daiCount ) return FALSE;

BYTE iBackCard[20];
iCnt = 0;
int j = 0, p = 0;
for ( int i = 0;i < iCardCount;i++ )
{
if ( j == 3 )
{
p++;
j = 0;
}
if ( GetCardNum( iCardList[i] ) == iTempNum[p] )
{
iBackCard[iCnt++] = iCardList[i];
iCardList[i] = 0;
j++;

}
}

for ( int i = 0;i < iCardCount;i++ )
{
if ( iCardList[i] != 0 ) iBackCard[iCnt++] = iCardList[i];
}

::CopyMemory( iCardList, iBackCard, sizeof( BYTE )*iCnt );

return TRUE;
}

//�Ƿ��Ĵ�������ͬ��+2���Ż�2���ƣ�
BOOL CUpGradeGameLogic::IsFourAndTwo( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 6 ) return FALSE;

SortCard( iCardList, NULL, iCardCount );

int iTempNum = -1;
for ( int i = 0;i < iCardCount - 3;i++ )
{
if ( GetCardNum( iCardList[i] ) == GetCardNum( iCardList[i+3] ) ) 
{
iTempNum = GetCardNum( iCardList[i] );
break;
}
}

if ( iTempNum < 0 ) return FALSE;

BYTE iTempCard[4];
int iCnt = 0;

for ( int i = 0;i < iCardCount;i++ )
{
if ( iTempNum != GetCardNum( iCardList[i] ) )
{
iTempCard[iCnt++] = iCardList[i];
continue;
}
}


if ( iCnt == 2 ) //�Ĵ�2����
{
if ( GetCardNum( iTempCard[0] ) == GetCardNum( iTempCard[1] ) ) return FALSE;

BYTE iBackCard[6];
int j = 0;
for ( int i = 0;i < iCardCount;i++ )
{
if ( GetCardNum( iCardList[i] ) == iTempNum ) iBackCard[j++] = iCardList[i];
}
iBackCard[4] = ( GetCardNum( iTempCard[0] ) > GetCardNum( iTempCard[1] ) ) ? iTempCard[0] : iTempCard[1];
::CopyMemory( iCardList, iBackCard, sizeof( iBackCard ) );
}
else if ( iCnt == 4 ) //�Ĵ�2����
{
if ( !( ( IsDouble( iTempCard, 2 ) && IsDouble( &iTempCard[2], 2 ) ) && ( GetCardNum( iTempCard[0] ) != GetCardNum( iTempCard[2] ) ) ) ) return FALSE;

BYTE iBackCard[8];
int j = 0;
for ( int i = 0;i < iCardCount;i++ )
{
if ( GetCardNum( iCardList[i] ) == iTempNum ) iBackCard[j++] = iCardList[i];
}
for ( int i = 0;i < 4;i++ )
{
iBackCard[j+i] = iTempCard[i];
}
::CopyMemory( iCardList, iBackCard, sizeof( iBackCard ) );
}


return TRUE;
}

//�Ƿ�510K
BOOL CUpGradeGameLogic::Is510KBomb( BYTE iCardList[], int iCardCount )
{
if ( iCardCount != 3 ) return FALSE;

bool h5 = false, 
h10 = false, 
hk = false;

SortCard( iCardList, NULL, iCardCount );
if ( GetCardNum( iCardList[0] ) == 13 ) hk = true;//13 Is K
if ( GetCardNum( iCardList[1] ) == 10 ) h10 = true;
if ( GetCardNum( iCardList[2] ) == 5 ) h5 =true;
if ( hk && h10 && h5 ) return TRUE;
return FALSE;
}


//�Ƿ�ը��
BOOL CUpGradeGameLogic::IsBomb( BYTE iCardList[], int iCardCount )
{
if ( iCardCount < 4 ) return FALSE;

int pd1 = GetCardNum( iCardList[0] );

for ( int i = 0;i < iCardCount;i++ )
{
if ( GetCardNum( iCardList[i] ) != pd1 ) return FALSE;
else continue;
}

return TRUE;
}

//�Ƿ���
BOOL CUpGradeGameLogic::IsRocket( BYTE iCardList[], int iCardCount )
{
if ( iCardCount != 4 ) return FALSE;
SortCard( iCardList, NULL, iCardCount );
return ( ( iCardList[0] == 0x4F )&&( iCardList[1] == 0x4F )&&( iCardList[2] == 0x4E )&&( iCardList[3] == 0x4E ) );
}

//�Ƿ�ͬ��
BOOL CUpGradeGameLogic::IsSameHua( BYTE iCardList[], int iCardCount )
{
if ( iCardCount <= 0 ) return FALSE;

int iFirstHua = GetCardHuaKind( iCardList[0], FALSE );

for ( int i = 0;i < iCardCount;i++ )
{
if ( GetCardHuaKind( iCardList[i], FALSE ) != iFirstHua ) return FALSE;
else continue;
}

return TRUE;
}
*/

/*void CUpGradeGameLogic::SetCard( BYTE iCardList[], int iCardCount )
{
if ( iCardCount <= 0 ) return;
ClearPSInfo( );

//add node
for ( int i = 0;i < iCardCount;i++ )
{
int pd = GetCardNum( iCardList[i] );
bool flag = false;

for ( POSITION pos = m_ptrPSList.GetHeadPosition( );pos != NULL; )
{
PSTypeNode * ps = ( PSTypeNode * ) m_ptrPSList.GetNext( pos );
if ( !ps ) continue;
if ( pd == ps->mps )
{
ps->mcount++;
flag = true;
break;
}
}

if ( !flag )
{
PSTypeNode * pn = new PSTypeNode( );
pn->mps = pd;
pn->mcount = 1;
m_ptrPSList.AddTail( pn );
}
}

CPtrList temp;
bool Is2In = false;

for ( POSITION pos = m_ptrPSList.GetHeadPosition( );pos != NULL; )
{
PSTypeNode * pi = ( PSTypeNode * ) m_ptrPSList.GetNext( pos );
if ( !pi ) continue;
PSTypeNode *pn = new PSTypeNode( );
pn->mps = pi->mps;
pn->mcount = pi->mcount;
temp.AddTail( pn );
if ( pn->mps == 2 ) Is2In = true;
}

ClearPSInfo( );
//sort
for ( POSITION pos = temp.GetHeadPosition( );pos != NULL; )
{
PSTypeNode *pi = ( PSTypeNode * ) temp.GetNext( pos );
if ( !pi ) continue;
PSTypeNode *pn = new PSTypeNode( );
pn->mps = pi->mps;
pn->mcount = pi->mcount;

if ( m_ptrPSList.IsEmpty( ) )
{
m_ptrPSList.AddTail( pn );
continue;
}

bool bAdd = false;
for ( POSITION pos1 = m_ptrPSList.GetHeadPosition( );pos1 != NULL; )
{	
POSITION tp = pos1;
PSTypeNode *pii = ( PSTypeNode * ) m_ptrPSList.GetNext( pos1 );
if ( !pii ) continue;

if ( ( pn->mcount > pii->mcount ) || ( ( pn->mcount == pii->mcount ) && ( pn->mps > pii->mps ) ) ) 
{
m_ptrPSList.InsertBefore( tp, pn );
bAdd = true;
continue;
}

}

if ( !bAdd )
{
m_ptrPSList.AddTail( pn );
}
}

while ( !temp.IsEmpty( ) )
{
PSTypeNode *ps = ( PSTypeNode * ) temp.RemoveHead( );
//delete ps;
}
temp.RemoveAll( );

if ( Is2In )
{
PSTypeNode *pii = ( PSTypeNode * ) m_ptrPSList.RemoveHead( );
if ( !pii ) return;
if ( pii->mps == 14 )//14ΪA
{
m_ptrPSList.AddTail( pii );
}
else
{
m_ptrPSList.AddHead( pii );
}
}

return;
}



void CUpGradeGameLogic::ClearPSInfo( )
{
while ( !m_ptrPSList.IsEmpty( ) )
{
PSTypeNode *ps = ( PSTypeNode * ) m_ptrPSList.RemoveHead( );
//delete ps;
}
m_ptrPSList.RemoveAll( );

return;
}

*/
//�õ����л�ɫ
BYTE CUpGradeGameLogic::GetCardListHua( BYTE iCardList[], int iCardCount )
{
	int iHuaKind=GetCardHuaKind( iCardList[0] );
	if ( GetCardHuaKind( iCardList[iCardCount-1] )!=iHuaKind ) return UG_ERROR_HUA;
	return iHuaKind;
}

//��ȡ����
BYTE CUpGradeGameLogic::GetCardShape( BYTE iCardList[], int iCardCount )
{
	/***************************************************
	ͬ��˳>��֧>��«>ͬ��>˳��>����>����>����>ɢ��
	***************************************************/

	//if ( iCardCount <= 0 ) 	return UG_ERROR_KIND;//�Ƿ���
	//if ( IsBlackJack( iCardList, iCardCount, false ) ) return SH_BLACK_JACK;//�ڽܿ�

	//return SH_OTHER;//ɢ��
	return 0;
}

//�Ƚ��������ƵĴ�С
int CUpGradeGameLogic::CompareCard( BYTE iFirstCard[], int iFirstCount, BYTE iSecondCard[], int iSecondCount, bool* bPatternSpe, BYTE iFirstUpCard[], BYTE iSecondUpCard[] )
{
	//***************************************************/
	BYTE iFirCard[5], iSecCard[5];

	::CopyMemory( iFirCard, iFirstCard, sizeof( BYTE )*iFirstCount );
	::CopyMemory( iSecCard, iSecondCard, sizeof( BYTE )*iSecondCount );


	SortCard( iFirCard, NULL, iFirstCount );
	SortCard( iSecCard, NULL, iSecondCount );

	BYTE iFirstCardShape = GetShape( iFirCard, iFirstCount, bPatternSpe, iFirstUpCard );
	BYTE iSecondCardShape = GetShape( iSecCard, iSecondCount, bPatternSpe, iSecondUpCard );

	if ( iFirstCardShape != iSecondCardShape ) 
	{
		return ( iFirstCardShape - iSecondCardShape > 0 ) ? 1 : -1;
	}
	else
	{
		if ( bPatternSpe != nullptr && iFirstCardShape >= UG_BULL_SILVER )
		{
			BYTE MaxFir = GetMaxCard( iFirCard, iFirstCount ), 
				MaxSec = GetMaxCard( iSecCard, iSecondCount );
			BYTE BombFir = 0;
			BYTE BombSec = 0;
			switch( iFirstCardShape )
			{
			case UG_BULL_DRAGON:
				return ( GetCardHuaKind( MaxFir ) - GetCardHuaKind( MaxSec ) > 0 ) ? 1 : -1;
				break;
			case UG_BULL_BOMB:
				BombFir = GetBombNum( iFirCard, iFirstCount );
				BombSec = GetBombNum( iSecCard, iSecondCount );
				return ( GetCardBulk( BombFir ) - GetCardBulk( BombSec ) > 0 ) ? 1 : -1;
				break;
			case UG_FIVE_SMALL:
			case UG_BULL_GOLD:
			case UG_BULL_SILVER:
				if ( GetCardBulk( MaxFir ) != GetCardBulk( MaxSec ) ) //����ţţ�ͱȽ������
					return ( GetCardBulk( MaxFir ) - GetCardBulk( MaxSec ) > 0 ) ? 1 : -1;
				else //�����Ҳһ���ͱȽ�����ƻ�
				{
					return ( GetCardHuaKind( MaxFir ) - GetCardHuaKind( MaxSec ) > 0 ) ? 1 : -1;
				}
				break;
			case UG_BULL_HuLu:
				{
					//��«�Ʋ����ܴ�С���
					BYTE byHuluFir = GetHuLuNum( iFirCard, iFirstCount );
					BYTE byHuluSec = GetHuLuNum( iSecCard, iSecondCount );
					return byHuluFir > byHuluSec ? 1 : -1;
				}

			default:
				break;
			}
		}

		BYTE MaxFir = GetMaxCard( iFirCard, iFirstCount ), 
			MaxSec = GetMaxCard( iSecCard, iSecondCount );
		if ( GetCardBulk( MaxFir ) != GetCardBulk( MaxSec ) ) //����ţţ�ͱȽ������
			return ( GetCardBulk( MaxFir ) - GetCardBulk( MaxSec ) > 0 ) ? 1 : -1;
		else //�����Ҳһ���ͱȽ�����ƻ�
		{
			return ( GetCardHuaKind( MaxFir ) - GetCardHuaKind( MaxSec ) > 0 ) ? 1 : -1;
		}
	}
	//	break;
	//}
	return 0;
}

//�Աȵ���
BOOL CUpGradeGameLogic::CompareOnlyOne( BYTE iFirstCard, BYTE iNextCard )
{
	int iFirstNum = GetCardNum( iFirstCard );//������
	int iNextNum  = GetCardNum( iNextCard );//������

	if ( iFirstCard == 0x4F ) return FALSE;//����
	if ( iNextCard == 0x4F ) return TRUE;

	if ( iFirstCard == 0x4E ) return FALSE;//С��
	if ( iNextCard == 0x4E ) return TRUE;

	if ( iFirstNum == 2 ) return FALSE;//2
	if ( iNextNum == 2 ) return TRUE;

	return ( ( iNextNum - iFirstNum ) > 0 ? TRUE : FALSE );//����

	/*int iFristHua=GetCardHuaKind( iFirstCard, FALSE );
	int iNextHua=GetCardHuaKind( iNextCard, FALSE );
	if ( iFristHua!=iNextHua )
	{
	//��ͬ��ɫ�Ա�
	if ( iFristHua==UG_NT_CARD ) return TRUE;
	return ( iNextHua!=UG_NT_CARD );
	}

	//ͬ��ɫ�Ա�
	return GetCardBulk( iFirstCard, FALSE )>=GetCardBulk( iNextCard, FALSE );*/
}

//�Զ����ƺ���
BOOL CUpGradeGameLogic::AutoOutCard( BYTE iHandCard[], int iHandCardCount, BYTE iBaseCard[], int iBaseCardCount, 
	BYTE iResultCard[], int & iResultCardCount, BOOL bFirstOut )
{
	if ( bFirstOut == TRUE )
	{
		iResultCard[0] = iHandCard[iHandCardCount-1];
		iResultCardCount = 1;
	}
	return TRUE;
}


//��� 0 λ�˿�
int CUpGradeGameLogic::RemoveNummCard( BYTE iCardList[], int iCardCount )
{
	int iRemoveCount=0;
	for ( int i=0; i<iCardCount; i++ )
	{
		if ( iCardList[i]!=0 ) iCardList[i-iRemoveCount]=iCardList[i];
		else iRemoveCount++;
	}
	return iRemoveCount;
}

//�����˿�
//bFaceCard: true -> �л���, 	false -> �޻���
BYTE CUpGradeGameLogic::RandCard( BYTE iCard[], int iCardCount, bool bhaveKing )
{
	static const BYTE m_CardArray[54]={
		0x01, 0x02 , 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 		//���� 2 - A
		0x11, 0x12 , 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 		//÷�� 2 - A
		0x21, 0x22 , 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 		//���� 2 - A
		0x31, 0x32 , 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 		//���� 2 - A
		0x4E, 0x4F};
		//С�����


		BYTE iSend=0, iStation=0, iCardList[162];
		static int temp = 0;
		if ( temp > 9999999999 )
			temp = 0;
		srand( ( unsigned )GetCurrentTime( )+( temp++ ) );


		//int j = 0, n = 0;
		//if( iCardCount!=28 )
		//  for ( int i = 0;i < iCardCount;i += 13 )
		//    {
		//	  j = n * 13;
		//	  ::CopyMemory( &iCardList[i], &m_CardArray[j], sizeof( BYTE )*13 );
		//	   n++;
		//    }
		//else //���Ż�ɫֻȡ8-A
		//{
		//	for ( int i = 0;i < iCardCount;i += 7 )
		//    {
		//	  j = n * 13 + 6;
		//	  ::CopyMemory( &iCardList[i], &m_CardArray[j], sizeof( BYTE )*7 );
		//	   n++;
		//    }
		//}
		if ( bhaveKing )
		{
			for ( int i=0;i<iCardCount;i+=54 )
				::CopyMemory( &iCardList[i], m_CardArray, sizeof( m_CardArray ) );
		}
		else
		{
			for ( int i=0;i<iCardCount;i+=52 )
				::CopyMemory( &iCardList[i], m_CardArray, sizeof( m_CardArray ) );
		}


		do
		{
			iStation=rand( )%( iCardCount-iSend );
			iCard[iSend]=iCardList[iStation];
			iSend++;
			iCardList[iStation]=iCardList[iCardCount-iSend];
		} while ( iSend<iCardCount );

		//for ( int i=0;i<108;i++ )
		//	::CopyMemory( &iCard[i], &m_CardArray[53], 1 );

		return iCardCount;
}

//ɾ���˿�
int CUpGradeGameLogic::RemoveCard( BYTE iRemoveCard[], int iRemoveCount, BYTE iCardList[], int iCardCount )
{
	//��������
	if ( ( iRemoveCount>iCardCount ) ) return 0;

	//��Ҫɾ����������
	int iDeleteCount=0;
	for ( int i=0; i<iRemoveCount; i++ )
	{
		for ( int j=0; j<iCardCount; j++ )
		{
			if ( iRemoveCard[i]==iCardList[j] )
			{
				iDeleteCount++;
				iCardList[j] = 0;
				break;
			}
		}
	}
	RemoveNummCard( iCardList, iCardCount );
	if ( iDeleteCount!=iRemoveCount ) return 0;

	return iDeleteCount;
}
int CUpGradeGameLogic::GetPoint( int Card )
{
	if ( Card == 0x00 )
		return 0;
	switch ( GetCardNum( Card ) )
	{
	case 10:
		return 10;
	case 11:
		return 10;
	case 12:
		return 10;
	case 13:
		return 10;
	case 14:
		return 1;
	case 15:
		return 10;
	case 16:
		return 10;
	default:
		return GetCardNum( Card );
	}
}

//int CUpGradeGameLogic::GetCardPoint( BYTE Card[], int CardCount, bool split )
//{
//	int CardPoint = 0;
//	for ( int i = 0; i < CardCount; i++ )
//	  CardPoint += GetPoint( Card[i] );
//	if ( CardPoint > 21 )
//	{
//		for ( int i = 0; i < CardCount; i++ )
//		{
//			//�����A, ���Ƶ���������21, A���Զ���Ϊ1�㼴��ȥ10��
//			if ( GetCardNum( Card[i] ) == 14 && CardPoint > 21 )
//			      CardPoint -= 10;
//		}
//	}
//	if ( CardPoint >21 )
//		CardPoint = -1;//����21��ͱ���
//	if ( CardCount == 2 && !split && CardPoint == 21 )
//		return 22;//�ڽܿ�����Ϊ22�������
//	
//	 return CardPoint;

//}
// bPatternSpe: true -> �л����淨, 	false -> û�����淨
// ���ͻ��˵��ã�iUpCard����ΪNULL
int CUpGradeGameLogic::GetShape( BYTE iCardList[], int iCardCount, bool* bPatternSpe, BYTE iUpCard[] )
{
	if ( bPatternSpe != nullptr )
	{
		if ( IsShunZiNiu( iCardList, iCardCount ) && IsTonghuaNiu( iCardList, iCardCount ) && bPatternSpe[NN_TongHuaShun] ) 
		{
			return UG_BULL_TongHuaShun;
		}
		if ( IsBombBull( iCardList, iCardCount ) && bPatternSpe[NN_ZhaDan] ) 
		{
			return UG_BULL_BOMB;
		}

		if ( IsHuLu( iCardList, iCardCount )  && bPatternSpe[NN_HuLu] )
		{
			return UG_BULL_HuLu;
		}

		if ( IsTonghuaNiu( iCardList, iCardCount )  && bPatternSpe[NN_TongHua] )
		{
			return UG_BULL_TongHua;
		}

		if ( IsSilverBull( iCardList, iCardCount )  && bPatternSpe[NN_WuHua] ) 
		{
			return UG_BULL_WuHua;
		}

		if ( IsShunZiNiu( iCardList, iCardCount ) && bPatternSpe[NN_ShunZi] )
		{
			return UG_BULL_ShunZi;
		}

		/*
		if ( IsDragonBull( iCardList, iCardCount ) ) 
		{
		return UG_BULL_DRAGON;
		}

		if ( IsBombBull( iCardList, iCardCount ) ) 
		{
		return UG_BULL_BOMB;
		}

		if ( IsFiveSmall( iCardList, iCardCount ) ) 
		{
		return UG_FIVE_SMALL;
		}

		if ( IsGoldBull( iCardList, iCardCount ) ) 
		{
		return UG_BULL_GOLD;
		}

		if ( IsSilverBull( iCardList, iCardCount ) ) 
		{
		return UG_BULL_SILVER;
		}
		*/
	} 

	if ( iUpCard )
	{
		for( int i=0;i<3;i++ )
		{
			if( 255==iUpCard[i] )
			{
				return UG_NO_POINT;
			}
		}

		if ( !IsBull( iUpCard, 3 ) )
		{
			return UG_NO_POINT;
		}
	}

	if( IsBullBull( iCardList, iCardCount ) )
	{
		return UG_BULL_BULL;
	}

	switch( IsHaveNote( iCardList, iCardCount ) )
	{
	case 1:
		return UG_BULL_ONE;
	case 2:
		return UG_BULL_TWO;
	case 3:
		return UG_BULL_THREE;
	case 4:
		return UG_BULL_FOUR;
	case 5:
		return UG_BULL_FIVE;
	case 6:
		return UG_BULL_SIX;
	case 7:
		return UG_BULL_SEVEN;
	case 8:
		return UG_BULL_EIGHT;
	case 9:
		return UG_BULL_NINE;

	}
	return UG_NO_POINT;

}

//�������е���
int CUpGradeGameLogic::CountPoint( BYTE  iCardList[], int iCardCount )
{
	int point = 0;
	for ( int i=0;i<iCardCount;i++ )
	{
		int temp = GetPoint( iCardList[i] );
		if ( temp == 14 )
			temp = 1;
		point += temp;
	}
	return point;
}
//ͳ��ѡ��ָ���������Ƿ�������20, 10, 0�������Ϊ��0ֵ, ��ʾ���µ�����, ����0��ʾ������

int CUpGradeGameLogic::CanSumIn( BYTE iCardList[], int iCardCount, int iSelectNum )
{
	int total = CountPoint( iCardList, iCardCount );
	for ( int i=0; i<3; i++ )
	{
		for ( int j=i+1; j<4; j++ )
			for ( int k = j+1; k<iCardCount; k++ )
			{
				int temp = GetPoint( iCardList[i] )+GetPoint( iCardList[j] )+GetPoint( iCardList[k] );
				if ( temp==30||temp==10||temp==20 )
				{
					return total-temp;
				}

			}
	}
	return -1;
}
/// �ж��Ƿ�ţţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBullBull( BYTE  iCardList[], int iCardCount )
{
	int total=CountPoint( iCardList, iCardCount );
	if( CanSumIn( iCardList, iCardCount, 3 )==-1 )
		return false;
	if ( ( total > 0 ) && ( total % 10 == 0 ) )
		return true;
	return false;
}

/// �ж��Ƿ��ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsGoldBull( BYTE  iCardList[], int iCardCount )
{
	if ( iCardCount != 5 )
	{
		return false;
	}
	for ( int i=0; i<iCardCount; i++ )
	{
		//��ţ��JQK
		if ( GetCardNum( iCardList[i] ) != 11 && GetCardNum( iCardList[i] ) != 12
			&& GetCardNum( iCardList[i] ) != 13 && GetCardNum( iCardList[i] ) != 15
			&& GetCardNum( iCardList[i] ) != 16 )
		{
			return false;
		}
	}
	return true;
}
/// �ж��Ƿ���ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsSilverBull( BYTE  iCardList[], int iCardCount )
{
	if ( iCardCount != 5 )
	{
		return false;
	}
	for ( int i=0; i<iCardCount; i++ )
	{
		//��ţ��10 J Q K ��С��
		if ( GetCardNum( iCardList[i] ) != 11 && GetCardNum( iCardList[i] ) != 12
			&& GetCardNum( iCardList[i] ) != 13 && GetCardNum( iCardList[i] ) != 15
			&& GetCardNum( iCardList[i] ) != 16 &&  GetCardNum( iCardList[i] ) != 10 )
		{
			return false;
		}
	}
	return true;
}
/// �ж��Ƿ�ը��ţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBombBull( BYTE  iCardList[], int iCardCount, BYTE* pNum )
{
	if ( iCardCount != 5 )
	{
		return false;
	}

	int temp[17]={0};

	for ( int i=0; i<iCardCount; i++ )
	{
		temp[GetCardNum( iCardList[i] )]++;
	}
	for ( int i=0; i<17; i++ )
	{
		if ( temp[i] == 4 )
		{
			if ( pNum )
			{
				// Find Number
				for ( int j=0; j<iCardCount; j++ )
				{
					if ( GetCardNum( iCardList[j] ) == i )
					{
						*pNum = iCardList[j];
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}
/// �ж��Ƿ���Сţ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsFiveSmall( BYTE  iCardList[], int iCardCount )
{
	if ( iCardCount != 5 )
	{
		return false;
	}

	int iPoint = 0;
	for ( int i=0; i<iCardCount; i++ )
	{
		iPoint += GetPoint( iCardList[i] );
	}

	if ( iPoint <= 10 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

// һ����
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsDragonBull( BYTE  iCardList[], int iCardCount )
{
	if ( iCardCount != 5 )
	{
		return false;
	}

	int iPointArr[5] = {0};

	// GetPoint
	for ( int i=0; i<iCardCount; i++ )
	{
		iPointArr[i] = GetPoint( iCardList[i] );
	}

	//	number compare
	bool bReturnFalse = true;
	for ( int j=1; j<iCardCount+1; j++ ) // j is iPointArr[i]
	{
		bReturnFalse = true;
		for ( int i=0; i<iCardCount; i++ ) // i is iPointArr Index
		{
			if( j == iPointArr[i] )
			{
				bReturnFalse = false;
				break;
			}
		}

		if ( bReturnFalse )
		{
			return false;
		}
	}

	//	Find Dragon Bull
	return true;
}

///�Ƿ�Ϊ�е�
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
int CUpGradeGameLogic::IsHaveNote( BYTE  iCardList[], int iCardCount )
{
	int Note = CanSumIn( iCardList, iCardCount, 3 );
	return ( Note % 10 );
}
///�õ�������������( ����ɫ )
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
int CUpGradeGameLogic::GetMaxCard( BYTE  iCardList[], int iCardCount )
{
	int temp = 0;
	BYTE card = 0;
	for ( int i = 0; i < iCardCount; i++ )
	{
		if ( temp == 0 )
		{
			temp = GetCardBulk( iCardList[i] );
			card = iCardList[i];
		}
		else
		{
			if ( temp < GetCardBulk( iCardList[i] ) )
			{
				temp = GetCardBulk( iCardList[i] );
				card =iCardList[i];
			}
			else if ( temp == GetCardBulk( iCardList[i] ) )
			{
				if( GetCardHuaKind( card ) < GetCardHuaKind( iCardList[i] ) )
				{
					temp = GetCardBulk( iCardList[i] );
					card =iCardList[i];
				}
			}
		}

	}
	return card;
}

/// ��ȡը����ը������ֵ
/// return value: error 0, else ը������ֵ
BYTE CUpGradeGameLogic::GetBombNum( BYTE  iCardList[], int iCardCount )
{
	BYTE iNum = 0;

	if ( !IsBombBull( iCardList, iCardCount, &iNum ) )
	{
		iNum = 0;
	}

	return iNum;
}
/// �ж��Ƿ���ţ
/// @param BYTE  iCardList[] ���б�
/// @param int iCardCoun ������
bool CUpGradeGameLogic::IsBull( BYTE  iCardList[], int iCardCount )
{
	int temp=0;
	for ( int i=0; i<iCardCount; i++ )
	{
		temp += GetPoint( iCardList[i] );
	}
	return temp % 10 == 0;
}

//����==iCard�ĵ�������iCardList�е����
int  CUpGradeGameLogic::GetSerialBySpecifyCard( BYTE iCardList[], int iStart, int iCardCount, BYTE iCard )
{
	for( int i = iStart;i < iCardCount;i ++ )
	{
		if( iCardList[i] == iCard )
			return i;
	}
	return -1;
}

BOOL CUpGradeGameLogic::GetBull( BYTE iCardList[], int iCardCount, BYTE iBullCard[] )
{
	int total = CountPoint( iCardList, iCardCount );
	for ( int i = 0; i < 3; i++ )
	{
		for ( int j = i + 1; j < 4; j++ )
		{
			for ( int k = j + 1; k < iCardCount; k++ )
			{
				int temp = GetPoint( iCardList[i] ) + GetPoint( iCardList[j] ) + GetPoint( iCardList[k] );
				if ( temp == 30 || temp == 10 || temp == 20 )
				{
					iBullCard[0]=iCardList[i];
					iBullCard[1]=iCardList[j];
					iBullCard[2]=iCardList[k];
					return true;
				}
			}
		}
	}

	return false;
}

//���ţ����Ϣ
//bPatternSpe:	�Ƿ��ǻ����淨
//Notice: byCardCount( �Ƶ����� ), �ñ�������ΪSH_USER_CARD
//		  ����ֵΪ�ɹ����
//		  �ɹ����趨 TBullInfo��Ӧ���±���
//BYTE	byUnderCount;		//��������
//BYTE	byUnderCard[3];		//���Ƶ�������
//BYTE	byUpCard[2];		//��������2����
//int		iShape;				//��ţ����
bool CUpGradeGameLogic::GetBullInfo( BYTE byUserCard[], BYTE byCardCount, UserTanPai& TBullInfo, bool* bPatternSpe )
{
	if ( NULL == byUserCard || SH_USER_CARD != byCardCount )
	{
		return false;
	}

	BYTE bResult[3];
	memset( bResult, 0, sizeof( bResult ) );
	if ( GetBull( byUserCard, SH_USER_CARD, bResult ) )
	{
		::CopyMemory( TBullInfo.byUnderCard, bResult, sizeof( bResult ) );
	}
	else
	{
		memset( TBullInfo.byUnderCard, 0, sizeof( TBullInfo.byUnderCard ) );
		TBullInfo.byUnderCard[0] = byUserCard[0];
		TBullInfo.byUnderCard[1] = byUserCard[1];
		TBullInfo.byUnderCard[2] = byUserCard[2];
	}

	//Set Up Card
	{
		BYTE byTmpCard[SH_USER_CARD];
		memcpy( byTmpCard, byUserCard, sizeof( byTmpCard ) );

		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			for( int j=0;j<3;j++ )
			{
				if ( TBullInfo.byUnderCard[j] == byTmpCard[i] )
				{
					byTmpCard[i] = 0xFF;
					break;
				}
			}
		}

		int iIndex = 0;
		for ( int i=0; i<SH_USER_CARD; i++ )
		{
			if ( 0xFF != byTmpCard[i] )
			{
				TBullInfo.byUpCard[iIndex] = byTmpCard[i];
				iIndex++;
				if ( iIndex >=2 )
				{
					break;
				}
			}
		}
	}

	TBullInfo.iShape = GetShape( byUserCard, byCardCount, bPatternSpe, TBullInfo.byUnderCard );

	return true;
}
