
#include "MMG402.h"
#include "MMG402Examples.h"

#include "MMG402IO\MMG402IOSingle1DCImage.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialAverage.h"

void SampleDim1OperatorCompare()
{
  MMG402Domain::IMMG402Domain<double, 1>* domain = new MMG402Domain::MMG402RectDomain<double, 1>(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(-5), MMG402Domain::MMG402DomainPoint<double, 1>(5)));
  MMG402Grid::IMMG402Grid<double, 1>* grid = new MMG402Grid::MMG402GridUniform<double, 1>(domain,  new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(400)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1> DT;
  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialAverage<double, 1, double, 1> DA;
  

  MESymbol u("u");
  MESymbol Func("f");

  MMG402Func::IMMG402Func<double, 1, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, 1, double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> pInitialData(grid, pInitial);

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(pInitialData.CreateCopy(), 0));

  ti.pGrid = grid;
  ti.equationExpression = DT(u) + D2(u, 0);
  ti.targetSymbol = u;
  ti.dt = 0.001;

  ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f")));


  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1> solver;
  solver.Init(&ti);

  int itemsSize = 1600;
  solver.GetOutput()->pData->SetStorageMaxDeep(itemsSize);


  for(int i = 0; i < itemsSize; i ++) {
    solver.MakeIteration();
  }

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 1, double, 1>* pOut = solver.GetOutput();

  MMG402IO::MMG402IOSingle1DCImage<double, double> writer("grad.bmp", 0, 1);
  writer.Write(pOut->pData, "D2.bmp");

  solver.GetOutput()->pData->Clear();

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1> solver2;
  ti.equationExpression = DT(u) + 10 * DA(u) + Func;
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(pInitialData.CreateCopy(), 0));

  solver2.Init(&ti);
  for(int i = 0; i < itemsSize; i ++) {
    solver2.MakeIteration();
  }

  pOut = solver2.GetOutput();
  writer.Write(pOut->pData, "DA.bmp");
}