#include "StdAfx.h"
//#include "GamePlace.h"
#include "GameListCtrl.h"
//#include "PlaceResource.h"
//#include "..\\..\\��Ϣ�ļ�\\GamePlaceMessage.h"

BEGIN_MESSAGE_MAP(CGameListCtrl, CTreeCtrl)
END_MESSAGE_MAP()

#define SET_LINE_COLOR          (TV_FIRST+40)		//������ɫ��Ϣ						
#define LS_TIME_OUT				60L		//�б�ʱ��									

//���ؼ�ָ��
CGameListCtrl					* CAFCGameListItem::m_pTreeCtrl=NULL;
void CGameListCtrl::IEnterARoomOrLeave(UINT uRoomID,bool bEnter)//������뿪һ�����䡣��������ӷ���������������������
{
	CAFCRoomItem * pGameRoomItem=FindRoomItem(uRoomID);//��÷�����б��ָ��
	if(!pGameRoomItem)return;
	int dd=0;
	if(bEnter)dd=1;//pGameRoomItem->m_RoomInfo.uPeopleCount ++;
	else dd=-1;//pGameRoomItem->m_RoomInfo.uPeopleCount --;

	pGameRoomItem->m_RoomInfo.uPeopleCount+=dd;
	
	if(pGameRoomItem->m_RoomInfo.uPeopleCount  < 0)pGameRoomItem->m_RoomInfo.uPeopleCount= 0;
	
	{
		TCHAR szBuffer[100];
		//��������������������������
		wsprintf(szBuffer,TEXT("(%3d)%s"),pGameRoomItem->m_RoomInfo.uPeopleCount,pGameRoomItem->m_RoomInfo.szGameRoomName);
		SetItemText(pGameRoomItem->m_hCurrentItem,szBuffer);
	}
	CAFCNameItem *p=FindNameItem(pGameRoomItem->m_RoomInfo.uRoomID);
	if(p)
	{
		p->m_NameInfo.m_uOnLineCount += dd;
		if(p->m_NameInfo.m_uOnLineCount < 0)p->m_NameInfo.m_uOnLineCount =0;
		///p->UpdateOnLineCount();
		UpdateOnLineCount();
		//ExpandItem(2);
	}
}

//��������������
bool CGameListCtrl::UpdateOnLineCount()
{
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem==NULL)continue;
		UINT kd=0;
		for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
			if (pNameItem!=NULL)
			{
				if(pNameItem->m_NameInfo.m_uOnLineCount < 0)pNameItem->m_NameInfo.m_uOnLineCount=0;
				pNameItem->UpdateOnLineCount();
				///uOnLineCount += pNameItem->m_uOnLineCount;
				kd += pNameItem->m_NameInfo.m_uOnLineCount;
			}
		}

		uOnLineCount +=kd;
		if(kd<0)
			kd=0;
		{
			TCHAR szBuffer[100];
			wsprintf(szBuffer,TEXT("(%3d)%s"),kd,pKindItem->m_KindInfo.szKindName);//m_RoomInfo.szGameRoomName);
			SetItemText(pKindItem->m_hCurrentItem,szBuffer);
		}
	}
	
	
	//������ʾ
	//TCHAR szBuffer[100];
	//wsprintf(szBuffer,TEXT(Glb().m_Tree[0]),uOnLineCount);//��һ����������������
	//SetItemText(pGameKindItem->m_hCurrentItem,szBuffer);

return true;
}

/*
//��������
void CGameListCtrl::UpdateAllOnLineData()
{
	
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem==NULL)continue;
		UINT kd=0;
		for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
			if (pNameItem!=NULL)
			{
				///uOnLineCount += pNameItem->m_uOnLineCount;
				kd += uOnLineCount;
			}
		}

		uOnLineCount +=kd;

			
		{
			TCHAR szBuffer[100];
			wsprintf(szBuffer,TEXT("(%3d) %s"),kd,pKindItem->m_KindInfo.szKindName);//m_RoomInfo.szGameRoomName);
			SetItemText(pKindItem->m_hCurrentItem,szBuffer);
		}
	}
	
	
	//������ʾ
	TCHAR szBuffer[100];
	wsprintf(szBuffer,TEXT("��Ϸ�㳡 ( ��ǰ�������� %d �� )"),uOnLineCount);
	SetItemText(pGameKindItem->m_hCurrentItem,szBuffer);


}
*/

