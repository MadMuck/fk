#include "stdafx.h"
#include "NewGameClient.h"
#include "../Common/IGameFrame.h"
#include "UI_ForLogic/IUserInterface.h"
#include "UI_ForLogic/GameIDDefines.h"
#include "GameControlsIDDefine.h"
#include "Common/UIMessageDefines.h"

static  bool  AIChooseCard = false ;   

void MsgPrint(const char *lpszFormat, ...)
{
	va_list pArg;
	va_start(pArg, lpszFormat);
	char szMessage[1024] = { 0 };
#ifdef WIN32
	_vsnprintf(szMessage, 1023, lpszFormat, pArg);
#else
	vsnprintf(szMessage, 1023, lpszFormat, pArg);
#endif
	va_end(pArg);
	OutputDebugString(szMessage);
}

IGameImpl *CreateNewGameImpl(IGameFrame* pIBase, IUserInterface *pIUI)
{
	return dynamic_cast<IGameImpl*>(new CNewGameClient(pIBase, pIUI));
}
void DestroyNewGameImpl(IGameImpl **ppImp)
{
	delete *ppImp;
	*ppImp = NULL;
}
//���캯��
CNewGameClient::CNewGameClient(IGameFrame *pGameFrame, IUserInterface *pUI)
{
	m_pGameFrame = pGameFrame;
	m_pUI = pUI;

	m_SoundEngineGame = createIrrKlangDevice();
	m_SoundEnginBg = createIrrKlangDevice() ; 
	InitialGameData() ; 
}
//��������
CNewGameClient::~CNewGameClient()
{
	/*if(m_SoundEngineGame)
	{
		m_SoundEngineGame->drop();
	}

	if(m_SoundEnginBg)
	{
		m_SoundEnginBg->drop() ; 
	} */

}
//-----------------------------------------------------------------------------------
/// ��ʼ��
int CNewGameClient::Initial()
{
	//����UI
	wchar_t wszUIFile[MAX_PATH];
#ifdef NOT_FLAT
	::swprintf_s(wszUIFile,L"%d_UI_3D.dat",NAME_ID);
#else
	::swprintf_s(wszUIFile,L"%d_UI.dat",NAME_ID);
#endif
	m_pUI->LoadData(wszUIFile);
	return 0;
}
//-----------------------------------------------------------------------------------
/// ��ʼ��UI
int CNewGameClient::InitUI()
{
	//���س���
	ShowSuperSet(false);
	ResetGameDialog();

	//���˰�ť
	SetBtVisible(CTN_1014089_BTN_2004000,false);
	SetBtEnable(CTN_1014089_BTN_2004000,false);
	
	return 0;
}
//---------------------------------------------------------------------------------------
//����UI���� 
void	CNewGameClient::ResetGameDialog()
{
	//����������;
	for(int i=0;i<4;i++)
	{
		SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000214+i,0,true);
		SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000218+i,0,true);
		SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000210+i,"",true);
		//׼��ͼƬ
		//SetImageVisible(CTN_1014089_IMG_1004070+i,false);
		//��ׯ
		SetImageVisible(CTN_1014089_IMG_2000132+i,false);
		//����ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000137+i,false);
		//��ׯ����
		SetContainVisible(CTN_1014089_CTN_1008000+i,false);
		//ׯ��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004060+i,false);
	}

	SetBtVisible(CTN_1014089_BTN_1004006,false);
	//ʱ������
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011,false,0);
	SetContainVisible(CTN_1014089_CTN_1004100,false);

	

	//̯������
	SetContainVisible(CTN_1014089_CTN_2000206,false);

	//������ע����
	for(int i=0;i<4;i++)
	{
		//�������ֿռ���ʾ����
		SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+i,0,false);
	}
	SetContainVisible(CTN_1014089_CTN_2000246,true);
	
	//��ţ��ţ��ʾ��
	SetImageVisible(CTN_1014089_IMG_2000230,false);

	//�����Ʊ�
	SetImageVisible(CTN_1014089_IMG_1005003,false);
	
	//�ƿؼ�����
	SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, NULL,0, false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003030,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003050,NULL,0,false);
	
	//��ţ�ƿؼ�
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,NULL,0,false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,NULL,0, false);

	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003021,NULL,0, false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003022,NULL,0, false);

	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003031,NULL,0, false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003032,NULL,0, false);

	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003051,NULL,0, false);
	SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003052,NULL,0, false);

	//ţ����ʾ
	SetImageVisible(CTN_1014089_CTN_200300_IMG_2003013,false);
	SetImageVisible(CTN_1014089_CTN_200300_IMG_2003023,false);
	SetImageVisible(CTN_1014089_CTN_200300_IMG_2003033,false);
	SetImageVisible(CTN_1014089_CTN_200300_IMG_2003053,false);

	//��ע��������
	SetContainVisible(CTN_1014089_CTN_2000180,false);

	//��ʾ̯�ƺ����Ű�ť
	SetContainVisible(CTN_1014089_CTN_2000206,true);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
	SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);

	//ʱ��λ�û�ԭ
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000268));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,false,point.x,point.y);

	SetContainVisible(CTN_2000280_CTN_1003510,false);
	SetContainVisible(CTN_2000280,true);
	//���������Ŀ�ѡ��
	SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",true);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+i,false,false);
		SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+i,false,false);
		SetControlVisible(CTN_1014089_CTN_2000246_Num_2000251+i,false);
		SetControlVisible(CTN_1014089_CTN_2000246_IMG_2000247+i,false);
		//׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}

	SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,false);

}
/// ��ҽ���
int CNewGameClient::GameUserCome(void)
{
	
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 
	return 0;
}
//���ÿؼ��Ƿ�ɼ�
void CNewGameClient::SetControlVisible(const int iControlID,bool bFlag)
{
	IBCInterFace  *pControl = NULL ; 
	pControl = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID)); 
	if(NULL != pControl)
	{
		pControl->SetControlVisible(bFlag) ; 
	}
}
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ����
void	CNewGameClient::SetBtEnable(const int iButtonID, bool bFlag)
{

	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetEnable(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
//���ð�ť�Ƿ�ɼ�
void	CNewGameClient::SetBtVisible(const int iButtonID, bool bFlag)
{
	IButton *pButton = NULL;
	pButton = dynamic_cast<IButton *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pButton)
	{
		pButton->SetControlVisible(bFlag);
	}
}


//-------------------------------------------------------------------------------
/// ��״̬
void CNewGameClient::SetStationParameter(BYTE bGameStation)
{
	m_byGameStation = bGameStation ; 
}
//-------------------------------------------------------------------------------
///��ȡ��Ϸ״̬
BYTE CNewGameClient::GetStationParameter()
{
	return m_byGameStation;
	
}
//---------------------------------------------------------------------------------------
///��ʼ����Ϸ�е�����
void	CNewGameClient::InitialGameData()
{
	m_iVersion=DEV_HEIGHT_VERSION;
	m_iVersion2=DEV_LOW_VERSION;

	m_iMyDeskStation = 255;		///����Լ�����Ϸλ��
	m_byNtStation	 = 255;		//ׯ��λ��
	m_iHaveCattleTip=2;

	m_bIsSuper=false;
	m_bSoundPlay = false;		//�Ƿ���Բ�������
	m_bSoundBgPlay = false;		//������Ч
	
	
	////������Ϸ����
	ResetGameData();
	LoadGameSet();
}

//---------------------------------------------------------------------------------------
//��ȡ��Ϸ����
void	CNewGameClient::LoadGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	
	//������Ч
	m_bSoundBgPlay	= ("1" == f.GetKeyVal(key,"BgSound","1"));
	//��Ϸ��Ч
	m_bSoundPlay	= ("1" == f.GetKeyVal(key,"GameSound","1"));

}

//---------------------------------------------------------------------------------------
//������Ϸ����
void	CNewGameClient::SaveGameSet()
{
	CString nid;
	nid.Format("%d",NAME_ID);
	CString s = CINIFile::GetAppPath ();/////����·��
	CINIFile f( s +nid +"_c.ini");
	CString key = TEXT("Game");	

	IRadioButton *pRadioButton = NULL;
	//������Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_2000280_CTN_1003510_RButton_1003501));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			//����û�в��� ��ô�����Ҫ��ʼ���ű���������
			if (!m_bSoundBgPlay)
			{
				m_bSoundBgPlay = true;
				PlayBgSound(true);
			}
			f.SetKeyValString(key,"BgSound","1");
		}
		else
		{
			//������ڲ��� ��ô��Ҫֹͣ��
			if (m_bSoundBgPlay)
			{
				m_bSoundBgPlay = false;
				if (NULL != m_SoundEnginBg)
				{
					m_SoundEnginBg->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"BgSound","0");
		}
	}
	//��Ϸ��Ч
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(CTN_2000280_CTN_1003510_RButton_1003502));
	if (NULL != pRadioButton)
	{
		if (pRadioButton->GetIsSelect())
		{
			m_bSoundPlay = true;
			f.SetKeyValString(key,"GameSound","1");
		}
		else
		{
			//������ڲ�����Ч ��ʱ��Ҫֹͣ
			if (m_bSoundPlay)
			{
				m_bSoundPlay = false;
				if (NULL != m_SoundEngineGame)
				{
					m_SoundEngineGame->stopAllSounds();
				}
			}
			f.SetKeyValString(key,"GameSound","0");
		}
	}
}
//------------------------------------------------------------------------------
//��ʾ��Ϸ����
void	CNewGameClient::ShowGameSet(bool bShow)
{
	SetContainVisible(CTN_2000280_CTN_1003510,bShow);
	SetBtVisible(CTN_2000280_CTN_1003510_BTN_1003504,bShow);
	SetBtVisible(CTN_2000280_CTN_1003510_BTN_1003505,bShow);
	SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003501,m_bSoundBgPlay,bShow);
	SetRButtonSelected(CTN_2000280_CTN_1003510_RButton_1003502,m_bSoundPlay,bShow);
}
//------------------------------------------------------------------------------
/// ������Ϣ
int CNewGameClient::OnUIMessage(TUIMessage* pMessage)
{
	//��ť�������Ϣ
	if(pMessage->_uMessageType == UI_LBUTTONUP)
	{
		if(m_pGameFrame->GetIsWatching())
		{
			return 0;
		}
		switch(pMessage->_uControlID)
		{
		case CTN_1014089_BTN_1003500:	//���ð�ť
			{
				ShowGameSet(true);
				break;
			}
		case CTN_2000280_CTN_1003510_BTN_1003504:	//ȷ������
			{
				SaveGameSet();
				ShowGameSet(false);
				break;
			}
		case CTN_2000280_CTN_1003510_BTN_1003505:
			{
				ShowGameSet(false);
				break;
			}
		case CTN_1014089_BTN_1004006://��ʼ��ť
			{
				OnControlHitBeginBt();
				break;
			}
		case CTN_1014089_CTN_1008000_BTN_1008002://��ׯ��ť
			{
				OnHandleHitNtBt();
				break;
			}
		case CTN_1014089_CTN_1008000_BTN_1008001://������ׯ��ť
			{
				OnHandleHitGiveUpNtBt();
				break;
			}
		case CTN_1014089_CTN_2000180_BTN_2000198://��ע
		case CTN_1014089_CTN_2000180_BTN_2000199:
		case CTN_1014089_CTN_2000180_BTN_2000200:
		case CTN_1014089_CTN_2000180_BTN_2000201:
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_2000180_BTN_2000198;
				OnHandleStartBet(iHitButton);
				break;
			}
		case CTN_1014089_CTN_2000206_BTN_2000207://����/̯��
		case CTN_1014089_CTN_2000206_BTN_2000208:
		case CTN_1014089_CTN_2000206_BTN_2000209:
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_2000206_BTN_2000207;
				OnHandleIsShowCardBt(iHitButton);

				break;
			}
		case CTN_1014089_CTN_200300_OperateCard_2003010:	//����ɲ�����
			{
				OnOperateCardBt();
				break;
			}
		case CTN_1014089_CTN_1004100_BTN_2000231: //<������Ť 
		case CTN_1014089_CTN_1004100_BTN_2000232: //<�뿪��Ť 
			{
				//��ǰ��ť����
				int iHitButton = pMessage->_uControlID - CTN_1014089_CTN_1004100_BTN_2000231;
				OnClickContinueBt(iHitButton);
				break;
			}
		case CTN_1014089_BTN_2004000:	//���˰�ť
			{
				if (m_bIsSuper)
				{
					//��ʾ����
					ShowSuperSet(true);
				}
				break;
			}
		case CTN_2000280_CTN_100200_RButton_100201:
		case CTN_2000280_CTN_100200_RButton_100202:
		case CTN_2000280_CTN_100200_RButton_100203:
		case CTN_2000280_CTN_100200_RButton_100204:
			{
				bool bSeled = GetRButtonSelected(pMessage->_uControlID);
				
				for(int i=0; i<PLAY_COUNT;i++)
				{
					SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+i,false,(STATE_NULL != m_iUserStation[i]));
				}

				if (bSeled)
				{
					SetRButtonSelected(pMessage->_uControlID,bSeled);
					m_SuperSetData.byMaxDesk = m_pGameFrame->ViewStation2DeskStation(pMessage->_uControlID- CTN_2000280_CTN_100200_RButton_100201);

					//�����СҲ��ѡ�� ��ô��ȡ����
					if (GetRButtonSelected(pMessage->_uControlID+10))
					{
						SetRButtonSelected(pMessage->_uControlID+10,false);
					}
					if (m_SuperSetData.byMaxDesk == m_SuperSetData.byMinDesk)
					{
						m_SuperSetData.byMinDesk = 255;
					}
				}
				else
				{
					m_SuperSetData.byMaxDesk = 255;
				}
				break;
			}
		case CTN_2000280_CTN_100200_RButton_100211:
		case CTN_2000280_CTN_100200_RButton_100212:
		case CTN_2000280_CTN_100200_RButton_100213:
		case CTN_2000280_CTN_100200_RButton_100214:
			{
				bool bSeled = GetRButtonSelected(pMessage->_uControlID);
				for(int i=0; i<PLAY_COUNT;i++)
				{
					SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+i,false,(STATE_NULL != m_iUserStation[i]));
				}
				if (bSeled)
				{
					SetRButtonSelected(pMessage->_uControlID,bSeled);
					m_SuperSetData.byMinDesk = m_pGameFrame->ViewStation2DeskStation(pMessage->_uControlID- CTN_2000280_CTN_100200_RButton_100211);

					//������Ҳ��ѡ�� ��ô��ȡ����
					if (GetRButtonSelected(pMessage->_uControlID-10))
					{
						SetRButtonSelected(pMessage->_uControlID-10,false);
					}
					if (m_SuperSetData.byMinDesk == m_SuperSetData.byMaxDesk)
					{
						m_SuperSetData.byMaxDesk = 255;
					}
				}
				else
				{
					m_SuperSetData.byMinDesk = 255;
				}
				break;
			}
		case CTN_2000280_CTN_100200_BTN_100231:
			{
				//���ͳ�����������
				SuperUserSet();
				break;
			}
		case CTN_2000280_CTN_100200_BTN_100232:
			{
				ShowSuperSet(false);
				break;
			}
		default:
			{
				break;
			}
		}
	}
	if(pMessage->_uMessageType == UI_CATOONFINISH)
	{
		switch(pMessage->_uControlID)
		{
		case CTN_2000128_ACTION_2000128:			//���������Ϣ
		case CTN_2000129_ACTION_2000129:  
		case CTN_2000130_ACTION_2000130:
		case CTN_2000131_ACTION_2000131:
			{
				OnSendCardFinish();
				break;
			}
		case CTN_1014089_CTN_200400_CTN_2004030_Time_2004031:	//�Լ��ĵ���ʱ
			{
				if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_NEXT || GetStationParameter()==GS_WAIT_ARGEE)
				{
					ResetGameDialog();
					ResetGameData();
					//������Ŷӳ�������ȥ�Ŷ�
					/*if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
					{
						m_pGameFrame->JoinQueue();
					}
					else*/
					{
						m_pGameFrame->CloseClient();
					}
				}
				else
				{
					ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,0,0);
				}
				
			}
		}
	}
	return 0;
}



