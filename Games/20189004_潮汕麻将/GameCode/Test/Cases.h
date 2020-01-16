#ifndef CASES_H
#define CASES_H

#include "TestScore.h"
#include "TestWinType.h"
#include "TestPaiXing.h"
#include "TestLiquidation.h"

#define RunCase(CaseName) Test##CaseName().Run()

#endif