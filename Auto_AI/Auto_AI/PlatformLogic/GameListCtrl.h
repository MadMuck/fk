#pragma once
#include "StdAfx.h"
//#include "GamePlace.h"
#include "comstruct.h"

//��˵��
class CGameListCtrl;
struct MSG_GP_SR_OnLineStruct;
#include <vector>
using namespace std;

/*******************************************************************************************************/
//��Ϸ�б�����
class CAFCGameListItem
{
	//���ݱ���
public:
	UINT										m_uDataType;					//��������
	LONG										m_uUpdateTime;					//����ʱ��
	CPtrArray									m_ItemPtrArray;					//��������
	CAFCGameListItem							* m_pParentItem;				//����ָ��

	//��ͼ����
public:
	HTREEITEM									m_hUpdateItem;					//��ȡ����
	HTREEITEM									m_hCurrentItem;					//��ǰ����
	static CGameListCtrl						* m_pTreeCtrl;					//���ؼ�ָ��

	//��������
public:
	//��������
	CAFCGameListItem();
	//��������
	virtual ~CAFCGameListItem();

	//���ܺ���
public:
	//��ʼ����
	bool Init(CAFCGameListItem * pParentItem);
	//������Ϸ����
	bool AddGameKind(ComKindInfo * pGameKind);
	//������Ϸ����
	bool AddGameName(ComNameInfo * pGameName);
	//������Ϸ����
	bool AddGameRoom(ComRoomInfo * pGameRoom);
	//�����ڲ�����
	bool AddInsideData(GameInsideInfo * pInsideData, UINT uIconIndex);
	//չ������
	bool ExpandItem(int iExpendCount);
	//�Ƿ���Ҫ����
	bool IsNeedUpdate();
	//���»�ȡ������
	bool UpdateGetListItem();
	//ɾ����ʱ���� 
	bool DeleteOutTimeItem();

	//��������
protected:
	//��������
	HTREEITEM AddTreeData(TCHAR * szText, UINT uIconIndex, LPARAM lParam);
};

/*******************************************************************************************************/

//��Ϸ��������
class CAFCKindItem : public CAFCGameListItem
{
	//��������
public:
	ComKindInfo								m_KindInfo;						//��������

	//��������
public:
	//���캯��
	CAFCKindItem() 
	{ 
		m_uDataType=GLK_NO_DATA;
		memset(&m_KindInfo,0,sizeof(m_KindInfo)); 
		m_KindInfo.Head.uSize=sizeof(m_KindInfo);
		m_KindInfo.Head.bDataType=GLK_GAME_KIND;
	};
	//��������
	virtual ~CAFCKindItem() {};
};

/*******************************************************************************************************/

//��Ϸ��������
class CAFCNameItem : public CAFCGameListItem
{
	//��������
public:
	///UINT										m_uOnLineCount;					//��������
	ComNameInfo								m_NameInfo;						//��������	
	//��������
public:
	//���캯��
	CAFCNameItem() 
	{ 
		m_hUpdateItem=NULL;
		m_uDataType=GLK_GAME_NAME;
		memset(&m_NameInfo,0,sizeof(m_NameInfo)); 
		m_NameInfo.Head.uSize=sizeof(m_NameInfo);
		m_NameInfo.Head.bDataType=GLK_GAME_NAME;
	};
	//��������
	virtual ~CAFCNameItem() {};
	//������������
	bool UpdateOnLineCount();
};

/*******************************************************************************************************/

//��Ϸ��������
class CAFCRoomItem : public CAFCGameListItem
{
	//��������
public:
	ComRoomInfo								m_RoomInfo;						//��������
public:
	//���캯��
	CAFCRoomItem() 
	{ 
		m_uDataType=GLK_GAME_ROOM;
		memset(&m_RoomInfo,0,sizeof(m_RoomInfo)); 
		m_RoomInfo.Head.uSize=sizeof(m_RoomInfo);
		m_RoomInfo.Head.bDataType=GLK_GAME_ROOM;
	};
	//��������
	virtual ~CAFCRoomItem() {};
};