//���������Ϣ
void CNewGameClient::OnSendCardFinish()
{
	PlayGameSound(SOUND_XIAZHU,false);
	SetImageVisible(CTN_1014089_IMG_1005003,false);
	m_bySendCount[m_bySendCardPos]++;
	//���ƴ����ۼ�
	m_bySendCardCount++;	
	
	//չʾ��
	int iView = m_pGameFrame->DeskStation2View(m_bySendCardPos);
	if (2 == iView)
	{
		SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_bySendCardPos],m_bySendCount[m_bySendCardPos], true);
	}
	else
	{
		ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[m_bySendCardPos],m_bySendCount[m_bySendCardPos]);
	}
	
	//���ѷ����ƴ�˳��������ɾ��
	m_bySendCardPos = (m_bySendCardPos+1)%PLAY_COUNT;
	
	//�ж��Ƿ���Խ�������
	while (m_bySendCardCount < m_sendQueue.size())
	{
		if (m_sendQueue.at(m_bySendCardCount) != 0)
		{
			SendCard(m_sendQueue.at(0),m_bySendCardPos);
			break;
		}
		else
		{
			m_bySendCardCount++;
			m_bySendCardPos = (m_bySendCardPos+1)%PLAY_COUNT;
		}
	}
}
/*
���ܣ�����������뿪��ť��Ӧ����
�βΣ�iHitButton,��ť����
����ֵ��void
*/
void CNewGameClient::OnClickContinueBt(int iHitButton)
{
	if(0==iHitButton)//����
	{
		PlayGameSound(SOUND_HIT_BEGIN,false);
		ResetGameDialog();
		//����ʾ��ʼ��ť
		SetBtVisible(CTN_1014089_BTN_1004006,false);
		OnControlHitBeginBt();
	}
	else			//�뿪
	{
		m_pGameFrame->CloseClient();
	}
}
/*
���ܣ�����ɲ�������Ӧ����
�βΣ�void
����ֵ��void
*/
void CNewGameClient::OnOperateCardBt()
{
	if(GS_OPEN_CARD!=m_byGameStation)
	{
		return;
	}
	PlayGameSound(SOUND_CLICKCARD,false);
	int iUpCount=0;
	iUpCount=GetUpCardNum(CTN_1014089_CTN_200300_OperateCard_2003010);
	
	if(0==iUpCount || 2==iUpCount)
	{
		//̯�ư�ť����
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);
	}
	else
	{
		//̯�ư�ť������
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		m_bIsHitTip=false;
	}
}
/*
���ܣ����̯�ơ����Ű�ť������
�βΣ�iHitButton ,��ť����
����ֵ��void
*/
void CNewGameClient::OnHandleIsShowCardBt(int iHitButton)
{
	if(GS_OPEN_CARD!=m_byGameStation)
	{
		return;
	}

	if(0==iHitButton)//̯��
	{
		//��������̯�ƣ���֤�ڳ���������ţ
		OnHandleIsShowCardBt(2);

		PlayGameSound(SOUND_TANPAI,false);

		UserTanPai TUserTanPai;
		
		if(m_bIsHitTip)//������ʾ������ţ
		{
			for(int i=0; i<3; i++)
			{
				if (m_bUnderCard[i] != 255)
				{
					TUserTanPai.byUnderCount++;
				}
			}
		}
		else
		{
			int iUpCount=0;
			iUpCount=GetUpCardNum(CTN_1014089_CTN_200300_OperateCard_2003010);
			//��ȡ�������������
			BYTE bUnderCard[3];
			memset(bUnderCard,0,sizeof(bUnderCard));
			if (0 == iUpCount || 2== iUpCount )
			{
				if (2 == iUpCount)
				{
					//��ȡ�����������
					BYTE bTempUp[2];
					memset(bTempUp,255,sizeof(bTempUp));
					GetUpCard(CTN_1014089_CTN_200300_OperateCard_2003010,bTempUp);
					memset(bUnderCard,0,sizeof(bUnderCard));
					//ɨ������������������
					for(int i=0,j=0;i<CARD_NUMS;i++)
					{
						if(bTempUp[0]==m_DeskReadyCard[m_iMyDeskStation][i] || bTempUp[1]==m_DeskReadyCard[m_iMyDeskStation][i])
						{
							continue;
						}
						bUnderCard[j]=m_DeskReadyCard[m_iMyDeskStation][i];
						j++;
					}
					TUserTanPai.byUnderCount = 3;
					memcpy(m_bUnderCard,bUnderCard,sizeof(m_bUnderCard));
					memcpy(m_bUpCard,bTempUp,sizeof(m_bUpCard));
					//���ؿɲ�����
					SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010,m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation],false);
					//�������ŵ���
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,m_bUnderCard,3,true);
					//������������������
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,m_bUpCard,2,true);
				}
				else
				{
					memset(m_bUnderCard,0,sizeof(m_bUnderCard));
					//�ж��Ƿ���ţ
					BYTE bHaveCard[3];
					memset(bHaveCard,255,sizeof(bHaveCard));
					if(m_Logic.GetBull(m_DeskReadyCard[m_iMyDeskStation], 5, bHaveCard))
					{
						int j=0;
						int k=0;
						bool bHave=false;
						for(int i=0;i<5;i++)
						{
							bHave=false;
							for(k=0;k<3;k++)
							{
								if(m_DeskReadyCard[m_iMyDeskStation][i]==bHaveCard[k])
								{
									bHave=true;
									break;
								}
							}
							if(bHave)
							{
								continue;
							}
							m_bUpCard[j]=m_DeskReadyCard[m_iMyDeskStation][i];
							j++;
						}
					}

					memcpy(m_bUnderCard,bHaveCard,sizeof(m_bUnderCard));
					for(int i=0; i<3; i++)
					{
						if (m_bUnderCard[i] != 255)
						{
							TUserTanPai.byUnderCount++;
						}
					}
				}
			}
		}
		memcpy(TUserTanPai.byUnderCard,m_bUnderCard,sizeof(TUserTanPai.byUnderCard));
		m_pGameFrame->SendGameData(&TUserTanPai,sizeof(TUserTanPai),MDM_GM_GAME_NOTIFY,ASS_USER_OPEN,0);

		m_pGameFrame->KillTimer(ID_OPEN_CARD) ;
	}
	else if(1==iHitButton)		//��ʾ
	{
		PlayGameSound(SOUND_TIP,false);
		//�ж��Ƿ���ţ
		BYTE bHaveCard[3];
		memset(bHaveCard,255,sizeof(bHaveCard));
		if(m_Logic.GetBull(m_DeskReadyCard[m_iMyDeskStation], 5, bHaveCard))
		{
			int j=0;
			int k=0;
			bool bHave=false;
			for(int i=0;i<5;i++)
			{
				bHave=false;
				for(k=0;k<3;k++)
				{
					if(m_DeskReadyCard[m_iMyDeskStation][i]==bHaveCard[k])
					{
						bHave=true;
						break;
					}
				}
				if(bHave)
				{
					continue;
				}
				m_bUpCard[j]=m_DeskReadyCard[m_iMyDeskStation][i];
				j++;
			}
			m_bIsHitTip=true;
			memcpy(m_bUnderCard,bHaveCard,sizeof(m_bUnderCard));

			SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_iMyDeskStation],m_bySendCount[m_iMyDeskStation], false);
	
			//չʾ�����������,�������������
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,m_bUnderCard,3, true);
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,m_bUpCard,2,true);
			
			SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
			//̯�ư�ť����
			SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);
		}
	}
	else			//����
	{
		PlayGameSound(SOUND_CHONGPAI,false);
		SetImageVisible(CTN_1014089_IMG_2000230,false);
		m_bIsHitTip=false;

		memset(m_bUnderCard,255,sizeof(m_bUnderCard));
		memset(m_bUpCard,255,sizeof(m_bUpCard));
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,true);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);
		
		//��ʾ�Լ��Ŀɲ�������
		SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation], true);
		//����������ţ�ƿؼ�
		SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,m_DeskReadyCard[m_iMyDeskStation],0, false);
		SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,m_DeskReadyCard[m_iMyDeskStation],0, false);
	}
}
/*
���ܣ������˷�����ע��Ϣ
�βΣ�iHitButton ,��ť����
����ֵ��void
*/
void CNewGameClient::OnHandleStartBet(int iHitButton)
{
	if(m_byGameStation!=GS_NOTE)
	{
		return;
	}
	if(iHitButton>3)//��עʱ�䵽�Զ�Ϊ�û���ע��Сֵ
	{
		tagUserProcess userBet;
		userBet.iNoteType= 3;
		userBet.iVrebType = TYPE_NOTE;				//��ע����
		m_pGameFrame->SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
		
	}
	else		//�û�������ע
	{
		tagUserProcess userBet;
		userBet.iNoteType= iHitButton;
		userBet.iVrebType = TYPE_NOTE;				//��ע����
		
		m_pGameFrame->SendGameData(&userBet,sizeof(userBet),MDM_GM_GAME_NOTIFY,ASS_USER_NOTE,0);
	}
	
	
}
/*
���ܣ������˷��ͷ�����ׯ��Ϣ
�βΣ�void
����ֵ��void
*/
void CNewGameClient::OnHandleHitGiveUpNtBt()
{
	//���ŷ�����ׯ����
	;
	CallScoreStruct callScore;
	callScore.iValue=0;
	callScore.bDeskStation = m_iMyDeskStation;			
	callScore.bCallScoreflag = false;
	//���ͽ�ׯ��Ϣ
	m_pGameFrame->SendGameData(&callScore,sizeof(callScore),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);
	PlayGameSound(SOUND_GIVEUP,false);
}
/*
���ܣ������˷��ͽ�ׯ��Ϣ
�βΣ�void
����ֵ��void
*/
void CNewGameClient::OnHandleHitNtBt()
{
	CallScoreStruct callScore;
	callScore.iValue=1;
	callScore.bDeskStation = m_iMyDeskStation;			
	callScore.bCallScoreflag = false;
	//���ͽ�ׯ��Ϣ
	m_pGameFrame->SendGameData(&callScore,sizeof(callScore),MDM_GM_GAME_NOTIFY,ASS_USER_SCORE,0);
	PlayGameSound(SOUND_JIAOZHUANG,false);
}
/*
���ܣ���ҵ����ʼ��ť�����Ϳ�ʼ��Ϣ
������void
����ֵ��void
*/
void CNewGameClient::OnControlHitBeginBt()
{
	if (GetStationParameter()==GS_WAIT_SETGAME || GetStationParameter()==GS_WAIT_NEXT || GetStationParameter()==GS_WAIT_ARGEE)
	{
		//�Ŷӳ� �����Ŷ�
		/*if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
		{
			m_pGameFrame->JoinQueue();
		}
		else*/
		{
			m_pGameFrame->ClickContinue();
		}
		//�����˷��Ϳ�ʼ��Ϣ
		//m_pGameFrame->SendGameData(NULL,0,MDM_GM_GAME_NOTIFY,ASS_GM_AGREE_GAME,0);
	}
	
	PlayGameSound(SOUND_HIT_BEGIN,false);
	KillAllGameTime();
	//���ؿ�ʼ��ť
	SetBtVisible(CTN_1014089_BTN_1004006,FALSE);
	//��ʼ��ť������
	SetBtEnable(CTN_1014089_BTN_1004006,FALSE);
	m_pGameFrame->KillTimer(ID_BEGIN_TIME) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
	m_pGameFrame->KillTimer(ID_WAIT_NEXT) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
	
}

