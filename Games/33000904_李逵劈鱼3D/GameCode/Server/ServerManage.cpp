#include "StdAfx.h"
#include "ServerManage.h"
#include "tinyxml\tinyxml.h"
#include "tinyxml\tinystr.h"
#include <random>
#include "../../GameCode/command/LogModule.h"

//************************************
// Method:    rand_Mersenne
// FullName:  rand_Mersenne
// Access:    public 
// Returns:   int
// Qualifier: 利用梅森素数产生随机数介于[_Min,_Max)
//			  VS版本必须大于2008，较小数必须小于或等于较大数
//			  否则将返回0
// Parameter: const int _Min
// Parameter: const int _Max
//************************************
int rand_Mersenne(const int _Min,const int _Max)
{
	if (_Min > _Max){ return 0;}
	if (_MSC_VER < 1500 ){ return 0;}
	random_device rd;
	mt19937 mt(rd());
	uniform_int_distribution<> dis(_Min,_Max);
	return dis(mt);
}


void DebugPrintf( const char *p, ...)
{
	CTime time = CTime::GetCurrentTime();
	char szFilename[256];
	sprintf(szFilename, "%d-%d%d%d-log.txt", NAME_ID,time.GetYear(), time.GetMonth(), time.GetDay());
	FILE *fp = fopen(szFilename, "a");
	if (NULL == fp)
	{
		return;
	}
	va_list arg;
	va_start(arg, p);
	vfprintf(fp, p, arg);
	fprintf(fp,"\n");

	fclose(fp);
}

bool CheckFish20Config(DWORD game_id, int* catch_count, double* catch_probability) {
	std::vector<Fish20Config>::iterator iter;
	for (iter = g_fish20_config_.begin(); iter != g_fish20_config_.end(); ++iter) {
		Fish20Config& config = *iter;
		if (game_id == config.game_id) {
			if (config.catch_count <= 0) {
				iter = g_fish20_config_.erase(iter);
				return false;
			}
			*catch_count = config.catch_count;
			*catch_probability = config.catch_probability;
			return true;
		}
	}
	return false;
}

// 返回值 0：黑名单  1 白名单 -1 正常
int CheckUserFilter(DWORD game_id) 
{
	std::vector<DWORD>::iterator iter;
	for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) 
	{
		if ((*iter) == game_id) return 0;
	}

	for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) 
	{
		if ((*iter) == game_id) return 1;
	}

	return -1;
}

void AddFish20Config(DWORD game_id, int catch_count, double catch_probability) {
	std::vector<Fish20Config>::iterator iter;
	for (iter = g_fish20_config_.begin(); iter != g_fish20_config_.end(); ++iter) {
		Fish20Config& config = *iter;
		if (game_id == config.game_id) {
			if (catch_count == 0) {
				iter = g_fish20_config_.erase(iter);
			} else {
				config.catch_count = catch_count;
				config.catch_probability = catch_probability;
			}
			return;
		}
	}

	Fish20Config config;
	config.game_id = game_id;
	config.catch_count = catch_count;
	config.catch_probability = catch_probability;
	g_fish20_config_.push_back(config);
}

void AddUserFilter(DWORD game_id, unsigned char operate_code) 
{
	std::vector<DWORD>::iterator iter;
	if (operate_code == 0) 
	{
		for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter) 
		{
			if ((*iter) == game_id) return;
		}
		g_balck_list_.push_back(game_id);
	} 
	else if (operate_code == 1) 
	{
		for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter) 
		{
			if ((*iter) == game_id) return;
		}
		g_white_list_.push_back(game_id);
	} 
	else 
	{
		for (iter = g_balck_list_.begin(); iter != g_balck_list_.end(); ++iter)
		{
			if ((*iter) == game_id) 
			{
				iter = g_balck_list_.erase(iter);
				break;
			}
		}
		for (iter = g_white_list_.begin(); iter != g_white_list_.end(); ++iter)
		{
			if ((*iter) == game_id) 
			{
				iter = g_white_list_.erase(iter);
				break;
			}
		}
	}
}

///拦截框架消息
bool CServerGameDesk::HandleFrameMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	//switch(pNetHead->bAssistantID)
	//{
	//case ASS_GM_FORCE_QUIT:		//强行退出//安全退出
	//	{
	//		//UserFourceQuit(bDeskStation);
	//		return true;
	//	}
	//}
	return __super::HandleFrameMessage( bDeskStation,  pNetHead,  pData,  uSize,  uSocketID,  bWatchUser);
}

//游戏数据包处理函数
bool CServerGameDesk::HandleNotifyMessage(BYTE bDeskStation, NetMessageHead * pNetHead, void * pData, UINT uSize, UINT uSocketID, bool bWatchUser)
{
	if (bWatchUser)
	{
		return false;
	}
	switch (pNetHead->bAssistantID)
	{
	case SUB_C_USER_CHOICE_TEAM:
		{
			assert(uSize == sizeof(CMD_C_CHOICE_TEAM));
			if (uSize != sizeof(CMD_C_CHOICE_TEAM)) 
			{
				return true;
			}
			CMD_C_CHOICE_TEAM* choice_team = static_cast<CMD_C_CHOICE_TEAM*>(pData);
			return OnSubUserChoiceTeam(bDeskStation,choice_team->bychoiceTeam);
		}
	case SUB_C_USER_FIRE: 
		{
			assert(uSize == sizeof(CMD_C_UserFire));
			if (uSize != sizeof(CMD_C_UserFire)) 
			{
				return true;
			}
			CMD_C_UserFire* user_fire = static_cast<CMD_C_UserFire*>(pData);
			if (NULL == user_fire)
			{
				return true;
			}
			return OnSubUserFire(bDeskStation, user_fire->bullet_kind, user_fire->angle, user_fire->bullet_mulriple, user_fire->lock_fishid,user_fire->bullet_id,user_fire->vector_x,user_fire->vector_y,user_fire->bIsRobot);
		}
	case SUB_C_CATCH_FISH: 
		{
			assert(uSize == sizeof(CMD_C_CatchFish));
			if (uSize != sizeof(CMD_C_CatchFish)) 
				return true;
			CMD_C_CatchFish* hit_fish = static_cast<CMD_C_CatchFish*>(pData);
			if(NULL == hit_fish) return true;
			if (bWatchUser) 
			{
				return true;
			}

			//通过真人发来的消息帮助机器人打到鱼
			if (hit_fish->chair_id < 0 || hit_fish->chair_id >= PLAY_COUNT)
			{
				return true;
			}
			if (m_pUserInfo[hit_fish->chair_id] != NULL)
			{
				if (m_pUserInfo[hit_fish->chair_id]->m_UserData.isVirtual)
				{
					bDeskStation = hit_fish->chair_id;
				}
			}
			else
			{
				return true;
			}
			return OnSubCatchFish(bDeskStation, hit_fish->fish_id, hit_fish->bullet_kind, hit_fish->bullet_id, hit_fish->bullet_mulriple,hit_fish->bIsCatch);
		}
	case SUB_C_CATCH_SWEEP_FISH: 
		{
			assert(uSize == sizeof(CMD_C_CatchSweepFish));
			if (uSize != sizeof(CMD_C_CatchSweepFish)) 
				return true;
			CMD_C_CatchSweepFish* catch_sweep = static_cast<CMD_C_CatchSweepFish*>(pData);
			if (NULL == catch_sweep)
			{
				return true;
			}
			bDeskStation = catch_sweep->chair_id;
			return OnSubCatchSweepFish(bDeskStation, catch_sweep->fish_id, catch_sweep->catch_fish_id, catch_sweep->catch_fish_count);
		}
	case SUB_C_HIT_FISH_I: 
		{
			assert(uSize == sizeof(CMD_C_HitFishLK));
			if (uSize != sizeof(CMD_C_HitFishLK)) 
				return true;
			CMD_C_HitFishLK* hit_fish = static_cast<CMD_C_HitFishLK*>(pData);
			//if (bWatchUser) 
			//	return true;
			return OnSubHitFishLK(bDeskStation, hit_fish->fish_id);
		}
	case SUB_C_STOCK_OPERATE: 
		{
			assert(uSize == sizeof(CMD_C_StockOperate));
			if (uSize != sizeof(CMD_C_StockOperate)) 
				return true;
			CMD_C_StockOperate* stock_operate = static_cast<CMD_C_StockOperate*>(pData);

			return OnSubStockOperate(bDeskStation, stock_operate->operate_code);
		}
	case SUB_C_USER_FILTER: 
		{
			assert(uSize == sizeof(CMD_C_UserFilter));
			if (uSize != sizeof(CMD_C_UserFilter)) 
				return true;
			CMD_C_UserFilter* user_filter = static_cast<CMD_C_UserFilter*>(pData);

			return OnSubUserFilter(bDeskStation, user_filter->game_id, user_filter->operate_code);
		}
	case SUB_C_ANDROID_STAND_UP: 
		{
			return true;
		}
	case SUB_C_FISH20_CONFIG:
		{
			assert(uSize == sizeof(CMD_C_Fish20Config));
			if (uSize != sizeof(CMD_C_Fish20Config)) return true;
			CMD_C_Fish20Config* fish20_config = static_cast<CMD_C_Fish20Config*>(pData);
			return OnSubFish20Config(bDeskStation, fish20_config->game_id, fish20_config->catch_count, fish20_config->catch_probability);
		}
	case SUB_C_ANDROID_BULLET_MUL: 
		{
// 			assert(uSize == sizeof(CMD_C_AndroidBulletMul));
// 			if (uSize != sizeof(CMD_C_AndroidBulletMul)) return true;
// 			CMD_C_AndroidBulletMul* android_bullet_mul = static_cast<CMD_C_AndroidBulletMul*>(pData);
// 			if (m_pUserInfo[bDeskStation] == NULL) 
// 				return true;
// 			if (!m_pUserInfo[bDeskStation]->m_UserData.isVirtual) 
// 				return true;
// 			ServerBulletInfo* bullet_info = GetBulletInfo(android_bullet_mul->chair_id, android_bullet_mul->bullet_id);
// 			if (bullet_info == NULL)
// 				return true;
// 			if (m_i64fish_score_[android_bullet_mul->chair_id]/* + bullet_info->bullet_mulriple*/ < android_bullet_mul->bullet_mulriple) 
// 			{
// 				return true;
// 			}
// 
// 			CMD_S_UserFire user_fire;
// 			user_fire.bullet_kind = bullet_info->ibullet_kind;
// 			user_fire.bullet_id = bullet_info->ibullet_id;
// 			user_fire.angle = 0.f;
// 			user_fire.chair_id = bDeskStation;
// 			user_fire.android_chairid = android_chairid_;
// 			user_fire.bullet_mulriple = bullet_info->ibullet_mulriple;
// 			user_fire.fish_score = bullet_info->ibullet_mulriple - android_bullet_mul->bullet_mulriple;
// 			user_fire.lock_fishid = 0;
// 
// 			SendDataToAllPlayers(&user_fire, sizeof(CMD_S_UserFire), SUB_S_USER_FIRE);
// 			m_i64fish_score_[android_bullet_mul->chair_id] -= android_bullet_mul->bullet_mulriple;
// 
// 			bullet_info->ibullet_kind = android_bullet_mul->bullet_kind;
// 			bullet_info->ibullet_mulriple = android_bullet_mul->bullet_mulriple;

			return true;
		}

	case C_S_ADD_SPECIAL_SIG:
		{
			if(uSize != sizeof(C_S_ADD_SPECIAL)) return false;
			return OnAddSpecial(bDeskStation,pData);
		}
	case C_S_DELETE_SPECIAL_SIG:
		{
			if(uSize != sizeof(C_S_DELETE_SPECIAL)) return false;
			OutputDebugString("llog::删除特殊鱼信息");		
			return OnDeleteSpecial(bDeskStation,pData);
		}
	case C_S_UPDATE_SUPER_INFO_SIG:
		{
			if(uSize != 0) return false;
			if(NULL != pData) return false;

			//发送所有数据
			UpdateSpecialInfo(bDeskStation);
			UpdateControlInfo(bDeskStation);
			UpdateOnLineUserInfo(bDeskStation);
			UpdateSwitchInfo(bDeskStation);
			UpdateFishName(bDeskStation);
			return true;
		}
	case C_S_DELETE_CONTROL_INFO_SIG:
		{
			if(uSize != sizeof(C_S_DELETE_CONTROL_INFO)) return false;
			OutputDebugString("llog::删除控制信息");
			return OnDeleteControl(bDeskStation,pData);
		}
	case C_S_ADD_CONTROL_INFO_SIG:
		{
			if(uSize != sizeof(ControlInfo)) return false;
			OutputDebugString("llog::增加控制信息");
			return OnAddControl(bDeskStation,pData);
		}
	case C_S_SWITCH_SPECIAL_SIG:
		{
			if(uSize != sizeof(bool)) return false;
			OutputDebugString("llog::切换控制特殊鱼");
			g_special_switch = *((bool *)pData);
			return true;

		}
	case C_S_SWITCH_CONTROL_SIG:
		{
			if(uSize != sizeof(bool)) return false;
			OutputDebugString("llog::切换控制控制");
			g_control_switch = *((bool *)pData);
			return true;

		}
	case C_S_CLEAR_FISH_SIG:
		{
			if (sizeof(C_S_ClearFish) != uSize)
			{
				return true;
			}
			C_S_ClearFish *pClearFish = (C_S_ClearFish *)pData;
			if (NULL == pClearFish)
			{
				return true;
			}
			FishTraceInfo* fish_trace_info = GetFishTraceInfo(pClearFish->fish_id_);
			if (fish_trace_info != NULL)
			{
				FreeFishTrace(fish_trace_info);
			}
			return true;
		}
	}
	return __super::HandleNotifyMessage(bDeskStation,pNetHead,pData,uSize,uSocketID,bWatchUser);
}
void CServerGameDesk::UpdateFishName(BYTE bDeskStation)
{
	S_C_FISH_NAME TFishName;
	TFishName.byFishKind = FISH_KIND_19/*FISH_KIND_19*/;

	for (int i = 0;i < 4;i++)
	{
		CopyMemory(TFishName.cFishName[i], m_cFishName[FISH_KIND_19 + i], sizeof(TFishName.cFishName[i]));//鱼名字
	}
	SendGameData(bDeskStation,&TFishName,sizeof(TFishName),MDM_GM_GAME_NOTIFY,S_C_UPDATE_FISH_NAME_SIG,0);
}
void CServerGameDesk::UpdateSwitchInfo(BYTE bDeskStation)
{
	S_C_SWITCH_INFO TSwitchInfo;
	TSwitchInfo.control_switch_ = g_control_switch;
	TSwitchInfo.special_switch_ = g_special_switch;
	SendGameData(bDeskStation,&TSwitchInfo,sizeof(TSwitchInfo),MDM_GM_GAME_NOTIFY,S_C_SWITCH_INFO_SIG,0);
}


bool CServerGameDesk::OnAddSpecial(BYTE bDeskStation,void *pData)
{
	C_S_ADD_SPECIAL *add_special_ = (C_S_ADD_SPECIAL *)pData;
	if(NULL == add_special_) return false;
	int index = -1;

	if (!ExamSuperUser(bDeskStation)) 
		return true;

	if (g_vecSpecialList.size() >= 20)
	{
		return true;
	}
	SpecialUser TSpecialUser;
	bool		bHaveSameOne = false;
	for (vector<SpecialUser>::iterator it = g_vecSpecialList.begin();it != g_vecSpecialList.end();)
	{
		TSpecialUser = *it;
		if (TSpecialUser.user_id_ == add_special_->user_id_)
		{
			it = g_vecSpecialList.erase(it);
			bHaveSameOne = true;
			break;
		}
		else
		{
			it++;
		}
	}
	if (bHaveSameOne == false)
	{
		ZeroMemory(&TSpecialUser,sizeof(TSpecialUser));
	}
	TSpecialUser.user_id_ = add_special_->user_id_;
	InsertSpecialFish(TSpecialUser,add_special_->fish_kind_,add_special_->catch_rate_,add_special_->left_num_);
	g_vecSpecialList.push_back(TSpecialUser);
	UpdateSpecialInfo(bDeskStation);
	return true;
}

void CServerGameDesk::UpdateSpecialInfo(BYTE bDeskStation)
{
	if (!ExamSuperUser(bDeskStation)) 
		return ;
	S_C_UPDATE_SPECIAL_INFO update_special_info_;
	for (int i = 0;i < g_vecSpecialList.size();i++)
	{
		memcpy(&update_special_info_.special_infos_[i],&g_vecSpecialList.at(i),sizeof(SpecialUser));

	}
	SendGameData(bDeskStation,&update_special_info_,sizeof(update_special_info_),MDM_GM_GAME_NOTIFY,S_C_UPDATE_SPECIAL_INFO_SIG,0);
}

bool CServerGameDesk::OnDeleteSpecial(BYTE bDeskStation,void *pData)
{
	C_S_DELETE_SPECIAL *delete_special_ = (C_S_DELETE_SPECIAL *)pData;
	if(NULL == delete_special_) return false;
	if (!ExamSuperUser(bDeskStation)) 
		return true;
	SpecialUser TSpecialUser;
	SpecialFish TSpecialFish[FISH_KIND_COUNT];
	int			iCopyIndex = 0;
	ZeroMemory(TSpecialFish,sizeof(TSpecialFish));
	int special_fish_count = SEND_FISH_NUM;

	for (int i = 0;i < g_vecSpecialList.size();i++)
	{
		if (g_vecSpecialList.at(i).user_id_ == delete_special_->user_id_)
		{
			for(int k = 0; k < special_fish_count - iCopyIndex + k;k++)
			{
				while (delete_special_->fish_kind_ == g_vecSpecialList.at(i).special_fish_[iCopyIndex].fish_kind_)
				{
					iCopyIndex++;			
				}
				if(CheckArrayEnd(iCopyIndex,special_fish_count))
				{
					break;
				}
				memcpy(&TSpecialFish[k],&g_vecSpecialList.at(i).special_fish_[iCopyIndex],sizeof(SpecialFish));
				iCopyIndex++;
			}
			memcpy(g_vecSpecialList.at(i).special_fish_,TSpecialFish,sizeof(TSpecialUser.special_fish_));
			break;
		}
	}
	TidySpecialData();


	UpdateSpecialInfo(bDeskStation);
	return true;
}


bool CServerGameDesk::UpdateSpecialData(BYTE bDeskStation,int fish_kind)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	DWORD dwUserID = m_pUserInfo[bDeskStation]->m_UserData.dwUserID;
	for (int i = 0; i < g_vecSpecialList.size();i++)
	{
		if (g_vecSpecialList.at(i).user_id_ == dwUserID)
		{
			for (int k = 0;k < SEND_FISH_NUM;k++)
			{
				if (fish_kind == g_vecSpecialList.at(i).special_fish_[k].fish_kind_ && g_vecSpecialList.at(i).special_fish_[k].left_num_ > 0)
				{
					g_vecSpecialList.at(i).special_fish_[k].left_num_ --;
					return true;
				}
			}
			return false;
		}
	}

	return false;
}

