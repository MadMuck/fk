#ifndef EXTENSION_PROCEDURE_CONTROLLER_H
#define EXTENSION_PROCEDURE_CONTROLLER_H

#include "GameContext.h"
#include "ProcedureController.h"

class ExtensionProcedureController : public GameContextExtension
{
protected:
    ProcedureController *m_ProcedureController;
public:
    ExtensionProcedureController();
    ~ExtensionProcedureController();

    void OnGameStart();

    void OnGameFinish();

	void OnGameReset();

    ProcedureController *GetProcedureController();

    int RunningProcedureType() const;

	const ProcedureController::JobList &GetProcedureHistory() const;

	void ClearHistory();

	bool IsInLatestHistory(int procedureType) const;

    void SetProcedure(int procedureType, const Procedure &procedure);

    void StartProcedure(int procedureType);

    void Update(float deltaTime);

	void Clear();

	void SetProcedureChangedCallback(ProcedureChangedCallback *callback);

    int LatestProcedureType() const;
};

#endif