/*------------------------------------------------------------------------------*/
//����
/*
���ܣ�������ţ����
�βΣ�iHaveCattle����ţֵ
����ֵ��void
*/
void CNewGameClient::ShowHaveCattleAction(const int iImageID,int iCardShape,bool bShow,BYTE byDeskStation)
{
	CString imageSrc;
	switch(iCardShape)
	{
	case UG_NO_POINT:									//��ţ0x00
		{
			imageSrc.Format("./image/0Num.png");
			break;
		}
	case UG_BULL_ONE:									//ţһ0x01
		{
			imageSrc.Format("./image/1Num.png");
			break;
		}
	case UG_BULL_TWO:									//ţ��0x02
		{
			imageSrc.Format("./image/2Num.png");
			break;
		}
	case UG_BULL_THREE:									//ţ�� 0x03
		{
			imageSrc.Format("./image/3Num.png");
			break;
		}
	case UG_BULL_FOUR:									//ţ��0x04
		{
			imageSrc.Format("./image/4Num.png");
			break;
		}
	case UG_BULL_FIVE:									//ţ��0x05 
		{
			imageSrc.Format("./image/5Num.png");
			break;
		}
	case UG_BULL_SIX:									//ţ��0x06
		{
			imageSrc.Format("./image/6Num.png");
			break;
		}
	case UG_BULL_SEVEN:								//ţ��0x07
		{
			imageSrc.Format("./image/7Num.png");
			break;
		}
	case UG_BULL_EIGHT:								//ţ��0x08 
		{
			imageSrc.Format("./image/8Num.png");
			break;
		}
	case UG_BULL_NINE:								//ţ�� 0x09
		{
			imageSrc.Format("./image/9Num.png");
			break;
		}
	case UG_BULL_BULL:								//ţţ0x0A
		{
			imageSrc.Format("./image/10Num.png");
			break;
		}
	default:
		{
			break;
		}
	}
	SetImageLoadSrc(iImageID,imageSrc,bShow);
}
//-------------------------------------------------------------------------------------------------------
//������������
void	CNewGameClient::PlayShapeSound(int iCardShape,bool bShow,BYTE byDeskStation)
{
	switch(iCardShape)
	{
	case UG_NO_POINT:									//��ţ0x00
		{
			PlayGameSound(SOUND_NIUNIU_0,false,byDeskStation);
			break;
		}
	case UG_BULL_ONE:									//ţһ0x01
		{
			PlayGameSound(SOUND_NIUNIU_1,false,byDeskStation);
			break;
		}
	case UG_BULL_TWO:									//ţ��0x02
		{
			PlayGameSound(SOUND_NIUNIU_2,false,byDeskStation);
			break;
		}
	case UG_BULL_THREE:									//ţ�� 0x03
		{
			PlayGameSound(SOUND_NIUNIU_3,false,byDeskStation);
			break;
		}
	case UG_BULL_FOUR:									//ţ��0x04
		{
			PlayGameSound(SOUND_NIUNIU_4,false,byDeskStation);
			break;
		}
	case UG_BULL_FIVE:									//ţ��0x05 
		{
			PlayGameSound(SOUND_NIUNIU_5,false,byDeskStation);
			break;
		}
	case UG_BULL_SIX:									//ţ��0x06
		{
			PlayGameSound(SOUND_NIUNIU_6,false,byDeskStation);
			break;
		}
	case UG_BULL_SEVEN:								//ţ��0x07
		{
			PlayGameSound(SOUND_NIUNIU_7,false,byDeskStation);
			break;
		}
	case UG_BULL_EIGHT:								//ţ��0x08 
		{
			PlayGameSound(SOUND_NIUNIU_8,false,byDeskStation);
			break;
		}
	case UG_BULL_NINE:								//ţ�� 0x09
		{
			PlayGameSound(SOUND_NIUNIU_9,false,byDeskStation);
			break;
		}
	case UG_BULL_BULL:								//ţţ0x0A
		{
			PlayGameSound(SOUND_NIUNIU_10,false,byDeskStation);
			break;
		}
	default:
		{
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------------
//��ʾ���˽������
void	CNewGameClient::ShowSuperSet(bool bFlag)
{
	SetContainVisible(CTN_2000280_CTN_100200,bFlag);
	//���������е�
	SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",false);
	for(int i=0; i<PLAY_COUNT; i++)
	{
		SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+i,false,false);
		SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+i,false,false);
	}

	//����Ҫ������
	UserInfoForGame sUser;
	int iView =0;
	if (m_bIsSuper)
	{
		for(int i=0; i<PLAY_COUNT; i++)
		{
			memset(&sUser,0,sizeof(sUser));
			if (STATE_NULL != m_iUserStation[i] && m_pGameFrame->GetUserInfo(i,sUser))
			{
				iView = m_pGameFrame->DeskStation2View(i);
				SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+iView,(m_SuperSetData.byMaxDesk == i),true);
				SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+iView,false/*(m_SuperSetData.byMinDesk == i)*/,true);
			}
		}

		if(!m_SuperSetData.bSetSuccese)
		{
			SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",true);
		}
		else
		{
			SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"�������óɹ�",true);
		}
	}
}
//���ͳ�����������
void	CNewGameClient::SuperUserSet()
{
	if(GetStationParameter() < GS_ROB_NT || GetStationParameter() >=GS_OPEN_CARD)
	{
		return;
	}

	if (!m_bIsSuper)
	{
		return;
	}
	if (m_SuperSetData.byMaxDesk == m_SuperSetData.byMinDesk || (m_SuperSetData.byMinDesk == 255 && m_SuperSetData.byMaxDesk == 255))
	{
		return;
	}

	m_pGameFrame->SendGameData(&m_SuperSetData,sizeof(m_SuperSetData),MDM_GM_GAME_NOTIFY,ASS_SUPER_USER_SET,0);

}

//--------------------------------------------------------------------------------------------------------
//���õ�ѡ��ؼ��Ƿ�ѡ��
void	CNewGameClient::SetRButtonSelected(const int iRButtonID,bool bSelected,bool bShow )
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		pRadioButton->SetIsSelect(bSelected);
		pRadioButton->SetControlVisible(bShow);

	}
}
//--------------------------------------------------------------------------------------------------------
//��ȡ��ѡ��ؼ��Ƿ�ѡ��
bool	CNewGameClient::GetRButtonSelected(const int iRButtonID)
{
	IRadioButton *pRadioButton = NULL;
	pRadioButton = dynamic_cast<IRadioButton *>(m_pUI->GetIControlbyID(iRButtonID));
	if (NULL != pRadioButton)
	{
		return pRadioButton->GetIsSelect();
	}
	return false;
}
//--------------------------------------------------------------------------------------------------------
//���ó���ؼ���ʾֵ
void	CNewGameClient::SetShCtrlInfo(const int iShCtrlID, int iNum, bool bFlag)
{
	IShCtrl *pShCtrl = NULL;
	pShCtrl = dynamic_cast<IShCtrl *>(m_pUI->GetIControlbyID(iShCtrlID));
	if (NULL != pShCtrl)
	{
	pShCtrl->SetShNote(iNum);
	pShCtrl->SetControlVisible(bFlag);
	}

}
//-----------------------------------------------------------------------------------------
//���ÿؼ�λ��
void   CNewGameClient::SetControlPos(const int iControlID,bool bFlag,int x,int y)
{
	IBCInterFace *pContain = NULL;
	pContain = dynamic_cast<IBCInterFace *>(m_pUI->GetIControlbyID(iControlID));
	if (NULL != pContain)
	{
		pContain->SetGameXY(x,y); 
		pContain->SetControlVisible(bFlag);
	}
}
//-----------------------------------------------------------------------------------------
//���ò��ɲ����ƿؼ�����
void	CNewGameClient::SetOperateCardInfo(const int iOperateCardID, BYTE byCardList[],BYTE byCardCount, bool bShow)
{
	IOperateCard *pOperateCard = NULL;
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iOperateCardID)); 
	if(NULL != pOperateCard)
	{
		pOperateCard->SetCardList(byCardList,byCardCount);
		pOperateCard->SetControlVisible(bShow);

	} 
}
//-----------------------------------------------------------------------------------------
//���ò��ɲ����ƿؼ�����
void	CNewGameClient::SetNoMoveCardInfo(const int iNoMoveCardID,BYTE byCardList[],BYTE byCardCount, bool bShow)
{
	INoMoveCard *pNoMoveCard = NULL;
	pNoMoveCard = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iNoMoveCardID)); 
	if(NULL != pNoMoveCard)
	{
		pNoMoveCard->SetCardList(byCardList,byCardCount);
		pNoMoveCard->SetControlVisible(bShow);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ�������ʾָ�����ļ�
void	CNewGameClient::SetImageLoadSrc(const int iImageID,CString sImagSrc,bool bFlag)
{
		
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetControlVisible(bFlag);
	}
}