//��������
bool CGameListCtrl::HandleNameOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount)
{
	
	for (UINT i=0;i<uCount;i++) 
	{
		CAFCNameItem * pNameItem=FindNameItem((pOnLineCountPtr+i)->uID);
		if (pNameItem!=NULL)
		{
			pNameItem->m_NameInfo.m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
            pNameItem->UpdateOnLineCount();
			pNameItem->ExpandItem (1);
		}
	}
    UpdateOnLineCount();


	return true;
}

CAFCNameItem *CGameListCtrl::FindNameItem(UINT uID)
{
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL)
				{
					if(pNameItem->m_NameInfo.uNameID == uID)return pNameItem;
				}
			}
		}
	}
	return NULL;
}

//��������
bool CGameListCtrl::HandleIOnLineData(MSG_GP_SR_OnLineStruct * pOnLineCountPtr, UINT uCount)
{
	for (UINT i=0;i<uCount;i++) 
	{
//		CAFCNameItem * pNameItem=FindNameItem(FindKindItem((pOnLineCountPtr+i)->uKindID),(pOnLineCountPtr+i)->uNameID);
		//if (pNameItem!=NULL) pNameItem->m_uOnLineCount=(pOnLineCountPtr+i)->uOnLineCount;
	}
	return true;
}

//չ������
bool CGameListCtrl::ExpandItemFirstRoom()
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����

	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL)
				{
					pNameItem->ExpandItem (4);
					return true;
				}
			}
		}
	}
	return true;
}

/*******************************************************************************************************/
CAFCRoomItem *CGameListCtrl::FindRoomItem(UINT uRoomID)
{
	//ͳ������
	UINT uOnLineCount=0L;
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL)
				{
					for (INT_PTR rr=0;rr<pNameItem->m_ItemPtrArray.GetCount();rr++)
					{
						CAFCRoomItem * proomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt (rr);
						if(proomItem->m_RoomInfo.uRoomID == uRoomID)
						{							
							return proomItem;
						}
					}
				}
			}
		}
	}
	return NULL;
}

//��������
bool CGameListCtrl::HandleRoomOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount)
{
	
	for (UINT i=0;i<uCount;i++) 
	{
		CAFCRoomItem * pRoomItem=FindRoomItem((pOnLineCountPtr+i)->uID);
		if (pRoomItem!=NULL)
		{
			pRoomItem->m_RoomInfo.uPeopleCount=(pOnLineCountPtr+i)->uOnLineCount;

			{
				TCHAR szBuffer[100];
				if(pRoomItem->m_RoomInfo.uPeopleCount<0)
					pRoomItem->m_RoomInfo.uPeopleCount=0;
				wsprintf(szBuffer,TEXT("(%3d)%s"),pRoomItem->m_RoomInfo.uPeopleCount,pRoomItem->m_RoomInfo.szGameRoomName);
				SetItemText(pRoomItem->m_hCurrentItem,szBuffer);
			}
		}
	}
	return true;
}
//��������
CAFCGameListItem::CAFCGameListItem()
{
	m_uUpdateTime=0;
	m_hCurrentItem=NULL;
	m_pParentItem=NULL;
	m_uDataType=GLK_NO_DATA;
	return;
}

//��������
CAFCGameListItem::~CAFCGameListItem()
{
	for (int i=0;i<m_ItemPtrArray.GetCount();i++)
	{
		delete (CAFCGameListItem *)m_ItemPtrArray.GetAt(i);
		m_ItemPtrArray.SetAt(i,NULL);
	}
	return;
}

//��ʼ����
bool CAFCGameListItem::Init(CAFCGameListItem * pParentItem)
{
	m_pParentItem=pParentItem;
	return true;
}