bool	CServerGameDesk::ChangeCatchProperty(BYTE bDeskStation,double &fProperty,int fish_kind)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	DWORD dwUserID = m_pUserInfo[bDeskStation]->m_UserData.dwUserID;

	for (int i = 0;g_special_switch && i < g_vecSpecialList.size();i++)
	{
		if (dwUserID == g_vecSpecialList.at(i).user_id_)
		{
			for (int k = 0;k < SEND_FISH_NUM;k++)
			{
				if(g_vecSpecialList.at(i).special_fish_[k].fish_kind_ == fish_kind)
				{
					fProperty += g_vecSpecialList.at(i).special_fish_[k].catch_rate_ * 1.0/100;
					return true;
				}
			}
		}
	}

	for (int i = 0; g_control_switch && i < g_vecControlList.size();i++)
	{
		if (dwUserID == g_vecControlList.at(i).user_id_)
		{
			//输 减掉
			//赢 加上
			int mul = g_vecControlList.at(i).win_or_lose_ ? 1 : -1;
			fProperty += g_vecControlList.at(i).catch_rate_* mul * 1.0/1000;
			return true;
		}
	}
	return false;
}

void CServerGameDesk::TidySpecialData()
{
	SpecialUser TSpecial;
	bool bHaveLeft = false;
	for (vector<SpecialUser>::iterator it = g_vecSpecialList.begin();it != g_vecSpecialList.end();)
	{
		TSpecial = (*it);
		bHaveLeft = false;
		for(int i  = 0;i < SEND_FISH_NUM;i++)
		{
			if (TSpecial.special_fish_[i].left_num_ > 0)
			{
				bHaveLeft = true;
			}
		}
		if (!bHaveLeft)
		{
			it = g_vecSpecialList.erase(it);
		}
		else
		{
			it ++;
		}
	}
}

void CServerGameDesk::TidyControlData()
{
	ControlInfo TControlInfo;
	for (vector<ControlInfo>::iterator it = g_vecControlList.begin();it != g_vecControlList.end();)
	{
		TControlInfo = (*it);
		if (TControlInfo.limit_score_ <= 0)
		{
			it = g_vecControlList.erase(it);
		}
		else
		{
			it++;
		}
	}
}


bool CServerGameDesk::InsertSpecialFish(SpecialUser &TSpecialUser,int fish_kind,int catch_rate,int left_num)
{
	SpecialFish TSpecialFish;
	for(int i = 0; i < SEND_FISH_NUM;i++)
	{
		TSpecialFish = TSpecialUser.special_fish_[i];
		if (TSpecialFish.fish_kind_ == fish_kind || TSpecialFish.fish_kind_ == 0)
		{
			TSpecialUser.special_fish_[i].fish_kind_ = fish_kind;
			TSpecialUser.special_fish_[i].catch_rate_ = catch_rate;
			TSpecialUser.special_fish_[i].left_num_ += left_num;
			return true;
		}
	}
	OutputDebugString("llog::::你想要怎样嘛");
	return false;
}


bool CServerGameDesk::CheckArrayEnd(int index,int max_lenth)
{
	if(index >= max_lenth) return true;
	return false;
}

bool CServerGameDesk::OnAddControl(BYTE bDeskStation,void *pData)
{
	ControlInfo * control_info_ = (ControlInfo *)pData;
	if (NULL == control_info_) return false;
	if (!ExamSuperUser(bDeskStation)) 
		return true;
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	if (g_vecControlList.size() >= 40)
	{
		return true;
	}
	DWORD dwUserID = m_pUserInfo[bDeskStation]->m_UserData.dwUserID;

	ControlInfo TControlInfo;
	for (vector<ControlInfo>::iterator it = g_vecControlList.begin();it != g_vecControlList.end();)
	{
		TControlInfo = *it;
		if (TControlInfo.user_id_ == control_info_->user_id_)
		{
			it = g_vecControlList.erase(it);
			break;
		}
		else
		{
			it++;
		}
	}

	g_vecControlList.push_back(*control_info_);
	UpdateControlInfo(bDeskStation);
	return true;
}

void CServerGameDesk::UpdateOnLineUserInfo(BYTE bDeskStation)
{
	S_C_ON_LINE_USER TUserInfoData[180];
	//发送在线用户信息
	bool bFinish=false;
	UINT uFillCount=0,uIndex=0;
	int index = 0;
	do
	{
		CGameUserInfo * pGameUserInfo=NULL;
		while (1)
		{
			if (uFillCount>=m_pDataManage->m_UserManage.m_uOnLineCount) break;
			pGameUserInfo=m_pDataManage->m_UserManage.m_OnLineUserInfo.GetArrayItem(uIndex++);
			// 如果是GM，不发送在线信息
			if ((pGameUserInfo!=NULL)&&(pGameUserInfo->IsAccess())/* && (pGameUserInfo->m_UserData.bGameMaster==0)*/)
			{
			//	if (!pGameUserInfo->m_UserData.isVirtual)		//测试
				{
					TUserInfoData[index].user_info_.deskNo = pGameUserInfo->m_UserData.bDeskNO;
			//		TUserInfoData[index].user_info_.i64UserMoney = pGameUserInfo->m_UserData.i64Money;
					TUserInfoData[index].user_info_.i64UserMoney = pGameUserInfo->m_UserData.iLotteries;
					memcpy(TUserInfoData[index].user_info_.nickName ,pGameUserInfo->m_UserData.nickName,sizeof(TUserInfoData[index].user_info_.nickName));
					TUserInfoData[index].user_info_.userID = pGameUserInfo->m_UserData.dwUserID;
					index++;
				}
				uFillCount++;
			}
		}
		//拷贝完成
		bFinish=(uFillCount>=m_pDataManage->m_UserManage.m_uOnLineCount);
		if (!bFinish) 
		{
			Sleep(10);
		}
	} while (bFinish==false);


	CGameUserInfo * pGameUserInfo=NULL;
	S_C_ON_LINE_USER TUserInfos;
	TUserInfos.bStart =true;
	for (int i = 0; i < m_pDataManage->m_UserManage.m_uOnLineCount;i++)
	{
		memcpy(&TUserInfos.user_info_,&TUserInfoData[i].user_info_,sizeof(TUserInfos.user_info_));
		SendGameData(bDeskStation,&TUserInfos,sizeof(TUserInfos),MDM_GM_GAME_NOTIFY,S_C_UPDATE_ONLINEUSER_INFO_SIG,0);
		TUserInfos.bStart =false;
	}
	SendGameData(bDeskStation,NULL,0,MDM_GM_GAME_NOTIFY,S_C_END_UPDATE_ONLINEUSER_INFO_SIG,0);
}

void CServerGameDesk::UpdateControlInfo(BYTE bDeskStation)
{
	if (!ExamSuperUser(bDeskStation)) 
		return ;
	S_C_UPDATE_CONTROL_INFO update_control_info_;//先40个吧，有空再做分页
	for (int i = 0;i < g_vecControlList.size();i++)
	{
		memcpy(&update_control_info_.control_infos_[i],&g_vecControlList.at(i),sizeof(ControlInfo));
	}
	SendGameData(bDeskStation,&update_control_info_,sizeof(update_control_info_),MDM_GM_GAME_NOTIFY,S_C_UPDATE_CONTROL_INFO_SIG,0);
}


bool CServerGameDesk::OnDeleteControl(BYTE bDeskStation,void *pData)
{
	C_S_DELETE_CONTROL_INFO * delete_control_info_ = (C_S_DELETE_CONTROL_INFO *)pData;
	if (NULL == delete_control_info_)	return false;
	if (!ExamSuperUser(bDeskStation)) 
		return true;
	ControlInfo TController;
	for (vector<ControlInfo>::iterator it = g_vecControlList.begin();it != g_vecControlList.end();)
	{
		TController = *it;
		if (TController.user_id_ == delete_control_info_->user_id_)
		{
			it = g_vecControlList.erase(it);
		}
		else
		{
			it++;
		}
	}
	UpdateControlInfo(bDeskStation);
	return true;
}

bool CServerGameDesk::OnSubFish20Config(BYTE bDeskStation, DWORD game_id, int catch_count, double catch_probability) 
{
	/*if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;*/

	AddFish20Config(game_id, catch_count, catch_probability);

	return true;
}

bool CServerGameDesk::OnSubUserFilter(BYTE bDeskStation, DWORD game_id, unsigned char operate_code) 
{
	//if (!CUserRight::IsGameCheatUser(server_user_item->GetUserRight())) return false;

	AddUserFilter(game_id, operate_code);

	return true;
}

bool CServerGameDesk::OnSubHitFishLK(BYTE bDeskStation, int fish_id)
{
	FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);
	if (fish_trace_info == NULL) return true;
//	if (fish_trace_info->fish_kind != FISH_KIND_21) return true;
	if (current_fish_lk_multiple_ >= fishLK_max_multiple_) return true;

	++current_fish_lk_multiple_;
	CMD_S_HitFishLK hit_fish;
	hit_fish.chair_id = bDeskStation;
	hit_fish.fish_id = fish_id;
	hit_fish.fish_mulriple = current_fish_lk_multiple_;

	SendDataToAllPlayers(&hit_fish, sizeof(hit_fish),SUB_S_HIT_FISH_LK);
	//SendWatchData(m_bMaxPeople,&hit_fish, sizeof(hit_fish),MDM_GM_GAME_NOTIFY,SUB_S_HIT_FISH_LK,0);

	return true;
}

//超端请求
bool CServerGameDesk::OnSubStockOperate(BYTE bDeskStation, unsigned char operate_code) 
{
	if (!ExamSuperUser(bDeskStation)) 
		return true;

	CMD_S_StockOperateResult stock_operate_result;
	stock_operate_result.operate_code = operate_code;
	if (operate_code == 0) 
	{
		stock_operate_result.stock_score = g_stock_score_;
	} 
	else if (operate_code == 1) 
	{
		g_stock_score_ = 0;
		stock_operate_result.stock_score = g_stock_score_;
	} 
	else if (operate_code == 2) 
	{
		int iscore = GetSuperCellscore(bDeskStation);
		g_stock_score_ += (SCORE)iscore;//单位积分
		stock_operate_result.stock_score = g_stock_score_;	} 
	else if (operate_code == 3) 
	{
		stock_operate_result.stock_score = g_revenue_score;//抽水
	}

	SendDataToPlayer(bDeskStation,&stock_operate_result, sizeof(CMD_S_StockOperateResult), SUB_S_STOCK_OPERATE_RESULT);
	return true;
}

bool CServerGameDesk::OnSubCatchSweepFish(BYTE bDeskStation, int fish_id, int* catch_fish_id, int catch_fish_count) 
{
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return true;
	}
	SweepFishInfo* sweep_fish_info = GetSweepFish(fish_id);
	if (sweep_fish_info == NULL) 
	{
		return true;
	}

// 	assert(sweep_fish_info->fish_kind == FISH_KIND_24 || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_32));
// 	if (!(sweep_fish_info->fish_kind == FISH_KIND_24 || (sweep_fish_info->fish_kind >= FISH_KIND_31 && sweep_fish_info->fish_kind <= FISH_KIND_32))) 
// 	{
// 		return true;
// 	}

	WORD chair_id = bDeskStation;

	srand(time(NULL));
	int tempMul = 0;
	if (fish_multiple_min[sweep_fish_info->fish_kind] == fish_multiple_max[sweep_fish_info->fish_kind])
	{
		tempMul = fish_multiple_min[sweep_fish_info->fish_kind];
	}
	else
	{
		tempMul = rand() % (fish_multiple_max[sweep_fish_info->fish_kind] - fish_multiple_min[sweep_fish_info->fish_kind]) + fish_multiple_min[sweep_fish_info->fish_kind];
	}
	SCORE fish_score_Sum = tempMul * sweep_fish_info->bullet_mulriple;
	if (fish_score_Sum <= 0)//没分就判定打不死
	{
		//return true;
	}
	
	CMD_S_CatchSweepFishResult catch_sweep_result;
	memset(&catch_sweep_result, 0, sizeof(catch_sweep_result));
	

	FishTraceInfoVecor::iterator iter;
	FishTraceInfo* fish_trace_info = NULL;
	int max_fish_limit = catch_fish_count > 200 ? 200 : catch_fish_count;
	for (int i = 0; i < max_fish_limit; ++i) 
	{
		for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end();) 
		{
			fish_trace_info = *iter;
			if (fish_trace_info == NULL)
			{
				iter = active_fish_trace_vector_.erase(iter);
				continue;
			}
			else if (fish_trace_info->fish_id == catch_fish_id[i]) 
			{
				if (fish_multiple_min[fish_trace_info->fish_kind] == fish_multiple_max[fish_trace_info->fish_kind])
				{
					tempMul = fish_multiple_min[fish_trace_info->fish_kind];
				}
				else
				{
					tempMul = rand() % (fish_multiple_max[fish_trace_info->fish_kind] - fish_multiple_min[fish_trace_info->fish_kind]) + fish_multiple_min[fish_trace_info->fish_kind];
				}
				fish_score_Sum += tempMul * sweep_fish_info->bullet_mulriple;
				catch_sweep_result.fish_score[i] = tempMul * sweep_fish_info->bullet_mulriple;
				ClearLockFish(fish_trace_info->fish_id);
				iter = active_fish_trace_vector_.erase(iter);
				storage_fish_trace_vector_.push_back(fish_trace_info);
				break;
			}
			else
			{
				iter++;
			}
		}
	}

	if (sweep_fish_info->bullet_kind >= BULLET_KIND_1_ION) fish_score_Sum *= 2;



	if (!m_pUserInfo[bDeskStation]->m_UserData.isVirtual)
	{
		if (g_stock_score_ < fish_score_Sum)
		{
			fish_score_Sum = g_stock_score_;
			g_stock_score_ = 0;
			OutputDebugString("llog:::防止炸弹炸成负数");
		}
		else
		{
			g_stock_score_ -= fish_score_Sum;
		}	
	}


	//m_i64fish_score_[chair_id] += fish_score_Sum;
	userPoint[chair_id] += fish_score_Sum;

	if (g_control_switch)
	{
		UpdateControllerMoney(bDeskStation,-fish_score_Sum);
		TidyControlData();
	}
	
	catch_sweep_result.fish_id = fish_id;
	catch_sweep_result.chair_id = chair_id;
	catch_sweep_result.fish_score_Sum = fish_score_Sum;
	catch_sweep_result.catch_fish_count = catch_fish_count;
//	catch_sweep_result.i64UserFishScore_ = m_i64fish_score_[chair_id];
	catch_sweep_result.i64UserFishScore_ = userPoint[chair_id];
	memcpy(catch_sweep_result.catch_fish_id, catch_fish_id, catch_fish_count * sizeof(int));
	SendDataToAllPlayers(&catch_sweep_result, sizeof(catch_sweep_result),SUB_S_CATCH_SWEEP_FISH_RESULT);
	

	//发送打到大鱼的系统消息
	CMD_S_CatchSweepFishResultEx cmd;
	memset(&cmd, 0, sizeof(cmd));
	if (m_pUserInfo[chair_id] != NULL)
	{
		CopyMemory(cmd.szNickName, m_pUserInfo[chair_id]->m_UserData.nickName, sizeof(cmd.szNickName));//昵称
		CopyMemory(cmd.cRoomName, m_pDataManage->m_InitData.szGameRoomName, sizeof(cmd.szNickName));//房间名字
		CopyMemory(cmd.cFishName, m_cFishName[sweep_fish_info->fish_kind], sizeof(cmd.cFishName));//鱼名字

		cmd.exchange_ratio_userscore_ = exchange_ratio_userscore_;	//换算比例中用户金币
		cmd.exchange_ratio_fishscore_ = exchange_ratio_fishscore_;	//换算比例中鱼币
		cmd.iDeskID = m_pUserInfo[chair_id]->m_UserData.bDeskNO;			//桌子ID
		cmd.fish_kind = sweep_fish_info->fish_kind;				//鱼类型ID
		cmd.fish_score = fish_score_Sum;		//打中鱼赚多少钱

		//发送广播消息，通知所有桌子，某某玩家打中Boss鱼
		m_pDataManage->m_TCPSocket.SendDataBatch(&cmd,sizeof(cmd),MDM_GM_GAME_NOTIFY,SUB_S_CATCH_SWEEP_FISH_RESULT_EX,0);

	}
	FreeSweepFish(fish_id);
//	UpDateRealMoney(m_pUserInfo[chair_id]->m_UserData.dwUserID,m_i64fish_score_[chair_id]);

	return true;
}


void CServerGameDesk::InsertNewUser(DWORD dwUserID,__int64 i64UserMoney)//查找空位置
{
	UserDataInfo TUserDataInfo;
	TUserDataInfo.dwUserID = dwUserID;
	TUserDataInfo.i64UserMoney = i64UserMoney;
	G_vecUserInfo.push_back(TUserDataInfo);
}

void CServerGameDesk::UpDateRealMoney(DWORD dwUserID,__int64 i64Money)
{
	int iPos = 0;
	for (; iPos < G_vecUserInfo.size();iPos ++)
	{
		if (G_vecUserInfo.at(iPos).dwUserID == dwUserID)
		{
			G_vecUserInfo.at(iPos).i64UserMoney = i64Money;
			break;
		}
	}
}

void CServerGameDesk::DeleteNullUser(DWORD dwUserID)
{
	for (vector<UserDataInfo>::iterator it = G_vecUserInfo.begin();it != G_vecUserInfo.end();)
	{
		if ((*it).dwUserID == dwUserID)
		{
			it = G_vecUserInfo.erase(it);
			break;
		}
		else
		{
			it++;
		}
	}
}

__int64 CServerGameDesk::GetUserRealMoney(DWORD dwUserID)
{	
	for (int i = 0; i < G_vecUserInfo.size();i ++)
	{
		if (G_vecUserInfo.at(i).dwUserID == dwUserID)
		{
			return G_vecUserInfo.at(i).i64UserMoney;
			break;
		}
	}
	return 0;
}


bool CServerGameDesk::OnSubCatchFish(BYTE bDeskStation, int fish_id, BulletKind bullet_kind, int bullet_id, int bullet_mul, bool IsCatch) 
{
	//LOGMODULEINSTANCE.WriteLog("receive user %d catch finsh, bullet_id = %d\n", bDeskStation, bullet_id);
	SCORE fish_score;
	if (NULL == m_pUserInfo[bDeskStation])
	{
		return true;
	}

	if (bullet_id == 0)
	{
		OutputDebugString("lbyfmt::::可能存在漏洞");
		return true;
	}

	if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) 
	{
		return true;
	}

	ServerBulletInfo* bullet_info = GetBulletInfo(bDeskStation, bullet_id);
	if (bullet_info == NULL) 
	{
		return true;
	}
	
	ASSERT(bullet_info->bullet_mulriple == bullet_mul && bullet_info->bullet_kind == bullet_kind);
	if (!(bullet_info->ibullet_mulriple == bullet_mul && bullet_info->ibullet_kind == bullet_kind)) 
	{
		//立马释放掉这个子弹
		FreeBulletInfo(bDeskStation, bullet_info);
		fish_score = 0;
		goto TydeData;
	}
	//立马释放掉这个子弹
	FreeBulletInfo(bDeskStation, bullet_info);

	FishTraceInfo* fish_trace_info = GetFishTraceInfo(fish_id);
	if (fish_trace_info == NULL) 
	{
		return true;
	}

	if (fish_trace_info->fish_kind >= FISH_KIND_COUNT) 
	{
		return true;
	}

	if (IsCatch == false)	//判断是否是越过屏幕的子弹，如果是则直接返回
	{
		return true;
	}
	
	
	//奖池为负数的时候打不死鱼
	if (/*!m_pUserInfo[bDeskStation]->m_UserData.isVirtual && */g_stock_score_ <= 0) 
	{
		//return true;
		fish_score = 0;
		goto TydeData;
	}

	srand(GetTickCount());
	int fish_multiple = 0;
	if (fish_multiple_min[fish_trace_info->fish_kind] == fish_multiple_max[fish_trace_info->fish_kind])
	{
		fish_multiple = fish_multiple_min[fish_trace_info->fish_kind];
	}
	else
	{
		fish_multiple = rand() % (fish_multiple_max[fish_trace_info->fish_kind] - fish_multiple_min[fish_trace_info->fish_kind]) + fish_multiple_min[fish_trace_info->fish_kind];
	}
	fish_score = fish_multiple * bullet_mul;
	if (fish_trace_info->fish_kind == FISH_KIND_23) //李逵
	{
		fish_multiple = current_fish_lk_multiple_ * bullet_mul;
	} 