//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ��Ƿ�ɼ�
void	CNewGameClient::SetImageVisible(const int iImageID, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetControlVisible(bFlag);
	}
}
//-----------------------------------------------------------------------------------------
/// ��Ϸ��Ϣ
int CNewGameClient::HandleGameMessage(WORD nMainID,WORD nSubId,BYTE * buffer,int nLen)
{	
	switch(nSubId)
	{
	case ASS_GM_GAME_STATION:   ///<����״̬(ƽ̨���õ�)
		{
			SetGameStation(buffer, nLen);    
			break;        
		}
	case ASS_GM_AGREE_GAME:			//ͬ����Ϸ
		{
			if(nLen!=sizeof(MSG_GR_R_UserAgree))
			{
				return 0;
			}	
			OnAgreeGame(buffer);
			break;
		}
	case ASS_CALL_SCORE:				//���ܽ�ׯ��Ϣ
		{
			if(nLen!=sizeof(CallScoreStruct))
			{
				return 0;
			}
			OnHandleCallScoreMessage(buffer);
			SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,true);
			break;
		}
	case ASS_CALL_SCORE_RESULT:			//���ܽ�ׯ�����Ϣ
		{
			if (nLen != sizeof(CallScoreStruct)) 
			{
				return 0;
			}
			OnHandleCallScoreResultMessage(buffer);
			break;
		}
	case ASS_CALL_SCORE_FINISH:			//���ܽ�ׯ������Ϣ
		{
			if (nLen != sizeof(CallScoreStruct)) 
			{
				return 0;
			}
			OnHandleCallScoreFinishMessage(buffer);
			break;
		}
	case ASS_CALL_NOTE:				//��ʼ��ע
		{
			if (nLen !=sizeof(BeginUpgradeStruct))
			{
				return 0;
			}
			OnHandleBetBegin(buffer);
			break;
		}
	case ASS_CALL_NOTE_RESULT:			//��ע���
		{
			if (nLen != sizeof(NoteResult)) 
			{
				return 0;
			}
			OnHandleBetResult(buffer);
			break;
		}
	case ASS_CALL_SEND_CARD:				//��ʼ����
		{

			if(nLen != sizeof(SendAllCardStruct))
			{
				return FALSE;
			}
			
			OnHandleSendCardBegin(buffer);
			SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,false);
			break;
		}
	case ASS_CALL_SEND_FINISH:		//���ƽ���
		{
			OnHandleSendFinish();
			break;
		}
	case ASS_CALL_OPEN:			//֪ͨ���ư�ţ
		{
			if(nLen != sizeof(BeginPlayStruct))
			{
				return FALSE;
			}
			OnHandleGameBegin(buffer);
			break;
		}
	case ASS_CALL_OPEN_RESULT:			//̯��
		{
			if(nLen != sizeof(UserTanPai))
			{
				return FALSE;
			}
			OnHandleOpenCard(buffer);
			break;
		}
	case ASS_CONTINUE_END:		//��Ϸ����
		{
			if(nLen != sizeof(GameEndStruct))
			{
				return FALSE;
			}
			OnGameFinish(buffer);
			break;
		}
	
	case ASS_CUT_END:		//��ҵ���
		{
			if(nLen != sizeof(GameCutStruct))
			{

				return FALSE;
			}
			OnHandlePlayerCut(buffer);
			break;
		}
	case ASS_SALE_END:		//����˳�
		{
			if(nLen != sizeof(GameCutStruct))
			{
				return FALSE;
			}
			OnHandlePlayerSafeEnd(buffer);
			break;
		}
	case ASS_SUPER_USER:	//���˿ͻ���
		{  
			if(nLen!=sizeof(SUPERSTATE))
			{
				return 0;
			}	

			HandleSuperMessage(buffer);			
			break;
		}
	case ASS_SUPER_USER_SET_RESULT:	//�������ý��
		{
			if(nLen!=sizeof(SuperUserSetData))
			{
				return 0;
			}	
			SuperUserSetData * pSuperSet = (SuperUserSetData *)buffer;
			if (NULL == pSuperSet)
			{
				return 0;
			}

			memcpy(&m_SuperSetData, pSuperSet, sizeof(m_SuperSetData));
			ShowSuperSet(true);
			break;
		}
	default:
		{
			break;
		}

	}
	return 0;
}
//����ͬ����Ϸ��Ϣ
void CNewGameClient::OnAgreeGame(void *buffer)
{
	MSG_GR_R_UserAgree * agreeGame=(MSG_GR_R_UserAgree *)buffer;
	if(NULL==agreeGame)
	{
		return ;
	}
	
	//���Լ�
	if (m_iMyDeskStation == agreeGame->bDeskStation)
	{
		ResetGameDialog();
		KillAllGameTime();

		//���ؿ�ʼ��ť
		SetBtVisible(CTN_1014089_BTN_1004006,FALSE);
		//��ʼ��ť������
		SetBtEnable(CTN_1014089_BTN_1004006,FALSE);
		//ʱ�ӵ���ʱ
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,0);
	}
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(agreeGame->bDeskStation);
	//��ʾ׼��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004070+viewStation,TRUE);
}
/*
@brief��������������˳���Ϣ
@param����ָ��
@return��void
*/
void CNewGameClient::OnHandlePlayerSafeEnd(void *buffer)
{
	GameCutStruct *playerSafeEnd = (GameCutStruct *)buffer ;
	if(NULL==playerSafeEnd)
	{

		return;
	}
}
/*
@brief���������˷���������Ϸ����˳���Ϣ
@param����ָ��
@return��void
*/
void CNewGameClient::OnHandlePlayerCut(void *buffer)
{
	GameCutStruct *playerCut = (GameCutStruct *)buffer ;
	if(NULL==playerCut)
	{

		return;
	}
	SetStationParameter(GS_WAIT_NEXT);
	//��ʾ��ʼ��ť���ص��ȴ���ʼ״̬
	KillAllGameTime();
	ResetGameData();
	ResetGameDialog();
}
/*
@brief���������û���Ϣ
@param����ָ��
@return��void
*/
void	CNewGameClient::HandleSuperMessage(void *buffer)
{

	SUPERSTATE *pSuperUser = (SUPERSTATE *) buffer;
	if (NULL == pSuperUser)
	{
		return ;
	}
	
	if(m_pGameFrame->GetIsWatching())
	{
		return;
	}

	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation(); 

	if(m_iMyDeskStation == pSuperUser->byDeskStation)
	{
		m_bIsSuper	 = pSuperUser->bEnable;	//�Ƿ񳬼��û�

		//��ʾ���˰�ť
		SetBtVisible(CTN_1014089_BTN_2004000,true);
		SetBtEnable(CTN_1014089_BTN_2004000,true);
	}
}
/*
���ܣ��������˷���������Ϸ̯����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleOpenCard(void *buffer)
{
	UserTanPai *pUserTanPaiResult = (UserTanPai *)buffer ;
	if(NULL==pUserTanPaiResult)
	{
		return;
	}
	if (pUserTanPaiResult->byDeskStation != m_iMyDeskStation)
	{
		int iView = m_pGameFrame->DeskStation2View(pUserTanPaiResult->byDeskStation);
		//���ضԼҵ�����
		SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[pUserTanPaiResult->byDeskStation],0,false);
		if (pUserTanPaiResult->iShape == UG_NO_POINT || 0 == pUserTanPaiResult->byUnderCount)
		{
			//����0�ŵ���
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003022+iView*10,m_DeskReadyCard[pUserTanPaiResult->byDeskStation],0,false);
			//����5������������
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003021+iView*10,m_DeskReadyCard[pUserTanPaiResult->byDeskStation],m_iDeskCardCount[pUserTanPaiResult->byDeskStation],true);
		}
		else
		{
			//����3�ŵ���
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003021+iView*10,pUserTanPaiResult->byUnderCard,3,true);
			//����2������������
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003022+iView*10,pUserTanPaiResult->byUpCard,2,true);
		}
		//��ʾ������ʾ
		ShowHaveCattleAction(CTN_1014089_CTN_200300_IMG_2003023+iView*10,pUserTanPaiResult->iShape,true,pUserTanPaiResult->byDeskStation);
	}
	else
	{
		SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation], false);
		//��ţ
		if (pUserTanPaiResult->iShape == UG_NO_POINT)
		{
			//����0�ŵ���
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,m_DeskReadyCard[m_iMyDeskStation],0,false);
			//����5������������
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation],true);
		}
		else
		{
			//����0�ŵ���
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,pUserTanPaiResult->byUnderCard,3,true);
			//����5������������
			SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,pUserTanPaiResult->byUpCard,2,true);
		}
		//��ʾ������ʾ
		ShowHaveCattleAction(CTN_1014089_CTN_200300_IMG_2003013,pUserTanPaiResult->iShape,true,pUserTanPaiResult->byDeskStation);
	}
	PlayShapeSound(pUserTanPaiResult->iShape,true,pUserTanPaiResult->byDeskStation);


	if (pUserTanPaiResult->byDeskStation == m_iMyDeskStation)
	{
		m_pGameFrame->KillTimer(ID_OPEN_CARD) ;
		for(int i=0;i<3;i++)
		{
			SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207+i,false);
		}
	}	
}
/*
���ܣ��������˷���������Ϸ������Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnGameFinish(void *buffer)
{
	GameEndStruct *gameFinish = (GameEndStruct *)buffer ;
	if(NULL==gameFinish)
	{
		return;
	}
	
	SetStationParameter(GS_WAIT_NEXT);
	//����̯������
	SetContainVisible(CTN_1014089_CTN_2000206,false);

	//��ʾ��������
	ShowResultDlg(gameFinish,true);

	if(gameFinish->iChangeMoney[m_iMyDeskStation]>=0)
	{
		PlayGameSound(SOUND_WIN,false);
	}
	else
	{
		PlayGameSound(SOUND_LOSE,false);
	}

	//ʱ��λ�û�ԭ
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000268));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,true,point.x,point.y);

	//��ʾ��ʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iBeginTime);

	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel=m_iBeginTime;
	//�������ƶ�ʱ��
	m_pGameFrame->SetTimer(ID_WAIT_NEXT,1000);

}
/*--------------------------------------------------------------------------------------*/
//��ʾ�����
void	CNewGameClient::ShowResultDlg(GameEndStruct *pGameFinish,bool bShow)
{
	SetContainVisible(CTN_1014089_CTN_1004100,bShow);

	//���������еĿؼ�
	for (int i=0; i<PLAY_COUNT;i++)
	{
		SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000210+i,"",false);
		SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000214+i,"",false);
		SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000218+i,"",false);
		SetImageVisible(CTN_1014089_CTN_1004100_IMG_2000222+i,false);
		SetImageVisible(CTN_1014089_CTN_1004100_IMG_2000226+i,false);
	}

	if (NULL == pGameFinish)
	{
		return;
	}

	UserInfoForGame UserInfo;
	memset(&UserInfo,0,sizeof(UserInfo));
	int iIndex = 0;
	CString imageSrc;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if (STATE_NULL == m_iUserStation[i])
		{
			continue;
		}
		//����״̬�����
		if (pGameFinish->iUserState[i] != -1 && m_pGameFrame->GetUserInfo(i,UserInfo))
		{
			//�ǳ�
			SetTextShowText(CTN_1014089_CTN_1004100_TEXT_2000210+iIndex,UserInfo.szNickName,bShow);
			//����
			//SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000214+iIndex,pGameFinish->iTurePoint[i],bShow);
			//���
			SetTextinfo(CTN_1014089_CTN_1004100_TEXT_2000218+iIndex,pGameFinish->iChangeMoney[i],bShow);
			//����
			imageSrc.Format("./image/%dNum.png",pGameFinish->iCardShape[i]);
			SetImageLoadSrc(CTN_1014089_CTN_1004100_IMG_2000222+iIndex,imageSrc,bShow);
			//��Ӯ���
			if (pGameFinish->iChangeMoney[i]>0)
			{
				imageSrc.Format("./image/win.png");
			}
			else if (pGameFinish->iChangeMoney[i]<0)
			{
				imageSrc.Format("./image/lost.png");
			}
			else
			{
				imageSrc.Format("./image/equal.png");
			}
			SetImageLoadSrc(CTN_1014089_CTN_1004100_IMG_2000226+iIndex,imageSrc,bShow);

			iIndex++;
		}
	}
}
/*--------------------------------------------------------------------------------------*/
/*
���ܣ��������˷���������Ϸ��ʼ��Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleGameBegin(void *buffer)
{
	BeginPlayStruct *gameBegin = (BeginPlayStruct *)buffer ;
	if(NULL==gameBegin)
	{
		return;
	}
	
	SetStationParameter(GS_OPEN_CARD);

	

	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,true,point.x,point.y);
	
	//������ע��ʱ��
	m_bCurrentClockLabel=m_iThinkTime;
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iThinkTime);
	//�������ƶ�ʱ��
	m_pGameFrame->SetTimer(ID_OPEN_CARD,1000);

}
/*
���ܣ��������˷������ķ��ƽ�����Ϣ
�βΣ�void 
����ֵ��void
*/
void CNewGameClient::OnHandleSendFinish()
{
	
	if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
	}
	else
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,true);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,true);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,true);
	}
	

	//չʾ��
	int iView = 0;
	for(int i=0; i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		if (2 == iView)
		{
			SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[i],m_iDeskCardCount[i], true);
		}
		else
		{
			ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[i],m_bySendCount[i]);
		}
	}
}
/*
���ܣ��������˷������ķ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleSendCardBegin(void *buffer)
{
	
	SendAllCardStruct *pSendAllCard = (SendAllCardStruct *)buffer ;
	if(NULL==pSendAllCard)
	{
		return;
	}
	
	//��ע��� ��ʼ���� �������м�ʱ��
	ClearAllClock();
	//��ǰ���Ƶ�λ��
	m_bySendCardPos = pSendAllCard->iStartPos;
	m_bySendCardCount = 0;
	SetStationParameter(GS_SEND_CARD);
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//ÿ�������˿˵���Ŀ
		m_iDeskCardCount[i]=pSendAllCard->iUserCardCount[i];
	}
	memcpy(m_DeskReadyCard,pSendAllCard->iUserCard,sizeof(m_DeskReadyCard));

	//��ҷ�������
	memset(m_bySendCount,0,sizeof(m_bySendCount));

	SendQueue(pSendAllCard->iStartPos);

	if(!m_sendQueue.empty())
	{
		SendCard(m_sendQueue.at(0),m_bySendCardPos);
	}
}
/*
���ܣ��������˷������Ľ�ׯ������Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleCallScoreFinishMessage(void *buffer)
{
	CallScoreStruct *scoreFinish=(CallScoreStruct *)buffer;
	if(NULL==scoreFinish)
	{
		return ;
	}
	
	//���������ׯ/����ׯͼƬ
	m_byNtStation=scoreFinish->bDeskStation;
	
	//���ؽ�ׯ��ť
	SetContainVisible(CTN_1014089_CTN_1008000,FALSE);
	//ɾ����ׯ��ʱ��
	m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
		
	//���������ׯ/����ׯͼƬ
	CLearScratchNt();
	//�������ʱ��
	ClearAllClock();

}
/*
���ܣ��������˷������Ľ�ׯ�����Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleCallScoreResultMessage(void *buffer)
{
	CallScoreStruct *scoreResult=(CallScoreStruct *)buffer;
	if(NULL==scoreResult)
	{
		return ;
	}
	
	if(m_iMyDeskStation==scoreResult->bDeskStation)//��ǰ��ׯ���Լ�
	{
		//���ؽ�ׯ��ť
		SetContainVisible(CTN_1014089_CTN_1008000,FALSE);
		//ɾ����ׯ��ʱ��
		m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
	}

	if(0==scoreResult->iValue)//������ׯ
	{
		//��ʾ������ׯͼƬ
		//��λ��ת��ͼλ��
		int viewStation=m_pGameFrame->DeskStation2View(scoreResult->bDeskStation);
		//������ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000132+viewStation,FALSE);
		//��ʾ����ׯͼƬ
		SetImageVisible(CTN_1014089_IMG_2000137+viewStation,TRUE);
	}

	int iView = m_pGameFrame->DeskStation2View(scoreResult->bDeskStation);
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView*10,false,m_iCallScoreTime);

}
/*
���ܣ��������˷������Ľ�ׯ��Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleCallScoreMessage(void *buffer)
{

	CallScoreStruct *score=(CallScoreStruct *)buffer;	
	if(NULL==score)
	{
		return ;
	}
	
	ResetGameData();
	//������Ϸ��ʼ��Ч
	PlayGameSound(SOUND_BEGIN_GAME,false);
	
	//����״̬
	SetStationParameter(GS_ROB_NT);

	//������ҵ�״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		m_iUserStation[i] = score->byUserState[i];
	}

	//����Ҫ������
	UserInfoForGame sUser;
	int iView =0;
	if (m_bIsSuper)
	{
		for(int i=0; i<PLAY_COUNT; i++)
		{
			memset(&sUser,0,sizeof(sUser));
			if (STATE_NULL != m_iUserStation[i] && m_pGameFrame->GetUserInfo(i,sUser))
			{
				iView = m_pGameFrame->DeskStation2View(i);
				SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100201+iView,(m_SuperSetData.byMaxDesk == i),true);
				SetRButtonSelected(CTN_2000280_CTN_100200_RButton_100211+iView,false/*(m_SuperSetData.byMinDesk == i)*/,true);
			}
		}

		if(!m_SuperSetData.bSetSuccese)
		{
			SetTextShowText(CTN_2000280_CTN_100200_TEXT_100220,"��ǰδ����",true);
		}
	}


	m_bCurrentOperationStation = score->bDeskStation;
	
	if(m_bCurrentOperationStation != 255)
	{
		if(m_bCurrentOperationStation==m_iMyDeskStation)
		{
			//��ʾ��ׯ��ť
			SetContainVisible(CTN_1014089_CTN_1008000,TRUE);
		}
		//��λ��ת��ͼλ��
		int viewStation=m_pGameFrame->DeskStation2View(m_bCurrentOperationStation);
		//������Ӧ��ׯ��ʱ��
		OnStartClock(viewStation,m_iCallScoreTime);
	}
	for(int i=0;i<PLAY_COUNT;i++)
	{
		//����׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,FALSE);
	}
}
/*
���ܣ�������Ϸ��ʼ��ע��Ϣ
�βΣ���ָ��
����ֵ��void
*/
void CNewGameClient::OnHandleBetBegin(void *buffer)
{
	BeginUpgradeStruct *beginGame=(BeginUpgradeStruct *)buffer;	
	if(NULL==beginGame)
	{
		return ;
	}
	
	//���ÿ�ʼ������ʼλ��
	//������Ϸ״̬
	SetStationParameter(GS_NOTE);
	m_bTimeOutCount=0;	

	
	::CopyMemory(m_iLimitNote,&beginGame->iLimitNote,sizeof(beginGame->iLimitNote));	//���ע
	memcpy(m_i64UserNoteLimite,beginGame->i64UserNoteLimite,sizeof(m_i64UserNoteLimite));


	m_iUpGradePeople = beginGame->iNt;                    //ׯ��
	m_iThisTurnLimit=beginGame->iLimitNote[m_iMyDeskStation];

	//��ʾׯ��λ��
	int iView = m_pGameFrame->DeskStation2View(m_iUpGradePeople);
	SetImageVisible(CTN_1014089_IMG_1004060+iView,true);
	
	
	if(255!=m_byNtStation)
	{
		//ʱ��λ�øı�
		POINT point;
		IText *pText;
		pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
		point.x=pText->GetGameX();
		point.y=pText->GetGameY();
		SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,true,point.x,point.y);
		//��ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iXiaZhuTime);

		if (m_iMyDeskStation!=m_byNtStation && m_iUserStation[m_iMyDeskStation] != STATE_NULL)
		{
			//������ע��ʱ��
			m_bCurrentClockLabel=m_iXiaZhuTime;
			m_pGameFrame->SetTimer(ID_NOTE,1000);
			//��ʾ��ע
			DisplayBet(m_iThisTurnLimit);
		}
	}
}

