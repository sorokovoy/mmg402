#ifndef MMG402TestingEngineTemporal_H
#define MMG402TestingEngineTemporal_H

#include "MMG402\libs\MExp\MESymbol.h"

#include "MMG402\MMG402Domain\IMMG402Domain.h"
#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"
#include "MMG402\MMG402Solver\MMG402SolverOutput.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"

#include "IMMG402TestingEngineTemporalResultValidator.h"

#include "MMG402TestingEngineTemporalTimeMesher.h"
#include "MMG402TestingRegistryEquationOperatorsExplicitTemporal.h"
#include "MMG402TestingRegistryEquationOperatorsImplicitTemporal.h"
#include "MMG402TestingSymbolFuncAdapter.h"
#include "MMG402TestingOperatorSymbolicElevator.h"
#include "MMG402\MMG402Func\MMG402FuncStorage.h"

#include <vector>

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
struct MMG402TestingEngineTemporalTestParams {
  MESymbol baseFunc;

  MMG402Expression::MMG402OperatorSchemeType operatorType;
  int operatorId;

  // will be setted automatic in MMG402TestingEngineTemporal::Prepare
  MESymbol diffOperatorSymbol;

  MMG402Domain::IMMG402Domain<SpaceScalarType, SpaceDim>* pDomain;
  MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* pGrid;
  int iterationCount;
  double dt;
  MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolver;
};

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporal
{
public:
  static bool RunAutoTest(MMG402TestingEngineTemporalTestParams<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iParams, const std::vector<IMMG402TestingEngineTemporalResultValidator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*> iValidators)
  {
    Prepare(iParams);
    for (int i = 0; i < iParams.iterationCount; i++) {
      iParams.pSolver->MakeIteration();
    }

    const MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pOut = iParams.pSolver->GetOutput();
    for (unsigned int i = 0; i < iValidators.size(); i++) {
      iValidators[i]->PrepareData(pOut, iParams.baseFunc);
      result.push_back(iValidators[i]);
    }

    for (unsigned int i = 0; i < iValidators.size(); i++) {
      if (!iValidators[i]->AnalyzeResults()) {
        std::stringstream message;
        message << "Error at result " << iValidators[i]->GetId() << " ";
        iValidators[i]->Print(message);
        std::cout << message.str() << std::endl;
        hasNegative = true;
      }
    }
  }

  static std::vector<IMMG402TestingEngineTemporalResultPrinter*> RunTest(MMG402TestingEngineTemporalTestParams<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iParams, const std::vector<IMMG402TestingEngineTemporalResultValidator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*> iValidators)
  {
    std::vector<IMMG402TestingEngineTemporalResultPrinter*> result;
    Prepare(iParams);
    std::cout << "Base func : " << iParams.baseFunc << std::endl;
    std::cout << "Operator  : " << iParams.diffOperatorSymbol << std::endl;

    MMG402TestingEngineTemporalTimeMesher* timeMesher = new MMG402TestingEngineTemporalTimeMesher();
    for (int i = 0; i < iParams.iterationCount; i++) {
      timeMesher->Set();
      iParams.pSolver->MakeIteration();
      timeMesher->Loop();
    }
    result.push_back(timeMesher);

    const MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pOut = iParams.pSolver->GetOutput();
    for (unsigned int i = 0; i < iValidators.size(); i++) {
      iValidators[i]->PrepareData(pOut, iParams.baseFunc);
      result.push_back(iValidators[i]);
    }
    return result;
  }

  static void PrintAllResults(const std::vector<IMMG402TestingEngineTemporalResultPrinter*>& iResults, const MMG402TestingEngineTemporalTestParams<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iParams, bool IsNoPreambula)
  {
    std::stringstream message;
    if(!IsNoPreambula) {
      message << "Base func : " << iParams.baseFunc << std::endl;
      message << "Operator  : " << iParams.diffOperatorSymbol << std::endl;
    }

    for (unsigned int i = 0; i < iResults.size(); i++) {
      iResults[i]->Print(message);
    }
    std::cout << message.str() << std::endl;
  }

  static void Prepare(MMG402TestingEngineTemporalTestParams<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iParams)
  {
    MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& ti = *(new MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
    MExp::MESymbol u("u"); // targetSymbol
    MExp::MESymbol func("f"); // right part
    MExp::MESymbol dOperator;

    if (iParams.operatorType == MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal) {
      dOperator = MMG402TestingRegistryEquationOperatorsExplicitTemporal::CreateOperator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iParams.operatorId, u, func);
    } else if (iParams.operatorType == MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal) {
      dOperator = MMG402TestingRegistryEquationOperatorsImplicitTemporal::CreateOperator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iParams.operatorId, u, func);
    } else {
      throw new std::exception("Preparation for this sheme type not supported");
    }
    iParams.diffOperatorSymbol = dOperator;
    //std::cout << dOperator << std::endl;

    MExp::MESymbol dRightPart = MMG402TestingOperatorSymbolicElevator::GetInstance().EvaluateDfOperator(iParams.baseFunc, dOperator);
    //std::cout << iParams.baseFunc << std::endl;
    dRightPart = -dRightPart;
    //std::cout << dRightPart << std::endl;

    MMG402Func::IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pInitial = new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_InitialData), iParams.baseFunc);
    MMG402NumericData::MMG402NumDataFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim> pInitialData(iParams.pGrid, pInitial);

    ti.pData = new MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
    ti.pData->SetStorageMaxDeep(iParams.iterationCount + 1);
    ti.pData->Push(new MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(pInitialData.CreateCopy(), 0));

    ti.pGrid = iParams.pGrid;
    ti.equationExpression = dOperator;
    ti.targetSymbol = u;
    ti.dt = iParams.dt;

    //std::cout << dOperator << std::endl;

    MMG402Func::MMG402FuncStorage* fs = new MMG402Func::MMG402FuncStorage();
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_BoundaryData), iParams.baseFunc));
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_RightAdditionalFunction, "f"), dRightPart));
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Special, "x"), MESymbol("x")));
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Special, "y"), MESymbol("y")));
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Special, "z"), MESymbol("z")));
    fs->SetFunc(new MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Special, "v"), MESymbol("v")));
    ti.pFuncStorage = fs;

    iParams.pSolver->Init(&ti);
  }
};

#endif // MMG402TestingEngineTemporal_H
