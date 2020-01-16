#include "Procedure.h"

Procedure::Procedure(ProcedureImpl *impl):m_Impl(impl)
{}

void Procedure::OnPrepare()
{
	m_Impl->OnPrepare();
}

void Procedure::OnStart()
{
    m_Impl->OnStart();
}

void Procedure::OnEnd()
{
    m_Impl->OnEnd();
}
