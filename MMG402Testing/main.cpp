//#include <iostream>

#include "MMG402UnitTest.h"

#include "MMG402TestingRegistryFucntion.h"
#include "MMG402TestingRegistryDomain.h"
#include "MMG402TestingRegistryGrid.h"
#include "MMG402TestingRegistryEquationOperatorsExplicitTemporal.h"
#include "MMG402TestingRegistryEquationOperatorsImplicitTemporal.h"

#include "MMG402TestingEngineTemporalValidatorErrorEstimate.h"

#include "MMG402\libs\GUtils\GURandom.h"

#include "MMG402TestingOperatorSymbolicArifmetic.h"
#include "MMG402TestingOperatorSymbolicDx.h"
#include "MMG402TestingEngineTemporal.h"


#include "MMG402\MMG402Domain\MMG402RectDomain.h"

#include "MMG402\MMG402Grid\MMG402GridUniform.h"
#include "MMG402\MMG402Grid\MMG402GridIndexerLinear.h"

#include "MMG402\MMG402NumericData\MMG402NumDataDensy.h"
#include "MMG402\MMG402NumericData\MMG402NumDataFuncAdapter.h"
#include "MMG402\MMG402NumericData\MMG402NumDataFuncAdapter.h"

#include "MMG402\MMG402Expression\MMG402SymbolArifmetic.h"

#include "MMG402\MMG402Func\MMG402FuncTrancatedSphere.h"
#include "MMG402\MMG402Func\MMG402FuncConstant.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitDt.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitDt.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverTemporalUniversal.h"

#include <stdio.h>
#include "MMG402\MMG402Domain\MMG402DomainPoint.h"

void RunAllDiffShemeTest();

int main(void)
{
  GURand::BaseRand();
  RunAllUnitTest();
  RunAllDiffShemeTest();
  return 0;
}

template<int SpaceDim>
void RunDiffShemeTest(MMG402Expression::MMG402OperatorSchemeType iShemeType, double Dt)
{
  int funcCount = MMG402TestingRegistryFucntion::GetInstance().GetTFucntionCount(SpaceDim);
  int opCount = -1;
  if(iShemeType == MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal) {
    opCount = MMG402TestingRegistryEquationOperatorsExplicitTemporal::GetCount();
  } else if(iShemeType == MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal) {
    opCount = MMG402TestingRegistryEquationOperatorsImplicitTemporal::GetCount();
  } else {
    throw new std::exception("Not implemented");
  }

  for(int f = 0; f < funcCount; f++) {
    for(int o = 0; o < opCount; o++) {
      RunDiffShemeTest<SpaceDim>(iShemeType, o, f, Dt);
    }
  }
}

template<int SpaceDim>
void RunDiffShemeTest(MMG402Expression::MMG402OperatorSchemeType iShemeType, int iOperatorId, int iFuncId, double Dt)
{
  MMG402TestingEngineTemporalTestParams<double, SpaceDim, double, 1> input;
  input.pDomain = new MMG402RectDomain<double, SpaceDim>(MMG402Rectangle<double, SpaceDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(-5), MMG402Domain::MMG402DomainPoint<double, SpaceDim>(5)));
  input.pGrid   = new MMG402GridUniform<double, SpaceDim>(input.pDomain, new MMG402GridIndexerLinear<SpaceDim>(IMMG402GridIndexer<SpaceDim>::GridIndex(40)));
  //input.pGridIndexator = new MMG402GridIndexerLinear<SpaceDim>(IMMG402GridIndexer<SpaceDim>::GridIndex(30));

  MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>* pSolver = new MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>();
  MMG402SolverTemporalUniversalFlags flags;
  flags.IsNoMatrixRecalculate = false;
  pSolver->SetSolverFlags(flags);
  input.pSolver = pSolver;

  input.baseFunc = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(SpaceDim, iFuncId);
  input.dt = Dt;
  input.iterationCount = 50;
  input.operatorId = iOperatorId;
  input.operatorType = iShemeType;

  std::vector<IMMG402TestingEngineTemporalResultValidator<double, SpaceDim, double, 1>*> validators;
  //validators.push_back(new MMG402TestingEngineTemporalValidatorErrorEstimateLastLayer<double, SpaceDim, double, 1>());
  validators.push_back(new MMG402TestingEngineTemporalValidatorErrorEstimateOnAllLayers<double, SpaceDim, double, 1>());

  std::vector<IMMG402TestingEngineTemporalResultPrinter*>& res = MMG402TestingEngineTemporal<double, SpaceDim, double, 1>::RunTest(input, validators);
  MMG402TestingEngineTemporal<double, SpaceDim, double, 1>::PrintAllResults(res, input, true);
}

void RunAllDiffShemeTest()
{
  //RunDiffShemeTest<2>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal, 1, 0, 0.01);
  RunDiffShemeTest<2>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal, 0.01);
  //RunDiffShemeTest<2>(MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal,  0.01);
 // RunDiffShemeTest<1>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal,  0.005);
 // printf("========================\n");
  //RunDiffShemeTest<2>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal,  0.005);
}