//�����ϲ���һ��Ҷ�ӽӵ�����
HTREEITEM CAFCGameListItem::AddTreeData(TCHAR * szText, UINT uIconIndex, LPARAM lParam)
{
	TV_INSERTSTRUCT InsertInf;
	memset(&InsertInf,0,sizeof(InsertInf));
	InsertInf.item.cchTextMax=100;
	InsertInf.hParent=m_hCurrentItem;///////���ӵ�!!!!!!!!!!!!!!!!!!
	InsertInf.hInsertAfter=TVI_LAST;
	InsertInf.item.iImage=uIconIndex;
	InsertInf.item.iSelectedImage=uIconIndex;
	InsertInf.item.pszText=szText;
	InsertInf.item.lParam=lParam;
	InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
	return m_pTreeCtrl->InsertItem(&InsertInf);
}

//չ��������
bool CAFCGameListItem::ExpandItem(int iExpendCount)
{
	if ((iExpendCount==-1)||(iExpendCount>0))//�������ŵ���-1��������Ŵ���0��������Ų�����0��
	{
		if (m_hCurrentItem!=NULL) m_pTreeCtrl->Expand(m_hCurrentItem,TVE_EXPAND);//�����ǰ�Ϊ�գ���ôչ����ǰ��
		if ((iExpendCount>1)||(iExpendCount==-1))//�������ŵ���-1��������Ŵ���1��������Ų�����1��������0��
		{
			if (iExpendCount>0) iExpendCount--;//�������Ŵ���0����ô����ż�1
			for (int i=0;i<m_ItemPtrArray.GetCount();i++)//������������
			{
				CAFCGameListItem * pListItem=(CAFCGameListItem *)m_ItemPtrArray.GetAt(i);//////////�õ� ��Ϸ�б�����
				if (pListItem!=NULL) pListItem->ExpandItem(iExpendCount);//�����Ϸ�б����Ϊ�գ���ôչ����Ϸ�б�����
			}
		}
		return true;
	}
	return false;
}
//����ӷ�������ȡ������
bool CAFCGameListItem::UpdateGetListItem()//////////�����б�AddTreeData
{
	//if (m_uDataType == GLK_GAME_NAME)/////////////���������������Ϸ����
	//{
	//	if (m_ItemPtrArray.GetCount()==0)////������
	//	{
	//		TCHAR szBuffer[100];
	//		if ((m_uUpdateTime+LS_TIME_OUT) <= (LONG)time(NULL)) 
	//			wsprintf(szBuffer,TEXT("��ȡ��Ϣ..."));//�������ʱ��+�б�ʱ��С�ڵ�ǰʱ�䣬��ô�Ͷ�ȡ��Ϣ
	//		else 
	//			wsprintf(szBuffer,TEXT("û�з���"));//������㳬ʱ����ʾû��ʵ��
	//		TCHAR sz[200];
	//		wsprintf(sz,"%d,%d",m_uUpdateTime,(LONG)time(NULL));
	//		//Writelog(sz);
	//		if (m_hUpdateItem==NULL) 
	//			m_hUpdateItem=AddTreeData(szBuffer,IND_NO_ROOM,NULL);///�����ȡ�������ڿգ���ô��ȡ����=�������ʾ//û�з����ͼ��
	//		else
	//			m_pTreeCtrl->SetItemText(m_hUpdateItem,szBuffer);//���ؼ�ָ��������ʾ����
	//	}
	//	else if (m_hUpdateItem!=NULL) ///������
	//	{
	//		for (INT_PTR i=0;i<m_ItemPtrArray.GetCount();i++)//ѭ����������
	//		{
	//			CAFCRoomItem * pRoomItem=(CAFCRoomItem *)m_ItemPtrArray.GetAt(i);///��Ϸ�����������������
	//			pRoomItem->m_uUpdateTime=(LONG)time(NULL);//����ʱ��=��ǰʱ��
	//		}
	//		m_pTreeCtrl->DeleteItem(m_hUpdateItem);//ɾ����ȡ����
	//		m_hUpdateItem=NULL;////////��ȡ����Ϊ��
	//	}
	//}
	return true;
}
//�Ƿ���Ҫ���� 
bool CAFCGameListItem::IsNeedUpdate()//�Ƿ���Ҫ����
{
	if ((m_uDataType==GLK_GAME_NAME)&&((m_uUpdateTime+LS_TIME_OUT)<=time(NULL))) return true;////////���������������Ϸ���֣����Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
	return false;
}
//ɾ����ʱ����
bool CAFCGameListItem::DeleteOutTimeItem()
{
	if (m_uDataType==GLK_GAME_NAME)//���������������Ϸ����
	{
		for (INT_PTR i=0;i<m_ItemPtrArray.GetCount();i++)//////////ѭ����������
		{
			CAFCRoomItem * pRoomItem=(CAFCRoomItem *)m_ItemPtrArray.GetAt(i);//��Ϸ�����������������
			if ((pRoomItem!=NULL)&&((pRoomItem->m_uUpdateTime+LS_TIME_OUT)<=(LONG)time(NULL)))//�����Ϸ�������Ϊ�գ������Ҹ���ʱ��+�б�ʱ��С�ڵ�ǰʱ��
			{
				m_ItemPtrArray.RemoveAt(i--);///////ɾ����������
				m_pTreeCtrl->DeleteItem(pRoomItem->m_hCurrentItem);/////ɾ��������
				delete pRoomItem;////ɾ����Ϸ��������
			}
		}
		UpdateGetListItem();/////�����б�
	}
	return true;
}
//������Ϸ����AddTreeData
bool CAFCGameListItem::AddGameKind(ComKindInfo * pGameKind)
{
	try
	{
        CAFCKindItem *pKind = m_pTreeCtrl->FindKindItem(pGameKind->uKindID);
        if(pKind != NULL)
        {
            pKind->m_KindInfo = *pGameKind;
            return true;
        }

		CAFCKindItem * pGameKindItem=new CAFCKindItem;//����Ϸ��������
		if (pGameKindItem!=NULL)/////////�����Ϸ�������Ϊ��
		{
			//��������

			pGameKindItem->Init(this);////
			pGameKindItem->m_KindInfo=*pGameKind;/////////��������

			m_ItemPtrArray.Add(pGameKindItem);////��������
			pGameKindItem->m_hCurrentItem=AddTreeData(pGameKindItem->m_KindInfo.szKindName,0,(LPARAM)pGameKindItem);///////������Ϸ����AddTreeData
			return true;
		}
	}
	catch (...) {}
	return false;
}