/*
���ܣ�������ע���
�βΣ���ָ��
����ֵ����
*/
void CNewGameClient::OnHandleBetResult(void *buffer)
{
	NoteResult *betResult=(NoteResult *)buffer;	
	if(NULL==betResult)
	{
		return ;
	}
	//dwjtest
	//if(TYPE_OPEN==betResult->bAddStyle)//̯��������ܽ������ͷ���Ҳ����յ���ע��Ϣ
	//{
	//	return;
	//}
	
	PlayGameSound(SOUND_XIAZHU,false);
	if(m_iMyDeskStation==betResult->iOutPeople)//��ǰ����Ѿ���ע
	{
		//��ֹ��ע
		ClearAllBet();
	}
	
	//��¼����ÿ���˵���ע��
	m_iTotalGameNote[betResult->iOutPeople]=betResult->iCurNote;
	//��λ��ת��ͼλ��
	int iView = m_pGameFrame->DeskStation2View(betResult->iOutPeople);
	//������ע��ʱ��
	//ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView*10,false,0);
	//������ע
	//SetContainVisible(CTN_1014089_CTN_2000246,true);
	//��Ӧ�����ʾ��ע
	SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
	SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,betResult->iCurNote,true);
	//for(int i=0;i<PLAY_COUNT;i++)
	//{
	//	//��λ��ת��ͼλ��
	//	iView=m_pGameFrame->DeskStation2View(i);
	//	if(m_iTotalGameNote[i]>0)
	//	{
	//		SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
	//		SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,m_iTotalGameNote[i],true);
	//	}
	//	else
	//	{
	//		SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,false);
	//		SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,0,false);
	//	}
	//}
}
/*
���ܣ���ʾ�ĸ���עѡ��
�βΣ�����������ע��
����ֵ��void
*/
void CNewGameClient::DisplayBet(__int64 i64MaxBetCount)
{

	__int64 i64UserCurrentMoney=0;//�û���ǰ�����
	UserInfoForGame UserInfo ;
	memset(&UserInfo,0,sizeof(UserInfo));
	//������ע�����ɼ�
	SetContainVisible(CTN_1014089_CTN_2000180,true);
	//����û��Ľ����
	if(m_pGameFrame->GetUserInfo(m_iMyDeskStation, UserInfo))
	{
		i64UserCurrentMoney=UserInfo.i64Money;
	}
	if(i64MaxBetCount>0)
	{
		for(int i=0;i<4;i++)
		{
			if(m_i64UserNoteLimite[m_iMyDeskStation][i]<=i64UserCurrentMoney)
			{
				//������ע
				BetOptionSet(m_i64UserNoteLimite[m_iMyDeskStation][i],i,true);
			}
			else
			{
				//��������ע
				BetOptionSet(m_i64UserNoteLimite[m_iMyDeskStation][i],i,false);
			}
		}
	}
}
/*
���ܣ���עѡ������
�βΣ�iOption,Ҫ���õ���עѡ�IsUse���Ƿ����
����ֵ��void
*/
void CNewGameClient::BetOptionSet(__int64 i64Num,int iOption,bool IsUse)
{
	//���ð�ť
	SetBtEnable(CTN_1014089_CTN_2000180_BTN_2000198+iOption,IsUse);
	SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+iOption,IsUse);
	//�������ֿؼ�
	SetInumInfo(CTN_1014089_CTN_2000180_Num_2000202+iOption,i64Num,IsUse);
}

