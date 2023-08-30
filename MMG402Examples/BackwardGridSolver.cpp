
#include "MMG402.h"
#include "MMG402Examples.h"

#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzerExpGrid.h"

void PrintGrid(GUAutoPtr<MMG402NumericData::IMMG402NumData<double, 2, double, 1>> ipGrid)
{
  MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = ipGrid->GetMasterGrid()->GetIndexer()->GetIndexMaxValue();
  for (unsigned int i = 0; i < index(0); i++) {
    for (unsigned int j = 0; j < index(1); j++) {
      MMG402Grid::IMMG402GridIndexer<2>::GridIndex curr;
      curr(0) = i;
      curr(1) = j;
      printf(" %5.2f ", ipGrid->GetData(curr)(0));
    }
    printf("\n");
  }
  printf("\n");
}

void BackwardGridSolver()
{
  // prepare data for backward solver
  MMG402Domain::IMMG402Domain<double, 2>* domain = new MMG402Domain::MMG402RectDomain<double, 2>(MMG402Domain::MMG402Rectangle<double, 2>(MMG402Domain::MMG402DomainPoint<double, 2>(-5, -5), MMG402Domain::MMG402DomainPoint<double, 2>(5, 5)));
  MMG402Grid::IMMG402Grid<double, 2>* grid = new MMG402Grid::MMG402GridUniform<double, 2>(domain,  new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(30, 30)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 2, double, 1> ti;

  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 2, double, 1> D1;
  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 2, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 2, double, 1> DT;
  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 2, double, 1> solver;

  MESymbol u("u");
  MESymbol Func("f");

  MESymbol eq = DT(u) + D2(u, 0) + D2(u, 1) + Func;

  MMG402Func::IMMG402Func<double, 2, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, 2, double, 1>(MMG402Domain::MMG402DomainPoint<double, 2>(0, 0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(0.1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 2, double, 1> pInitialData(grid, pInitial);

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>();
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 2, double, 1>(pInitialData.CreateCopy(), 0));

  ti.pGrid = grid;
  ti.equationExpression = eq;
  ti.targetSymbol = u;
  ti.dt = 0.02;

  ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));
  ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f")));


  solver.Init(&ti);

  // make iterations
  for (int i = 0; i < 120; i++) {
    solver.MakeIteration();
  }

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 2, double, 1>* pOut = solver.GetOutput();

  MMG402BackwardSolver::MMG402RegressionAnalyzerInput<double, 2, double, 1> rinp;
  rinp.averageLayerCount = 100;

  //rinp.dependencyVariables.push_back(D1(u, 0) + D1(u, 1));
  rinp.dependencyVariables.push_back(D2(u, 0) + D2(u, 1));

  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);
  rinp.equationExpression = (MESymbol(MMG402BackwardSolver::UT1) - u) / dt;
  rinp.dt = ti.dt;
  rinp.pData = solver.GetOutput()->pData;
  rinp.pFuncStorage = ti.pFuncStorage;
  rinp.pGrid = ti.pGrid;
  rinp.targetSymbol = ti.targetSymbol;

  MMG402BackwardSolver::MMG402RegressionAnalyzerExpGrid<double, 2, double, 1> analyzer;
  analyzer.Init(&rinp);

  analyzer.MakeIteration();
  const MMG402BackwardSolver::MMG402RegressionAnalyzerExpGridOutput<double, 2, double, 1>* pOut1 = analyzer.GetOutput();

  for (int i = 0; i < 100; i++) {
    analyzer.MakeIteration();
  }
  PrintGrid(pOut1->dependencyCoefs[0]);
  //PrintGrid(pOut1->dependencyCoefs[1]);
}