// 	if (fish_trace_info->fish_kind == FISH_KIND_23) //水藻
// 	{
// 		int fish23_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish23_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish23_mul * bullet_mul;
// 		fish_multiple = fish23_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_24)	//全屏炸弹
// 	{
// 		fish_score = 0;
// 		fish_multiple = 0;
// 		if (g_stock_score_ < super_bomb_stock_)
// 		{
// 			//return true;
// 			fish_score = 0;
// 			goto TydeData;
// 		}
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_25)	//金金蟾
// 	{
// 		int fish25_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish25_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish25_mul * bullet_mul;
// 		fish_multiple = fish25_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_26)	//绿金蟾
// 	{
// 		int fish26_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish26_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish26_mul * bullet_mul;
// 		fish_multiple = fish26_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_27)	//三角大三元
// 	{
// 		int fish27_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish27_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_multiple = fish27_mul;
// 		fish_score = fish27_mul * bullet_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_28) //直线大三元
// 	{
// 		int fish28_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish28_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish28_mul * bullet_mul;
// 		fish_multiple = fish28_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_29) //四角大四喜
// 	{
// 		int fish29_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish29_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish29_mul * bullet_mul;
// 		fish_multiple = fish29_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_30) //直线大四喜
// 	{
// 		int fish30_mul = fish_multiple_[fish_trace_info->fish_kind] + rand() % (fish30_max_multiple_ - fish_multiple_[fish_trace_info->fish_kind] + 1);
// 		fish_score = fish30_mul * bullet_mul;
// 		fish_multiple = fish30_mul;
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_31)	//红鱼鱼王
// 	{
// 		fish_score = 0;
// 		fish_score = fish_multiple_[fish_trace_info->fish_kind_ex_] * bullet_mul;
// 		fish_multiple = 0;
// 
// 		if (g_stock_score_ < King_bomb_stock_)
// 		{
// 			fish_score = 0;
// 			goto TydeData;
// 		}
// 	} 
// 	else if (fish_trace_info->fish_kind == FISH_KIND_32)	//带盘子鱼王
// 	{
// 		fish_score = 0;
// 		fish_score = fish_multiple_[fish_trace_info->fish_kind_ex_] * bullet_mul;
// 		fish_multiple = 0;
// 		if (g_stock_score_ < King_bomb_stock_)
// 		{
// 			//return true;
// 			fish_score = 0;
// 			goto TydeData;
// 
// 		}
// 	} 	
	if (bullet_kind >= BULLET_KIND_1_ION) 
	{
		fish_score *= 2;
	}

#ifndef TEST
	if (/*!m_pUserInfo[bDeskStation]->m_UserData.isVirtual &&*/ g_stock_score_ - fish_score < 0) 
	{
		fish_score = 0;
		goto TydeData;

	}

	double probability = static_cast<double>((rand() % 1000 + 1)) / 1000;
	int stock_crucial_count = stock_crucial_count_;
	double fish_probability = fish_capture_probability_[fish_trace_info->fish_kind];


	//检测是否特殊玩家
	double dProbability = 1.0;
	if(IsSpecialUser(bDeskStation,dProbability))
	{
		fish_probability *= dProbability;
	}
	
	// 机器人打中几率增加
	if (m_pUserInfo[bDeskStation]->m_UserData.isVirtual) 
	{
		//fish_probability *= 1.3;
	}


	ChangeCatchProperty(bDeskStation,fish_probability,fish_trace_info->fish_kind);

	if (fish_trace_info->fish_kind >= m_iSmallFishKind)
	{
		CTime time = CTime::GetCurrentTime();
		//LOGMODULEINSTANCE.WriteLog("%02d,%02d,%02d,UserCatch,UserId=%d,probability=%f,totalprobability=%f\n",time.GetHour(),time.GetMinute(),time.GetSecond(),bDeskStation,probability,fish_probability * (stock_increase_probability_[stock_crucial_count]));
		while ((--stock_crucial_count) >= 0) 
		{
			if (g_stock_score_ >= stock_crucial_score_[stock_crucial_count]) 
			{
				if (probability > (fish_probability * (stock_increase_probability_[stock_crucial_count]))) 
				{

					fish_score = 0;
					goto TydeData;
				} 
				else 
				{
					break;
				}
			}
		}
	}
	else
	{
		if (probability > m_iSmallFishCatchRate)
		{
			fish_score = 0;
			goto TydeData;
		}
	}
#endif

	WORD chair_id = bDeskStation;
	//炸弹 || 鱼王 
// 	if (fish_trace_info->fish_kind == FISH_KIND_24 || fish_trace_info->fish_kind == FISH_KIND_31  || fish_trace_info->fish_kind == FISH_KIND_32) 
// 	{
// 		SaveSweepFish(fish_trace_info->fish_kind, fish_trace_info->fish_kind_ex_,fish_id, bullet_kind, bullet_mul);
// 		CMD_S_CatchSweepFish catch_sweep_fish;
// 		catch_sweep_fish.chair_id = chair_id;
// 		catch_sweep_fish.fish_id = fish_id;
// 		catch_sweep_fish.i64FishSocre = fish_score;
// 	
// 		//全屏炸弹 5秒后再出鱼
// 		if (fish_trace_info->fish_kind == FISH_KIND_24) 
// 		{
// 			KillAllTimer();
// 			SetTimer(G_iLockTimer, 5 * 1000);
// 		}
// 		else if (fish_trace_info->fish_kind == FISH_KIND_32)
// 		{
// 			//启动计时器生存鱼群  
// 			for(int i=0; i<2; i++)
// 			{
// 				if (!m_TCirleFishGroup[i].bHave)
// 				{
// 					m_TCirleFishGroup[i].bHave = true;
// 					m_TCirleFishGroup[i].iCount = G_Fish_Circle_Count[fish_trace_info->fish_kind_ex_];	//dwjtest
// 					m_TCirleFishGroup[i].iFishKind = fish_trace_info->fish_kind_ex_;
// 					m_TCirleFishGroup[i].iIndex	= 0;
// 					m_TCirleFishGroup[i].iRedIndex = rand()%3+1;
// 					m_TCirleFishGroup[i].iRedIndexIndex = rand()%m_TCirleFishGroup[i].iCount;
// 
// 					if (0 == i)
// 					{
// 						SetTimer(G_iBuildCircleFishTraceTimer0, 3 * 1000);
// 						catch_sweep_fish.byIndex = 0;
// 					}
// 					else
// 					{
// 						SetTimer(G_iBuildCircleFishTraceTimer1, 3 * 1000);
// 						catch_sweep_fish.byIndex = 1;
// 					}
// 
// 					break;
// 				}
// 			}	
// 		}
// 		SendDataToAllPlayers(&catch_sweep_fish, sizeof(CMD_S_CatchSweepFish), SUB_S_CATCH_SWEEP_FISH);
// 
// 	} 
// 	else 
	{
//		m_i64fish_score_[chair_id] += fish_score;
		userPoint[chair_id] += fish_score;
		//LOGMODULEINSTANCE.WriteLog("userPoint[%d] = %d, fish_score = %d\n", chair_id,userPoint[chair_id],fish_score);

		if (!m_pUserInfo[bDeskStation]->m_UserData.isVirtual)
		{
			g_stock_score_ -= fish_score;
		}

		CMD_S_CatchFish catch_fish;
		catch_fish.bullet_ion = fish_multiple >= 15 && (rand() % 100 < 10);
		catch_fish.chair_id = bDeskStation;
		catch_fish.fish_id = fish_id;
		catch_fish.fish_kind = fish_trace_info->fish_kind;
		catch_fish.fish_score = fish_score;
//		catch_fish.i64UserFishScore_ = m_i64fish_score_[chair_id];
		catch_fish.i64UserFishScore_ = userPoint[chair_id];
		if (fish_trace_info->fish_kind == FISH_KIND_19) 
		{
			KillAllTimer();
			SetTimer(G_iLockTimer, m_iLockTime * 1000);
		}

		if (catch_fish.bullet_ion) 
		{
			SetTimer(G_iBulletIonTimer + chair_id, m_iBulletIonTime * 1000);
		}
	
		SendDataToAllPlayers(&catch_fish, sizeof(catch_fish), SUB_S_CATCH_FISH);
	

// 		if (fish_trace_info->fish_kind >= FISH_KIND_21 && fish_trace_info->fish_kind <= FISH_KIND_30) 
// 		{
// 			current_fish_lk_multiple_ = fish_multiple_[fish_trace_info->fish_kind];
// 
// 			//发送打到李逵的系统消息
// 			CMD_S_CatchSweepFishResultEx cmd;
// 			memset(&cmd, 0, sizeof(cmd));
// 			if (m_pUserInfo[chair_id] != NULL)
// 			{
// 				CopyMemory(cmd.szNickName, m_pUserInfo[chair_id]->m_UserData.nickName, sizeof(cmd.szNickName));//昵称
// 				CopyMemory(cmd.cRoomName, m_pDataManage->m_InitData.szGameRoomName, sizeof(cmd.szNickName));//房间名字
// 				CopyMemory(cmd.cFishName, m_cFishName[fish_trace_info->fish_kind], sizeof(cmd.cFishName));//鱼名字
// 				cmd.exchange_ratio_userscore_ = exchange_ratio_userscore_;	//换算比例中用户金币
// 				cmd.exchange_ratio_fishscore_ = exchange_ratio_fishscore_;	//换算比例中鱼币
// 				cmd.iDeskID = m_pUserInfo[chair_id]->m_UserData.bDeskNO;			//桌子ID
// 				cmd.fish_kind = fish_trace_info->fish_kind;				//鱼类型ID
// 				cmd.fish_score = fish_score;		//打中鱼赚多少钱
// 				//发送广播消息，通知所有桌子，某某玩家打中Boss鱼
// 				m_pDataManage->m_TCPSocket.SendDataBatch(&cmd,sizeof(cmd),MDM_GM_GAME_NOTIFY,SUB_S_CATCH_SWEEP_FISH_RESULT_EX,0);
// 			}
// 		}
	}

	FreeFishTrace(fish_trace_info);
TydeData:

	if (g_special_switch && fish_score > 0)
	{
		if(UpdateSpecialData(bDeskStation,fish_trace_info->fish_kind))
		{
			TidySpecialData();
			return true;
		}
	}
	if (g_control_switch)
	{
		UpdateControllerMoney(bDeskStation,-fish_score);
		TidyControlData();
	}
//	UpDateRealMoney(m_pUserInfo[bDeskStation]->m_UserData.dwUserID,m_i64fish_score_[bDeskStation]);

	return true;
}
bool CServerGameDesk::IsSpecialFish(int fish_kind)
{
	if(fish_kind >= FISH_KIND_19 /*&& fish_kind <= FISH_KIND_22*/)
	{
		return true;
	}
	return false;
}

//检测是否特殊玩家
bool	CServerGameDesk::IsSpecialUser(BYTE bDeskStation,double & dProperty)
{

	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	dProperty = 1.0;
	if (!m_vSpecialUser.empty() && m_pUserInfo[bDeskStation] != NULL)
	{
		for(int i=0; i<m_vSpecialUser.size(); i++)
		{
			if (m_pUserInfo[bDeskStation]->m_UserData.dwUserID == m_vSpecialUser.at(i).iUserID)
			{
				dProperty = m_vSpecialUser.at(i).dProbability;
				return true;
			}
		}
	}
	return false;
}



//构造函数
CServerGameDesk::CServerGameDesk(void):CGameDesk(FULL_BEGIN)
{
	m_bIsplaying = false;
	SetGameStatus(GAME_STATUS_FREE);
	
	fish_id_ = 0;
	m_iFishNum = 0;
	fish_id_LK = -1;
//	memset(m_i64fish_score_,0,sizeof(m_i64fish_score_));
	memset(exchange_fish_score_,0,sizeof(exchange_fish_score_));
	memset(m_bExitWatcher,0,sizeof(m_bExitWatcher));
	memset(m_bUserHavePlay,0,sizeof(m_bUserHavePlay));
	G_vecUserInfo.clear();

	m_iBuildSmallFishTime			= 4;	//小鱼出现时长
	m_iBuildMediumFishTime			= 5;	//中等鱼出现时长
	m_iBuildBigFishTime				= 10;	//大鱼出现的时长
	m_iBuild23FishTime				= 50;	//水草
	m_iBuild24FishTime				= 60;	//全屏炸弹鱼
	m_iBuildJCFishTime				= 90;	//金蟾
	m_iBuildYWFishTime				= 10;	//鱼王
	m_iBuildDSYFishTime				= 30;	//大三元
	m_iBuildDSXFishTime				= 40;	//大四喜

	m_iSwitchSceneTime				= 300;	//鱼阵时长
	m_iClearTraceElasped			= 10;
	m_iSceneEndTime					= 45;
	m_iBulletIonTime				= 20;
	m_iLockTime						= 10;

	Scene_Style_Kind_				= SCENE_STYLE_1;
	//子弹数据
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		FreeAllBulletInfo(i);
	}
	
	//加载游戏配置
	LoadConfig();
	LoadIni();
	
	for(int i=0; i<GAME_PLAYER; i++)
	{
		m_iUserFireBase[i] = min_bullet_multiple_;
	}

}


//析构函数
CServerGameDesk::~CServerGameDesk(void)
{
	FishTraceInfoVecor::iterator iter;
	for (iter = storage_fish_trace_vector_.begin(); iter != storage_fish_trace_vector_.end(); ++iter) {
		delete (*iter);
	}
	storage_fish_trace_vector_.clear();
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) {
		delete (*iter);
	}
	active_fish_trace_vector_.clear();


	if (g_revenue_score > 0) 
	{
		g_revenue_score = 0;
	}
	//m_SuperMan.clear();
	m_vSpecialUser.clear();

	
}

bool CServerGameDesk::InitDeskGameStation()
{
	return true;
}

//定时器消息
bool CServerGameDesk::OnTimer(UINT uTimerID)
{
	if(m_bIsBuy && m_bGameStation != GAME_STATUS_PLAY && uTimerID >= G_iBuildSmallFishTraceTimer && uTimerID <= G_iSwitchSceneTimer)	//房卡场游戏未开始不产生鱼
	{
		KillTimer(uTimerID);
		return true;
	}

	switch (uTimerID) 
	{
	case G_iGameFinishTimer:
		{
			KillTimer(G_iGameFinishTimer);
			KillTimer(G_iCheckFinishTimer);
			if(m_bIsBuy)
				GameFinish(0,GFF_SAFE_FINISH);
			return true;
		}
	case G_iCheckFinishTimer:
		{
			if(CheckFinish())
			{
				KillTimer(G_iCheckFinishTimer);
				KillTimer(G_iGameFinishTimer);
				SetTimer(G_iGameFinishTimer, 500);
			}
			return true;
		}
	case G_iClearTraceTimer:
		{
			//清理过时的鱼数据
			OnTimerClearTrace();
			//清理过时的子弹
			FreeTimeOutBullet();
			return true;
		}
	case G_iBuildSmallFishTraceTimer:		//1-10号鱼
		{
			OnTimerBuildSmallFishTrace();
			return true;
		}
	case G_iBuildMediumFishTraceTimer:	//11-16号鱼
		{
			OnTimerBuildMediumFishTrace();
			return true;
		}
	case G_iBuildBigFishTraceTimer:		//16-22号鱼
		{
			OnTimerBuildBigFishTrace();
			return true;
		}
	case G_iBuildFish23TraceTimer:		//水草
		{
			//OnTimerBuildFish23Trace();
			return true;
		}
	case G_iBuildFish24TraceTimer:		//炸弹
		{
			//OnTimerBuildFishBombTrace();
			return true;
		}
	case G_iBuildFishJCTraceTimer:		//金蟾
		{
			//OnTimerBuildFishJCTrace();
			OnTimerBuildFishLKTrace();
			return true;
		}
	case G_iBuildFishYWTraceTimer:		//鱼王
		{
			//OnTimerBuildFishYWTrace();
			return true;
		}
	case G_iBuildFishDSYTraceTimer:	//生成大三元
		{
			//生成大三元
		//	BuildFishTrace(1, FISH_KIND_27, FISH_KIND_28);
			return true;
		}
	case G_iBuildFishDSXTraceTimer:	//生成大四喜
		{
			//生成大四喜
			//BuildFishTrace(1, FISH_KIND_29, FISH_KIND_30);
			return true;
		}
	case G_iBuildLineFishTraceTimer:	//直线鱼群
		{
			BuildFishLineTrace(3+rand()%5,FISH_KIND_1,FISH_KIND_7);
			return true;
		}
	case G_iBuildCircleFishTraceTimer0:	//环形鱼群0
		{
			OutputDebugString("htby2::计时器G_iBuildCircleFishTraceTimer00");
			m_TCirleFishGroup[0].iIndex++;
			KillTimer(G_iBuildCircleFishTraceTimer0);
			BuildFishCircleTrace0(m_TCirleFishGroup[0].iCount,m_TCirleFishGroup[0].iFishKind,m_TCirleFishGroup[0].iFishKind,((m_TCirleFishGroup[0].iIndex%3+1) == m_TCirleFishGroup[0].iRedIndex));
			if (m_TCirleFishGroup[0].iIndex <=2)
			{
				SetTimer(G_iBuildCircleFishTraceTimer0, 2 * 1000);
			}
			else
			{
				m_TCirleFishGroup[0].bHave = false;
			}
			return true;
		}
	case G_iBuildCircleFishTraceTimer1:	//环形鱼群1
		{
			OutputDebugString("htby2::计时器G_iBuildCircleFishTraceTimer11");
			m_TCirleFishGroup[1].iIndex++;
			KillTimer(G_iBuildCircleFishTraceTimer1);
			BuildFishCircleTrace1(m_TCirleFishGroup[1].iCount,m_TCirleFishGroup[1].iFishKind,m_TCirleFishGroup[1].iFishKind,((m_TCirleFishGroup[1].iIndex%3+1) == m_TCirleFishGroup[1].iRedIndex));
			if (m_TCirleFishGroup[1].iIndex <=2)
			{
				SetTimer(G_iBuildCircleFishTraceTimer1, 2 * 1000);
			}
			else
			{
				m_TCirleFishGroup[1].bHave = false;
			}
			return true;
		}
	case G_iSwitchSceneTimer:		//鱼阵
		{
 			OnTimerSwitchScene();
			return true;
		}
	case G_iSceneEndTimer:
		{
			OnTimerSceneEnd();
			return true;
		}
	
	case G_iLockTimer:		//打死全屏炸弹后多少秒才开始生成鱼和定屏时长
		{
			OnTimerLockTimeout();
			return true;
		}
	case G_iBulletIonTimer:
	case G_iBulletIonTimer + 1:
	case G_iBulletIonTimer + 2:
	case G_iBulletIonTimer + 3:
	case G_iBulletIonTimer + 4:
	case G_iBulletIonTimer + 5:
	case G_iBulletIonTimer + 6:
	case G_iBulletIonTimer + 7:
		{
			OnTimerBulletIonTimeout(WPARAM(uTimerID - G_iBulletIonTimer));
			return true;
		}
	
	case kLKScoreTimer:
		{
			OnTimerLKScore();
			return true;
		}
	case G_iAutoFireTimer:
		{
			for(auto itr = userLeftTime.begin(); itr != userLeftTime.end(); ++itr)
			{
				if(itr->second >= 10.0f)
					AutoFire(itr->first);
				itr->second += AutoFireTime;
			}
			return true;
		}
	default:
		{
			ASSERT(FALSE);
			break;
		}
	}

	return __super::OnTimer(uTimerID);
}