/*
���ܣ���ȡ�������
�βΣ�iControlID,�ɲ�����ID,bCardList�������ֵ
����ֵ��void
*/
void CNewGameClient::GetUpCard(const int iControlID,BYTE bCardList[])
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID)); 
	memset(bCardList,255,sizeof(bCardList));
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->GetUpCardList(bCardList) ; 
	}
}
/*
���ܣ���ȡ������Ƶ�����
�βΣ�iControlID,�ɲ�����ID
����ֵ��������Ƶ�����
*/
int CNewGameClient::GetUpCardNum(const int iControlID)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	BYTE bCardList[5];
	int iUpCount=0;
	memset(bCardList,255,sizeof(bCardList));
	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->GetUpCardList(bCardList) ; 
	}
	for(int i=0;i<5;i++)
	{
		if(255==bCardList[i])
		{
			continue;
		}
		iUpCount++;
	}
	return iUpCount;
}
/*
���ܣ�������ʾ������
�βΣ�iControlID,�ɲ�����ID,bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowUpCard(const int iControlID,const int iControlID2,BYTE bUnderCardList[],BYTE bUnderCardCount,BYTE bUpCardList[],BYTE bUpCardCount)
{
	INoMoveCard  *pNoOperateCard1 = NULL ; 
	pNoOperateCard1 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 
	if(NULL != pNoOperateCard1)
	{
		pNoOperateCard1->SetControlVisible(true) ; 
		pNoOperateCard1->SetCardList(bUnderCardList,bUnderCardCount);
	}
	INoMoveCard  *pNoOperateCard2 = NULL ; 
	pNoOperateCard2 = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID2))  ;
	if(NULL != pNoOperateCard2)
	{
		pNoOperateCard2->SetControlVisible(true) ; 
		pNoOperateCard2->SetCardList(bUpCardList,bUpCardCount);
	}
}
/*
���ܣ����Լ�����
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowCurrentCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{
	IOperateCard  *pOperateCard = NULL ; 
	pOperateCard = dynamic_cast<IOperateCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperateCard)
	{
		pOperateCard->SetControlVisible(true) ; 
		pOperateCard->SetCardList(bCardList , cardCount) ; 
	}
}
/*
���ܣ����Լ�����
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowCurrentNoMoveCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{

	INoMoveCard  *pOperate = NULL ;
	BYTE bTempList[5]={0};
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperate)
	{
		pOperate->SetControlVisible(true) ; 
		pOperate->SetCardList(bTempList , cardCount) ; 
	}
}
/*
���ܣ�չʾ̯����ҵ���
�βΣ�bCardList[]��ֵ;cardCountҪ��������
����ֵ��void
*/
void CNewGameClient::ShowOpenCard(const int iControlID,BYTE bCardList[],BYTE cardCount)
{
	INoMoveCard  *pOperate = NULL ; 
	pOperate = dynamic_cast<INoMoveCard *>(m_pUI->GetIControlbyID(iControlID))  ; 

	if(NULL != pOperate)
	{
		pOperate->SetControlVisible(true) ; 
		pOperate->SetCardList(bCardList , cardCount) ; 
	}
}
/*
���ܣ��������������ׯ/����ׯͼƬ
�βΣ�void
����ֵ��void
*/
void CNewGameClient::CLearScratchNt()
{
	for(int i=0;i<4;i++)
	{
		SetImageVisible(CTN_1014089_IMG_2000132+i,FALSE);
		SetImageVisible(CTN_1014089_IMG_2000137+i,FALSE);
	}
}
/*
���ܣ��������ʱ��
�βΣ�void
����ֵ��void
*/
void CNewGameClient::ClearAllClock()
{
	KillAllGameTime();
	//�������м�ʱ��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+i,false,0);
	}
}

/*
���ܣ���ֹ��ע
�βΣ�void
����ֵ��void
*/
void CNewGameClient::ClearAllBet()
{
	//�ر���ע��ʱ��
	m_pGameFrame->KillTimer(ID_NOTE) ;
	//���ð�ť
	for(int i=0;i<4;i++)
	{
		SetBtEnable(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
		SetBtVisible(CTN_1014089_CTN_2000180_BTN_2000198+i,false);
	}
	//�������ֿؼ�
	for(int i=0;i<4;i++)
	{
		SetInumPro(CTN_1014089_CTN_2000180_Num_2000202+i,0,2,false);
	}
	
}

//������Ӧ��ׯ��ʱ��
void CNewGameClient::OnStartClock(int viewStation,BYTE bCallScoreTime)
{
	//��ʾ��ʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+viewStation*10,true,bCallScoreTime);

	m_bCurrentClockLabel=m_iCallScoreTime;
	//������ʱ��
	m_pGameFrame->SetTimer(ID_CALL_SCORE_TIME,1000);	
}

//--------------------------------------------------------------------------------------------------------
//���������ؼ��ɼ�
void	CNewGameClient::SetContainVisible(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetControlVisible(bFlag);
	}
}

//����ͼƬ�ؼ�������ʾͼƬ�ĳߴ�
void CNewGameClient::SetPicSize(const int iButtonID,CString sImagSrc, bool bFlag,int x,int y,int w,int h)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iButtonID));
	if (NULL != pImage)
	{
		pImage->LoadPic(CA2W(sImagSrc));
		pImage->SetSrcX(x); 
		pImage->SetSrcY(y); 
		pImage->SetSrcW(w); 
		pImage->SetSrcH(h); 
		pImage->SetControlVisible(bFlag);
	}
}
/*
���ܣ����������ؼ�����Ӧ�κ����ʱ��
������iContainID:�ؼ�ID��bFlag���Ƿ���Ӧ
return:void
*/
void  CNewGameClient::SetContainEnable(const int iContainID,bool bFlag)
{
	IContain *pContain = NULL;
	pContain = dynamic_cast<IContain *>(m_pUI->GetIControlbyID(iContainID));
	if (NULL != pContain)
	{
		pContain->SetEnable(bFlag);
	}
}

//---------------------------------------------------------------------------------------
///ɾ�����ж�ʱ��,����ע����ʤ������ʾ��ʱ����
void CNewGameClient:: KillAllGameTime() 
{
	m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ; 

	m_pGameFrame->KillTimer(ID_NOTE) ; 

	m_pGameFrame->KillTimer(ID_OPEN_CARD) ; 

	m_pGameFrame->KillTimer(ID_BEGIN_TIME);

	m_pGameFrame->KillTimer(ID_WAIT_NEXT);

}


