#ifndef PROCEDURE_CHANGED_DELEGATE_H
#define PROCEDURE_CHANGED_DELEGATE_H

#include "Procedure.h"

template<typename T>
class ProcedureChangedDelegate : public ProcedureChangedCallback
{
	T *m_Delegate;
public:

	ProcedureChangedDelegate(T *delegate):m_Delegate(delegate)
	{
	}

	void OnBeforeStart(int latest, int current)
	{
		m_Delegate->OnBeforeProcedureStart(latest, current);
	}

	void OnChanged(int latest, int current)
	{
		m_Delegate->OnProcedureChanged(latest, current);
	}

};

#endif;