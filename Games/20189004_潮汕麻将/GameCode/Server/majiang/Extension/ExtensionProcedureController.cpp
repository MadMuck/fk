#include "ExtensionProcedureController.h"
#include "ProcedureType.h"

ExtensionProcedureController::ExtensionProcedureController()
{
    m_ProcedureController = new ProcedureController();
}

ExtensionProcedureController::~ExtensionProcedureController()
{
    delete m_ProcedureController;
}

void ExtensionProcedureController::OnGameStart()
{
    StartProcedure(ProcedureType::GameStart);
}

void ExtensionProcedureController::OnGameFinish()
{
    StartProcedure(ProcedureType::Liquidation);
}

void ExtensionProcedureController::OnGameReset()
{
	Clear();
}

ProcedureController *ExtensionProcedureController::GetProcedureController()
{
    return m_ProcedureController;
}

int ExtensionProcedureController::RunningProcedureType() const
{
    return m_ProcedureController->RunningProcedureType();
}

const ProcedureController::JobList &ExtensionProcedureController::GetProcedureHistory() const
{
	return m_ProcedureController->GetProcedureHistory();
}

void ExtensionProcedureController::ClearHistory()
{
	m_ProcedureController->ClearHistory();
}

bool ExtensionProcedureController::IsInLatestHistory(int procedureType) const
{
	return m_ProcedureController->IsInLatestHistory(procedureType);
}

void ExtensionProcedureController::SetProcedure(int procedureType, const Procedure &procedure)
{
    m_ProcedureController->SetProcedure(procedureType, procedure);
}

void ExtensionProcedureController::StartProcedure(int procedureType)
{
    m_ProcedureController->StartProcedure(procedureType);
}

void ExtensionProcedureController::Update(float deltaTime)
{
    m_ProcedureController->Update(deltaTime);
}

void ExtensionProcedureController::Clear()
{
	m_ProcedureController->Clear();
}

void ExtensionProcedureController::SetProcedureChangedCallback(ProcedureChangedCallback *callback)
{
	m_ProcedureController->SetProcedureChangedCallback(callback);
}

int ExtensionProcedureController::LatestProcedureType() const
{
    return m_ProcedureController->LatestProcedureType();
}
