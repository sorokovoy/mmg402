
#include "MMG402.h"
#include "MMG402Examples.h"

void runSimpleImplicit()
{
  MMG402Domain::IMMG402Domain<double, 2>* domain = new MMG402Domain::MMG402RectDomain<double, 2>(MMG402Domain::MMG402Rectangle<double, 2>(MMG402Domain::MMG402DomainPoint<double, 2>(-5, -5), MMG402Domain::MMG402DomainPoint<double, 2>(5, 5)));
  MMG402Grid::IMMG402Grid<double, 2>* grid = new MMG402Grid::MMG402GridUniform<double, 2>(domain,  new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(100, 100)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 2, double, 1> ti;

  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 2, double, 1> D1;
  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 2, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorImplicitDt<double, 2, double, 1>   DT;
  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 2, double, 1> solver;

  MESymbol u("u");
  MESymbol Func("f");

  MESymbol eq = DT(u) + D1(u, 0) + D1(u, 1) + D2(u, 0) + D2(u, 1) + Func;

  MMG402Func::IMMG402Func<double, 2, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, 2, double, 1>(MMG402Domain::MMG402DomainPoint<double, 2>(0, 0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(0), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 2, double, 1> pInitialData(grid, pInitial);

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>();
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 2, double, 1>(pInitialData.CreateCopy(), 0));

  ti.pGrid = grid;
  ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(100, 100));
  ti.equationExpression = eq;
  ti.targetSymbol = u;
  ti.dt = 0.1;

  ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));
  ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f")));


  solver.Init(&ti);

  solver.MakeIteration();
  solver.MakeIteration();
  solver.MakeIteration();

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 2, double, 1>* pOut = solver.GetOutput();
}