//������Ϸ����
bool CAFCGameListItem::AddGameName(ComNameInfo * pGameName)
{
	try
	{
        CAFCKindItem *pKind = m_pTreeCtrl->FindKindItem(pGameName->uKindID);
        if(pKind != NULL)
        {
            CAFCNameItem *pName = m_pTreeCtrl->FindNameItem(pKind, pGameName->uNameID);
            if(pName != NULL)
            {
                pName->m_NameInfo=*pGameName;
                return true;
            }
        }
		CAFCNameItem * pGameNameItem=new CAFCNameItem;
		if (pGameNameItem!=NULL)
		{
			//��������
			pGameNameItem->Init(this);
			pGameNameItem->m_NameInfo=*pGameName;
			m_ItemPtrArray.Add(pGameNameItem);
			pGameNameItem->m_hCurrentItem=AddTreeData(pGameNameItem->m_NameInfo.szGameName,0,(LPARAM)pGameNameItem);
			return true;
		}
	}
	catch (...) {}
	return false;
}

//������Ϸ����
bool CAFCGameListItem::AddGameRoom(ComRoomInfo * pGameRoom)
{
	try
	{
        CAFCKindItem *pKind = m_pTreeCtrl->FindKindItem(pGameRoom->uKindID);
        if(pKind != NULL)
        {
            CAFCNameItem *pName = m_pTreeCtrl->FindNameItem(pKind, pGameRoom->uNameID);
            if(pName != NULL)
            {
                CAFCRoomItem *pRoom = m_pTreeCtrl->FindRoomItem(pName, pGameRoom->uRoomID);
                if(pRoom !=  NULL)
                {
                    //��������		
                    pRoom->m_RoomInfo=*pGameRoom;
                    pRoom->m_uUpdateTime=(LONG)time(NULL);
                    return true;
                }
            }
        }
		CAFCRoomItem * pGameRoomItem=new CAFCRoomItem;
		if (pGameRoomItem!=NULL)
		{
			//��������		
			pGameRoomItem->Init(this);
			pGameRoomItem->m_RoomInfo=*pGameRoom;
			pGameRoomItem->m_uUpdateTime=(LONG)time(NULL);
			m_ItemPtrArray.Add(pGameRoomItem);
            TCHAR szBuffer[100];
            wsprintf(szBuffer,TEXT("(%3d)%s"),pGameRoom->uPeopleCount,pGameRoom->szGameRoomName);///��ʾ����
			pGameRoomItem->m_hCurrentItem=AddTreeData(szBuffer,0,(LPARAM)pGameRoomItem);
			return true;
		}
	}
	catch (...) {}
	return false;
}

