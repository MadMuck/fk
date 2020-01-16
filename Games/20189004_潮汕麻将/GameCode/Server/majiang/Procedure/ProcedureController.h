/*
游戏过程控制器
*/

#ifndef PROCEDURE_CONTROLLER_H
#define PROCEDURE_CONTROLLER_H

#include "ProcedureType.h"
#include "Procedure.h"
#include <map>
#include <list>

class ProcedureChangedCallback
{
public:
	virtual ~ProcedureChangedCallback(){}
	virtual void OnBeforeStart(int latestType, int current) = 0;
	virtual void OnChanged(int latestType, int current) = 0;
};

class ProcedureController
{
public:
    typedef std::map<int, Procedure> ProcedureList;
    typedef std::list<int> JobList;

protected:
    ProcedureList m_Procedures;
    JobList m_Jobs;
	JobList m_JobsHistory;
    int m_RunningProcedureType;
    int m_LatestProcedureType;
	ProcedureChangedCallback *m_ProcedureChangedCallback;
public:
    ProcedureController();

    int RunningProcedureType() const;

	const JobList &GetProcedureHistory() const;

	void ClearHistory();

	bool IsInLatestHistory(int procedureType) const;

    void SetProcedure(int procedureType, const Procedure &procedure);

    void StartProcedure(int procedureType);

    void Update(float deltaTime);

    int LatestProcedureType() const;

    void Clear();

	void SetProcedureChangedCallback(ProcedureChangedCallback *callbackObj);
};

#endif
