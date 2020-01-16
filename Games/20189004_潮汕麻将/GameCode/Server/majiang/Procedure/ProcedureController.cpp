#include "ProcedureController.h"

ProcedureController::ProcedureController():m_RunningProcedureType(ProcedureType::Idle),
m_LatestProcedureType(ProcedureType::Idle),m_ProcedureChangedCallback(NULL)
{}

int ProcedureController::RunningProcedureType() const
{
    return m_RunningProcedureType;
}

const ProcedureController::JobList &ProcedureController::GetProcedureHistory() const
{
	return m_JobsHistory;
}

void ProcedureController::ClearHistory()
{
	m_JobsHistory.clear();
}

bool ProcedureController::IsInLatestHistory(int procedureType) const
{
	bool ret = false;
	int recallCnt = 0;
	for(JobList::const_iterator itr = m_JobsHistory.begin();
		itr != m_JobsHistory.end();
		++itr, ++recallCnt)
	{
		if(*itr == procedureType)
		{
			ret = true;
			break;
		}
	}
	return ret;
}

void ProcedureController::SetProcedure(int procedureType, const Procedure &procedure)
{
    m_Procedures.erase(procedureType);
    std::pair<std::map<int, Procedure>::iterator, bool > res = m_Procedures.insert(std::make_pair(procedureType,procedure));
	res.first->second.SetType(procedureType);
}

void ProcedureController::StartProcedure(int procedureType)
{
    m_Jobs.push_back(procedureType);
}

void ProcedureController::Update(float deltaTime)
{
    if(m_Jobs.size() > 0)
    {
        ProcedureList::iterator itr = m_Procedures.find(m_LatestProcedureType);
        if(itr != m_Procedures.end())
        {
            // 前一个过程结束
            itr->second.OnEnd();
        }

        m_LatestProcedureType = m_RunningProcedureType;
        m_RunningProcedureType = *m_Jobs.begin();
        m_Jobs.pop_front();
        itr = m_Procedures.find(m_RunningProcedureType);

		if(itr != m_Procedures.end())
		{
			itr->second.OnPrepare();
		}

		m_JobsHistory.push_front(m_RunningProcedureType);
		if(m_JobsHistory.size() > 8)
		{
			m_JobsHistory.pop_back();
		}

		if(m_ProcedureChangedCallback)
		{
			m_ProcedureChangedCallback->OnBeforeStart(m_LatestProcedureType, m_RunningProcedureType);
		}

        if(itr != m_Procedures.end())
        {
            itr->second.OnStart();
        }

		if(m_ProcedureChangedCallback)
		{
			m_ProcedureChangedCallback->OnChanged(m_LatestProcedureType, m_RunningProcedureType);
		}
    }
}

int ProcedureController::LatestProcedureType() const
{
    return m_LatestProcedureType;
}

void ProcedureController::Clear()
{
    m_Procedures.clear();
    m_Jobs.clear();
	m_JobsHistory.clear();
    m_RunningProcedureType = m_LatestProcedureType = ProcedureType::Idle;
	m_ProcedureChangedCallback = NULL;
}

void ProcedureController::SetProcedureChangedCallback(ProcedureChangedCallback *callbackObj)
{
	m_ProcedureChangedCallback = callbackObj;
}
