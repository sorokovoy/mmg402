#ifndef MMG402TestingEngineTemporalConvergenceTest_H
#define MMG402TestingEngineTemporalConvergenceTest_H

#include "MMG402.h"
#include "MMG402TestingEngineTemporalValidatorL2.h"

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
struct MMG402TestingEngineTemporalConvergenceTestInput {
  MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolverInput;
  MMG402Func::IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTargetFunc;
  IMMG402TestingEngineTemporalResultValidator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pValidator;

  double Tmin;
  double Tmax;
  int devisionBegin;
  int devisionCount;
};

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalConvergenceTest
{
public:
  void RunTest(MMG402TestingEngineTemporalConvergenceTestInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iInput)
  {
    GUAssertR(iInput.Tmin < iInput.Tmax);
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pInitialData = iInput.pSolverInput->pData->Pop(0)->pData->CreateCopy();
    delete iInput.pSolverInput->pData;

    int currentDevision = iInput.devisionBegin;
    MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolverAsTemporal = (MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)(iInput.pSolverInput->pSolver);

    for (int iter = 0; iter < iInput.devisionCount; iter++) {
      GUTimerInit(calcTime);

      iInput.pSolverInput->pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
      iInput.pSolverInput->pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(pInitialData->CreateCopy(), iInput.Tmin));
      iInput.pSolverInput->pData->SetStorageMaxDeep(currentDevision + 1);
      iInput.pSolverInput->dt = (iInput.Tmax - iInput.Tmin) / currentDevision;

      pSolverAsTemporal->SetDataProxyReference(iInput.pSolverInput->pData->Pop(0)->pData);
      for (int i = 0; i < currentDevision; i++) {
        iInput.pSolverInput->pSolver->MakeIteration();
      }

      const MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pOut = (MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)iInput.pSolverInput->pSolver->GetOutput();

      iInput.pValidator->PrepareData(pOut, *iInput.pTargetFunc);
      std::stringstream ss;
      iInput.pValidator->Print(ss);
      printf("ST %4.2E -> %s Time %f\n", iInput.pSolverInput->dt, ss.str().c_str(), GUTimerGetDiffInSec(calcTime));
      GUTimerReset(calcTime);

      delete iInput.pSolverInput->pData;
      currentDevision = currentDevision * 2;
    }
    printf("\n");
  }
};

#endif