//创建小鱼轨迹
bool CServerGameDesk::OnTimerBuildSmallFishTrace() 
{
	BuildFishTrace(1 + rand() % 3, FISH_KIND_1, FISH_KIND_10);
	return true;
}

//创建中等鱼轨迹
bool CServerGameDesk::OnTimerBuildMediumFishTrace() 
{
	BuildFishTrace(1 + rand() % 2, FISH_KIND_11, FISH_KIND_16);
	return true;
}

bool CServerGameDesk::OnTimerBuildBigFishTrace()
{
	BuildFishTrace(1 + rand() % 2, FISH_KIND_17, FISH_KIND_22);
	return true;
}

//创建第23号鱼轨迹--水草
bool CServerGameDesk::OnTimerBuildFish23Trace() 
{
	//BuildFishTrace(1, FISH_KIND_23, FISH_KIND_23);
	return true;
}
//创建第24号鱼轨迹--炸弹
bool CServerGameDesk::OnTimerBuildFishBombTrace() 
{
	//BuildFishTrace(1, FISH_KIND_24, FISH_KIND_24);
	return true;
}

//创建第25-26号鱼轨迹 -金蟾
bool CServerGameDesk::OnTimerBuildFishJCTrace() 
{
	//BuildFishTrace(1, FISH_KIND_25, FISH_KIND_26);
	return true;
}

//创建第30-31号鱼轨迹-鱼王
bool CServerGameDesk::OnTimerBuildFishYWTrace() 
{
	OutputDebugString("dwjlog::生成鱼王");
	//BuildFishTrace(1, FISH_KIND_31, FISH_KIND_32);
	return true;
}



//创建李逵轨迹
bool CServerGameDesk::OnTimerBuildFishLKTrace() 
{
	CMD_S_FishTrace fish_trace;
	m_iFishNum += 1;
	DWORD build_tick = GetTickCount();
	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_23;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();

	fish_trace.fish_id = fish_trace_info->fish_id;
	fish_trace.fish_kind = fish_trace_info->fish_kind;
	fish_trace.init_count = 3;
	fish_trace.trace_type = TRACE_BEZIER;
	fish_trace.TraceID = rand_Mersenne(0,50);
	BuildInitTrace(fish_trace.init_pos, fish_trace.init_count, fish_trace.fish_kind, fish_trace.trace_type);

	SendDataToAllPlayers(&fish_trace, sizeof(fish_trace),SUB_S_FISH_TRACE);
	//SendWatchData(m_bMaxPeople,&fish_trace,sizeof(fish_trace),MDM_GM_GAME_NOTIFY,SUB_S_FISH_TRACE,0);

	current_fish_lk_multiple_ = fish_multiple_min[FISH_KIND_23];
	fish_id_LK = fish_trace_info->fish_id;
	SetTimer(kLKScoreTimer, kLKScoreElasped * 1000);

	return true;
}

int CServerGameDesk::GetNewFishID() 
{
	++fish_id_;
	if (fish_id_ <= 0) fish_id_ = 1;
	return fish_id_;
}









bool CServerGameDesk::OnTimerClearTrace() 
{
	ClearFishTrace();
	return true;
}

bool CServerGameDesk::OnTimerBulletIonTimeout(WPARAM bind_param) 
{
	WORD chair_id = static_cast<WORD>(bind_param);
	CMD_S_BulletIonTimeout bullet_timeout;
	bullet_timeout.chair_id = chair_id;

	SendDataToAllPlayers(&bullet_timeout, sizeof(bullet_timeout),SUB_S_BULLET_ION_TIMEOUT);
	//SendWatchData(m_bMaxPeople,&bullet_timeout,sizeof(bullet_timeout),MDM_GM_GAME_NOTIFY,SUB_S_BULLET_ION_TIMEOUT,0);

	return true;
}

bool CServerGameDesk::OnTimerLockTimeout() 
{
	SendDataToAllPlayers(NULL, 0,SUB_S_LOCK_TIMEOUT);
	//SendWatchData(m_bMaxPeople,NULL, 0,MDM_GM_GAME_NOTIFY,SUB_S_LOCK_TIMEOUT,0);

	StartAllGameTimer();

	KillTimer(G_iLockTimer);
	return true;
}

//计时器_转换场景
bool CServerGameDesk::OnTimerSwitchScene()
{
	KillAllTimer();
	ClearFishTrace(true);
	special_scene_ = true;
	SetTimer(G_iSceneEndTimer, m_iSceneEndTime * 1000);

	//Scene_Style_Kind_ =  static_cast<SceneStyle>((Scene_Style_Kind_+1)%SCENE_STYLE_COUNT);
	Scene_Style_Kind_ =  static_cast<SceneStyle>(SCENE_KIND_1);
	
	if (next_scene_kind_ == SCENE_KIND_1) 
	{
		BuildSceneKind1();
	} 
	else if (next_scene_kind_ == SCENE_KIND_2) 
	{
		BuildSceneKind2();
	} 
	else if (next_scene_kind_ == SCENE_KIND_3) 
	{
		//BuildSceneKind3();
	} 
	else if (next_scene_kind_ == SCENE_KIND_4) 
	{
		//BuildSceneKind4();
	} 
	else if (next_scene_kind_ == SCENE_KIND_5) 
	{
		//BuildSceneKind5();
	}

	//next_scene_kind_ = static_cast<SceneKind>((next_scene_kind_ + 1) % SCENE_KIND_COUNT);
	return true;
}

bool CServerGameDesk::OnTimerSceneEnd() 
{
	special_scene_ = false;
	StartAllGameTimer();
	KillTimer(G_iSceneEndTimer);
	SendDataToAllPlayers(NULL, 0,SUB_S_SCENE_END);
	//SendWatchData(m_bMaxPeople,NULL, 0,MDM_GM_GAME_NOTIFY,SUB_S_SCENE_END,0);
	return true;
}

bool CServerGameDesk::OnTimerLKScore() 
{
	if (current_fish_lk_multiple_ >= fishLK_max_multiple_) 
	{
		KillTimer(kLKScoreTimer);
		return true;
	}
	CMD_S_HitFishLK hit_fish;
	hit_fish.chair_id = 3;
	hit_fish.fish_id = fish_id_LK;
	hit_fish.fish_mulriple = ++current_fish_lk_multiple_;

	SendDataToAllPlayers(&hit_fish, sizeof(hit_fish),SUB_S_HIT_FISH_LK);
	//SendWatchData(m_bMaxPeople,&hit_fish, sizeof(hit_fish),MDM_GM_GAME_NOTIFY,SUB_S_HIT_FISH_LK,0);

	if (current_fish_lk_multiple_ == fishLK_max_multiple_) 
	{
		KillTimer(kLKScoreTimer);
	}
	return true;
}


//清除所的计时器
void CServerGameDesk::KillAllTimer()
{
	KillTimer(G_iBuildSmallFishTraceTimer);
	KillTimer(G_iBuildMediumFishTraceTimer);
	KillTimer(G_iBuildBigFishTraceTimer);
	KillTimer(G_iBuildFish23TraceTimer);
	KillTimer(G_iBuildFish24TraceTimer);
	KillTimer(G_iBuildFishJCTraceTimer);
	KillTimer(G_iBuildFishYWTraceTimer);
	KillTimer(G_iBuildFishDSYTraceTimer);
	KillTimer(G_iBuildFishDSXTraceTimer);

	KillTimer(G_iBuildLineFishTraceTimer);

	KillTimer(G_iBuildCircleFishTraceTimer0);
	KillTimer(G_iBuildCircleFishTraceTimer1);

	KillTimer(G_iLockTimer);
	m_TCirleFishGroup[0].bHave = false;
	m_TCirleFishGroup[1].bHave = false;

}

//重置游戏状态
bool CServerGameDesk::ReSetGameState(BYTE bLastStation)
{
	KillAllTimer();
	KillTimer(G_iBulletIonTimer);
	KillTimer(G_iLockTimer);
	KillTimer(G_iClearTraceTimer);
	
	KillTimer(G_iSwitchSceneTimer);
	KillTimer(G_iSceneEndTimer);
	KillTimer(kLKScoreTimer);
	KillTimer(G_iGameFinishTimer);
	KillTimer(G_iAutoFireTimer);
	KillTimer(G_iCheckFinishTimer);

	m_bIsplaying = false;
	SetGameStatus(GAME_STATUS_FREE);

	m_iFishNum = 0;
//	memset(m_i64fish_score_,0,sizeof(m_i64fish_score_));
	memset(exchange_fish_score_,0,sizeof(exchange_fish_score_));
	
	for (int i = 0; i < FISH_KIND_COUNT; ++i)
	{
		FreeSweepFish(i);
	}

	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		FreeAllBulletInfo(i);
	}

	userShoot.clear();
	userPoint.clear();
	userAutoFireId.clear();

	for(auto itr = userLeftTime.begin(); itr != userLeftTime.end(); ++itr)
	{
		itr->second = 0.0f;
	}

	return TRUE;
}

//游戏开始
bool CServerGameDesk::GameBegin(BYTE bBeginFlag)
{
	if (__super::GameBegin(bBeginFlag)==false) 
		return false;

	SetGameStatus(GAME_STATUS_PLAY);
	StartAllGameTimer();
	//鱼阵计时器
	SetTimer(G_iSwitchSceneTimer, m_iSwitchSceneTime * 1000);

	if(m_bIsBuy) //房卡场
	{
		if(m_iRunGameCount == 0)	//创建房间第一局开始时重置大结算数据
		{
			m_CalculateBoard.clear();
		}

		//记录游戏开始时间
		m_lStartTime = time(NULL);

		userPoint.clear();
		userShoot.clear();

		int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;

		for(int i = 0; i < playercount; ++i)
		{
			userPoint[i] = m_pUserInfo[i]->m_UserData.iLotteries;; //初始积分
			userShoot[i] = m_iShootInit;//初始炮数
		}

		for(int i = 0; i < playercount; ++i)
		{
			//清除玩家离线、断线状态
			if(userLeftTime.find(i) != userLeftTime.end())
			{
				userLeftTime.erase(userLeftTime.find(i));
			}
		}

		SetTimer(G_iGameFinishTimer, m_iGameTime * 60 * 1000);						//结束游戏
		SetTimer(G_iAutoFireTimer, AutoFireTime * 1000);						//自动开火计时器

		S_C_GAME_BEGIN msg;
		for(int i =0;i < playercount;i++)
		{
			msg.PointsInit[i] = m_pUserInfo[i]->m_UserData.iLotteries;;
		}
		msg.ShootInit = m_iShootInit;

		SendDataToAllPlayers(&msg,sizeof(msg),SUB_S_GAME_BEGIN);
	}

	return TRUE;
}

//发给所有人
void CServerGameDesk::SendDataToAllPlayers(void * pData, UINT uSize, BYTE bAssID)
{
	for (int i=0;i < PLAY_COUNT;i++) 
	{
		if (m_pUserInfo[i] == NULL)
		{
			continue;
		}
	
		SendGameData(i,pData,uSize,MDM_GM_GAME_NOTIFY,bAssID,0);
	}
}
//发给指定玩家
void CServerGameDesk::SendDataToPlayer(BYTE bDeskStation, void * pData, UINT uSize, BYTE bAssID)
{
	SendGameData(bDeskStation,pData,uSize,MDM_GM_GAME_NOTIFY,bAssID,0);
}

//游戏结束
bool CServerGameDesk::GameFinish(BYTE bDeskStation, BYTE bCloseFlag)
{
	switch(bCloseFlag)
	{
	case GFF_SAFE_FINISH:	//一局游戏结束 
		{	
			SCORE temp_point[PLAY_COUNT];
			SCORE iChangeMoney[PLAY_COUNT];
			bool temp_cut[PLAY_COUNT];
			memset(temp_point,0,sizeof(temp_point));
			memset(iChangeMoney,0,sizeof(iChangeMoney));
			memset(temp_cut,false,sizeof(temp_cut));
			int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;

			//小结算
			S_C_GameFinish msg;

			if(m_iPlayingMethod & (1 << 1))		//1V1
			{
				for(int i = 0; i < playercount; ++i)
				{
					temp_point[i] = (userPoint[i]/* - m_iPointsInit*/) * m_DeskBasePoint;
				}

				int baodi = (temp_point[0] + temp_point[1]) / 2;
				temp_point[0] = temp_point[0] - baodi;
				temp_point[1] = -temp_point[0];

				msg.isWin[0] = temp_point[0] > temp_point[1] ? WIN : (temp_point[0] == temp_point[1] ? DEUCE : LOSE);
				msg.isWin[1] = temp_point[1] > temp_point[0] ? WIN : (temp_point[1] == temp_point[0] ? DEUCE : LOSE);
			}
			else if(m_iPlayingMethod & (1 << 2)) //2V2
			{
				int teamA_Point = 0;
				int teamB_Point = 0;

				for(int i = 0; i < playercount; ++i)
				{
					temp_point[i] = (userPoint[i] /*- m_iPointsInit*/) * m_DeskBasePoint;

					if(userTeam[i] == TEAM_A)
					{
						teamA_Point += temp_point[i];
					}
					else if(userTeam[i] == TEAM_B)
					{
						teamB_Point += temp_point[i];
					}
				}

				int baodi = (teamA_Point + teamB_Point) / 2;

				for(int i = 0; i < playercount; ++i)
				{
					if(userTeam[i] == TEAM_A)
					{
						temp_point[i] = teamA_Point - baodi;
						msg.isWin[i] = teamA_Point > teamB_Point ? WIN : (teamA_Point == teamB_Point ? DEUCE : LOSE);
					}
					else if(userTeam[i] == TEAM_B)
					{
						temp_point[i] = -(teamA_Point - baodi);
						msg.isWin[i] = teamB_Point > teamA_Point ? WIN : (teamB_Point == teamA_Point ? DEUCE : LOSE);
					}
				}
			}
			else if(m_iPlayingMethod & (1 << 3)) //4人混战
			{
				int totalPoint = 0;
				for(auto itr = userPoint.begin(); itr != userPoint.end(); ++itr)
				{
					totalPoint += itr->second;
				}

				int averagePoint = totalPoint / playercount;
				for(int i = 0; i < playercount; ++i)
				{
					temp_point[i] = userPoint[i] - averagePoint;
					msg.isWin[i] = temp_point[i] > 0 ? WIN : (0 == temp_point[i] ? DEUCE : LOSE);
				}
			}

			int ratio = m_iPlayingMethod & OneRatioTen ? 10 : 1;
			for(int i = 0; i < playercount; ++i)
			{
				temp_point[i] *= ratio;
			}

			ChangeUserPointint64(temp_point,temp_cut);
			__super::RecoderGameInfo(iChangeMoney);

			memcpy(msg.iuserPoint,temp_point,sizeof(msg.iuserPoint));
			SendDataToAllPlayers(&msg,sizeof(msg),S_C_GAME_FINISH_SIG);

			//记录大结算数据
			for(int i = 0; i < PLAY_COUNT; ++i)
			{
				m_CalculateBoard.iuserPoint[i] += temp_point[i];
			}

			if(m_bIsBuy && m_iRunGameCount >= m_iVipGameCount)
			{
				//大结算
				SendScoreBoard();	
			}
			else
			{
				SendReaminInfo(true);
			}

			//记录下奖池
			RecoderStock();
			break;
		}
	case GFF_DISSMISS_FINISH:
		{
			DissMissRoomBefore();
			break;
		}
	}

	//重置数据
	ReSetGameState(bCloseFlag);
	__super::GameFinish(bDeskStation,bCloseFlag);
	return true;
}

////////////////////////////////////////////////////////////////
/////////////////////游戏辅助相关///////////////////////////////
////////////////////////////////////////////////////////////////

//判断是否正在游戏
bool CServerGameDesk::IsPlayGame(BYTE bDeskStation)
{
	if(m_bIsBuy && (m_bGameStation == GAME_STATUS_PLAY || m_iRunGameCount > 0))
	{
		return true;
	}
	return false;
	//__super::IsPlayGame(bDeskStation);
}

/*------------------------------------------------------------------------------*/
///判断此游戏桌是否开始游戏 (因为捕鱼比较特殊 玩家离开都返回false 让玩家离开桌子 而不是断线)
BOOL	CServerGameDesk::IsPlayingByGameStation()
{
	return FALSE;
}


//用户离开游戏桌
BYTE CServerGameDesk::UserLeftDesk(BYTE bDeskStation, CGameUserInfo * pUserInfo)
{
	//房卡场2V2游戏未开始，玩家选择阵营后离开
	if(m_iPlayingMethod & (1<<2) && !IsPlayGame(bDeskStation) && m_iRunGameCount == 0)
	{
		auto itr = userTeam.find(bDeskStation);
		if(itr != userTeam.end())	
		{
			int oriteam = userTeam[bDeskStation];
			userTeam.erase(itr);
			--teamNum[oriteam];
		}
	}

	//记录玩家离线、断线状态
	if(m_bGameStation == GAME_STATUS_PLAY && userLeftTime.find(bDeskStation) == userLeftTime.end())
	{
		userLeftTime[bDeskStation] = 0.0f;
	}

	m_bExitWatcher[bDeskStation] = false;

	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return __super::UserLeftDesk(bDeskStation,pUserInfo);;
	}
	int inum = 0;//玩家人数
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if (NULL == m_pUserInfo[i] || bDeskStation == i)//排除自己
			continue;
		++inum;
	}
	//金币场玩家玩了游戏才结算
	if (m_bUserHavePlay[bDeskStation] && !m_bIsBuy)
	{
		CalcScore(bDeskStation);//算分
	}
 	
 	DeleteNullUser(m_pUserInfo[bDeskStation]->m_UserData.dwUserID);
	if (inum == 0)//房间已经无人
	{
		if(m_bIsBuy)
			ReSetGameState(bDeskStation);
		else
			GameFinish(0,GF_SALE);;
	}