//�������ֿռ���ʾ����
void	CNewGameClient::SetInumInfo(const int iNumID, __int64 i64Num,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����
void	CNewGameClient::SetInumPro(const int iNumID, __int64 i64Num,BYTE bPro,bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		switch(bPro)
		{
		case 0:
			{
				NEW_NUM_STRUCT NumStruct;

				NumStruct.bShowPlusSignOrMinusSign = true;
				NumStruct.bShowComma = true;
				NumStruct.iShowCommaIndex = 3;
				NumStruct.iShowZeroType = 0;
				pNum->SetNewNumStruct(NumStruct);
				pNum->SetNewNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		case 1:
			{
				NEW_NUM_STRUCT NumStruct;

				NumStruct.bShowPlusSignOrMinusSign = false;
				NumStruct.bShowComma = true;
				NumStruct.iShowCommaIndex = 3;
				NumStruct.iShowZeroType = 0;
				pNum->SetNewNumStruct(NumStruct);
				pNum->SetNewNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		case 2:
			{
				pNum->SetNum(i64Num);
				pNum->SetControlVisible(bFlag);
				break;
			}
		}
		
	}
}
//--------------------------------------------------------------------------------------------------------
//�������ֿռ���ʾ����(�����Ӻźͼ��ż�����)
void	CNewGameClient::SetInumInfo_1(const int iNumID, __int64 i64Num, bool bFlag)
{
	INum *pNum = NULL;
	pNum = dynamic_cast<INum *>(m_pUI->GetIControlbyID(iNumID));
	if (NULL != pNum)
	{
		pNum->SetNum(i64Num);
		pNum->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����(��ʾ����)
void	CNewGameClient::SetTextinfo(const int iTextID,__int64 iNum, bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%I64d",iNum);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//�����ı��ؼ���ʾ����(��ʾ�ַ�)
void	CNewGameClient::SetTextShowText(const int iTextID,TCHAR ch[], bool bFlag)
{
	IText *pText = NULL;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(iTextID));
	if (NULL != pText)
	{
		CString sText;
		sText.Format("%s",ch);
		pText->SetText(CA2W(sText));
		pText->SetControlVisible(bFlag);
	}
}
//--------------------------------------------------------------------------------------------------------
//����ʱ�����ʾ����ʱ
void	CNewGameClient::SetTimePlay(const int iTimeID,bool bFlag, BYTE byMaxTime, BYTE byMinTime)
{
	ITime *pTime = NULL;
	pTime = dynamic_cast<ITime *>(m_pUI->GetIControlbyID(iTimeID));
	if (NULL != pTime)
	{
		pTime->SetShowMaxNum(byMaxTime);
		pTime->SetShowMinNum(byMinTime);
		pTime->SetPLay(bFlag);
		pTime->SetControlVisible(bFlag);
	}

}

//-----------------------------------------------------------------------------------------------
/// ������Ϸ�ĵ�ǰ״̬
void	CNewGameClient::SetGameStation(void * pBuffer,int nLen)
{
	m_iMyDeskStation = m_pGameFrame->GetMyDeskStation() ; 
	

	GameStation_Base *pGameStation = (GameStation_Base *)pBuffer;
	if (NULL == pGameStation)
	{
		return;
	}
	SetStationParameter(pGameStation->byGameStation);

	switch(GetStationParameter())
	{
	case GS_WAIT_SETGAME:		//��Ϸû�п�ʼ״̬
	case GS_WAIT_ARGEE:			//�ȴ���ҿ�ʼ״̬
	case GS_WAIT_NEXT:			//�ȴ���һ�̿�ʼ
		{
			if (nLen != sizeof(GameStation_WaiteAgree))
			{
				return;
			}
			SetStationParameter(GS_WAIT_ARGEE);
			HandleStartStation(pBuffer);
			break; 
		}
	case GS_ROB_NT:		//��ׯ״̬
		{
			if (nLen != sizeof(GameStation_RobNt))
			{
				return;
			}
			SetStationParameter(GS_ROB_NT);
		
			HandleRobNtStation(pBuffer);
			//SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,true);
			break;
		}
	case GS_NOTE:		//��ע״̬
		{
			if (nLen != sizeof(GameStation_Note))
			{
				return;
			}
			SetStationParameter(GS_NOTE);
		
			HandleNoteStation(pBuffer);
			break;;
		}
	case GS_SEND_CARD:	//����״̬
		{
			if (nLen != sizeof(GameStation_SendCard))
			{
				return;
			}
			SetStationParameter(GS_SEND_CARD);
			
			//����״̬
			HandleSendCardStation(pBuffer);
			//SetBtEnable(CTN_2000280_CTN_100200_BTN_100231,false);
			break;
		}
	case GS_OPEN_CARD:	//��Ϸ������
		{
			if (nLen != sizeof(GameStation_OpenCard))
			{
				return;
			}
			SetStationParameter(GS_OPEN_CARD);
			
			//��Ϸ������
			HandleOpenCardStation(pBuffer);
			
			break;
		}
	}
	PlayBgSound(true);
}
//��һ����������������ִ�д˺���
void CNewGameClient::HandleStartStation(void *buffer)
{
	GameStation_WaiteAgree *pGameStation = (GameStation_WaiteAgree *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}

	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iPalyCardCount= pGameStation->iAllCardCount;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

	//��ʾ�Ѿ�׼�����мҵ���ʾ׼��ͼƬ
	for(int i=0;i<PLAY_COUNT;i++)
	{
		int iViewStation=m_pGameFrame->DeskStation2View(i);
		if(pGameStation->bUserReady[i])//�Ѿ�׼��
		{
			SetImageVisible(CTN_1014089_IMG_1004070+iViewStation,true);
		}
		else
		{
			SetImageVisible(CTN_1014089_IMG_1004070+iViewStation,false);
		}
	}

	//�����ȴ���ʼ��ʱ��
	m_bCurrentClockLabel=m_iBeginTime;
	//������ŶӾͲ���ʾʱ��
	if (m_pGameFrame->GetGameInfo()->dwRoomRule & GRR_QUEUE_GAME)
	{
		SetBtVisible(CTN_1014089_BTN_1004006,false);
		//����ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,false,m_iBeginTime);
	}
	else
	{
		SetBtVisible(CTN_1014089_BTN_1004006,true);
		SetBtEnable(CTN_1014089_BTN_1004006,true);
		//��ʾʱ��
		ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iBeginTime);
		//�������ƶ�ʱ��
		m_pGameFrame->SetTimer(ID_BEGIN_TIME,1000);
	}
}
/*----------------------------------------------------------------------------------*/
//��ׯ״̬
void	CNewGameClient::HandleRobNtStation(void *buffer)
{
	GameStation_RobNt *pGameStation = (GameStation_RobNt *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}
	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iPalyCardCount= pGameStation->iAllCardCount;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
	
	//��ʾ�������ׯ״̬
	int iView = 0;
	for (int i=0;i<PLAY_COUNT; i++)
	{
		iView = m_pGameFrame->DeskStation2View(i);
		if (0 == pGameStation->byUserCallStation[i])
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,true);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,false);
		}
		else if (1 == pGameStation->byUserCallStation[i])
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,false);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,true);
		}
		else
		{
			SetImageVisible(CTN_1014089_IMG_2000137+iView,false);
			SetImageVisible(CTN_1014089_IMG_2000132+iView,false);
		}

		//׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+iView,false);
	}

	//��ʾ��ǰ��ׯ��ҵļ�ʱ��
	iView = m_pGameFrame->DeskStation2View(pGameStation->byCurrentCallScore);
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004010_Time_2004011+iView*10,true,m_iCallScoreTime);

	if (pGameStation->byCurrentCallScore == m_iMyDeskStation)
	{
		//��ʾ��ׯ��ť
		SetContainVisible(CTN_1014089_CTN_1008000,TRUE);
	}

	if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
	}
}
/*----------------------------------------------------------------------------------*/
//��ע״̬
void	CNewGameClient::HandleNoteStation(void *buffer)
{
	GameStation_Note *pGameStation = (GameStation_Note *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}

	//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iPalyCardCount= pGameStation->iAllCardCount;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;//���䱶��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
	//ׯ��λ��
	m_byNtStation	 = pGameStation->byNtStation;
	//��ע����
	m_iThisTurnLimit = pGameStation->iLimitNote[m_iMyDeskStation];
	memcpy(m_i64UserNoteLimite,pGameStation->i64UserNoteLimite,sizeof(m_i64UserNoteLimite));
	//��ʾׯ��λ��
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(m_byNtStation);
	//��ʾׯ��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004060+viewStation,TRUE);
	
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
	//��¼��ʾ����ÿ���˵���ע��
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(pGameStation->i64PerJuTotalNote[i]>0)
		{
			int iView=m_pGameFrame->DeskStation2View(i);
			//SetContainVisible(CTN_1014089_CTN_2000246,true);
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,pGameStation->i64PerJuTotalNote[i],true);
		}
	}
	//�Լ������ && ����ׯ�� && ������;�����
	if (m_iMyDeskStation!=m_byNtStation)	
	{
		if (pGameStation->iUserStation[m_iMyDeskStation] == STATE_PLAY_GAME)
		{
			//������ע��ʱ��
			m_bCurrentClockLabel=m_iXiaZhuTime;
			m_pGameFrame->SetTimer(ID_NOTE,1000);
			//��ʾ��ע
			DisplayBet(m_iThisTurnLimit);
		}		
	}

	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,true,point.x,point.y);
	//��ʾʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iXiaZhuTime);
	if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
	}
}
/*----------------------------------------------------------------------------------*/
//����״̬
void	CNewGameClient::HandleSendCardStation(void *buffer)
{
	GameStation_SendCard *pGameStation = (GameStation_SendCard *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}//��������
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iPalyCardCount= pGameStation->iAllCardCount;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
	m_byNtStation	= pGameStation->byNtStation;		//ׯ��λ��

	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));

	//�˿�������
	memcpy(m_DeskReadyCard,pGameStation->iUserCard,sizeof(m_DeskReadyCard));
	memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));
	//��ʾ�˿���
	int iView = 0;
	for (int i=0; i<PLAY_COUNT; i++)
	{
		if (0 != m_iDeskCardCount[i])
		{
			if (i == m_iMyDeskStation)
			{
				SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010,m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation],true);
			}
			else
			{
				iView = m_pGameFrame->DeskStation2View(i);
				ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[i],m_iDeskCardCount[i]);
			}
		}
		//׼��ͼƬ
		SetImageVisible(CTN_1014089_IMG_1004070+i,false);
	}

	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(pGameStation->i64PerJuTotalNote[i]>0)
		{
			int iView=m_pGameFrame->DeskStation2View(i);
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,pGameStation->i64PerJuTotalNote[i],true);
		}
	}	
	if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
	}
}
/*----------------------------------------------------------------------------------*/
//��Ϸ������
void	CNewGameClient::HandleOpenCardStation(void *buffer)
{
	GameStation_OpenCard *pGameStation = (GameStation_OpenCard *)buffer;
	if(NULL == pGameStation)
	{
		return ;
	}
	m_Logic.SetCardShape(pGameStation->iCardShape);
	//��������
	m_iThinkTime	= pGameStation->iThinkTime;
	m_iBeginTime	= pGameStation->iBeginTime;
	m_iSendCardTime = pGameStation->iSendCardTime;
	m_iCallScoreTime= pGameStation->iCallScoreTime;
	m_iXiaZhuTime	= pGameStation->iXiaZhuTime;
	m_iPalyCardCount= pGameStation->iAllCardCount;
	m_iRoomBasePoint= pGameStation->iRoomBasePoint ;	//���䱶��
	m_byNtStation	= pGameStation->byNtStation;		//ׯ��λ��
	
	//������ҵ�״̬
	memcpy(m_iUserStation,pGameStation->iUserStation,sizeof(m_iUserStation));
	//�˿�������
	memcpy(m_DeskReadyCard,pGameStation->iUserCard,sizeof(m_DeskReadyCard));
	memcpy(m_iDeskCardCount,pGameStation->iUserCardCount,sizeof(m_iDeskCardCount));


	//��Ұ�ţ״̬
	for(int i=0; i<PLAY_COUNT; i++)
	{
		//��;����Ĳ������κδ���
		if (m_iUserStation[i] == STATE_NULL)
		{
			continue;
		}
		int iView = m_pGameFrame->DeskStation2View(i);
		//�Ѿ���ţ��
		if (pGameStation->iUserStation[i] == STATE_OPEN_CARD )
		{
			//�Լ�
			if (iView == 2)
			{
				//�����Լ�������
				SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010, m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation], false);
				//��ţ
				if (pGameStation->iOpenShape[i] == UG_NO_POINT)
				{
					//����0�ŵ���
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,m_DeskReadyCard[m_iMyDeskStation],0,false);
					//����5������������
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,m_DeskReadyCard[m_iMyDeskStation],m_iDeskCardCount[m_iMyDeskStation],true);
				}
				else
				{
					//����0�ŵ���
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003011,pGameStation->byOpenUnderCard[i],3,true);
					//����5������������
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003012,pGameStation->byOpenUpCard[i],2,true);
				}
				//��ʾ������ʾ
				ShowHaveCattleAction(CTN_1014089_CTN_200300_IMG_2003013,pGameStation->iOpenShape[i],true,i);
			}
			else
			{
				//���ضԼҵ�����
				SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[i],0,false);
				if (pGameStation->iOpenShape[i] ==  UG_NO_POINT || 0 == pGameStation->byOpenUnderCount[i])
				{
					//����0�ŵ���
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003022+iView*10,m_DeskReadyCard[i],0,false);
					//����5������������
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003021+iView*10,m_DeskReadyCard[i],m_iDeskCardCount[i],true);
				}
				else
				{
					//����3�ŵ���
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003021+iView*10,pGameStation->byOpenUnderCard[i],3,true);
					//����2������������
					SetNoMoveCardInfo(CTN_1014089_CTN_200300_NoMoveCard_2003022+iView*10,pGameStation->byOpenUpCard[i],2,true);
				}
				//��ʾ������ʾ
				ShowHaveCattleAction(CTN_1014089_CTN_200300_IMG_2003023+iView*10,pGameStation->iOpenShape[i],true,i);
			}
		}
		else
		{
			if (m_iDeskCardCount[i] > 0)
			{
				if (i==m_iMyDeskStation)
				{
					SetOperateCardInfo(CTN_1014089_CTN_200300_OperateCard_2003010,m_DeskReadyCard[i],m_iDeskCardCount[i],true);

					if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
					{
						//��ʾ̯�ƺ����Ű�ť
						SetContainVisible(CTN_1014089_CTN_2000206,false);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
					}
					else
					{
						//��ʾ̯�ƺ����Ű�ť
						SetContainVisible(CTN_1014089_CTN_2000206,true);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,true);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,true);
						SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,true);
					}
				}
				else
				{
					ShowCurrentNoMoveCard(CTN_1014089_CTN_200300_NoMoveCard_2003020+iView*10,m_DeskReadyCard[i],m_iDeskCardCount[i]);
				}
			}
		}
	}

	//�û���ע���
	memcpy(m_iTotalGameNote,pGameStation->i64PerJuTotalNote,sizeof(m_iTotalGameNote));
	for(int i=0; i<PLAY_COUNT; i++)
	{
		if(pGameStation->i64PerJuTotalNote[i]>0 && m_iUserStation[i] != STATE_NULL)
		{
			int iView=m_pGameFrame->DeskStation2View(i);
			//SetControlVisible(CTN_1014089_CTN_2000246,true);
			SetImageVisible(CTN_1014089_CTN_2000246_IMG_2000247+iView,true);
			SetInumInfo(CTN_1014089_CTN_2000246_Num_2000251+iView,pGameStation->i64PerJuTotalNote[i],true);
		}
	}	
	//ʱ��λ�øı�
	POINT point;
	IText *pText;
	pText = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_CTN_200400_TEXT_2000267));
	point.x=pText->GetGameX();
	point.y=pText->GetGameY();
	SetControlPos(CTN_1014089_CTN_200400_CTN_2004030,true,point.x,point.y);
	//��ʾʱ��
	ShowTimer(CTN_1014089_CTN_200400_CTN_2004030_Time_2004031,true,m_iThinkTime);
	if(STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		//��ʾ̯�ƺ����Ű�ť
		SetContainVisible(CTN_1014089_CTN_2000206,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000209,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000207,false);
		SetBtEnable(CTN_1014089_CTN_2000206_BTN_2000208,false);
	}
}


//--------------------------------------------------------------------------------------------------------
//����ͼƬ�ؼ���λ��
void	CNewGameClient::SetImagePosition(const int iImageID, int iPositonX, int iPositonY, bool bFlag)
{
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(iImageID));
	if (NULL != pImage)
	{
		pImage->SetGameXY(iPositonX,iPositonY);
		pImage->SetControlVisible(bFlag);
	}
}
/// ��������״̬
int	CNewGameClient::ResetGameFrame(void)
{
	return 0;
}
/// ����뿪
int CNewGameClient::GameUserLeft(int nIndex)
{
	//��λ��ת��ͼλ��
	int viewStation=m_pGameFrame->DeskStation2View(nIndex);
	//��ʾ׼��ͼƬ
	SetImageVisible(CTN_1014089_IMG_1004070+viewStation,false);
	return 0;
}
/// ��Ҷ���
int CNewGameClient::GameUserOffLine(void)
{
	return 0;
}
// �Ƿ�������Ϸ������Ϸʵ��
// �Թ���ҷ���false����Ϸ���������Ϸ�з���true������״̬����false
bool CNewGameClient::IsPlayingGame()
{
	if (STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		return false;
	}

	if(m_byGameStation >= GS_ROB_NT && m_byGameStation < GS_WAIT_NEXT)
	{
		return true;
	}
	return false;
}

