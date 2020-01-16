#ifndef RESTATISTIC_H
#define RESTATISTIC_H

#include "Card.h"
#include "Statistic.h"
#include <vector>

class RestatisticResult
{
public:
    RestatisticResult();
    int key;
    int value;
};

class RestatisticMapperImpl
{
public:
    virtual ~RestatisticMapperImpl(){}
    virtual RestatisticResult Map(int key, int value) const = 0;
};

class RestatisticMapperRecycler
{
public:
    virtual ~RestatisticMapperRecycler(){}
    virtual void Recycle(RestatisticMapperImpl *impl) = 0;
};

class RestatisticMapper
{
    RestatisticMapperImpl *m_Impl;
    RestatisticMapperRecycler *m_Recycler;
public:
    RestatisticMapper(RestatisticMapperImpl *impl, RestatisticMapperRecycler *recycler);
    RestatisticResult Map(int key, int value) const;
    void Release();
};

class Restatistic
{
    std::vector<RestatisticMapper> m_Mappers;
public:
    void AddMapper(const RestatisticMapper &mapper);
    Statistic<int> Remap(const CardList &cards) const;
    void Release();
};

#endif
