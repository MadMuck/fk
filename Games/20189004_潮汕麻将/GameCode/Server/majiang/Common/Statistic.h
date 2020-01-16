#ifndef STATISTIC_H
#define STATISTIC_H

#include <iostream>
#include <vector>
#include <map>
#include <functional>
#include <algorithm>

template<typename T>
class Statistic
{
    std::vector<T> m_Values;
    std::map<T, int> m_Nums;
public:
    typedef std::vector<T> ValuesType;
    typedef std::vector<T, int> NumsType;

    Statistic(){}

    Statistic(const std::vector<T> &valList)
    {
        for(typename std::vector<T>::const_iterator itr = valList.begin();
            itr != valList.end();
            ++itr)
        {
            ++m_Nums[*itr];
        }

        for(typename std::map<T, int>::const_iterator itr = m_Nums.begin();
            itr != m_Nums.end();
            ++itr)
        {
            m_Values.push_back(itr->first);
        }

        std::sort(m_Values.begin(), m_Values.end());
    }

    const ValuesType &Values() const
    {
        return m_Values;
    }

    int Num(T val) const
    {
        int res = 0;
        typename std::map<T, int>::const_iterator itr = m_Nums.find(val);
        if(itr != m_Nums.end())
        {
            res = itr->second;
        }
        return res;
    }

    bool Sub(T val, int count)
    {
        bool res = (count <= 0);
        if(!res && Num(val) >= count)
        {
            m_Nums[val] -= count;
            res = true;
            if(m_Nums[val] == 0)
            {
                m_Nums.erase(val);
                m_Values.erase(std::find(m_Values.begin(), m_Values.end(), val));
            }
        }
        return res;
    }

    void Sub(const std::vector<T> &valList)
    {
        for(typename std::vector<T>::const_iterator itr = valList.begin();
            itr != valList.end();
            ++itr)
        {
            Sub(*itr, 1);
        }
    }

    void Add(T val, int count)
    {
        if(count <= 0) return ;

        typename std::map<T, int>::const_iterator itr = m_Nums.find(val);
        if(itr == m_Nums.end())
        {
            m_Values.push_back(val);
            std::sort(m_Values.begin(), m_Values.end());
        }
        m_Nums[val] += count;
    }

    void Add(const std::vector<T> &valList)
    {
        for(typename std::vector<T>::const_iterator itr = valList.begin();
            itr != valList.end();
            ++itr)
        {
            Add(*itr, 1);
        }
    }

	int Total() const
	{
		int total = 0;
		for(std::map<T, int>::const_iterator itr = m_Nums.begin(); itr != m_Nums.end(); ++itr)
		{
			total += itr->second;
		}
		return total;
	}

	template<typename CompType>
	ValuesType FindValuesWithNum(const CompType &comp, int num) const
	{
		ValuesType ret;
		for(ValuesType::const_iterator itr = m_Values.begin(); itr != m_Values.end(); ++itr)
		{
			if(comp(Num(*itr), num))
			{
				ret.push_back(*itr);
			}
		}
		return ret;
	}

	template<typename FuncType>
	ValuesType FindValues(const FuncType &func) const
	{
		ValuesType ret;
		for(ValuesType::const_iterator itr = m_Values.begin(); itr != m_Values.end(); ++itr)
		{
			if(func(*itr, Num(*itr)))
			{
				ret.push_back(*itr);
			}
		}
		return ret;
	}

	void Clear()
	{
		m_Values.clear();
		m_Nums.clear();
	}

    bool operator == (const Statistic &other) const
    {
        if(m_Values.size() != other.m_Values.size()) return false;

        bool ret = true;
        for(typename ValuesType::const_iterator itr = m_Values.begin();
            itr != m_Values.end();
            ++itr)
        {
            if(Num(*itr) != other.Num(*itr))
            {
                ret = false;
                break;
            }
        }

        return ret;
    }

	bool operator != (const Statistic &other) const
	{
		return !(*this == other);
	}
};

template<typename T>
std::ostream &operator << (std::ostream &ostrm, typename Statistic<T>::ValuesType &values)
{
	for(Statistic<T>::ValuesType::const_iterator itr = values.begin();
		itr != values.end();
		++itr)
	{
		ostrm << *itr <<" ";
	}
	return ostrm;
}

#endif
