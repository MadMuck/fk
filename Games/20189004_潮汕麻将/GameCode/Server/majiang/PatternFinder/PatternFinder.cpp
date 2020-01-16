#include "PatternFinder.h"
#include "Logger.h"
#include "UtilTemplate.h"
#include <list>
#include <set>

int PatternFinderMatchCnt = 0;
int PatternResultCnt = 0;

#define KeyType float

static KeyType __GetResultFlag(const CardList &cards)
{
    float avg = Util::Average(cards.begin(), cards.end(), 0.0f);
    float variance = Util::Variance(cards.begin(), cards.end(), avg);
    return cards.Count() * 10000 + avg * 100 + variance;
}

typedef std::map<KeyType, std::set<int> > PatternResultFlag;

static void StoreKeys(PatternResultFlag &flag, const std::list<KeyType> &keyList, int resultId)
{
	for(std::list<KeyType>::const_iterator itr = keyList.begin();
		itr != keyList.end();
		++itr)
	{
		flag[*itr].insert(resultId);
	}
}

static bool HasIntersect(const std::set<int> &resultSet1, const std::set<int> &resultSet2)
{
	const std::set<int> *src = &resultSet1;
	const std::set<int> *dest = &resultSet2;
	if(src->size() > resultSet2.size())
	{
		dest = &resultSet1;
		src = &resultSet2;
	}

	bool ret = false;
	for(std::set<int>::const_iterator itr = src->begin();
		itr != src->end();
		++itr)
	{
		if(dest->find(*itr) != dest->end())
		{
			ret = true;
			break;
		}
	}
	return ret;
}

static bool Ingore(const PatternResultFlag &flag, const std::list<KeyType> &keyList)
{
	if(keyList.size() < 2)
	{
		return false;
	}

	const std::set<int> *resultSet = NULL;
	int intersetCnt = 0;
	for(std::list<KeyType>::const_iterator itr = keyList.begin();
		itr != keyList.end();
		++itr)
	{
		PatternResultFlag::const_iterator keyResultSetItr = flag.find(*itr);
		if(keyResultSetItr != flag.end())
		{
			if(resultSet)
			{
				if(HasIntersect(*resultSet, keyResultSetItr->second))
				{
					++intersetCnt;
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			break;
		}

		resultSet = &keyResultSetItr->second;
	}
	return intersetCnt == keyList.size() - 1;
}


bool Find(CardStatistic &stat, const PatternList &patternList, int patternIdx,
    std::vector<MeldList> &result, MeldList &tmpResult, 
	std::list<KeyType> &keyList, PatternResultFlag &flag, 
	int &resultId, bool all)
{

    if(patternList.size() <= (size_t)patternIdx)
    {
        // fully matched
        result.push_back(tmpResult);
		//std::cout<<tmpResult<<" done"<<std::endl;
		StoreKeys(flag, keyList, resultId++);

        return true;
    }

    const Pattern &pattern = *(patternList.begin() + patternIdx);

    bool found = false;
    size_t tryIdx = 0;
    while(true)
    {
		//LOGGER_CONSOLE("trying "<<tryIdx<<" in layer "<<patternIdx);
        PatternResult patternResult = pattern.Find(stat, tryIdx);
		++ PatternResultCnt;
		
        if(patternResult.IsValid())
        {
            KeyType key = __GetResultFlag(patternResult.GetCards());
			keyList.push_back(key);
			if(Ingore(flag, keyList))
			{
				//std::cout<<tmpResult<<" "<<patternResult.GetCards()<<" ignore"<<std::endl;
				keyList.pop_back();
				continue;
			}

            patternResult.Consume(stat);
			//LOGGER_CONSOLE(stat.Values()<<" in layer "<<patternIdx<<"  "<<patternResult);
            tmpResult.push_back(patternResult);
            if(Find(stat, patternList, patternIdx + 1, result, tmpResult, keyList, flag, resultId, all))
            {
                found = true;

                if(!all)
				{
					//LOGGER_CONSOLE("matched, breaking from  "<<patternIdx);
					break;
				}
            }
			else
			{
				//std::cout<<tmpResult<<" failed"<<std::endl;
				//StoreKeys(flag, keyList, resultId++);
			}

            tmpResult.pop_back();
			keyList.pop_back();
            patternResult.Recover(stat);
        }
        else
        {
            found = false;
            break;
        }
    }

    return found;
}



MeldList PatternFinder::Match(const CardStatistic &stat, const PatternList &patternList)
{
    std::vector<MeldList> melds;
    CardStatistic tmpStat = stat;
    PatternResultFlag flag;
	std::list<KeyType> keyList;
	int resultId = 0;

    MeldList tmpResult;
    Find(tmpStat, patternList, 0, melds, tmpResult, keyList, flag, resultId, false);

    MeldList ret;
    if(tmpResult.size() > 0)
    {
        ret = melds[0];
    }
    
    return ret;
}

std::vector<MeldList> PatternFinder::MatchAll(const CardStatistic &stat, const PatternList &patternList)
{
    std::vector<MeldList> melds;
    CardStatistic tmpStat = stat;
    PatternResultFlag flag;
	std::list<KeyType> keyList;
	int resultId = 0;

    MeldList tmpResult;
    Find(tmpStat, patternList, 0, melds, tmpResult, keyList, flag, resultId, true);

    return melds;
}

std::ostream &operator << (std::ostream &ostrm, const MeldList &melds)
{
    for(MeldList::const_iterator itr = melds.begin();
        itr != melds.end();
        ++itr)
    {
        ostrm<<(*itr)<<" ";
    }
	return ostrm;
}
