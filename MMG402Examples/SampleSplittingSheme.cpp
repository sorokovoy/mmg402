
#include "MMG402.h"
#include "MMG402Examples.h"

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402SplittingSchemeSolverFacrory11111111111111 : public MMG402Solver::Temporal::IMMG402SplittingSchemeSolverFacrory<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateInstance() const
  {
    return new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
  }
};

void sampleSplittingSheme()
{
  MMG402Domain::IMMG402Domain<double, 2>* domain = new MMG402Domain::MMG402RectDomain<double, 2>(MMG402Domain::MMG402Rectangle<double, 2>(MMG402Domain::MMG402DomainPoint<double, 2>(-5, -5), MMG402Domain::MMG402DomainPoint<double, 2>(5, 5)));
  MMG402Grid::IMMG402Grid<double, 2>* grid = new MMG402Grid::MMG402GridUniform<double, 2>(domain,  new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(100, 100)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 2, double, 1> ti;

  //MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 2, double, 1> D1;
  //MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 2, double, 1> D2;

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 2, double, 1> solver;
  MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, 2, double, 1, 1> Split;

  MESymbol u("u");
  MESymbol Func("f");

  MMG402Func::IMMG402Func<double, 2, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, 2, double, 1>(MMG402Domain::MMG402DomainPoint<double, 2>(0, 0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(0), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 2, double, 1> pInitialData(grid, pInitial);

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>();
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 2, double, 1>(pInitialData.CreateCopy(), 0));

  ti.pGrid = grid;
  ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(100, 100));

  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 1, double, 1> D1;
  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 1, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorImplicitDt<double, 1, double, 1> DT;
  MESymbol spBase = DT(u) + D1(u, 0) + D2(u, 0);
  MESymbol sp1 = Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(0), new MMG402SplittingSchemeSolverFacrory11111111111111<double, 1, double, 1>());
  MESymbol sp2 = Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(1), new MMG402SplittingSchemeSolverFacrory11111111111111<double, 1, double, 1>());

  ti.equationExpression = (sp1 + sp2) / 2 + Func;
  ti.targetSymbol = u;
  ti.dt = 0.1;

  MMG402Func::IMMG402Func<double, 2, double, 1>* pBoundary = new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData));
  ti.pFuncStorage->SetFunc<double, 2, double, 1>(pBoundary);

  MMG402Func::IMMG402Func<double, 2, double, 1>* pRightPart = new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f"));
  ti.pFuncStorage->SetFunc<double, 2, double, 1>(pRightPart);

  // add reduction funcs for support splitting operator
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, 2, double, 1, 1>(pBoundary));
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, 2, double, 1, 1>(pRightPart));

  solver.Init(&ti);

  solver.MakeIteration();
  solver.MakeIteration();
  solver.MakeIteration();

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 2, double, 1>* pOut = solver.GetOutput();
}