//�����ڲ�����
bool CAFCGameListItem::AddInsideData(GameInsideInfo * pInsideData, UINT uIconIndex)
{
	try
	{
		CAFCInsideItem * pInsideItem=new CAFCInsideItem;
		if (pInsideItem!=NULL)
		{
			//��������
			pInsideItem->Init(this);
			pInsideItem->m_InsideInfo=*pInsideData;
			m_ItemPtrArray.Add(pInsideItem);
			pInsideItem->m_hCurrentItem=AddTreeData(pInsideItem->m_InsideInfo.szDisplayName,uIconIndex,(LPARAM)pInsideItem);
			return true;
		}
	}
	catch (...) {}
	return false;
}
	
/*******************************************************************************************************/

//���õ�����Ϸ������������
bool CAFCNameItem::UpdateOnLineCount()
{
	TCHAR szBuffer[100];
	if(m_NameInfo.m_uOnLineCount<0)
		m_NameInfo.m_uOnLineCount=0;
	wsprintf(szBuffer,TEXT("%s(%d)"),m_NameInfo.szGameName,m_NameInfo.m_uOnLineCount);
	m_pTreeCtrl->SetItemText(m_hCurrentItem,szBuffer);
	return true;
}

/*******************************************************************************************************/

//���캯��
CGameListCtrl::CGameListCtrl()
{
}

//��������
CGameListCtrl::~CGameListCtrl()
{
	for (int i=0;i<m_InsideArray.GetCount();i++)
	{
		delete (CAFCGameListItem *)m_InsideArray.GetAt(i);
		m_InsideArray.SetAt(i,NULL);
	}
	return;
}

//��ʼ������,ͬʱ���Ӹ��ӵ�
bool CGameListCtrl::Init()
{
	////������Ϸ�б�����
	//UINT uIconIndex[]=
	//{
	//	IND_LIST_ROOT,IND_ROOM_NO_ICON,IND_LIST_ROOT,IND_LIST_NO_INSTALL,
	//	IND_LIST_ROOT,IND_LIST_ROOT,IND_LIST_ROOT,
	//};
	//�����ϲ��������
	CAFCGameListItem::m_pTreeCtrl=this;
	//for (int i=0;i<Glb().m_TreeCount;i++)
	//{	
		GameInsideInfo InsideInfo;
		InsideInfo.Head.uSize=sizeof(InsideInfo);
		InsideInfo.Head.bDataType=GLK_GAME_USE;
		InsideInfo.uTrunID = 1000;
		lstrcpy(InsideInfo.szDisplayName,"root");
		if (!AddRootItem(&InsideInfo,100))//uIconIndex[0])) 
			return false;
	//}
	return true;
}
//�������
bool CGameListCtrl::AddRootItem(GameInsideInfo * pInsideData, UINT uIconIndex)
{
	try
	{
		CAFCInsideItem * pInsideItem=new CAFCInsideItem();
		if (pInsideItem!=NULL)
		{
			//��������
			pInsideItem->Init(NULL);
			pInsideItem->m_InsideInfo=*pInsideData;
			m_InsideArray.Add(pInsideItem);

			//��������
			TV_INSERTSTRUCT InsertInf;
			memset(&InsertInf,0,sizeof(InsertInf));
			InsertInf.item.cchTextMax=100;
			InsertInf.hInsertAfter=TVI_LAST;
			InsertInf.item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT|TVIF_PARAM;
			InsertInf.item.iImage=uIconIndex;
			InsertInf.item.iSelectedImage=uIconIndex;//ѡ���ͼ��
			InsertInf.item.pszText=pInsideData->szDisplayName;
			InsertInf.item.lParam=(LPARAM)pInsideItem;
			pInsideItem->m_hCurrentItem=InsertItem(&InsertInf);
			return true;
		}
	}
	catch (...) {}
	return false;
}