// 	//数据清零
	FreeAllBulletInfo(bDeskStation);
	userPoint[bDeskStation]		= 0;
	exchange_fish_score_[bDeskStation]	= 0;
	m_bUserHavePlay[bDeskStation]		= false;
	m_iUserFireBase[bDeskStation]		= min_bullet_multiple_;

	if (ExamSuperUser(bDeskStation))
	{
		try
		{
			SaveSetting();
		}
		catch (...)
		{

		}
	}
	return __super::UserLeftDesk(bDeskStation,pUserInfo);
}

//提前结束
BOOL CServerGameDesk::UserStop(BYTE bDeskStation, BOOL bArgee)
{
	//SendGameData((bDeskStation+2)%4,MDM_GM_GAME_NOTIFY,ASS_STOP_THING,0);
	return true;
}

bool CServerGameDesk::UserNetCut(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	//断线也做离开处理 
	//UserLeftDesk(bDeskStation,pLostUserInfo);

	//记录玩家离线、断线状态
	if(m_bGameStation == GAME_STATUS_PLAY && userLeftTime.find(bDeskStation) == userLeftTime.end())
	{
		userLeftTime[bDeskStation] = 0.0f;
	}
	__super::UserNetCut(bDeskStation,pLostUserInfo);
	return true;
}
bool CServerGameDesk::UserReCome(BYTE bDeskStation, CGameUserInfo *pLostUserInfo)
{
	if(!m_bIsBuy)
		m_bUserHavePlay[bDeskStation] = false;
	__super::UserReCome(bDeskStation, pLostUserInfo);
	return true ;
}

///用户坐到游戏桌
BYTE CServerGameDesk::UserSitDesk(MSG_GR_S_UserSit * pUserSit, CGameUserInfo * pUserInfo)
{
	return __super::UserSitDesk(pUserSit,pUserInfo);
}

//统计玩游戏玩家
BYTE CServerGameDesk::CountPlayer()
{
	BYTE count=0;
	for(int i=0;i<m_bMaxPeople;i++)
	{
		if(m_pUserInfo[i])
			count++;
	}
	return count;
}

void CServerGameDesk::StartAllGameTimer() 
{
	if(m_bIsBuy && m_bGameStation != GAME_STATUS_PLAY)	//房卡场游戏未开始不产生鱼
	{
		KillAllTimer();
		return;
	}
	
	SetTimer(G_iBuildSmallFishTraceTimer, m_iBuildSmallFishTime * 1000);		//小鱼
	SetTimer(G_iBuildMediumFishTraceTimer, m_iBuildMediumFishTime * 1000);		//中鱼
	SetTimer(G_iBuildBigFishTraceTimer, m_iBuildBigFishTime * 1000);			//大鱼
	SetTimer(G_iBuildFish23TraceTimer, m_iBuild23FishTime * 1000);				//水草
	SetTimer(G_iBuildFish24TraceTimer, m_iBuild24FishTime * 1000);				//炸弹
	SetTimer(G_iBuildFishJCTraceTimer, m_iBuildJCFishTime * 1000);				//金蟾
	SetTimer(G_iBuildFishYWTraceTimer, m_iBuildYWFishTime * 1000);				//鱼王
	SetTimer(G_iBuildFishDSYTraceTimer, m_iBuildDSYFishTime * 1000);			//大三元
	SetTimer(G_iBuildFishDSXTraceTimer, m_iBuildDSXFishTime * 1000);			//大四喜
	
	SetTimer(G_iBuildLineFishTraceTimer,m_iBuildMediumFishTime * 1000);			//线性规则鱼群
	
	
	SetTimer(G_iClearTraceTimer, m_iClearTraceElasped * 1000);
}

void CServerGameDesk::BuildInitTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type)
{
	assert(init_count >= 2 && init_count <= 3);
	srand(GetTickCount() + rand() % kResolutionWidth);
	WORD chair_id = rand() % GAME_PLAYER;
	int center_x = kResolutionWidth / 2;
	int center_y = kResolutionHeight / 2;
	int factor = rand() % 2 == 0 ? 1 : -1;
	switch (chair_id) 
	{
	case 0:
	case 1:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x)+ (rand() % center_x)/2);
		init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
		if (factor>0)
		{
			init_pos[2].x=static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		}
		else
		{
			init_pos[2].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
		}
		init_pos[2].y = static_cast<float>(center_y);
		break;
	case 2:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
		init_pos[2].y = static_cast<float>(kResolutionHeight + fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 3:
		init_pos[0].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[1].x = static_cast<float>(center_x - (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[2].x = -static_cast<float>(fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
		break;
	case 5:
	case 6:
	case 4:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = kResolutionHeight + static_cast<float>(fish_bounding_box_height_[fish_kind] * 2);
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y - (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
		init_pos[2].y = static_cast<float>(-fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 7:
		init_pos[0].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[1].x = static_cast<float>(center_x + (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[2].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
		break;
	}

	if (trace_type == TRACE_LINEAR && init_count == 2) 
	{
		init_pos[1].x = init_pos[2].x;
		init_pos[1].y = init_pos[2].y;
	}
}
//创建N条鱼同一条轨迹
void CServerGameDesk::BuildInitLineTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,DWORD &dwChar_id)
{
	assert(init_count >= 2 && init_count <= 3);
	srand(GetTickCount() + rand() % kResolutionWidth);
	dwChar_id = rand() % GAME_PLAYER;
	int center_x = kResolutionWidth / 2;
	int center_y = kResolutionHeight / 2;
	int factor = rand() % 2 == 0 ? 1 : -1;
	switch (dwChar_id) 
	{
	case 0:
	case 1:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x)+ (rand() % center_x)/2);
		init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
		if (factor>0)
		{
			init_pos[2].x=static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		}
		else
		{
			init_pos[2].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
		}
		init_pos[2].y = static_cast<float>(center_y);
		break;
	case 2:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = 0.f - static_cast<float>(fish_bounding_box_height_[fish_kind]) * 2;
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
		init_pos[2].y = static_cast<float>(kResolutionHeight + fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 3:
		init_pos[0].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[1].x = static_cast<float>(center_x - (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[2].x = -static_cast<float>(fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
		break;
	case 5:
	case 6:
	case 4:
		init_pos[0].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[0].y = kResolutionHeight + static_cast<float>(fish_bounding_box_height_[fish_kind] * 2);
		init_pos[1].x = static_cast<float>(center_x + factor * (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y - (rand() % center_y));
		init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
		init_pos[2].y = static_cast<float>(-fish_bounding_box_height_[fish_kind] * 2);
		break;
	case 7:
		init_pos[0].x = static_cast<float>(-fish_bounding_box_width_[fish_kind] * 2);
		init_pos[0].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[1].x = static_cast<float>(center_x + (rand() % center_x));
		init_pos[1].y = static_cast<float>(center_y + factor* (rand() % center_y));
		init_pos[2].x = static_cast<float>(kResolutionWidth + fish_bounding_box_width_[fish_kind] * 2);
		init_pos[2].y = static_cast<float>(center_y - factor* (rand() % center_y));
		break;
	}

	if (trace_type == TRACE_LINEAR && init_count == 2) 
	{
		init_pos[1].x = init_pos[2].x;
		init_pos[1].y = init_pos[2].y;
	}
}

//创建N条鱼的圈轨迹
void CServerGameDesk::BuildInitCircleTrace(FPoint init_pos[5], int init_count, FishKind fish_kind, TraceType trace_type,int fish_count,int iIndex)
{
	assert(init_count >= 2 && init_count <= 3);
	srand(GetTickCount() + rand() % kResolutionWidth);

	int center_x = kResolutionWidth / 2;
	int center_y = kResolutionHeight / 2;
	int factor = rand() % 2 == 0 ? 1 : -1;

	float fAngle = 360.0f/((float)fish_count);


	init_pos[0].x = static_cast<float>(center_x);
	init_pos[0].y = static_cast<float>(center_y);
	init_pos[1].x = static_cast<float>(center_x + sin((float)(iIndex*fAngle))*100);
	init_pos[1].y = static_cast<float>(center_y + cos((float)(iIndex*fAngle))*100);
	init_pos[2].x = static_cast<float>(center_x + sin((float)(iIndex*fAngle))*1440);
	init_pos[2].y = static_cast<float>(center_y + cos((float)(iIndex*fAngle))*900);
	//init_pos[2].x = static_cast<float>(center_x - factor * (rand() % center_x));
	//init_pos[2].y = static_cast<float>(kResolutionHeight + fish_bounding_box_height_[fish_kind] * 2);
	

	if (trace_type == TRACE_LINEAR && init_count == 2) 
	{
		init_pos[1].x = init_pos[2].x;
		init_pos[1].y = init_pos[2].y;
	}
}

void CServerGameDesk::BuildFishTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,FishKind fish_kind_start_ex, FishKind fish_kind_end_ex) 
{

	if (fish_kind_start > FISH_KIND_20)		//配合U3D，只要20种鱼
	{
		return ; 
	}

	
	m_iFishNum += fish_count;

	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	WORD send_size = 0;
	CMD_S_FishTrace* fish_trace = reinterpret_cast<CMD_S_FishTrace*>(tcp_buffer);

	DWORD build_tick = GetTickCount();
	srand(build_tick + fish_count * 123321);

	for (int i = 0; i < fish_count; ++i) 
	{
		if (send_size + sizeof(CMD_S_FishTrace) > sizeof(tcp_buffer)) 
		{
			SendDataToAllPlayers(tcp_buffer, send_size,SUB_S_FISH_TRACE);
			//SendWatchData(m_bMaxPeople,tcp_buffer, send_size,MDM_GM_GAME_NOTIFY,SUB_S_FISH_TRACE,0);

			send_size = 0;
		}

		FishTraceInfo* fish_trace_info	= ActiveFishTrace();
		fish_trace_info->fish_kind		= static_cast<FishKind>(fish_kind_start + (rand() + i) % (fish_kind_end - fish_kind_start + 1));

				
		fish_trace_info->build_tick		= build_tick;
		fish_trace_info->fish_id		= GetNewFishID();


		fish_trace->fish_id = fish_trace_info->fish_id;
		fish_trace->fish_kind_ex[0] = static_cast<FishKind>(fish_kind_start_ex + (rand() + i) % (fish_kind_end_ex - fish_kind_start_ex + 1));
		fish_trace->fish_kind_ex[1] = fish_trace->fish_kind_ex[0];
		fish_trace->fish_kind_ex[2] = fish_trace->fish_kind_ex[1];
		fish_trace->fish_kind_ex[3] = fish_trace->fish_kind_ex[1];
		fish_trace->fish_kind_ex[4] = fish_trace->fish_kind_ex[1];

		fish_trace_info->fish_kind_ex_	= fish_trace_info->fish_kind;
		

		fish_trace->fish_kind = fish_trace_info->fish_kind;
		if (fish_trace->fish_kind_ex[0] == FISH_KIND_1 || fish_trace->fish_kind_ex[0] == FISH_KIND_2) 
		{
			fish_trace->init_count = 2;
			fish_trace->trace_type = TRACE_LINEAR;
		} 
		else 
		{
			fish_trace->init_count = 3;
			fish_trace->trace_type = TRACE_BEZIER;
		}

		fish_trace->TraceID = rand_Mersenne(0,50);
	
		BuildInitTrace(fish_trace->init_pos, fish_trace->init_count, fish_trace->fish_kind, fish_trace->trace_type);
		send_size += sizeof(CMD_S_FishTrace);
		++fish_trace;
	}
	
	if (send_size > 0) 
	{
		
		SendDataToAllPlayers(tcp_buffer, send_size,SUB_S_FISH_TRACE);
	}
}

//创建N条鱼的直线鱼轨迹
void CServerGameDesk::BuildFishLineTrace(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end)
{
	if (fish_kind_start > fish_kind_end)
	{
		return ;
	}
	m_iFishNum += fish_count;

	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	WORD send_size = 0;
	CMD_S_FishTrace* fish_trace = reinterpret_cast<CMD_S_FishTrace*>(tcp_buffer);

	
	DWORD build_tick = GetTickCount();
	srand(build_tick + fish_count * 123321);


	FPoint init_pos[5];
	int init_count;
	DWORD dwChar_id;
	BYTE itemp = rand_Mersenne(0,50);//线性鱼轨迹

	FishKind Fish_Tmp_Kind = static_cast<FishKind>(fish_kind_start + (rand()) % (fish_kind_end - fish_kind_start + 1));

	for (int i = 0; i < fish_count; ++i) 
	{
		if (send_size + sizeof(CMD_S_FishTrace) > sizeof(tcp_buffer)) 
		{
			SendDataToAllPlayers(tcp_buffer, send_size,SUB_S_FISH_TRACE);

			send_size = 0;
		}

		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = Fish_Tmp_Kind;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		fish_trace->fish_id = fish_trace_info->fish_id;
		fish_trace->fish_kind_ex[0] = Fish_Tmp_Kind;
		fish_trace->fish_kind_ex[1] = Fish_Tmp_Kind;
		fish_trace->fish_kind_ex[2] = fish_trace->fish_kind_ex[1];
		fish_trace->fish_kind_ex[3] = fish_trace->fish_kind_ex[1];
		fish_trace->fish_kind_ex[4] = fish_trace->fish_kind_ex[1];
		fish_trace->fish_kind = fish_trace_info->fish_kind;

		fish_trace->SceneFish = 1;	//直线鱼阵
		fish_trace->TraceID = itemp;

		fish_trace_info->fish_kind_ex_	= fish_trace_info->fish_kind;


		if (fish_trace_info->fish_kind == FISH_KIND_1 || fish_trace_info->fish_kind == FISH_KIND_2) 
		{
			fish_trace->init_count = 2;
			fish_trace->trace_type = TRACE_LINEAR;
		} 
		else 
		{
			fish_trace->init_count = 3;
			fish_trace->trace_type = TRACE_BEZIER;
		}
		if (i == 0)
		{
			BuildInitLineTrace(fish_trace->init_pos, fish_trace->init_count, fish_trace->fish_kind, fish_trace->trace_type,dwChar_id);
			memcpy(&init_pos,fish_trace->init_pos,sizeof(init_pos));

			init_count = fish_trace->init_count;
		}
		else
		{
			memcpy(fish_trace->init_pos,init_pos,sizeof(fish_trace->init_pos));
			switch(dwChar_id)
			{

			case 0:
			case 1:
			case 2:
				fish_trace->init_pos[0].y -= 100*i;
				break;
			case 3:
				fish_trace->init_pos[0].x += 100*i;
				break;
			case 5:
			case 6:
			case 4:
				fish_trace->init_pos[0].y += 100*i;
				break;
			case 7:
				fish_trace->init_pos[0].x -= 100*i;
				break;

			}

			fish_trace->init_count = init_count;
		}


		send_size += sizeof(CMD_S_FishTrace);
		++fish_trace;
	}

	if (send_size > 0) 
	{

		SendDataToAllPlayers(tcp_buffer, send_size,SUB_S_FISH_TRACE);
	}
}


//创建N条鱼圆形轨迹
void CServerGameDesk::BuildFishCircleTrace0(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed)
{

	CMD_S_GroupFishTrace TSwitchScene;
	CMD_S_GroupFishTrace* switch_scene = &TSwitchScene;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;
	for (int i=0; i<5; i++)
	{
		switch_scene->fish_red_kind[i] = FISH_KIND_1;
	}

	for (int i = 0; i < fish_count; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = static_cast<FishKind>(fish_kind_start + (rand() + i) % (fish_kind_end - fish_kind_start + 1));;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->fish_kind_ex_	= fish_trace_info->fish_kind;

		switch_scene->fish_id[i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[i] = fish_trace_info->fish_kind;

		for (int i=0; i<5; i++)
		{
			switch_scene->fish_red_kind[i] = fish_trace_info->fish_kind_ex_;
		}
	}

	switch_scene->fish_count += fish_count;
	switch_scene->byIndex = 0;
	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_GroupFishTrace),SUB_S_FISH_GROUP_TRACE);

	return;

}



//创建N条鱼圆形轨迹
void CServerGameDesk::BuildFishCircleTrace1(int fish_count, FishKind fish_kind_start, FishKind fish_kind_end,bool bHaveRed)
{

	CMD_S_GroupFishTrace TSwitchScene;
	CMD_S_GroupFishTrace* switch_scene = &TSwitchScene;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;
	for (int i=0; i<5; i++)
	{
		switch_scene->fish_red_kind[i] = FISH_KIND_1;
	}

	
	for (int i = 0; i < fish_count; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = static_cast<FishKind>(fish_kind_start + (rand() + i) % (fish_kind_end - fish_kind_start + 1));;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();
		fish_trace_info->fish_kind_ex_	= fish_trace_info->fish_kind;

		//鱼王
// 		if (bHaveRed && i == m_TCirleFishGroup[1].iRedIndexIndex)
// 		{
// 			fish_trace_info->fish_kind = FISH_KIND_31;
// 		}

		switch_scene->fish_id[i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[i] = fish_trace_info->fish_kind;

		for (int i=0; i<5; i++)
		{
			switch_scene->fish_red_kind[i] = fish_trace_info->fish_kind_ex_;
		}

	}

	switch_scene->fish_count += fish_count;
	switch_scene->byIndex = 1;
	OutputDebugString("htby2::发送鱼群1");
	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_GroupFishTrace),SUB_S_FISH_GROUP_TRACE);

	return;

}




void CServerGameDesk::BuildSceneKind1() 
{

	CMD_S_SwitchScene TSwitchScene;
	CMD_S_SwitchScene* switch_scene = &TSwitchScene;
	switch_scene->scene_kind = next_scene_kind_;
	switch_scene->Scene_Style_=Scene_Style_Kind_;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;

	BYTE iTempFishKind = 0;
	for (int i=FISH_KIND_10; i<=FISH_KIND_15; i++)
	{
		for(int j=0; j<10; j++)
		{
			FishTraceInfo* fish_trace_info = ActiveFishTrace();
			fish_trace_info->build_tick = build_tick;
			fish_trace_info->fish_kind =static_cast<FishKind>(FISH_KIND_10+iTempFishKind);		 
			fish_trace_info->fish_id = GetNewFishID();

			switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
			switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
			switch_scene->fish_count++;
		}
		iTempFishKind++;
	}

	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_SwitchScene),SUB_S_SWITCH_SCENE);
	//SendWatchData(m_bMaxPeople,switch_scene, sizeof(CMD_S_SwitchScene),MDM_GM_GAME_NOTIFY,SUB_S_SWITCH_SCENE,0);
}


void CServerGameDesk::BuildSceneKind2() 
{
	CMD_S_SwitchScene TSwitchScene;
	CMD_S_SwitchScene* switch_scene = &TSwitchScene;
	switch_scene->scene_kind = next_scene_kind_;
	switch_scene->Scene_Style_=Scene_Style_Kind_;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;

	BYTE iTempFishKind = 0;
	for (int i=FISH_KIND_12; i<=FISH_KIND_17; i++)
	{
		for(int j=0; j<10; j++)
		{
			FishTraceInfo* fish_trace_info = ActiveFishTrace();
			fish_trace_info->build_tick = build_tick;
			fish_trace_info->fish_kind =static_cast<FishKind>(FISH_KIND_12+iTempFishKind);		 
			fish_trace_info->fish_id = GetNewFishID();

			switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
			switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
			switch_scene->fish_count++;
		}
		iTempFishKind++;
	}


	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_SwitchScene),SUB_S_SWITCH_SCENE);
	//SendWatchData(m_bMaxPeople,switch_scene, sizeof(CMD_S_SwitchScene),MDM_GM_GAME_NOTIFY,SUB_S_SWITCH_SCENE,0);
}

/*

void CServerGameDesk::BuildSceneKind3() 
{
	//	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene TSwitchScene;
	CMD_S_SwitchScene* switch_scene = &TSwitchScene;
	switch_scene->scene_kind = next_scene_kind_;
	switch_scene->Scene_Style_=Scene_Style_Kind_;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;
	int iRandRed = 0;
	iRandRed = rand()%50;
	for (int i = 0; i < 50; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_1;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[0] = FISH_KIND_1;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 50;
	iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_3;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_3;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[1] = FISH_KIND_3;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;

	iRandRed = rand()%30;
	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_4;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_4;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[2] = FISH_KIND_4;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 30;

	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_16;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();

	switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
	switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
	switch_scene->fish_count += 1;
	iRandRed = rand()%50;
	for (int i = 0; i < 50; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_1;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[3] = FISH_KIND_1;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 50;

	iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_2;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_2;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[4] = FISH_KIND_2;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;

	iRandRed = rand()%30;
	for (int i = 0; i < 30; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_5;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_5;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[5] = FISH_KIND_5;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 30;

	fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_17;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();

	switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
	switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
	switch_scene->fish_count += 1;

	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_SwitchScene),SUB_S_SWITCH_SCENE);
	//SendWatchData(m_bMaxPeople,switch_scene, sizeof(CMD_S_SwitchScene),MDM_GM_GAME_NOTIFY,SUB_S_SWITCH_SCENE,0);
}

void CServerGameDesk::BuildSceneKind4() 
{
	//	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene TSwitchScene;
	CMD_S_SwitchScene* switch_scene = &TSwitchScene;
	switch_scene->scene_kind = next_scene_kind_;
	switch_scene->Scene_Style_=Scene_Style_Kind_;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_11;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_12;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_13;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_14;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_15;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_16;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_17;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;
	for (int i = 0; i < 8; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_18;
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 8;

	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_SwitchScene),SUB_S_SWITCH_SCENE);
	//SendWatchData(m_bMaxPeople,switch_scene, sizeof(CMD_S_SwitchScene),MDM_GM_GAME_NOTIFY,SUB_S_SWITCH_SCENE,0);
}

void CServerGameDesk::BuildSceneKind5() 
{
	//	BYTE tcp_buffer[SOCKET_TCP_PACKET] = { 0 };
	CMD_S_SwitchScene TSwitchScene;
	CMD_S_SwitchScene* switch_scene = &TSwitchScene;
	switch_scene->scene_kind = next_scene_kind_;
	switch_scene->Scene_Style_=Scene_Style_Kind_;
	DWORD build_tick = GetTickCount();
	switch_scene->fish_count = 0;
	int iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_1;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_1;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[0] = FISH_KIND_1;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;

	iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_2;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_2;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[1] = FISH_KIND_2;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;

	iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_5;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_5;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[2] = FISH_KIND_5;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;
	iRandRed = rand()%40;
	for (int i = 0; i < 40; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_3;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_3;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[3] = FISH_KIND_3;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 40;
	iRandRed = rand()%24;
	for (int i = 0; i < 24; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_4;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_4;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[4] = FISH_KIND_4;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 24;
	iRandRed = rand()%24;
	for (int i = 0; i < 24; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_6;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_6;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[5] = FISH_KIND_6;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 24;
	iRandRed = rand()%13;
	for (int i = 0; i < 13; ++i)
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_7;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_7;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[6] = FISH_KIND_7;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 13;
	iRandRed = rand()%13;
	for (int i = 0; i < 13; ++i) 
	{
		FishTraceInfo* fish_trace_info = ActiveFishTrace();
		fish_trace_info->fish_kind = FISH_KIND_6;
		fish_trace_info->fish_kind_ex_ = FISH_KIND_6;
		if (i == iRandRed)
		{
			fish_trace_info->fish_kind = FISH_KIND_31;
			switch_scene->fish_kindEx[7] = FISH_KIND_6;
		}
		fish_trace_info->build_tick = build_tick;
		fish_trace_info->fish_id = GetNewFishID();

		switch_scene->fish_id[switch_scene->fish_count + i] = fish_trace_info->fish_id;
		switch_scene->fish_kind[switch_scene->fish_count + i] = fish_trace_info->fish_kind;
	}
	switch_scene->fish_count += 13;

	FishTraceInfo* fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_20;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
	switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
	switch_scene->fish_count += 1;

	fish_trace_info = ActiveFishTrace();
	fish_trace_info->fish_kind = FISH_KIND_20;
	fish_trace_info->build_tick = build_tick;
	fish_trace_info->fish_id = GetNewFishID();
	switch_scene->fish_id[switch_scene->fish_count] = fish_trace_info->fish_id;
	switch_scene->fish_kind[switch_scene->fish_count] = fish_trace_info->fish_kind;
	switch_scene->fish_count += 1;

	SendDataToAllPlayers(switch_scene, sizeof(CMD_S_SwitchScene),SUB_S_SWITCH_SCENE);
	//SendWatchData(m_bMaxPeople,switch_scene, sizeof(CMD_S_SwitchScene),MDM_GM_GAME_NOTIFY,SUB_S_SWITCH_SCENE,0);
}

*/

void CServerGameDesk::ClearFishTrace(bool force) 
{
	if (force) 
	{
		std::copy(active_fish_trace_vector_.begin(), active_fish_trace_vector_.end(), std::back_inserter(storage_fish_trace_vector_));
		active_fish_trace_vector_.clear();
	} 
	else 
	{
		FishTraceInfoVecor::iterator iter;
		FishTraceInfo* fish_trace_info = NULL;
		DWORD now_tick = GetTickCount();
		for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end();) 
		{
			fish_trace_info = *iter;
			if (now_tick >= (fish_trace_info->build_tick + G_iFishAliveTime)) 
			{
				ClearLockFish(fish_trace_info->fish_id);
				iter = active_fish_trace_vector_.erase(iter);
				storage_fish_trace_vector_.push_back(fish_trace_info);
			} 
			else 
			{
				++iter;
			}
		}
	}
}