/*******************************************************************************************************/

//��Ϸ�ڲ�����
class CAFCInsideItem : public CAFCGameListItem//��Ϸ�б�
{
	//��������
public:
	GameInsideInfo								m_InsideInfo;					//�ڲ�����

	//��������
public:
	//���캯��
	CAFCInsideItem()
	{
		m_uDataType=GLK_GAME_USE;
		memset(&m_InsideInfo,0,sizeof(m_InsideInfo)); 
		m_InsideInfo.Head.uSize=sizeof(m_InsideInfo);
		m_InsideInfo.Head.bDataType=GLK_GAME_USE;
	}
	//��������
	virtual ~CAFCInsideItem() {};
};

/*******************************************************************************************************/

//��Ϸ�б������
class CGameListCtrl : public CTreeCtrl////////////��������һ������
{
	//��������
protected:
	CPtrArray									m_InsideArray;					//��Ϸ�ڲ�����
	DECLARE_MESSAGE_MAP()
	//��������
public:
	///void UpdateAllOnLineData();
	

//��������
    bool HandleNameOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount);
	// ������Ϸ
	CAFCNameItem *FindNameItem(UINT uID);
	// ���ҷ���
	CAFCRoomItem *FindRoomItem(UINT uRoomID);

	void IEnterARoomOrLeave(UINT uRoomID,bool bEnter);
	///void IEnterARoomOrLeave(CAFCRoomItem * pGameRoomItem,bool bEnter);
	bool ExpandItemFirstRoom();
	bool HandleRoomOnLineData(DL_GP_RoomListPeoCountStruct * pOnLineCountPtr, UINT uCount);
	//��������
	CGameListCtrl();
	//��������
	virtual ~CGameListCtrl();
	
	//���ܺ���
public:
	//��ʼ������
	bool Init();
	//չ������
	bool ExpandItem(int iExpendCount);
	//�������
	bool AddRootItem(GameInsideInfo * pInsideData, UINT uIconIndex);
	//��ȡ����
	CAFCGameListItem * GetKindParentItem() { return (CAFCGameListItem *)m_InsideArray.GetAt(0); }
	//��������
	bool HandleIOnLineData(MSG_GP_SR_OnLineStruct * pOnLineCountPtr, UINT uCount);
	//��������
	bool HandleKindData(ComKindInfo * pKindInfoPtr, UINT uKindCount);
	//��������
	bool HandleNameData(ComNameInfo * pNameInfoPtr, UINT uNameCount);
	//��������
	bool HandleRoomData(ComRoomInfo * pRoomInfoPtr, UINT uRoomCount, UINT uKindID, UINT uNameID);
	//����ȡ������
	bool FillGetListItem();
	//ɾ����ʱ���� 
	bool DeleteOutTimeRoom(UINT uKindID, UINT uNameID);
	//��������������
	bool UpdateOnLineCount();
	//��ȡ��������
	bool GetProcessName(CAFCRoomItem * pGameRoomItem, TCHAR * szProcessName, UINT uBufferSize);
	//��ȡ��Ϸ����
	bool GetGameName(CAFCRoomItem * pGameRoomItem, TCHAR * szGameName, UINT uBufferSize);
	void UpdataItem(CAFCGameListItem* pItem);
	//��������
//private:
	//������������
	CAFCKindItem * FindKindItem(UINT uKindID);
	//������������
	CAFCNameItem * FindNameItem(CAFCKindItem * pKindItem, UINT uNameID);
	//���ҷ�������
	CAFCRoomItem * FindRoomItem(CAFCNameItem * pNameItem, UINT uRoomID);
public:
	void UpdateAllIcon(void);
	void JoinAllItem(void);
public:
	bool ExpandAll(bool ExpName=true);
};

/*******************************************************************************************************/
