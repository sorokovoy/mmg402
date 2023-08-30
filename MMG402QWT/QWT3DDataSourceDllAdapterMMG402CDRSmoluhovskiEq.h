
#ifndef QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq_H
#define QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq_H

#include "MMG402.h"
#include "IQWT3DDataSourceDllAdapterMMG402.h"

const std::string PressigeFuncSymbolName1 = "PressigeFunc";

#include "../MMG402Smoluhovski/MMG402SmoluhovskiDestructionOperator.h"
#include "../MMG402Smoluhovski/MMG402SmoluhovskiProductionOperator.h"
#include "../MMG402Smoluhovski/MMG402SmoluhovskiKore.h"


// Convection diffusion reaction and smoluhovski equalation
template<int SpaceDim, int SmoluhovskiDim>
class QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq : public IQWT3DDataSourceDllAdapterMMG402<SpaceDim, SmoluhovskiDim>
{
public:
  virtual bool Initialize()
  {
    MMG402Domain::IMMG402Domain<double, SpaceDim>* domain = new MMG402Domain::MMG402RectDomain<double, SpaceDim>(MMG402Domain::MMG402Rectangle<double, SpaceDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(-10), MMG402Domain::MMG402DomainPoint<double, SpaceDim>(10)));
    MMG402Grid::IMMG402Grid<double, SpaceDim>* grid = new MMG402Grid::MMG402GridUniform<double, SpaceDim>(domain,  new MMG402Grid::MMG402GridIndexerLinear<SpaceDim>(MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex(20)));

    MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, SmoluhovskiDim>& ti = *(new MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, SmoluhovskiDim>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal));

    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, SmoluhovskiDim>& solver = *(new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, SmoluhovskiDim>());
    MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, SmoluhovskiDim, 1>& Split = *(new MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, SmoluhovskiDim, 1>());

    MESymbol u("u");

    MESymbol Pressige(PressigeFuncSymbolName1);
    MESymbol Func("f");

    MMG402Func::IMMG402Func<double, SpaceDim, double, SmoluhovskiDim>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, SpaceDim, double, SmoluhovskiDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(0), 2, MMG402Domain::MMG402DomainPoint<double, SmoluhovskiDim>(10), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
    MMG402NumericData::MMG402NumDataFuncAdapter<double, SpaceDim, double, SmoluhovskiDim> pInitialData(grid, pInitial);

    ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, SpaceDim, double, SmoluhovskiDim>();
    ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, SpaceDim, double, SmoluhovskiDim>(pInitialData.CreateCopy(), 0));

    ti.pGrid = grid;
    ti.targetSymbol = u;
    ti.dt = 0.01;

    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1Center<double, 1, double, SmoluhovskiDim>& D1 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1Center<double, 1, double, SmoluhovskiDim>());
    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, SmoluhovskiDim>& D2 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, SmoluhovskiDim>());
    MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, SmoluhovskiDim>& DT = *(new MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, SmoluhovskiDim>());

    MMG402Solver::Temporal::MMG402OperatorCellSolver<double, SpaceDim, double, SmoluhovskiDim, double>& CellOp = *(new MMG402Solver::Temporal::MMG402OperatorCellSolver<double, SpaceDim, double, SmoluhovskiDim, double>());

    MESymbol spBase = DT(u) + D1(u, 0) + D2(u, 0);

    MESymbol expr;
    for (int i = 0; i < SpaceDim; i++) {
      expr += Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(i), new MMG402SplittingSchemeSolverFacrory<double, 1, double, SmoluhovskiDim>());
    }
    expr = (expr + CellOp(u, CreateSmoluhovskiSolver())) / (SpaceDim + 1);
    //expr = CellOp(u, CreateSmoluhovskiSolver());
    ti.equationExpression = expr + Func;


    MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, SmoluhovskiDim>* pBoundaryDataFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, SmoluhovskiDim>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, SmoluhovskiDim>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, SmoluhovskiDim>(pBoundaryDataFunc);

    pRigtPartFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, SmoluhovskiDim>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, SmoluhovskiDim>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f"));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, SmoluhovskiDim>(pRigtPartFunc);

    //pPressigeFuc = new MMG402Func::MMG402FuncTrancatedSphere<double, 2, double, 1>(MMG402Domain::MMG402DomainPoint<double, 2>(0, 0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(5), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, PressigeFuncSymbolName));
    pPressigeFuc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, SmoluhovskiDim>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, SmoluhovskiDim>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, PressigeFuncSymbolName));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, SmoluhovskiDim>(pPressigeFuc);

    // for splitting scheme support
    ti.pFuncStorage->SetFunc<double, 1, double, SmoluhovskiDim>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, SmoluhovskiDim, 1>(pRigtPartFunc));
    ti.pFuncStorage->SetFunc<double, 1, double, SmoluhovskiDim>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, SmoluhovskiDim, 1>(pPressigeFuc));
    ti.pFuncStorage->SetFunc<double, 1, double, SmoluhovskiDim>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, SmoluhovskiDim, 1>(pBoundaryDataFunc));

    solver.Init(&ti);

    _pInput = &ti; // (MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>*) input.pSolver->GetInput();
    _pEvaluatedData = ((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, SmoluhovskiDim>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->pData;
    return true;
  }


  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1>* CreateSmoluhovskiSolver()
  {
    int pointCount = SmoluhovskiDim;
    MMG402Domain::MMG402RectDomain<double, 1>& domain = *(new MMG402Domain::MMG402RectDomain<double, 1>(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1.00E-010), MMG402Domain::MMG402DomainPoint<double, 1>(1))));
    MMG402Grid::MMG402GridUniform<double, 1>& grid = * (new MMG402Grid::MMG402GridUniform<double, 1>(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount))));

    MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1>& ti = *(new MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1>(MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal));

    MMG402SmoluhovskiDestructionOperator<double, double, 1>&  Dest = *(new MMG402SmoluhovskiDestructionOperator<double, double, 1>());
    MMG402SmoluhovskiProductionOperator<double, double, 1>& Prod = *(new MMG402SmoluhovskiProductionOperator<double, double, 1>());

    MESymbol u("u");
    MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

    ti.pGrid = &grid;
    ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount));

    ti.targetSymbol = u;

    // set equalation expression
    MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc = new SmoluhovskiKore1<double, double>();
    MESymbol dest = Dest(u, pCoreFunc);
    MESymbol prod = Prod(u, pCoreFunc);
    ti.equationSubExpressions.push_back(dest);
    ti.equationSubExpressions.push_back(prod);

    ti.equationExpression = u * MMG402Expression::MMG402Exp(dest * dt) + (0.5 * prod * (1.00 - MMG402Expression::MMG402Exp(dest * dt))) / dest;
    ti.dt = 1.00E-003;
    // end

    ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();

    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>* pSol = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>();
    pSol->Init(&ti);

    return &ti;
  }

  virtual bool MakeIteration()
  {
    IQWT3DDataSourceDllAdapterMMG402<SpaceDim, SmoluhovskiDim>::MakeIteration();
    pPressigeFuc->SetTemporalValue(((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->t);
    return true;
  }

  virtual int DataSourcePluginGetDataGroupsCount()
  {
    return 2 + SmoluhovskiDim;
  }

  virtual bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf)
  {
    switch (iGroupId) {
      case 0:
        sprintf_s(oBuf, 255, "Pressige");
        return true;
      case 1:
        sprintf_s(oBuf, 255, "Source");
        return true;
      default:
        sprintf_s(oBuf, 255, "P-%d", iGroupId - 2);
        return true;
    }
    return false;
  }

  virtual bool DataSourcePluginGetValueByIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& index, int iGroupId, double& oValue)
  {
    MMG402Domain::MMG402DomainPoint<double, SpaceDim> point;

    switch (iGroupId) {
      case 0:
        _pInput->pGrid->IndexToCoord(index, point);
        oValue = pPressigeFuc->GetValue(point)[0];
        break;
      case 1:
        _pInput->pGrid->IndexToCoord(index, point);
        oValue = pRigtPartFunc->GetValue(point)[0];
        break;
      default:
        oValue = _pEvaluatedData->GetData(index)[iGroupId - 2];
        break;
    }
    return true;
  }

protected:
  MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, SmoluhovskiDim>* pPressigeFuc;
  MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, SmoluhovskiDim>* pRigtPartFunc;
};

#endif //QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq_H