void CServerGameDesk::CalcScore(BYTE bDeskStation) 
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return;
	}
	//给离开的玩家结算
	//写入数据库				
	__int64 temp_point[PLAY_COUNT];
	__int64 iChangeMoney[PLAY_COUNT];
	bool temp_cut[PLAY_COUNT];
	memset(temp_point,0,sizeof(temp_point));
	memset(iChangeMoney,0,sizeof(iChangeMoney));
	memset(temp_cut,0,sizeof(temp_cut));

	//temp_point[bDeskStation] = (m_i64fish_score_[bDeskStation] - exchange_fish_score_[bDeskStation]) * exchange_ratio_userscore_ / exchange_ratio_fishscore_;

	//temp_point[bDeskStation] = (userPoint[bDeskStation] - m_pUserInfo[bDeskStation] ->m_UserData.i64Money);
	temp_point[bDeskStation] = (userPoint[bDeskStation] - m_pUserInfo[bDeskStation] ->m_UserData.iLotteries);
	//LOGMODULEINSTANCE.WriteLog("change user point, userPoint[%d] = %d, UserMoney[%d] = %d\n", bDeskStation, userPoint[bDeskStation],bDeskStation,m_pUserInfo[bDeskStation] ->m_UserData.i64Money);
	ChangeUserPointint64(temp_point,temp_cut,bDeskStation,1);
	__super::RecoderGameInfo(iChangeMoney);

	//LOGMODULEINSTANCE.WriteLog("RecoderGameInfo, iChangeMoney[%d] = %d\n", bDeskStation, iChangeMoney[bDeskStation]);

	//记录下奖池
	RecoderStock();
	//数据清零
	userPoint[bDeskStation] = 0;
	exchange_fish_score_[bDeskStation] = 0;
	m_bUserHavePlay[bDeskStation] = false;
}

void	CServerGameDesk::RecoderStock()
{
	CString s = CINIFile::GetAppPath ();/////本地路径
	CString nid;
	nid.Format("%d",NAME_ID);
	CINIFile f( s +nid +"_s.ini");
	CString key = "game";
	CString sTemp;
	sTemp.Format("%I64d",g_stock_score_);
	f.SetKeyValString(key,"StockScore ",sTemp);
}


//获取游戏状态信息
bool CServerGameDesk::OnGetGameStation(BYTE bDeskStation, UINT uSocketID, bool bWatchUser)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}

	InitGameRule();

	int curplayercount = 0;
	int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;
	for(int i = 0; i < playercount; ++i)
	{
		if(m_pUserInfo[i] != NULL)
			++curplayercount;
	}

	if(m_bIsBuy && !IsPlayGame(bDeskStation) && curplayercount == 1)
	{
		userTeam.clear();
		teamNum.clear();
	}
	else if(!m_bIsBuy && !m_bIsplaying)
	{
		m_bIsplaying = true;
		GameBegin(ALL_ARGEE);
	}

	//清除玩家离线、断线状态
	if(userLeftTime.find(bDeskStation) != userLeftTime.end())
	{
		userLeftTime.erase(userLeftTime.find(bDeskStation));
	}
	
	GameStation cmd;
	cmd.game_status = GetGameStatus();

	//不允许旁观
	if (bWatchUser)
	{
		return false;
		m_bExitWatcher[bDeskStation] = true;
	}

	m_bUserHavePlay[bDeskStation] = false;
	
	if (!OnEventSendGameScene(bDeskStation, cmd.game_status,cmd))
	{
		return true;
	}


	//给自己发送断线消息
	SendGameStation(bDeskStation,uSocketID,bWatchUser,&cmd,sizeof(GameStation));

	//给其他人发送玩家进入消息
	CMD_S_GameUserCome	TGameUserCome;
	if (m_pUserInfo[bDeskStation] != NULL)
	{
		//TGameUserCome.i64fish_score = m_pUserInfo[bDeskStation]->m_UserData.i64Money;
		TGameUserCome.dwDeskStation = bDeskStation;
		TGameUserCome.i64fish_score = cmd.GameStatus.fish_score[bDeskStation];//
		TGameUserCome.iRemainShootCount = cmd.GameStatus.iRemainShootCount[bDeskStation];
		InsertNewUser(m_pUserInfo[bDeskStation]->m_UserData.dwUserID,TGameUserCome.i64fish_score);
	}

	for (int i=0;i < PLAY_COUNT;i++) 
	{
		if (i == bDeskStation)
		{
			continue;
		}
		SendGameData(i,&TGameUserCome,sizeof(TGameUserCome),MDM_GM_GAME_NOTIFY,SUB_S_USER_COME,0);
	}
	
	//开房间2V2模式游戏第一局开没开始才发选择阵营通知
	if(m_bIsBuy && m_iPlayingMethod & (1<<2) && m_iRunGameCount == 0 && m_bGameStation != GAME_STATUS_PLAY/* && PLAY_COUNT == GetCurPlayerCount()*/)
	{
		S_C_CHOICE_TEAM_NTF msg;
		//所有玩家的阵营信息
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			if(userTeam.find(i) != userTeam.end())
				msg.byteamInfo[i] = userTeam[i];
			else
				msg.byteamInfo[i] = TEAM_NULL;
		}

		/*
		for(int i = 0; i < PLAY_COUNT; ++i)
		{
			//发选阵营通知给未选择阵营的玩家
			if(userTeam.find(i) == userTeam.end() || userTeam[i] == TEAM_NULL)
			{
				SendDataToPlayer(i,&msg,sizeof(msg),SUB_S_CHOICE_TEAM_NTF);
			}
		}
		*/

		if(userTeam.find(bDeskStation) == userTeam.end() || userTeam[bDeskStation] == TEAM_NULL)
		{
			SendDataToPlayer(bDeskStation,&msg,sizeof(msg),SUB_S_CHOICE_TEAM_NTF);
		}
	}

	//更新剩余局数及时间
	SendReaminInfo();

	return true;
}

bool CServerGameDesk::OnEventSendGameScene(WORD bDeskStation, BYTE game_status, GameStation& cmd) 
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	if (m_pUserInfo[bDeskStation] != NULL /*&& !m_bIsBuy*/)	//金币场为碎钻
	{
	//	userPoint[bDeskStation] = m_pUserInfo[bDeskStation]->m_UserData.i64Money;
		userPoint[bDeskStation] = m_pUserInfo[bDeskStation]->m_UserData.iLotteries;
	}

	switch (game_status) 
	{
	case GAME_STATUS_FREE:
	case GAME_STATUS_PLAY:
		//配置数据
		SendGameConfig(bDeskStation, cmd);
		SendSuperConfig(bDeskStation);
		//状态数据
		cmd.GameStatus.game_version = GAME_VERSION;
		cmd.GameStatus.Scene_Style_Kind_	=	 static_cast<SceneStyle>(Scene_Style_Kind_%SCENE_STYLE_COUNT);
		memcpy(cmd.GameStatus.iUserFireBase, m_iUserFireBase, sizeof(cmd.GameStatus.iUserFireBase));
//		memcpy(cmd.GameStatus.fish_score, m_i64fish_score_, sizeof(cmd.GameStatus.fish_score));
		for(int i = 0; i < GAME_PLAYER; ++i)
		{
			if(userPoint.find(i) != userPoint.end())
				cmd.GameStatus.fish_score[i] = userPoint[i];//剩余积分（碎钻）
			else
				cmd.GameStatus.fish_score[i] = 0;

			if(userShoot.find(i) != userShoot.end())
				cmd.GameStatus.iRemainShootCount[i] = userShoot[i]; //剩余炮数
			else
				cmd.GameStatus.iRemainShootCount[i] = 0;

			if(userTeam.find(i) != userTeam.end())
				cmd.GameStatus.byteamInfo[i] = userTeam[i];
			else
				cmd.GameStatus.byteamInfo[i] = TEAM_NULL;

			cmd.GameStatus.iPointsInit[i] = m_pUserInfo[bDeskStation]->m_UserData.iLotteries;//初始分数为碎钻
		}

		cmd.GameStatus.iPlayMode = m_iPlayingMethod;
	//	cmd.GameStatus.iPointsInit = m_iPointsInit; 
		cmd.GameStatus.iShootInit = m_iShootInit;

		memcpy(cmd.GameStatus.exchange_fish_score, exchange_fish_score_, sizeof(cmd.GameStatus.exchange_fish_score));
		//游戏消息
		BYTE mess[] = TEXT("键盘↑↓键加减炮，→←键上下分，空格键或鼠标左键发射子弹，F4关闭声音!");
		CopyMemory(cmd.GameMessage,mess,sizeof(BYTE)*MAX_PATH);
		return true;
	}
	return false;
}

void CServerGameDesk::SendSuperConfig(BYTE bDeskStation)
{
	S_C_SUPER_CONFIG TSuperConfig;
	TSuperConfig.bSuperUser = ExamSuperUser(bDeskStation);
	SendGameData(bDeskStation,&TSuperConfig,sizeof(TSuperConfig),MDM_GM_GAME_NOTIFY,S_C_SUPER_CONFIG_SIG,0);
}

bool CServerGameDesk::SendGameConfig(BYTE bDeskStation, GameStation& cmd) 
{
	cmd.GameConfig.exchange_ratio_userscore = exchange_ratio_userscore_;
	cmd.GameConfig.exchange_ratio_fishscore = exchange_ratio_fishscore_;
	cmd.GameConfig.exchange_count = exchange_count_;
	cmd.GameConfig.min_bullet_multiple = min_bullet_multiple_;
	cmd.GameConfig.max_bullet_multiple = max_bullet_multiple_;
	cmd.GameConfig.bomb_range_width = bomb_range_width_;
	cmd.GameConfig.bomb_range_height = bomb_range_height_;
	for (int i = 0; i < FISH_KIND_COUNT; ++i) 
	{
		cmd.GameConfig.fish_multiple_min[i] = fish_multiple_min[i];
		cmd.GameConfig.fish_multiple_max[i] = fish_multiple_max[i];
		cmd.GameConfig.fish_speed[i] = fish_speed_[i];
		cmd.GameConfig.fish_bounding_box_width[i] = fish_bounding_box_width_[i];
		cmd.GameConfig.fish_bounding_box_height[i] = fish_bounding_box_height_[i];
		cmd.GameConfig.fish_hit_radius[i] = fish_hit_radius_[i];
	}

	for (int i = 0; i < BULLET_KIND_COUNT; ++i) 
	{
		cmd.GameConfig.bullet_speed[i] = bullet_speed_[i];
		cmd.GameConfig.net_radius[i] = net_radius_[i];
	}

	return true;
}

FishTraceInfo* CServerGameDesk::GetFishTraceInfo(int fish_id) {
	FishTraceInfoVecor::iterator iter;
	FishTraceInfo* fish_trace_info = NULL;
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ++iter) {
		fish_trace_info = *iter;
		if (fish_trace_info->fish_id == fish_id) return fish_trace_info;
	}

	return NULL;
}

void CServerGameDesk::SaveSweepFish(FishKind fish_kind, FishKind fish_kind_ex_,int fish_id, BulletKind bullet_kind, int bullet_mulriple) 
{
	SweepFishInfo sweep_fish;
	sweep_fish.fish_kind = fish_kind;
	sweep_fish.fish_kind_ex_ = fish_kind_ex_;
	sweep_fish.fish_id = fish_id;
	sweep_fish.bullet_kind = bullet_kind;
	sweep_fish.bullet_mulriple = bullet_mulriple;
	sweep_fish_info_vector_.push_back(sweep_fish);
}

bool CServerGameDesk::FreeSweepFish(int fish_id) {
	std::vector<SweepFishInfo>::iterator iter;
	for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end();) {
		if ((*iter).fish_id == fish_id) {
			ClearLockFish(fish_id_);
			iter = sweep_fish_info_vector_.erase(iter);
			return true;
		}
		else
		{
			++iter;
		}
	}

	assert(!"FreeSweepFish Failed");
	return false;
}

//获得
SweepFishInfo* CServerGameDesk::GetSweepFish(int fish_id) {
	std::vector<SweepFishInfo>::iterator iter;
	for (iter = sweep_fish_info_vector_.begin(); iter != sweep_fish_info_vector_.end(); ++iter) {
		if ((*iter).fish_id == fish_id) {
			return &(*iter);
		}
	}
	return NULL;
}

//清除指定玩家指定子弹信息，并将清除的数据保存到仓库中
bool CServerGameDesk::FreeBulletInfo(WORD chairid, ServerBulletInfo* bullet_info) 
{
	//LOGMODULEINSTANCE.WriteLog("start free bullet_id=%d,chaireid=%d",bullet_info->ibullet_id,chairid);
	if (chairid < 0 || chairid >= PLAY_COUNT)
	{
		return false;
	}
	for (int i=0; i<MAX_BULLET_COUNT; i++)
	{
		if (m_tServerBulletInfo[chairid][i].ibullet_id == bullet_info->ibullet_id)
		{
			ZeroMemory(&m_tServerBulletInfo[chairid][i],sizeof(m_tServerBulletInfo[chairid][i]));
			//LOGMODULEINSTANCE.WriteLog("m_tServerBulletInfo[%d][%d] is free\n",chairid,i);
			break;
		}
	}

	assert(!"FreeBulletInfo Failed");
	return false;
}