//��������//�����ϼ�����Ϸ����AddTreeData
bool CGameListCtrl::HandleKindData(ComKindInfo * pKindInfoPtr, UINT uKindCount)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (UINT i=0;i<uKindCount;i++) 
	{
	    pGameKindItem->AddGameKind(pKindInfoPtr+i);//�����ϼ�����Ϸ����AddTreeData
	}
	return true;
}
//��������/////�����ϼ�����Ϸ����
bool CGameListCtrl::HandleNameData(ComNameInfo * pNameInfoPtr, UINT uNameCount)
{
	for (UINT i=0;i<uNameCount;i++)
	{
        CAFCKindItem * pKindItem=FindKindItem((pNameInfoPtr+i)->uKindID);
        if(pKindItem != NULL)   
	        pKindItem->AddGameName(pNameInfoPtr+i);///////�����ϼ�����Ϸ����
	}
	return true;
}
//��������
bool CGameListCtrl::HandleRoomData(ComRoomInfo * pRoomInfoPtr, UINT uRoomCount, UINT uKindID, UINT uNameID)
{
	CAFCNameItem * pNameItem=FindNameItem(FindKindItem(uKindID),uNameID);///������Ϸ����
	if (pNameItem!=NULL)
	{
		CAFCRoomItem * pRoomItem = NULL;
		for (UINT i=0;i<uRoomCount;i++)
        {
			pNameItem->AddGameRoom(pRoomInfoPtr+i);////////�������Ϸ�����������Ӳ��ҷ���ID
		}			
	}
	return true;
}
//������������
CAFCKindItem * CGameListCtrl::FindKindItem(UINT uKindID)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if ((pKindItem!=NULL)&&(pKindItem->m_KindInfo.uKindID==uKindID)) return pKindItem;
	}
	return NULL;
}

//������������
CAFCNameItem * CGameListCtrl::FindNameItem(CAFCKindItem * pKindItem, UINT uNameID)
{
	if (pKindItem!=NULL)
	{
		for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
			if ((pNameItem!=NULL)&&(pNameItem->m_NameInfo.uNameID==uNameID)) return pNameItem;
		}
	}
	return NULL;
}

//���ҷ�������
CAFCRoomItem * CGameListCtrl::FindRoomItem(CAFCNameItem * pNameItem, UINT uRoomID)
{
	if (pNameItem!=NULL)
	{
		for (INT_PTR i=0;i<pNameItem->m_ItemPtrArray.GetCount();i++)
		{
			CAFCRoomItem * pRoomItem=(CAFCRoomItem *)pNameItem->m_ItemPtrArray.GetAt(i);
			if ((pRoomItem!=NULL)&&(pRoomItem->m_RoomInfo.uRoomID==uRoomID)) return pRoomItem;
		}
	}
	return NULL;
}
//�����ȡ����
bool CGameListCtrl::FillGetListItem()
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL) 
		{
			for (INT_PTR j=0;j<pKindItem->m_ItemPtrArray.GetCount();j++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(j);
				if (pNameItem!=NULL) 
				{
					pNameItem->m_uUpdateTime=0;
					pNameItem->UpdateGetListItem();
				}
			}
		}			
	}
	return true;
}
//ɾ����ʱ���� 
bool CGameListCtrl::DeleteOutTimeRoom(UINT uKindID, UINT uNameID)
{
	CAFCNameItem * pNameItem=FindNameItem(FindKindItem(uKindID),uNameID);
	if (pNameItem!=NULL) pNameItem->DeleteOutTimeItem();
	return true;
}
//��ȡ��������
bool CGameListCtrl::GetProcessName(CAFCRoomItem * pGameRoomItem, TCHAR * szProcessName, UINT uBufferSize)
{
	if ((pGameRoomItem->m_pParentItem!=NULL)&&(pGameRoomItem->m_pParentItem->m_uDataType==GLK_GAME_NAME))
	{
		CAFCNameItem * pGameName=(CAFCNameItem *)pGameRoomItem->m_pParentItem;
		/*CopyMemory(szProcessName,pGameName->m_NameInfo.szGameProcess,__min(uBufferSize,sizeof(pGameName->m_NameInfo.szGameProcess)));*/
		szProcessName[uBufferSize/sizeof(TCHAR)-1]=0;
		return true;
	}
	return false;
}
//��ȡ��Ϸ����
bool CGameListCtrl::GetGameName(CAFCRoomItem * pGameRoomItem, TCHAR * szGameName, UINT uBufferSize)
{
	if ((pGameRoomItem->m_pParentItem!=NULL)&&(pGameRoomItem->m_pParentItem->m_uDataType==GLK_GAME_NAME))
	{
		CAFCNameItem * pGameName=(CAFCNameItem *)pGameRoomItem->m_pParentItem;
		CopyMemory(szGameName,pGameName->m_NameInfo.szGameName,__min(uBufferSize,sizeof(pGameName->m_NameInfo.szGameName)));
		szGameName[uBufferSize/sizeof(TCHAR)-1]=0;
		return true;
	}
	return false;
}



