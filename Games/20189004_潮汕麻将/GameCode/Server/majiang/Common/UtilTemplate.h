#ifndef UTIL_TEMPLATE_H
#define UTIL_TEMPLATE_H

namespace Util
{
    template<typename InputItr, typename T>
    T Average(InputItr begin, InputItr end, T init)
    {
        T accum = init;
        for(InputItr itr = begin; itr != end; ++itr)
        {
            accum += *itr;
        }

        accum /= (end - begin);
        return accum;
    }

    template<typename InputItr, typename T>
    T Variance(InputItr begin, InputItr end, T avg)
    {
        T diffAccum = T(0);
        for(InputItr itr = begin; itr != end; ++itr)
        {
            T diff = *itr - avg;
            diffAccum += (diff * diff);
        }
        diffAccum /= (end - begin);
        return diffAccum;
    }

	template<typename KeyType, typename ValueType>
	struct SortData
	{
		KeyType key;
		ValueType value;

		SortData(){}

		SortData(const KeyType &k, const ValueType &v):key(k), value(v){}

		bool operator < (const SortData &other) const
		{
			return key < other.key;
		}

		bool operator <= (const SortData &other) const
		{
			return key <= other.key;
		}

		bool operator > (const SortData &other) const
		{
			return key > other.key;
		}

		bool operator >= (const SortData &other) const
		{
			return key >= other.key;
		}

		bool operator == (const SortData &other) const
		{
			return !(*this < other) && !(*this > other);
		}

		bool operator != (const SortData &other) const
		{
			return !(*this == other);
		}
	};
}

#endif