//清理指定玩家所有子弹信息
void CServerGameDesk::FreeAllBulletInfo(WORD chairid) 
{
	if (chairid < 0 || chairid >= PLAY_COUNT)
	{
		return ;
	}
	memset(m_tServerBulletInfo[chairid],0,sizeof(m_tServerBulletInfo[chairid]));
}


//清理过时的子弹
void CServerGameDesk::FreeTimeOutBullet()
{
	for(int m=0; m<PLAY_COUNT; m++)
	{
		for (int i=0; i<MAX_BULLET_COUNT; i++)
		{
			if (!m_tServerBulletInfo[m][i].bActive)
			{
				continue;
			}
			if (GetTickCount() - m_tServerBulletInfo[m][i].dwBirthTime >= 10*1000)
			{
				ZeroMemory(&m_tServerBulletInfo[m][i],sizeof(m_tServerBulletInfo[m][i]));
			}
		}
	}
}


//获取子弹信息
ServerBulletInfo* CServerGameDesk::GetBulletInfo(WORD chairid, int bullet_id) 
{
	if (chairid < 0 || chairid >= PLAY_COUNT)
	{
		return NULL;
	}
	ServerBulletInfo* bullet_info = NULL;
	for (int i=0; i<MAX_BULLET_COUNT; i++)
	{
		if (m_tServerBulletInfo[chairid][i].ibullet_id == bullet_id)
		{
			bullet_info = &m_tServerBulletInfo[chairid][i];
			return bullet_info;
		}
	}
	return NULL;
}


void CServerGameDesk::UTF_8ToUnicode(wchar_t* pOut,char *pText)  
{     
	char* uchar = (char *)pOut;   
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);  
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);   
}   

void CServerGameDesk::UnicodeToGB2312(char* pOut,wchar_t uData)  
{   
	WideCharToMultiByte(CP_ACP,NULL,&uData,1,pOut,sizeof(wchar_t),NULL,NULL);   
} 

void CServerGameDesk::UTF_8ToGB2312(char*pOut, char *pText, int pLen)    
{     
	char Ctemp[4];     
	memset(Ctemp,0,4);   
	int i =0 ,j = 0;   
	while(i < pLen)   
	{    
		if(pText[i] >= 0)    
		{    
			pOut[j++] = pText[i++];   
		}   
		else   
		{    
			WCHAR Wtemp;   
			UTF_8ToUnicode(&Wtemp,pText + i);  
			UnicodeToGB2312(Ctemp,Wtemp);   
			pOut[j] = Ctemp[0];  
			pOut[j + 1] = Ctemp[1];    
			i += 3;     
			j += 2;    
		}     
	}
	
	pOut[j] ='\0';
	return;   
}   

