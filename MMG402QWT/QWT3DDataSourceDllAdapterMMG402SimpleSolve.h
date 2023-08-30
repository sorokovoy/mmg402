#ifndef QWT3DDataSourceDllAdapterMMG402SimpleSolve_H
#define QWT3DDataSourceDllAdapterMMG402SimpleSolve_H

#include "MMG402.h"
#include "IQWT3DDataSourceDllAdapterMMG402.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialAverage.h"

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402SplittingSchemeSolverFacrory : public MMG402Solver::Temporal::IMMG402SplittingSchemeSolverFacrory<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateInstance() const
  {
    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolver = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
    MMG402SolverTemporalUniversalFlags flags = pSolver->GetSolverFlags();
    flags.matrixType = MMG402Matrix::MMG402MatrixType::MMG402MatrixType3Diagonal;
    pSolver->SetSolverFlags(flags);
    return pSolver;
  }
};

template<int SpaceDim>
class QWT3DDataSourceDllAdapterMMG402SimpleSolve : public IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>
{
public:
  virtual bool Initialize()
  {
    MMG402Domain::IMMG402Domain<double, SpaceDim>* domain = new MMG402Domain::MMG402RectDomain<double, SpaceDim>(MMG402Domain::MMG402Rectangle<double, SpaceDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(-5), MMG402Domain::MMG402DomainPoint<double, SpaceDim>(5)));
    MMG402Grid::IMMG402Grid<double, SpaceDim>* grid = new MMG402Grid::MMG402GridUniform<double, SpaceDim>(domain,  new MMG402Grid::MMG402GridIndexerLinear<SpaceDim>(MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex(100)));

    MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>& ti = *(new MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>());

    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>& solver = *(new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, 1, 1>& Split = *(new MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, 1, 1>());

    MESymbol u("u");
    MESymbol Func("f");

    MMG402Func::IMMG402Func<double, SpaceDim, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, SpaceDim, double, 1>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(5), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));

    MMG402NumericData::MMG402NumDataFuncAdapter<double, SpaceDim, double, 1> pInitialData(grid, pInitial);

    ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, SpaceDim, double, 1>();
    ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, SpaceDim, double, 1>(pInitialData.CreateCopy(), 0));
    ti.pGrid = grid;

    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1>& D1 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1>& D2 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1>& DT = *(new MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1>());

    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialAverage<double, SpaceDim, double, 1>& DA = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialAverage<double, SpaceDim, double, 1>());

    MESymbol spBase = DT(u) + D1(u, 0, false);
    MESymbol sp1 = Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(0), new MMG402SplittingSchemeSolverFacrory<double, 1, double, 1>());
    MESymbol sp2 = Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(1), new MMG402SplittingSchemeSolverFacrory<double, 1, double, 1>());

    ti.equationExpression = (sp1);

    ti.targetSymbol = u;
    ti.dt = 0.01;

    MMG402Func::IMMG402Func<double, SpaceDim, double, 1>* pBoundary = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pBoundary);

    MMG402Func::IMMG402Func<double, SpaceDim, double, 1>* pRightPart = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f"));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pRightPart);

    // add reduction funcs for support splitting operator
    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pBoundary));
    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pRightPart));
    solver.Init(&ti);

    _pInput = &ti;
    _pEvaluatedData = ((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->pData;
    return true;
  }

  virtual bool MakeIteration()
  {
    IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>::MakeIteration();
    return true;
  }

  virtual int DataSourcePluginGetDataGroupsCount()
  {
    return 1;
  }

  virtual bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf)
  {
    switch (iGroupId) {
      case 0:
        sprintf_s(oBuf, 255, "Calculated value");
        return true;
    }
    return false;
  }

  virtual bool DataSourcePluginGetValueByIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& index, int iGroupId, double& oValue)
  {
    MMG402Domain::MMG402DomainPoint<double, SpaceDim> point;
    _pInput->pGrid->IndexToCoord(index, point);
    double calculatedValue = _pEvaluatedData->GetData(index)[0];

    switch (iGroupId) {
      case 0:
        oValue = calculatedValue;
        break;
      default:
        return false;
    }
    return true;
  }
};

#endif

