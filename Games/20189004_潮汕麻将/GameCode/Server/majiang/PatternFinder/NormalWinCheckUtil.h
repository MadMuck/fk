#ifndef NORMAL_WIN_CHECK_UTIL_H
#define NORMAL_WIN_CHECK_UTIL_H

#ifndef HAND_CARD_NUM
#define HAND_CARD_NUM 14
#endif

#include "Pattern.h"
#include "PatternResultCards.h"
#include "Util.h"
#include "PatternType.h"

class NormalWinOnFoundNonsense
{
public:
	void operator() (const std::vector<PatternResult> &melds)
	{}
};

template<typename T>
class NormalWinCheckUtil
{
public:
	typedef unsigned char uchar;

	struct CheckHuStruct
	{
		uchar  data[HAND_CARD_NUM][2];   //�����ݣ�0��ֵ��1����
		uchar  conut;
		void Add(uchar pai)
		{
			if(GetPaiCount(pai)>0)
			{
				for(int i=0;i<HAND_CARD_NUM;i++)
				{
					if(data[i][0] == pai && pai != 255)
						data[i][1]++;
				}
			}
			else
			{
				data[conut][0] = pai;
				data[conut][1] = 1;
				conut++;
			}
		};
		uchar GetPaiCount(uchar pai)
		{
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				if(data[i][0] == pai && pai != 255 && data[i][1] != 255)
					return data[i][1];
			}
			return 0;
		};
		uchar GetDataCount()
		{
			conut = 0;
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				if(data[i][0] != 255)
					conut++;
			}
			return conut;
		};
		uchar GetAllPaiCount()
		{
			uchar num = 0;
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				if(data[i][0] != 255)
					num += data[i][1];
			}
			return num;
		};
		void SetPaiCount(uchar pai,uchar num)
		{
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				if(data[i][0] == pai && pai != 255)
				{
					data[i][1] = num;
				}
			}
		}
		void Init()
		{
			memset(data,255,sizeof(data));
			for(int i=0;i<HAND_CARD_NUM;i++)
			{
				data[i][1] = 0;
			}
			conut=0;
		};
		CheckHuStruct()
		{
			Init();
		}
	};

	struct PingHuStruct
	{
		uchar  byType[6];    //��������
		uchar  data[6][4];   //����
		bool  isjing[6][4]; //�Ƿ�������
		uchar  count;        //�ڵ���
		PingHuStruct()
		{
			Init();
		};
		void AddData(uchar type,uchar pai[],bool jing[])
		{
			for(int i=0;i<6;i++)
			{
				if(byType[i] == 255)
				{
					byType[i] = type;
					data[i][0] = pai[0];data[i][1] = pai[1];data[i][2] = pai[2];data[i][3] = pai[3];
					isjing[i][0] = jing[0];isjing[i][1] = jing[1];isjing[i][2] = jing[2];isjing[i][3] = jing[3];
					count++;
					break;
				}
			}
		};
		void DeleteData(uchar type,uchar pai[],bool jing[])//ɾ��Ԫ��
		{
			for(int i=0;i<6;i++)
			{
				if(byType[i] == type && data[i][0] == pai[0 ]&& data[i][1] == pai[1] && isjing[i][0] == jing[0] && isjing[i][1] == jing[1])
				{
					byType[i] = 255;
					memset(data[i],255,sizeof(data[i]));
					memset(isjing[i],0,sizeof(isjing[i]));
					count--;
					break;
				}
			}
		};
		void SortData()
		{
			for(int i=0;i<5;i++)
			{
				if(byType[i] != 255)
					continue;
				for(int j=i;j<5;j++)
				{
					byType[j] = byType[j+1];
					memcpy(data[j],data[j+1],sizeof(data[j]));//����
					memcpy(isjing[j],isjing[j+1],sizeof(isjing[j]));//�Ƿ�������
				}
			}
			count = 0;
			for(int i=0;i<5;i++)
			{
				if(byType[i] != 255)
					count++;
			}
		};
		void Init()
		{
			memset(this,255,sizeof(PingHuStruct));
			memset(isjing,0,sizeof(isjing));
			count = 0;
		};
	};

	enum MeldType
	{
		MeldTYPE_AN_KE,
		MeldTYPE_SHUN_ZI,
		MeldTYPE_JIANG_PAI
	};

	bool checkResult;

	CardList ConvertToCardList(uchar *cards, int len)
	{
		CardList ret;
		for(int i = 0; i < len; ++i)
		{
			if(cards[i] != 255)
			{
				ret.Add(cards[i]);
			}
		}
		return ret;
	}

	bool MakePingHu(CheckHuStruct &PaiData,PingHuStruct &hupaistruct,int csnum)
	{
		if(PaiData.GetAllPaiCount()<=0)//�Ѿ�����
		{
			checkResult = true;
			std::vector<PatternResult> result;
			for(int i = 0; i < hupaistruct.count; ++i)
			{
				bool hasJoker = false;
				CardList isJoker;
				for(int j = 0; j < ARRAY_LEN(hupaistruct.isjing[i]); ++j)
				{
					if(hupaistruct.isjing[i][j])
					{
						hasJoker = true;
						isJoker.push_back(1);
					}
					else
						isJoker.push_back(0);
				}

				switch(hupaistruct.byType[i])
				{
				case MeldTYPE_JIANG_PAI:
				case MeldTYPE_AN_KE:
					result.push_back(PatternResult(new PatternResultCardsImpl(
						hasJoker ? PatternType_JokerSet : PatternType_Set,
						ConvertToCardList(hupaistruct.data[i], ARRAY_LEN(hupaistruct.data[i])), isJoker)));
					break;
				case MeldTYPE_SHUN_ZI:
					result.push_back(PatternResult(new PatternResultCardsImpl(
						hasJoker ? PatternType_JokerRun : PatternType_Run,
						ConvertToCardList(hupaistruct.data[i], ARRAY_LEN(hupaistruct.data[i])), isJoker)));
				}
			}

			if(3 == csnum)
			{
				result.push_back(PatternResult(new PatternResultCardsImpl(PatternType_ThreeJoker,CardList())));
			}

			m_OnFound(result);
			return true;
		}
		if(csnum >HAND_CARD_NUM)
		{
			csnum=0;
		}
		////ȡ��
		uchar pai=255;
		int count = 0,ijing = csnum;
		for(int i=0;i<PaiData.conut;i++)
		{
			if(PaiData.data[i][1]==255 || PaiData.data[i][1]==0)
				continue;
			pai = PaiData.data[i][0];   //��¼��ֵ
			count = PaiData.data[i][1]; //��¼����
			break;
		}

		uchar data[4];
		bool caishen[4];
		memset(data,255,sizeof(data));
		memset(caishen,0,sizeof(caishen));

		data[0] = pai;
		data[1] = pai;
		data[2] = pai;
		if(csnum>=2)//һ���ƺ� 2����
		{		
			caishen[0] = true;
			caishen[1] = true;
			hupaistruct.AddData(MeldTYPE_AN_KE,data,caishen);//�������
			PaiData.SetPaiCount(pai,count-1);//������


			MakePingHu(PaiData,hupaistruct,csnum-2);//�ݹ�

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_AN_KE,data,caishen);//�ָ�����
		}
		if(csnum >= 1 && count >= 2)//һ�Ų���������������
		{
			memset(caishen,0,sizeof(caishen));
			caishen[0] = true;
			hupaistruct.AddData(MeldTYPE_AN_KE,data,caishen);//�������
			PaiData.SetPaiCount(pai,count-2);//������

			MakePingHu(PaiData,hupaistruct,csnum-1);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_AN_KE,data,caishen);//�ָ�����
		}

		if(count>=3)//�����������
		{
			memset(caishen,0,sizeof(caishen));
			hupaistruct.AddData(MeldTYPE_AN_KE,data,caishen);//�������
			PaiData.SetPaiCount(pai,count-3);//������

			MakePingHu(PaiData,hupaistruct,csnum);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_AN_KE,data,caishen);//�ָ�����
		}

		if(pai >= 30)//���ƣ�ֻ���ǿ���
		{
			return false;
		}
		//��ͬ��������˳��
		if(csnum>=2 && pai%10 >=3)//��� xx+pai ��˳�ӣ�x�ǲ���
		{
			memset(caishen,0,sizeof(caishen));
			caishen[0] = true;
			caishen[1] = true;
			data[0] = pai-2;
			data[1] = pai-1;
			data[2] = pai;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-2);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����
		}

		if(csnum>=1 && pai%10 >=2 && pai%10<9 && PaiData.GetPaiCount(pai+1)>=1 && PaiData.GetPaiCount(pai-1)<4)//��� x+pai+(pai+1) ��˳�ӣ�x�ǲ���
		{
			memset(caishen,0,sizeof(caishen));
			caishen[0] = true;
			data[0] = pai-1;
			data[1] = pai;
			data[2] = pai+1;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-1);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)+1);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����
		}

		if(csnum>=2 && pai%10 >=2 && pai%10<9)//��� x+pai+(x) ��˳�ӣ�x�ǲ���
		{
			memset(caishen,0,sizeof(caishen));
			caishen[0] = true;
			caishen[2] = true;
			data[0] = pai-1;
			data[1] = pai;
			data[2] = pai+1;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-2);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����
		}

		if( pai%10<8 && PaiData.GetPaiCount(pai+1)>=1 && PaiData.GetPaiCount(pai+2)>=1)//��� pai+(pai+1)+(pai+2) ��˳��
		{
			memset(caishen,0,sizeof(caishen));
			data[0] = pai;
			data[1] = pai+1;
			data[2] = pai+2;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)-1);//������
			PaiData.SetPaiCount(pai+2,PaiData.GetPaiCount(pai+2)-1);//������

			MakePingHu(PaiData,hupaistruct,csnum);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)+1);//������
			PaiData.SetPaiCount(pai+2,PaiData.GetPaiCount(pai+2)+1);//������
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����	
		}

		if(csnum>=1 && pai%10<8 &&  PaiData.GetPaiCount(pai+1)<4 && PaiData.GetPaiCount(pai+2)>=1)//��� pai+(x)+(pai+2) ��˳��
		{
			memset(caishen,0,sizeof(caishen));
			caishen[1] = true;
			data[0] = pai;
			data[1] = pai+1;
			data[2] = pai+2;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������
			PaiData.SetPaiCount(pai+2,PaiData.GetPaiCount(pai+2)-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-1);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			PaiData.SetPaiCount(pai+2,PaiData.GetPaiCount(pai+2)+1);//������
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����	
		}

		if(csnum>=1 && pai%10<8 && PaiData.GetPaiCount(pai+1)>=1 && PaiData.GetPaiCount(pai+2)<4)//��� pai+(pai+1)+(x) ��˳��
		{
			memset(caishen,0,sizeof(caishen));
			caishen[2] = true;
			data[0] = pai;
			data[1] = pai+1;
			data[2] = pai+2;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-1);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			PaiData.SetPaiCount(pai+1,PaiData.GetPaiCount(pai+1)+1);//������
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����	
		}

		if(csnum>=2 && pai%10<8 )//��� pai+(x)+(x) ��˳��
		{
			memset(caishen,0,sizeof(caishen));
			caishen[1] = true;
			caishen[2] = true;
			data[0] = pai;
			data[1] = pai+1;
			data[2] = pai+2;
			hupaistruct.AddData(MeldTYPE_SHUN_ZI,data,caishen);//������ƣ�˳��
			PaiData.SetPaiCount(pai,count-1);//������

			MakePingHu(PaiData,hupaistruct,csnum-2);

			PaiData.SetPaiCount(pai,count);//�����ϻָ���
			hupaistruct.DeleteData(MeldTYPE_SHUN_ZI,data,caishen);//�ָ�����	
		}
		return false;
	}

	PingHuStruct	hupaistruct;
	PingHuStruct	TempHupaistruct;///�������ṹ����
	CheckHuStruct	HuTempData;		
	///���Ƽ����ʱ����(�޲���)
	CheckHuStruct	NoJingHuTempData;
	int m_byJingNum;
	CardStatistic m_Stat;
	CardList m_Jokers;

	void ChangeHandPaiData(const CardStatistic &stat, const CardList &jokers)
	{			
		HuTempData.Init();
		NoJingHuTempData.Init();///���Ƽ����ʱ����(�޲���)	
		m_byJingNum = 0;
		m_Stat = stat;
		m_Jokers = jokers;

		const CardStatistic::ValuesType &values = stat.Values();
		for(CardStatistic::ValuesType::const_iterator itr = values.begin();
			itr != values.end();
			++itr)
		{
			if(jokers.Find(*itr) == 0)
			{
				int cnt = stat.Num(*itr);
				for(int i = 0; i < cnt; ++i)
					NoJingHuTempData.Add(*itr);
			}
			else
			{
				m_byJingNum += stat.Num(*itr);
			}
		}
	}

	bool CheckPingHu(T onFound)
	{
		m_OnFound = onFound;
		return __CheckPingHu();
	}

	bool CheckPingHu()
	{
		return __CheckPingHu();
	}

	T GetFound()
	{
		return m_OnFound;
	}