bool CServerGameDesk::LoadConfig() 
{
	TCHAR file_name[MAX_PATH] = { 0 };
	_sntprintf(file_name, CountArray(file_name), TEXT("lkpy_config_%s.xml"), SKIN_FOLDER);
	g_AdminiList.clear();
	TiXmlDocument xml_doc;
	if (!xml_doc.LoadFile(CT2A(file_name), TIXML_ENCODING_UTF8)) 
	{
		return false;
	}
	const TiXmlElement* xml_element = xml_doc.FirstChildElement("Config");
	if (xml_element == NULL) 
	{
		return false;
	}
	
	//m_SuperMan.clear();
	m_vSpecialUser.clear();
	
	
	const TiXmlElement* xml_child = NULL;
	int fish_count = 0, bullet_kind_count = 0;
	for (xml_child = xml_element->FirstChildElement(); xml_child; xml_child = xml_child->NextSiblingElement()) 
	{
		if (!strcmp(xml_child->Value(), "Stock")) 
		{
			//库存(不能超过10个):stockScore库存分数,大于些值执行相应概率,从最大的开始判断,小于0将打不中鱼
			for (const TiXmlElement* xml_stock = xml_child->FirstChildElement(); xml_stock; xml_stock = xml_stock->NextSiblingElement()) 
			{
				xml_stock->Attribute("stockScore", &stock_crucial_score_[stock_crucial_count_]);
				xml_stock->Attribute("increaseProbability", &stock_increase_probability_[stock_crucial_count_]);
				++stock_crucial_count_;
				if (stock_crucial_count_ >= 10) 
					break;
			}
		}
		else if (!strcmp(xml_child->Value(), "ScoreExchange")) 
		{
			//金币和渔币的兑换(金币:渔币) exchangeCount:每次兑换数量
			const char* attri = xml_child->Attribute("exchangeRatio");
			char* temp = NULL;
			exchange_ratio_userscore_ = strtol(attri, &temp, 10);
			exchange_ratio_fishscore_ = strtol(temp + 1, &temp, 10);
			xml_child->Attribute("exchangeCount", &exchange_count_);
		} 
		else if (!strcmp(xml_child->Value(), "Cannon")) 
		{
			const char* attri = xml_child->Attribute("cannonMultiple");//大炮倍数
			char* temp = NULL;
			min_bullet_multiple_ = strtol(attri, &temp, 10);
			max_bullet_multiple_ = strtol(temp + 1, &temp, 10);
		}
		else if(!strcmp(xml_child->Value(), "VipRoomInit"))
		{
			const char* attri = xml_child->Attribute("PointsAndShoot");//初始积分及炮数
			char* temp = NULL;
			m_iPointsInit = strtol(attri, &temp, 10);
			m_iShootInit = strtol(temp + 1, &temp, 10);
		}
		else if (!strcmp(xml_child->Value(), "SmallFish"))
		{			
			xml_child->Attribute("catchRate", &m_iSmallFishCatchRate);
			m_iSmallFishKind = FISH_KIND_10;
		}
		else if(!strcmp(xml_child->Value(), "SuperMan")) 
		{
			//UserID:玩家ID
			int iUserID;
			xml_child->Attribute("UserID", &iUserID);
			g_AdminiList.push_back(iUserID);
		}
		else if (!strcmp(xml_child->Value(), "CellScore"))
		{
			xml_child->Attribute("score", &m_iCellScore);
		}
		else if (!strcmp(xml_child->Value(), "Bomb"))//炸弹(局部炸弹,超级炸弹) 当库存小于此值炸弹被击中的几率为0
		{
			int iTemp = 0;
			xml_child->Attribute("KingBomb", &iTemp);
			King_bomb_stock_ = iTemp;
			xml_child->Attribute("SuperBomb", &iTemp);
			super_bomb_stock_ = iTemp;
		} 
		else if (!strcmp(xml_child->Value(), "Fish")) 
		{
			//<!-- kind: 对应 CMD_Fish.h文件的FishKind的枚举值 不能更改 
			// name:只作为描述 speed:鱼的速度 multiple:鱼的倍数 BoundingBox:鱼的边框 hitRadius:击中半径 captureProbability:捕获概率(0-1)  -->
			//<!-- 特殊鱼 鱼王和超级炸弹是圆的 用BoundingBox的第1个参数作为直径 -->
			//<!-- 特殊鱼 局部炸弹的BoundingBox的后2个参数表示炸弹的涉及范围 -->
			//<!-- 特殊鱼 FISH_KIND_18(鲨鱼)FISH_KIND_19(龙)和FISH_KIND_21(李逵)的multiple是随机倍数的2个参数表示最小和最大 -->
			int fish_kind;
			
			xml_child->Attribute("kind", &fish_kind);
			if (fish_kind >= FISH_KIND_COUNT || fish_kind < 0) 
				return false;

			const char* cFishName = xml_child->Attribute("name");
			UTF_8ToGB2312(m_cFishName[fish_kind], (char*)cFishName, strlen(cFishName));

			xml_child->Attribute("speed", &fish_speed_[fish_kind]);

			const char* attri = xml_child->Attribute("multiple");
			char* temp = NULL;
			fish_multiple_min[fish_kind] = strtol(attri, &temp, 10);
			fish_multiple_max[fish_kind] = strtol(temp + 1, &temp, 10);
			if(FISH_KIND_23 == fish_kind)
				fishLK_max_multiple_ = fish_multiple_max[fish_kind];
				
			//xml_child->Attribute("multiple", &fish_multiple_[fish_kind]);

			attri = xml_child->Attribute("BoundingBox");
			temp = NULL;
			fish_bounding_box_width_[fish_kind] = strtol(attri, &temp, 10);
			fish_bounding_box_height_[fish_kind] = strtol(temp + 1, &temp, 10);
			xml_child->Attribute("hitRadius", &fish_hit_radius_[fish_kind]);
			xml_child->Attribute("captureProbability", &fish_capture_probability_[fish_kind]);
			++fish_count;
		} 
		else if (!strcmp(xml_child->Value(), "Bullet")) 
		{
			//kind:对应BulletKind name:描述 speed:子弹速度 netRadius:渔网的半径
			int bullet_kind;
			xml_child->Attribute("kind", &bullet_kind);
			if (bullet_kind >= BULLET_KIND_COUNT || bullet_kind < 0) 
				return false;
			xml_child->Attribute("speed", &bullet_speed_[bullet_kind]);
			xml_child->Attribute("netRadius", &net_radius_[bullet_kind]);
			++bullet_kind_count;
		}

		else if(!strcmp(xml_child->Value(), "SpecialUser")) 
		{
			//UserID:玩家ID CellScore:单位分数
			SpecialUserInfo sUser;
			memset(&sUser,0,sizeof(sUser));
			xml_child->Attribute("UserID", &sUser.iUserID);
			int SuperMan_CellScore;
			xml_child->Attribute("Probability", &sUser.dProbability);
			m_vSpecialUser.push_back(sUser);
		}
		else if(!strcmp(xml_child->Value(), "BuildFish")) 
		{
			//<!-- BuildFish:各种鱼生成间歇时间 BuildTime 单位秒-->
			//<!-- 鱼依次为0：小鱼出现时长、1：中等鱼出现时长、2：18号鱼出现时长、3：19号鱼出现时长、4：20号鱼出现时长-->
			//<!--        5：李逵出现时长、6：炸弹鱼出现时长、7：超级炸弹鱼出现时长、8：锁定炸弹鱼出现时长-->

			int iBuildID = -1;
			int iBuildTime = 0;
			xml_child->Attribute("BuildID", &iBuildID);
			xml_child->Attribute("BuildTime", &iBuildTime);
			if (iBuildID == 0)	//小鱼
			{
				if (iBuildTime > 0)
				{
					m_iBuildSmallFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 1)	//中鱼
			{
				if (iBuildTime > 0)
				{
					m_iBuildMediumFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 2)	//大鱼
			{
				if (iBuildTime > 0)
				{
					m_iBuildBigFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 3)	//三角大三元和大四喜
			{
				if (iBuildTime > 0)
				{
					m_iBuildDSYFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 4)	//直线大三元和大四喜
			{
				if (iBuildTime > 0)
				{
					m_iBuildDSXFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 5)		//水草
			{
				if (iBuildTime > 0)
				{
					m_iBuild23FishTime = iBuildTime;
				}
			}
			else if (iBuildID == 6)		//鱼王
			{
				if (iBuildTime > 0)
				{
					m_iBuildYWFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 7)		//炸弹
			{
				if (iBuildTime > 0)
				{
					m_iBuild24FishTime = iBuildTime;
				}
			}
			else if (iBuildID == 8)		//金蟾
			{
				if (iBuildTime > 0)
				{
					m_iBuildJCFishTime = iBuildTime;
				}
			}
			else if (iBuildID == 9)
			{
				if (iBuildTime > 0)
				{
					m_iSwitchSceneTime = iBuildTime;
				}
			}
		}
	}

	if (fish_count != FISH_KIND_COUNT) 
		return false;
	if (bullet_kind_count != BULLET_KIND_COUNT) 
		return false;

	return true;
}

//获取该玩家的单位分数
int CServerGameDesk::GetSuperCellscore(BYTE bDesk)
{
	return m_iCellScore;
}

//根据房间ID加载ini配置文件
BOOL CServerGameDesk::LoadExtIni(UINT uRoomID)
{
	//CString s = CINIFile::GetAppPath ();/////本地路径
	//CINIFile f( s +SKIN_FOLDER +".ini");

	//CString str;
	//str.Format("11100400_%d",uRoomID);
	//CString key =str;
	//m_iBeginNt = f.GetKeyVal(key,"beginnt",m_iBeginNt);
	//m_iEndNt = f.GetKeyVal(key,"endnt",m_iEndNt);
	//::memset(m_iPlayNtNum,m_iBeginNt,sizeof(m_iPlayNtNum));
	return TRUE;
}

//加载ini配置文件
BOOL CServerGameDesk::LoadIni()
{
	CString s = CINIFile::GetAppPath ();/////本地路径
	CString nid;
	nid.Format("%d",NAME_ID);
	CINIFile f( s +nid +"_s.ini");
	CString key = TEXT("game");
	g_stock_score_ = f.GetKeyVal(key,"StockScore",g_stock_score_);

	//key = TEXT("Admin");
	//g_AdminiList.clear();
	//g_AdminiList.push_back(f.GetKeyVal(key,"loginID0",0));
	//g_AdminiList.push_back(f.GetKeyVal(key,"loginID1",0));
	//g_AdminiList.push_back(f.GetKeyVal(key,"loginID2",0));
	//g_AdminiList.push_back(f.GetKeyVal(key,"loginID3",0));
	//g_AdminiList.push_back(f.GetKeyVal(key,"loginID4",0));
	return true;
}


BOOL CServerGameDesk::LoadAdminIni()
{
	g_vecControlList.clear();
	g_vecSpecialList.clear();
	CString s = CINIFile::GetAppPath ();/////本地路径
	CINIFile f( s +"adminmanager.ini");
	CString text = NULL;
	CString key = "ControlList";
	int iControlCount = 0;
	iControlCount = f.GetKeyVal(key,"ControlCount",iControlCount);

	ControlInfo control_info_;
	for (int i = 0; i < iControlCount;i++)
	{
		text.Format("UserId[%d]",i);
		control_info_.user_id_ = f.GetKeyVal(key,text,0);
		text.Format("CatchRate[%d]",i);
		control_info_.catch_rate_ = f.GetKeyVal(key,text,0);
		text.Format("LimitScore[%d]",i);
		control_info_.limit_score_ = f.GetKeyVal(key,text,0);
		text.Format("Win[%d]",i);
		control_info_.win_or_lose_ = f.GetKeyVal(key,text,0);
		g_vecControlList.push_back(control_info_);
	}


	key = "SpecialList";
	int iSpecialCount = 0;
	iSpecialCount = f.GetKeyVal(key,"SpecialCount",iSpecialCount);
	SpecialUser special_user_;
	for (int i = 0; i < iSpecialCount;i++)
	{
		text.Format("UserId[%d]",i);
		special_user_.user_id_ = f.GetKeyVal(key,text,0);
		for (int fish_num_ = 0;fish_num_ < SEND_FISH_NUM;fish_num_++)
		{
			text.Format("%d_FishKind[%d]",i,fish_num_);
			int fish_kind_ = f.GetKeyVal(key,text,0);
			if (fish_kind_ == 0) continue;
			special_user_.special_fish_[fish_num_].fish_kind_ = fish_kind_;

			text.Format("%d_LeftNum[%d]",i,fish_num_);
			int left_num_ = f.GetKeyVal(key,text,0);
			if (left_num_ == 0) continue;
			special_user_.special_fish_[fish_num_].left_num_ = left_num_;

			text.Format("%d_CatchRate[%d]",i,fish_num_);
			int catch_rate_ = f.GetKeyVal(key,text,0);
			if (catch_rate_ == 0) continue;
			special_user_.special_fish_[fish_num_].catch_rate_ = catch_rate_;
		}
		g_vecSpecialList.push_back(special_user_);
	}
	return TRUE;
}
FishTraceInfo* CServerGameDesk::ActiveFishTrace() 
{
	FishTraceInfo* fish_trace_info = NULL;
	if (storage_fish_trace_vector_.size() > 0) 
	{
		fish_trace_info = storage_fish_trace_vector_.back();
		storage_fish_trace_vector_.pop_back();
		active_fish_trace_vector_.push_back(fish_trace_info);
	}

	if (fish_trace_info == NULL) 
	{
		fish_trace_info = new FishTraceInfo;
		active_fish_trace_vector_.push_back(fish_trace_info);
	}

	return fish_trace_info;
}

bool CServerGameDesk::FreeFishTrace(FishTraceInfo* fish_trace_info) {
	FishTraceInfoVecor::iterator iter;
	for (iter = active_fish_trace_vector_.begin(); iter != active_fish_trace_vector_.end(); ) {
		if (fish_trace_info == *iter) {
			ClearLockFish(fish_trace_info->fish_id);
			iter = active_fish_trace_vector_.erase(iter);
			storage_fish_trace_vector_.push_back(fish_trace_info);

			m_iFishNum--;
			return true;
		}
		else
		{
			++iter;
		}
	}

	assert(!"FreeFishTrace Failed");
	return false;
}

//存在真人
bool CServerGameDesk::ExistUnAI()
{
	for (int i = 0; i < PLAY_COUNT; ++i)
	{
		if(m_pUserInfo[i] == NULL)
			continue;
		if (m_pUserInfo[i]->m_UserData.isVirtual == 0 || m_bExitWatcher[i])//存在真人
		{
			return true;
		}
	}

	return false;
}

bool CServerGameDesk::OnSubUserChoiceTeam(BYTE bDeskStation, BYTE choice) 
{
	if(NULL == m_pUserInfo[bDeskStation])
	{
		return true;
	}

	if(!(m_iPlayingMethod & (1<<2)))
	{
		return true;
	}

	if(choice != TEAM_A && choice != TEAM_B)
	{
		return true;
	}

	S_C_CHOICE_TEAM_RES msg;
	msg.byDeskStation = bDeskStation;

	if(teamNum[choice] > 1)
	{
		msg.bsuccess = false;
	}
	else
	{
		msg.bsuccess = true;
		teamNum[choice]++;
		userTeam[bDeskStation] = choice;
	}

	for(int i = 0; i < PLAY_COUNT; ++i)
	{
		if(userTeam.find(i) != userTeam.end())
			msg.byteamInfo[i] = userTeam[i];
		else
			msg.byteamInfo[i] = TEAM_NULL;
	}

	if(msg.bsuccess)
		SendDataToAllPlayers(&msg,sizeof(msg),SUB_S_CHOICE_TEAM_RES);
	else
		SendDataToPlayer(bDeskStation,&msg,sizeof(msg),SUB_S_CHOICE_TEAM_RES);

	return true;
}

bool CServerGameDesk::OnSubUserFire(BYTE bDeskStation, BulletKind bullet_kind, float angle, int bullet_mul, int lock_fishid,int iBullet_id,float vector_x,float vector_y ,bool bIsRobot,bool bAutoShoot) 
{
	//sdp机器人和真人一样消耗子弹2014.05.08
	// 没真实玩家机器人不打炮
	/*if (!ExistUnAI() ) 
	{
		return true;
	}*/

	if (NULL == m_pUserInfo[bDeskStation])
	{
		return true;
	}


	if (bullet_mul < min_bullet_multiple_ || bullet_mul > max_bullet_multiple_) 
	{
		return true;
	}

	WORD chair_id = bDeskStation;
// 	assert(m_i64fish_score_[chair_id] >= bullet_mul);
// 	if (m_i64fish_score_[chair_id] < bullet_mul) 
// 	{
// 		return true;
// 	}
//	assert(userShoot[chair_id] >= bullet_mul);
	if (userShoot[chair_id] < bullet_mul && m_bIsBuy)		//开房间按剩余炮数判断
	{
		return true;
	}
	else if(userPoint[chair_id] < bullet_mul && !m_bIsBuy)	//金币场按分数判断
	{
		return true;
	}

	ServerBulletInfo	TServerBulletInfo;
	TServerBulletInfo.bActive			= true;
	TServerBulletInfo.ibullet_id		= iBullet_id;
	TServerBulletInfo.ibullet_kind		= bullet_kind;
	TServerBulletInfo.ibullet_mulriple	= bullet_mul;
	TServerBulletInfo.dwBirthTime		= GetTickCount();


	for(int i=0; i<MAX_BULLET_COUNT; i++)
	{
		//服务端记录了子弹 才算发射成功
		if (!m_tServerBulletInfo[bDeskStation][i].bActive)
		{
			m_tServerBulletInfo[bDeskStation][i] = TServerBulletInfo;

//			LOGMODULEINSTANCE.WriteLog("create m_tServerBulletInfo[%d][%d],iBullet_id=%d\n",bDeskStation,i,iBullet_id);

			m_bUserHavePlay[chair_id] = true;
//			m_i64fish_score_[chair_id] -= bullet_mul;
			if(m_bIsBuy)
				userShoot[chair_id] -= bullet_mul;
			else
				userPoint[chair_id] -= bullet_mul;

			if (!m_pUserInfo[bDeskStation]->m_UserData.isVirtual)
			{
				g_stock_score_ += bullet_mul;
			}
			if (lock_fishid > 0 && GetFishTraceInfo(lock_fishid) == NULL) 
			{
				lock_fishid = 0;
			}
			IALockFUN_(bDeskStation,lock_fishid);


			//记录玩家当前的炮火倍数
			m_iUserFireBase[chair_id] = bullet_mul;

			CMD_S_UserFire user_fire;
			user_fire.bIsRobot = bIsRobot;
			user_fire.bullet_kind = bullet_kind;
			user_fire.bullet_id = iBullet_id;//GetBulletID(chair_id);
			user_fire.angle = angle;
			user_fire.chair_id = bDeskStation;
			user_fire.android_chairid = /*m_pUserInfo[bDeskStation]->m_UserData.isVirtual ? android_chairid_ :*/ INVALID_CHAIR;//sdp机器人和真人一样消耗子弹2014.05.08
			user_fire.bullet_mulriple = bullet_mul;
			user_fire.fish_score = userPoint[chair_id];
			user_fire.lock_fishid = lock_fishid;
			user_fire.iRemainShootCount = userShoot[chair_id];
			user_fire.bAutoShoot = bAutoShoot;
			
			user_fire.vector_x = vector_x;
			user_fire.vector_y = vector_y;
			
			SendDataToAllPlayers(&user_fire, sizeof(user_fire), SUB_S_USER_FIRE);
//			UpDateRealMoney(m_pUserInfo[bDeskStation]->m_UserData.dwUserID,m_i64fish_score_[bDeskStation]);
			UpdateControllerMoney(bDeskStation,bullet_mul);
			TidyControlData();

			if(m_bIsBuy)	//开房间才判断提前结束
			{
				bool noShoot = true;
				for(auto itr = userShoot.begin(); itr != userShoot.end(); ++itr)
				{
					if(itr->second > 0)
					{
						noShoot = false;
						break;
					}
				}

				if(noShoot)
				{
					SetTimer(G_iCheckFinishTimer, 200);
					KillTimer(G_iGameFinishTimer);
					SetTimer(G_iGameFinishTimer, 3 * 1000);
				}	
			}	

			return true;
		}
	}
	return true;
}



bool CServerGameDesk::UpdateControllerMoney(BYTE bDeskStation,__int64 i64ChangeMoney,bool bReset)
{
	if (m_pUserInfo[bDeskStation] == NULL)
	{
		return false;
	}
	DWORD dwUserID = m_pUserInfo[bDeskStation]->m_UserData.dwUserID;

	for (int i = 0; i < g_vecControlList.size();i++)
	{
		if (dwUserID == g_vecControlList.at(i).user_id_)
		{
			if (!bReset)
			{
				//发泡为列，赢，则 true 需要减去子弹倍数
				//捕获鱼，赢，则true需要加上鱼分数，都为1
				int mul = g_vecControlList.at(i).win_or_lose_ ? 1 : -1;
				g_vecControlList.at(i).limit_score_ += i64ChangeMoney * mul;
			}
			else
			{
				g_vecControlList.at(i).limit_score_ = i64ChangeMoney;
			}
			return true;
		}
	}
	return false;
}
int CServerGameDesk::GetBulletID(WORD chairid) 
{
	ASSERT(chairid < GAME_PLAYER);
	++m_iBullet_id_[chairid];
	if (m_iBullet_id_[chairid] <= 0) m_iBullet_id_[chairid] = 1;
	return m_iBullet_id_[chairid];
}
//---------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//修改奖池数据1(平台->游戏)
/// @param bAIWinAndLostAutoCtrl 机器人控制输赢开关
/// @return 是否成功
bool CServerGameDesk::SetRoomPond(bool	bAIWinAndLostAutoCtrl)
{
	return false;
}

//修改奖池数据2(平台->游戏)
/// @param m_iAIWantWinMoney 机器人输赢控制：机器人赢钱区域1,2,3 4所用断点
/// @param m_iAIWinLuckyAt 机器人输赢控制：机器人在区域1,2,3,4赢钱的概率
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iAIWantWinMoney[], int	iAIWinLuckyAt[])
{
	return false;
}

//修改奖池数据3(平台->游戏)
/// @param iReSetAIHaveWinMoney 机器人输赢控制：重置机器人已经赢钱的数目
/// @return 是否成功
bool CServerGameDesk::SetRoomPondEx(__int64	iReSetAIHaveWinMoney)
{
	return false;
}

//判断
BOOL CServerGameDesk::Judge()
{
	if(JudgeWiner())		//先处理胜者
	{
		return TRUE;
	}

	if (JudgeLoser())		//处理输者
	{
		return TRUE;
	}		

	return FALSE;
}
//================================================
//判断胜者
//================================================
BOOL CServerGameDesk::JudgeLoser()
{
	return FALSE;
}
//================================================
//判断胜者
//================================================
BOOL CServerGameDesk::JudgeWiner()
{
	return FALSE;
}
//GRM函数
//更新奖池数据(游戏->平台)
/// @return 机器人赢钱数
void CServerGameDesk::UpDataRoomPond(__int64 iAIHaveWinMoney)
{

}

bool CServerGameDesk::ExamSuperUser(BYTE bDesk)
{
	if (m_pUserInfo[bDesk] == NULL) 
		return false;
	for (int i = 0;i < g_AdminiList.size();i++)
	{
		if (g_AdminiList.at(i) == m_pUserInfo[bDesk]->m_UserData.dwUserID)
		{
			return true;
		}
	}
	return false;
}

void CServerGameDesk::SaveSetting()
{
	CString s = CINIFile::GetAppPath ();/////本地路径
	FILE *pf=fopen(s +"adminmanager.ini","wb");
	if (NULL == pf)
	{
		OutputDebugString("llog::打开文件失败");
		return;
	}
	fclose(pf);

	CINIFile f( s +"adminmanager.ini");
	CString value_text_ = NULL;
	CString	key_text_ = NULL;
	CString key = "ControlList";
	value_text_.Format("%d",g_vecControlList.size());
	f.SetKeyValString(key,"ControlCount",value_text_);
	ControlInfo TControlInfo;
	for (int i = 0; i < g_vecControlList.size();i++)
	{
		TControlInfo = g_vecControlList.at(i);
		key_text_.Format("UserId[%d]",i);
		value_text_.Format("%d",TControlInfo.user_id_);
		f.SetKeyValString(key,key_text_,value_text_);


		key_text_.Format("CatchRate[%d]",i);
		value_text_.Format("%d",TControlInfo.catch_rate_);
		f.SetKeyValString(key,key_text_,value_text_);

		key_text_.Format("LimitScore[%d]",i);
		value_text_.Format("%d",TControlInfo.limit_score_);
		f.SetKeyValString(key,key_text_,value_text_);

		key_text_.Format("Win[%d]",i);
		value_text_.Format("%d",TControlInfo.win_or_lose_);
		f.SetKeyValString(key,key_text_,value_text_);
	}


	key = "SpecialList";
	value_text_.Format("%d",g_vecSpecialList.size());
	f.SetKeyValString(key,"SpecialCount",value_text_);

	SpecialUser TSpecialUser;
	SpecialFish TSpecialFish;
	for (int i = 0; i < g_vecSpecialList.size();i++)
	{
		TSpecialUser = g_vecSpecialList.at(i);

		key_text_.Format("UserId[%d]",i);
		value_text_.Format("%d",TSpecialUser.user_id_);
		f.SetKeyValString(key,key_text_,value_text_);

		for (int fish_num_ = 0;fish_num_ < SEND_FISH_NUM;fish_num_++)
		{
			TSpecialFish = TSpecialUser.special_fish_[fish_num_];
			if (TSpecialFish.left_num_ == 0) continue;
			key_text_.Format("%d_FishKind[%d]",i,fish_num_);
			value_text_.Format("%d",TSpecialFish.fish_kind_);
			f.SetKeyValString(key,key_text_,value_text_);

			key_text_.Format("%d_LeftNum[%d]",i,fish_num_);
			value_text_.Format("%d",TSpecialFish.left_num_);
			f.SetKeyValString(key,key_text_,value_text_);

			key_text_.Format("%d_CatchRate[%d]",i,fish_num_);
			value_text_.Format("%d",TSpecialFish.catch_rate_);
			f.SetKeyValString(key,key_text_,value_text_);
		}
	}
}

void CServerGameDesk::IALockFUN_(BYTE byDeskStation,int &lock_fish_id_)
{
	if (NULL == m_pUserInfo[byDeskStation]) 
	{	
		return ;
	}
	if (!m_pUserInfo[byDeskStation]->m_UserData.isVirtual)
	{
		return;
	}
	if(active_fish_trace_vector_.size() <= 0)
	{
		return;
	}
	srand(GetTickCount() + byDeskStation);

	lock_fish_id_ = 0;
	FishTraceInfoVecor vecBigFish;
	vecBigFish.clear();
	for (FishTraceInfoVecor::iterator it = active_fish_trace_vector_.begin();it != active_fish_trace_vector_.end();it++)
	{
		if (((*it)->fish_kind  >= FISH_KIND_17) && ((*it)->fish_kind < FISH_KIND_COUNT))
		{
			DWORD now_tick = GetTickCount();
			if (now_tick - (*it)->build_tick <= 20*1000)
			{
				vecBigFish.push_back(*it);
			}
		}
	}

	if (vecBigFish.size() == 0)
	{		
		m_iLockFish[byDeskStation] = 0;
		return;
	}
	FishTraceInfo *pLastFish;
	if (m_iLockFish[byDeskStation] == 0)
	{
		pLastFish = (vecBigFish.at(0));
		for (int i = vecBigFish.size() -1;i >= 0;i--)
		{
			int fishCount = GetLockCount(  vecBigFish.at(i)->fish_id  );
			if (fishCount > 0)
			{
				continue;
			}
			pLastFish = vecBigFish.at(i);
			break;
		}
		lock_fish_id_ = pLastFish->fish_id;
		m_iLockFish[byDeskStation] = lock_fish_id_;
	}
	else
	{	
		lock_fish_id_ = m_iLockFish[byDeskStation];
	}
}


int	 CServerGameDesk::GetLockCount(int fish_id)
{
	int iLockCount = 0;
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_iLockFish[i] == fish_id)
		{
			iLockCount++;
		}
	}
	return iLockCount;
}

void CServerGameDesk::ClearLockFish(int fish_id_)
{
	for (int i = 0;i < PLAY_COUNT;i++)
	{
		if (m_iLockFish[i] == fish_id_)
		{
			m_iLockFish[i] = 0;
			break;
		}
	}
}

int CServerGameDesk::GetCurPlayerCount()
{
	int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;
	int curPlayerCount = 0;

	for(int i = 0; i < playercount; ++i)
	{
		if(m_pUserInfo[i] != NULL)
			++curPlayerCount;
	}

	return curPlayerCount;
}

void CServerGameDesk::SendReaminInfo(bool isFinish)
{
	S_C_RemainTime msg;
	//int gameCount = isFinish ? 1 : 0;
	msg.remainCount = m_iVipGameCount - m_iRunGameCount;
	msg.totalCount = m_iVipGameCount;

	if(!m_bIsBuy)
	{
		msg.remainCount = 0;
		msg.totalCount = 0;
	}

	msg.remainTime = m_iGameTime * 60 - (time(NULL) - m_lStartTime);
	if(isFinish || m_bGameStation != GAME_STATUS_PLAY)
	{
		msg.remainTime = m_iGameTime * 60;
	}
	
	msg.totalTime = m_iGameTime * 60;

	if(msg.remainTime > msg.totalTime || msg.totalTime < 0)
	{
		msg.remainTime = 0;
	}
	
	SendDataToAllPlayers(&msg,sizeof(msg),S_C_UPDATE_REMAIN_INFO_SIG);
}

void CServerGameDesk::SendScoreBoard()
{
	if(m_bIsBuy)
	{
		int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;

		if(m_iPlayingMethod & (1 << 1))		//1V1
		{
			m_CalculateBoard.isWin[0] = m_CalculateBoard.iuserPoint[0] > m_CalculateBoard.iuserPoint[1] ? WIN : (m_CalculateBoard.iuserPoint[0] == m_CalculateBoard.iuserPoint[1] ? DEUCE : LOSE);
			m_CalculateBoard.isWin[1] = m_CalculateBoard.iuserPoint[1] > m_CalculateBoard.iuserPoint[0] ? WIN : (m_CalculateBoard.iuserPoint[1] == m_CalculateBoard.iuserPoint[0] ? DEUCE : LOSE);
		}
		else if(m_iPlayingMethod & (1 << 2)) //2V2
		{
			int teamA_Point = 0;
			int teamB_Point = 0;

			for(int i = 0; i < playercount; ++i)
			{
				if(userTeam[i] == TEAM_A)
				{
					teamA_Point += m_CalculateBoard.iuserPoint[i];
				}
				else if(userTeam[i] == TEAM_B)
				{
					teamB_Point += m_CalculateBoard.iuserPoint[i];
				}
			}

			for(int i = 0; i < playercount; ++i)
			{
				if(userTeam[i] == TEAM_A)
				{
					m_CalculateBoard.isWin[i] = teamA_Point > teamB_Point ? WIN : (teamA_Point == teamB_Point ? DEUCE : LOSE);
				}
				else if(userTeam[i] == TEAM_B)
				{
					m_CalculateBoard.isWin[i] = teamB_Point > teamA_Point ? WIN : (teamB_Point == teamA_Point ? DEUCE : LOSE);
				}
			}
		}
		else if(m_iPlayingMethod & (1 << 3)) //4人混战
		{
			for(int i = 0; i < playercount; ++i)
			{
				m_CalculateBoard.isWin[i] = m_CalculateBoard.iuserPoint[i] > 0 ? WIN : (0 == m_CalculateBoard.iuserPoint[i] ? DEUCE : LOSE);
			}
		}

		SendDataToAllPlayers(&m_CalculateBoard,sizeof(m_CalculateBoard),S_C_UPDATE_CALCULATE_BOARD_SIG);

		//清理房间数据
		ReSetGameState(0);
		m_CalculateBoard.clear();
		userTeam.clear();
		teamNum.clear();
	}
}

void CServerGameDesk::DissMissRoomBefore()
{
	if(IsPlayGame(0))
		SendScoreBoard();
}

//判断是否可以开始游戏
bool CServerGameDesk::CanBeginGame()
{
	int playercount = (1 << 1) & m_iPlayingMethod ? 2 : 4;

	BYTE bReadyCount = 0;
	for (BYTE i = 0 ; i < playercount; ++i) 
	{
		if(m_pUserInfo[i] != NULL)
		{
			if(m_pUserInfo[i]->m_UserData.bUserState != USER_ARGEE)
				return false;
			else
				++bReadyCount;
		}
	}

	if(bReadyCount == playercount)
		return true;

	return false;
}

void CServerGameDesk::AutoFire(BYTE bDeskStation)
{
	srand(time(NULL));

	CMD_C_UserFire user_fire;
	user_fire.bullet_kind = BULLET_KIND_2_NORMAL;
	user_fire.vector_x = 0;
	user_fire.vector_y = 0;
	if(bDeskStation < 2)
		user_fire.angle = kFireAngle12[rand() % 20];
	else
		user_fire.angle = kFireAngle34[rand() % 20];
	user_fire.bullet_mulriple = min_bullet_multiple_;
	user_fire.lock_fishid = false;
	user_fire.bullet_id = ++userAutoFireId[bDeskStation];
	user_fire.bIsRobot = false;

	bool bAutoShoot = true;

	OnSubUserFire(bDeskStation, user_fire.bullet_kind, user_fire.angle, user_fire.bullet_mulriple, user_fire.lock_fishid,user_fire.bullet_id,user_fire.vector_x,user_fire.vector_y,user_fire.bIsRobot,bAutoShoot);
//	LOGMODULEINSTANCE.WriteLog("player %d auto fire\n",bDeskStation);
}

bool CServerGameDesk::CheckFinish()
{
	//所有玩家子弹用完且服务端已释放完所有子弹，提前结束游戏
	bool canGameFinish = true;
	bool noShoot = true;
	for(auto itr = userShoot.begin(); itr != userShoot.end(); ++itr)
	{
		if(itr->second > 0)
		{
			noShoot = false;
			break;
		}
	}

	if(noShoot)
	{
		int playerCount = (1 << 1) & m_iPlayingMethod ? 2 : 4;
		for(int i = 0; i < playerCount; ++i)
		{
			for (int j = 0; j < MAX_BULLET_COUNT; ++j)
			{
				if (m_tServerBulletInfo[i][j].bActive)
				{
					canGameFinish = false;
//					LOGMODULEINSTANCE.WriteLog("m_tServerBulletInfo[%d][%d] is active\n",i,j);
					break;
				}
			}

			if(!canGameFinish)
				break;
		}
	}
	else
		canGameFinish = false;


	return canGameFinish;
}

void CServerGameDesk::InitGameRule()
{
	if(!m_bIsBuy)
		return;
	else
	{
		tagPM *playmode = (tagPM *)m_szDeskConfig;

		m_iPlayingMethod = playmode->iPlayingMethod;

		if(0 == (m_iPlayingMethod & OneToOneFight) && 0 == (m_iPlayingMethod & TwoToTwoFight) && 0 == (m_iPlayingMethod & FourFight))
			m_iPlayingMethod |= FourFight;

		if(0 == (m_iPlayingMethod & OneRatioOne) && 0 == (m_iPlayingMethod & OneRatioTen))
			m_iPlayingMethod |= OneRatioOne;

		m_iGameTime = playmode->byGameTime;
	}
}

