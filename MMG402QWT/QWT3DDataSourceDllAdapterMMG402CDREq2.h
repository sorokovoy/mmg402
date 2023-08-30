
#ifndef QWT3DDataSourceDllAdapterMMG402CDREq2_H
#define QWT3DDataSourceDllAdapterMMG402CDREq2_H

#include "MMG402.h"
#include "IQWT3DDataSourceDllAdapterMMG402.h"


#include "..\MMG402Smoluhovski\MMG402OperatorCDR.h"

const std::string PressigeFuncSymbolName2 = "PressigeFunc11";


// Convection diffusion reaction equalation
template<int SpaceDim>
class QWT3DDataSourceDllAdapterMMG402CDREq2 : public IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>
{
public:
  virtual bool Initialize()
  {
    MMG402Domain::IMMG402Domain<double, SpaceDim>* domain = new MMG402Domain::MMG402RectDomain<double, SpaceDim>(MMG402Domain::MMG402Rectangle<double, SpaceDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(-10), MMG402Domain::MMG402DomainPoint<double, SpaceDim>(10)));
    MMG402Grid::IMMG402Grid<double, SpaceDim>* grid = new MMG402Grid::MMG402GridUniform<double, SpaceDim>(domain,  new MMG402Grid::MMG402GridIndexerLinear<SpaceDim>(MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex(50)));

    MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>& ti = *(new MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>());

    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, SpaceDim, double, 1>& FD1 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, SpaceDim, double, 1>());
//  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1Center<double, SpaceDim, double, 1>& FD1 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1Center<double, SpaceDim, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, SpaceDim, double, 1>& FD2 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, SpaceDim, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, SpaceDim, double, 1>& FDT = *(new MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, SpaceDim, double, 1>());

    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>& solver = *(new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>());
    MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, 1, 1>& Split = *(new MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, SpaceDim, double, 1, 1>());

    MESymbol u("u");

    MESymbol Pressige(PressigeFuncSymbolName2);
    MESymbol Func("f");

    MMG402Func::IMMG402Func<double, SpaceDim, double, 1>* pInitial = new MMG402Func::MMG402FuncTrancatedSphere<double, SpaceDim, double, 1>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(5), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_InitialData));
    MMG402NumericData::MMG402NumDataFuncAdapter<double, SpaceDim, double, 1> pInitialData(grid, pInitial);

    ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>();
    ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 2, double, 1>(pInitialData.CreateCopy(), 0));

    ti.pGrid = grid;
    ti.targetSymbol = u;
    ti.dt = 0.01;

    MMG402Solver::Temporal::MMG402OperatorCDR<double, double, 1, SpaceDim>& CDR = *(new MMG402Solver::Temporal::MMG402OperatorCDR<double, double, 1, SpaceDim>());
    MESymbol spBase = CDR(u);
   
    MESymbol expr;
    for (int i = 0; i < SpaceDim; i++) {
      expr += Split(spBase, MMG402Domain::MMG402DomainPoint<int, 1>(i), new MMG402SplittingSchemeSolverFacrory<double, 1, double, 1>());
    }
   // ti.equationSubExpressions.push_back(expr);
    ti.equationExpression = expr / SpaceDim;
    //ti.equationExpression = expr;
    //ti.equationExpression = FDT(u) + FD1(u, 0, false) + FD1(u, 1, false) - FD2(u, 0) - FD2(u, 1);;

    MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, 1>* pBoundaryDataFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pBoundaryDataFunc);

    pRigtPartFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f"));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pRigtPartFunc);

    //pPressigeFuc = new MMG402Func::MMG402FuncTrancatedSphere<double, 2, double, 1>(MMG402Domain::MMG402DomainPoint<double, 2>(0, 0), 2, MMG402Domain::MMG402DomainPoint<double, 1>(5), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, PressigeFuncSymbolName));
    pPressigeFuc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, PressigeFuncSymbolName));
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pPressigeFuc);


    MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>* pMuFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, 1>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, 1>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "Mu"));
    MMG402Func::MMG402FuncConstant<double, SpaceDim, double, SpaceDim>* pUFunc = new MMG402Func::MMG402FuncConstant<double, SpaceDim, double, SpaceDim>(MMG402NumericData::IMMG402NumData<double, SpaceDim, double, SpaceDim>::EqNumDataType(5), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "U"));

    ti.pFuncStorage->SetFunc<double, SpaceDim, double, 1>(pMuFunc);
    ti.pFuncStorage->SetFunc<double, SpaceDim, double, SpaceDim>(pUFunc);

    // for splitting scheme support
    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pRigtPartFunc));
    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pPressigeFuc));
    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pBoundaryDataFunc));

    ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, 1, 1>(pMuFunc));
    ti.pFuncStorage->SetFunc<double, 1, double, SpaceDim>(new MMG402Func::MMG402FuncSpaceDimReductor<double, SpaceDim, double, SpaceDim, 1>(pUFunc));

    solver.Init(&ti);

    _pInput = &ti; // (MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>*) input.pSolver->GetInput();
    _pEvaluatedData = ((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->pData;
    return true;
  }

  virtual bool MakeIteration()
  {
    IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>::MakeIteration();
    pPressigeFuc->SetTemporalValue(((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->t);
    return true;
  }

  virtual int DataSourcePluginGetDataGroupsCount()
  {
    return 3;
  }

  virtual bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf)
  {
    switch (iGroupId) {
      case 0:
        sprintf_s(oBuf, 255, "Value");
        return true;
      case 1:
        sprintf_s(oBuf, 255, "Pressige");
        return true;
      case 2:
        sprintf_s(oBuf, 255, "Source");
        return true;
    }
    return false;
  }

  virtual bool DataSourcePluginGetValueByIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& index, int iGroupId, double& oValue)
  {
    MMG402Domain::MMG402DomainPoint<double, SpaceDim> point;

    switch (iGroupId) {
      case 0:
        oValue = _pEvaluatedData->GetData(index)[0];
        break;
      case 1:
        _pInput->pGrid->IndexToCoord(index, point);
        oValue = pPressigeFuc->GetValue(point)[0];
        break;
      case 2:
        _pInput->pGrid->IndexToCoord(index, point);
        oValue = pRigtPartFunc->GetValue(point)[0];
        break;
      default:
        return false;
    }
    return true;
  }

protected:
  MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, 1>* pPressigeFuc;
  MMG402Func::IMMG402FuncTemporal<double, SpaceDim, double, 1>* pRigtPartFunc;
};

#endif //QWT3DDataSourceDllAdapterMMG402CDREq2_H