private:

	T m_OnFound;

	bool __CheckPingHu()
	{
		TempHupaistruct.Init();
		hupaistruct.Init();
		checkResult = false;

		uchar pai[4]={255};
		bool caishen[4]={0};

		//˫����������ֻ�з�񳤴��齫
		if(m_byJingNum>=2 && m_byJingNum!=255)//
		{
			hupaistruct.Init();//��ʼ�����ƽṹ
			memset(caishen,0,sizeof(caishen));
			memset(pai,255,sizeof(pai));

			bool finish = false;
			for(CardList::const_iterator itr = m_Jokers.begin(); 
				itr != m_Jokers.end();
				++itr)
			{
				if(m_Stat.Num(*itr) > 1)
				{
					pai[0] = *itr;
					pai[1] = *itr;
					finish = true;
					break;
				}
			}

			if(!finish)
			{
				int idx = 0;
				for(CardList::const_iterator itr = m_Jokers.begin(); 
					itr != m_Jokers.end();
					++itr)
				{
					if(m_Stat.Num(*itr) > 0)
					{
						pai[idx++] = *itr;
					}

					if(idx >= 2)
					{
						finish = true;
						caishen[0] = true;
						caishen[1] = true;
						break;
					}
				}
			}

			hupaistruct.AddData(MeldTYPE_JIANG_PAI,pai,caishen);//��ӽ�ͷ

			MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-2);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
		}
		//1������ļ��,һ�������һ��������
		if(m_byJingNum>=1 && m_byJingNum!=255)
		{
			for(int i=0;i<NoJingHuTempData.conut;i++)
			{
				if(NoJingHuTempData.data[i][1]<1 || NoJingHuTempData.data[i][1] == 255 ||NoJingHuTempData.data[i][0]==255)
					continue;
				hupaistruct.Init();//��ʼ�����ƽṹ
				NoJingHuTempData.data[i][1]-=1;

				memset(caishen,0,sizeof(caishen));
				memset(pai,255,sizeof(pai));
				pai[0] = NoJingHuTempData.data[i][0];
				pai[1] = NoJingHuTempData.data[i][0];
				caishen[0] = true;
				hupaistruct.AddData(MeldTYPE_JIANG_PAI,pai,caishen);

				MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum-1);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
				NoJingHuTempData.data[i][1]+=1;
			}
		}
		//���������ļ��,(�޾����齫HuTempData �� NoJingHuTempData��һ��������)
		for(int i=0;i<NoJingHuTempData.conut;i++)
		{
			if(NoJingHuTempData.data[i][1]<2 || NoJingHuTempData.data[i][0]==255)
				continue;
			hupaistruct.Init();//��ʼ�����ƽṹ
			NoJingHuTempData.data[i][1]-=2;

			memset(caishen,0,sizeof(caishen));
			memset(pai,255,sizeof(pai));
			pai[0] = NoJingHuTempData.data[i][0];
			pai[1] = NoJingHuTempData.data[i][0];
			hupaistruct.AddData(MeldTYPE_JIANG_PAI,pai,caishen);

			MakePingHu(NoJingHuTempData,hupaistruct,m_byJingNum);//���Ƽ��ݹ麯�����ڵݹ������ȫ�����
			NoJingHuTempData.data[i][1]+=2;
		}
		if(checkResult)
		{
			hupaistruct = TempHupaistruct;
			return true;
		}
		return false;
	}
};

struct NormalWinOnFoundResult
{
	std::vector<std::vector<PatternResult> > results;

	void operator() (const std::vector<PatternResult> &result)
	{
		results.push_back(result);
	}
};


// ������к��ƽ�������
class WinCheckUtilWithResult : public NormalWinCheckUtil<NormalWinOnFoundResult>
{
	NormalWinOnFoundResult m_Result;
public:

	const std::vector<std::vector<PatternResult> > &FindResult(const CardStatistic &stat)
	{
		return FindResult(stat, CardList());
	}

	const std::vector<std::vector<PatternResult> > &FindResult(const CardStatistic &stat, const CardList &jokers)
	{
		ChangeHandPaiData(stat, jokers);
		CheckPingHu(m_Result);
		m_Result = GetFound();
		return m_Result.results;
	}
};

#endif