///����˳���Ϸ
bool CNewGameClient::OnGameQuiting()
{
	if(m_pGameFrame->GetIsWatching())
	{
		return true ; 
	}


	if (STATE_NULL == m_iUserStation[m_iMyDeskStation])
	{
		return true;
	}
	CString szAppPath = CINIFile::GetAppPath();
	szAppPath.SetAt(szAppPath.GetLength()-1, TCHAR('\0'));
	int nPosOfSlash = szAppPath.ReverseFind(TCHAR('\\'));
	CString szAppPathParent = szAppPath.Left(nPosOfSlash);


	CString strKeyName  ; 
	strKeyName.Format("%d" ,NAME_ID) ; 
	CBcfFile File(szAppPathParent + "\\SpecialRule.bcf") ; 

	bool   bForceQuitAsAuto = File.GetKeyVal("ForceQuitAsAuto",strKeyName ,1) ; 

	if(m_byGameStation >= GS_ROB_NT && m_byGameStation < GS_WAIT_NEXT)
	{
		if(bForceQuitAsAuto)
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪��Ϸ�����йܴ��� ���Ƿ�Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}	
		}
		else
		{
			if(IDOK != AFCMessageBox("��������Ϸ��, �����뿪���ᱻ�۷����Ƿ����Ҫ�뿪��", "��ʾ",AFC_YESNO)) 
			{
				return false ; 
			}
			m_pGameFrame->SendGameData(NULL , 0 ,MDM_GM_GAME_FRAME,ASS_GM_FORCE_QUIT,0);
		}
	}
	return true;
}
/// ��ʱ����Ϣ
void CNewGameClient::OnTimer(int nTimerID)
{
	switch(nTimerID)
	{
	case ID_CALL_SCORE_TIME:
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��ׯʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_CALL_SCORE_TIME) ;
				if(m_bCurrentOperationStation==m_iMyDeskStation)//��ǰ�з����Լ�
				{
					OnHandleHitGiveUpNtBt();
				}
			}
			return ;
		}
	case ID_NOTE:	
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��עʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_NOTE) ;
				OnHandleStartBet(4);
				ClearAllBet();
			}
			return;
		}
	case ID_OPEN_CARD:	
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//
			{
				m_pGameFrame->KillTimer(ID_OPEN_CARD) ;//����ʱ�䵽
				//�Զ�̯��
				OnHandleIsShowCardBt(0);
			}
			return;
		}
	case ID_BEGIN_TIME:			//��ʼ�ȴ���ʱ��
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//��ʼ��Ϸ�ȴ�ʱ�䵽
			{
				m_pGameFrame->KillTimer(ID_BEGIN_TIME) ;
			}
			return;
		}
	case ID_WAIT_NEXT:
		{
			m_bCurrentClockLabel--;
			TimeWarning(m_bCurrentClockLabel);
			if(m_bCurrentClockLabel<=0)//
			{
				m_pGameFrame->KillTimer(ID_WAIT_NEXT) ;//��ʼ��Ϸ�ȴ�ʱ�䵽
			}
			return;
		}
	default:
		{
			return;
		}	
	}
}
/*----------------------------------------------------------------------------------------*/
//��ʾ��ʱ��
void	CNewGameClient::ShowTimer(const int iTimeID, bool bShow, BYTE byMax, BYTE byMin )
{
	SetContainVisible(CTN_1014089_CTN_200400_CTN_2004010,false);
	SetContainVisible(CTN_1014089_CTN_200400_CTN_2004020,false);
	SetContainVisible(CTN_1014089_CTN_200400_CTN_2004030,false);
	SetContainVisible(CTN_1014089_CTN_200400_CTN_2004040,false);

	if (iTimeID == CTN_1014089_CTN_200400_CTN_2004010_Time_2004011)
	{
		SetContainVisible(CTN_1014089_CTN_200400_CTN_2004010,bShow);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004020_Time_2004021)
	{
		SetContainVisible(CTN_1014089_CTN_200400_CTN_2004020,bShow);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004030_Time_2004031)
	{
		SetContainVisible(CTN_1014089_CTN_200400_CTN_2004030,bShow);
	}
	else if(iTimeID == CTN_1014089_CTN_200400_CTN_2004040_Time_2004041)
	{
		SetContainVisible(CTN_1014089_CTN_200400_CTN_2004040,bShow);
	}

	SetTimePlay(iTimeID,bShow,byMax,byMin);

}
/*----------------------------------------------------------------------------------------*/
//ʱ�侯ʾ��
void CNewGameClient::TimeWarning(int iTime)
{
	switch(iTime)
	{
	case 0:
		{
			PlayGameSound(SOUND_TIME_0,false);
			break;
		}
	case 1:
		{
			PlayGameSound(SOUND_TIME_1,false);
			break;
		}
	case 2:
		{
			PlayGameSound(SOUND_TIME_2,false);
			break;
		}
	case 3:
		{
			PlayGameSound(SOUND_TIME_3,false);
			break;
		}
	case 4:
		{
			PlayGameSound(SOUND_TIME_4,false);
			break;
		}
	case 5:
		{
			PlayGameSound(SOUND_TIME_5,false);
			break;
		}
	default:
		{
			break;
		}
	}
}
///��ʼ����������
void CNewGameClient::ResetGameData()
{
	m_bCurrentOperationStation=255;		//��ǰ�з���
	m_bCurrentClockLabel=0;			//��ǰʱ�ӱ�ǩ
	m_iCurrentClockControlID=0;		//��ǰ���Ƶ�ʱ��ID
	m_iCurrentCardCount=5;
	m_byNtStation=255;
	m_bIsHitTip=false;
	memset(m_iDeskCardCount,0,sizeof(m_iDeskCardCount));	//ÿ�������˿˵���Ŀ	
	memset(m_DeskReadyCard,0,sizeof(m_DeskReadyCard));	

	memset(m_iTotalGameNote,0,sizeof(m_iTotalGameNote));	//������Ϸ��ע��(ʵ��)

	memset(m_bUpCard,255,sizeof(m_bUpCard));					//�������
	memset(m_bUnderCard,255,sizeof(m_bUnderCard));			//�������
			
	memset(m_bSendCard,255,sizeof(m_bSendCard));			//��������
	memset(m_iUserStation,STATE_NULL,sizeof(m_iUserStation));		//�����Ϸ״̬
	memset(&m_SuperSetData,255,sizeof(m_SuperSetData));		//������������
	m_SuperSetData.bSetSuccese = false;

	//�������
	m_sendQueue.clear();			//����˳������
	m_bySendCardCount=0;			//��ǰ���ƴ���
}
//---------------------------------------------------------------------------------------
//���ű�����Ч
void	CNewGameClient::PlayBgSound(bool bISLoop)
{
	if(!m_SoundEnginBg)
	{
		return ;
	}

	if(!m_bSoundBgPlay)
	{
		return ;
	}
	
	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	wsprintf(szFileName,"Sound\\BG.ogg");
	m_SoundEnginBg->play2D(szFileName,bISLoop);

}
//---------------------------------------------------------------------------------------
//������Ϸ��Ч
void	CNewGameClient::PlayGameSound( int SoundID, bool bISLoop,BYTE byDeskStation )
{
	if(!m_SoundEngineGame)
	{
		return ;
	}
	if(!m_bSoundPlay)
	{
		return ;
	}

	char szFileName[MAX_PATH];
	memset(szFileName,0,sizeof(szFileName)) ; 
	if (SoundID >= SOUND_BEGIN_GAME && SoundID <= SOUND_GIVEUP)
	{
		switch(SoundID)
		{
		case SOUND_BEGIN_GAME:
			{
				wsprintf(szFileName,"Sound\\nn_start.wav");
				break;
			}
		case SOUND_TIME_0:
			{
				wsprintf(szFileName,"Sound\\time0.wav");
				break;
			}
		case SOUND_TIME_1:
			{
				wsprintf(szFileName,"Sound\\time1.wav");
				break;
			}
		case SOUND_TIME_2:
			{
				wsprintf(szFileName,"Sound\\time2.wav");
				break;
			}
		case SOUND_TIME_3:	
			{
				wsprintf(szFileName,"Sound\\time3.wav");
				break;
			}
		case SOUND_TIME_4:	
			{
				wsprintf(szFileName,"Sound\\time4.wav");
				break;
			}
		case SOUND_TIME_5:	
			{
				wsprintf(szFileName,"Sound\\time5.wav");
				break;
			}
		case SOUND_WIN:		
			{
				wsprintf(szFileName,"Sound\\win.wav");
				break;
			}
		case SOUND_LOSE:	
			{
				wsprintf(szFileName,"Sound\\lose.wav");
				break;
			}
		case SOUND_CHONGPAI:
			{
				wsprintf(szFileName,"Sound\\chongpai.wav");
				break;
			}
		case SOUND_XIAZHU:		
			{
				wsprintf(szFileName,"Sound\\chip.wav");
				break;
			}
		case SOUND_TIP:	
			{
				wsprintf(szFileName,"Sound\\Tip.wav");
				break;
			}
		case SOUND_TANPAI:		
			{
				wsprintf(szFileName,"Sound\\tanpai.wav");
				break;
			}
		case SOUND_CLICKCARD:	
			{
				wsprintf(szFileName,"Sound\\hitcard.wav");
				break;
			}
		case SOUND_HIT_BEGIN:
			{
				wsprintf(szFileName,"Sound\\hitbegin.wav");
				break;
			}
		case SOUND_JIAOZHUANG:
		case SOUND_GIVEUP:
			{
				wsprintf(szFileName,"Sound\\giveupnt.wav");
				break;
			}
		default:
			{
				break;
			}
		}
	}
	else 
	{
		UserInfoForGame UserInfo ; 
		memset(&UserInfo,0,sizeof(UserInfo));
		if (m_pGameFrame->GetUserInfo(byDeskStation, UserInfo))
		{
			switch(SoundID)
			{
			case SOUND_NIUNIU_0:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_0.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_1:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_1.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_2:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_2.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_3:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_3.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_4:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_4.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_5:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_5.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_6:		
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_6.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_7:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_7.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_8:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_8.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_9:	
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_9.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			case SOUND_NIUNIU_10:
				{
					wsprintf(szFileName,"Sound\\%s\\%s_nn_10.wav",(UserInfo.bBoy?"Boy":"Girl"),(UserInfo.bBoy?"Boy":"Girl"));
					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
	m_SoundEngineGame->play2D(szFileName,bISLoop);
}
/*
@brief:���ŷ��ƶ���
@param:ACTION_ID,�����ؼ�ID��END_ID �յ�ID
@return:void
*/
void CNewGameClient::ShowSendCardAction(const int ACTION_ID,const int END_ID)
{
	//��ö����ؼ�ID
	IMoveAction* pMoveAction = NULL;
	GETCTRL(IMoveAction,pMoveAction,m_pUI,ACTION_ID);

	//��÷����Ʊ�
	IImage *pImage = NULL;
	pImage = dynamic_cast<IImage *>(m_pUI->GetIControlbyID(CTN_1014089_IMG_1005003));

	POINT pStart,pEnd;

	//��÷������
	IText *pText_1 = NULL;
	pText_1 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(CTN_1014089_TEXT_1005019));
	if (NULL != pText_1)
	{
		pStart.x=pText_1->GetGameX();
		pStart.y=pText_1->GetGameY();
	}

	//��÷����յ�
	IText *pText_2 = NULL;
	pText_2 = dynamic_cast<IText *>(m_pUI->GetIControlbyID(END_ID));
	if (NULL != pText_2)
	{
		pEnd.x=pText_2->GetGameX();
		pEnd.y=pText_2->GetGameY();
	}
	
	
	if(NULL!=pMoveAction && NULL!=pImage)
	{
		pMoveAction->SetControlingCtrlID(CTN_1014089_IMG_1005003);
		pMoveAction->SetStartP(pStart);
		pMoveAction->SetEndP(pEnd);
		pMoveAction->SetTotalMoveTime((unsigned int)m_iSendCardTime);
		pMoveAction->SetControlVisible(true);
		pMoveAction->SetPlayState(true);
	}
}
//
/*
@brief:����
@param:ACTION_ID,�����ؼ�ID��END_ID �յ�ID,bView����ͼλ��
@return:void
*/
void CNewGameClient::PlaySendCard(BYTE Station)
{
	;
}
/*
@brief:����
@param:bCardValue,��ֵ��bSendPeople����ǰ������
@return:void
*/
void CNewGameClient::SendCard(BYTE bCardValue,BYTE bySendPeople)
{
	SetImageVisible(CTN_1014089_IMG_1005003,true);
	int iView = m_pGameFrame->DeskStation2View(bySendPeople);
	ShowSendCardAction(CTN_2000128_ACTION_2000128+iView,CTN_1014089_TEXT_1005020+iView);
}
/*
@brief:���㷢��˳��
@param:BCardList:�û��ƣ�iCardCount��ÿ����Ҫ�����Ƶ�������iStart��˭�ȷ���
@@return:void
*/
void CNewGameClient::SendQueue(int iStart)
{
	int iPos = iStart;
	m_sendQueue.clear();
	for(int m=0;m<SH_USER_CARD;m++)
	{
		iPos = iStart;
		for(int i=0; i<PLAY_COUNT; i++)
		{
			iPos = (iStart+i)%PLAY_COUNT;
			m_sendQueue.push_back(m_DeskReadyCard[iPos][m]);
		}
	}
}
/*
@brief:���㵱ǰ�Ƶ�ӵ����
@param:bCardList:�û���,bCardValue:Ҫ���ҵ�������,int iCardCount,ÿ�������ӵ�е�������
@@return:void
*/
BYTE CNewGameClient::GetSendStation(BYTE (*bCardList)[5],BYTE bCardValue,int iCardCount)
{
	UserInfoForGame userInfo;
	for(int i=0;i<PLAY_COUNT;i++)
	{
		if(m_pGameFrame->GetUserInfo(i,userInfo))
		{
			int viewStation=m_pGameFrame->DeskStation2View(i);
			for(int k=0;k<iCardCount;k++)
			{
				if(bCardValue==bCardList[viewStation][k])
				{
					return viewStation;
				}
			}
		}
	}
	return 255;//��Ч����
}
/*
@brief:��ǰ�������Ƿ���Ч
@param:bCardValue:������
@@return:BOOL
*/
BOOL CNewGameClient::IsUser(BYTE bCardValue)
{
	if(bCardValue>0 && bCardValue<80)
	{
		return true;
	}
	else
	{
		return false;
	}
}