//չ������
bool CGameListCtrl::ExpandItem(int iExpendCount)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	pGameKindItem->ExpandItem(iExpendCount);
	return true;
}

void CGameListCtrl::UpdataItem(CAFCGameListItem* pItem)
{
	CAFCNameItem * pNameItem;
	int i;
	for(i=0;i<=pItem->m_pParentItem->m_ItemPtrArray.GetCount();i++)
	{
		pNameItem = (CAFCNameItem *)(pItem->m_pParentItem->m_ItemPtrArray.GetAt(i));
		if(pNameItem->m_hCurrentItem==pItem->m_hCurrentItem)
			break;
	}
	//int selimage=CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
	//TVITEM item;
	//item.hItem=pItem->m_hCurrentItem;
	//item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	//item.iImage=selimage;
	//item.iSelectedImage=selimage;
	//TreeView_SetItem(m_hWnd,&item);
}

void CGameListCtrl::UpdateAllIcon(void)
{
	//int selimage;
	//CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	//for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	//{
	//	CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
	//	if (pKindItem!=NULL)
	//	{
	//		for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
	//		{
	//			CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
	//			if (pNameItem!=NULL)
	//			{
	//				selimage=CPlaceResource::GetListIconIndex(&(pNameItem->m_NameInfo));
	//				TVITEM item;
	//				item.hItem=pNameItem->m_hCurrentItem;
	//				item.mask=TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	//				item.iImage=selimage;
	//				item.iSelectedImage=selimage;
	//				TreeView_SetItem(m_hWnd,&item);
	//			}
	//		}
	//	}
	//}
}
void CGameListCtrl::JoinAllItem(void)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount();i++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
				if (pNameItem!=NULL)
				{
					pNameItem->m_pTreeCtrl->Expand(pNameItem->m_hCurrentItem,TVE_COLLAPSE);
				}
			}
			pKindItem->m_pTreeCtrl->Expand(pKindItem->m_hCurrentItem,TVE_COLLAPSE);
		}
	}
}
bool CGameListCtrl::ExpandAll(bool ExpName)
{
	CAFCGameListItem * pGameKindItem=GetKindParentItem();//��ȡ����
	for (INT_PTR i=0;i<pGameKindItem->m_ItemPtrArray.GetCount();i++)
	{
		CAFCKindItem * pKindItem=(CAFCKindItem *)pGameKindItem->m_ItemPtrArray.GetAt(i);
		if (pKindItem!=NULL)
		{
			for (INT_PTR i=0;i<pKindItem->m_ItemPtrArray.GetCount()&&ExpName;i++)
			{
				CAFCNameItem * pNameItem=(CAFCNameItem *)pKindItem->m_ItemPtrArray.GetAt(i);
				if (pNameItem!=NULL)
				{
					pNameItem->m_pTreeCtrl->Expand(pNameItem->m_hCurrentItem,TVE_EXPAND);
				}
			}
			pKindItem->m_pTreeCtrl->Expand(pKindItem->m_hCurrentItem,TVE_EXPAND);
		}
	}
	return true;
}
