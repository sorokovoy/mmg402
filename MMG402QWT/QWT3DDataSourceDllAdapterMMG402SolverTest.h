#ifndef MMG402EqDataSource_H
#define MMG402EqDataSource_H

#include "MMG402.h"
#include "IQWT3DDataSourceDllAdapterMMG402.h"

#include "..\MMG402Testing\MMG402TestingEngineTemporal.h"

template<int SpaceDim>
class QWT3DDataSourceDllAdapterMMG402SolverTest : public IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>
{
public:
  virtual bool Initialize()
  {
    MMG402TestingEngineTemporalTestParams<double, SpaceDim, double, 1> input;
    input.pDomain = new MMG402Domain::MMG402RectDomain<double, SpaceDim>(MMG402Domain::MMG402Rectangle<double, SpaceDim>(MMG402Domain::MMG402DomainPoint<double, SpaceDim>(-5), MMG402Domain::MMG402DomainPoint<double, SpaceDim>(5)));

    int gridSize = 40;
    input.pGrid   = new MMG402Grid::MMG402GridUniform<double, SpaceDim>(input.pDomain, new MMG402Grid::MMG402GridIndexerLinear<SpaceDim>(MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex(gridSize)));

    input.pSolver = new MMG402SolverTemporalUniversal<double, SpaceDim, double, 1>();

    input.baseFunc = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(2, 0);
    input.dt = 0.01;
    input.iterationCount = 5;
    input.operatorId = 1;
    input.operatorType = MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;

    MMG402TestingEngineTemporal<double, SpaceDim, double, 1>::Prepare(input);
    _pInput = (MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, 1>*) input.pSolver->GetInput();
    _pRealFunc =  dynamic_cast<IMMG402FuncTemporal<double, SpaceDim, double, 1>*>(_pInput->pFuncStorage->GetFunc<double, SpaceDim, double, 1>(MMG402FuncStorageKey(MMG402FuncDestination_BoundaryData)));
    _pEvaluatedData = ((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->pData;

    return true;
  }

  virtual bool MakeIteration()
  {
    IQWT3DDataSourceDllAdapterMMG402<SpaceDim, 1>::MakeIteration();
    _pRealFunc->SetTemporalValue(((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, 1>*) _pInput->pSolver->GetOutput())->pData->Pop(0)->t);
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
        sprintf_s(oBuf, 255, "Real value");
        return true;
      case 1:
        sprintf_s(oBuf, 255, "Calculated value");
        return true;
      case 2:
        sprintf_s(oBuf, 255, "Diff value");
        return true;
    }
    return false;
  }

  virtual bool DataSourcePluginGetValueByIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& index, int iGroupId, double& oValue)
  {
    MMG402Domain::MMG402DomainPoint<double, SpaceDim> point;
    _pInput->pGrid->IndexToCoord(index, point);
    double realValue = _pRealFunc->GetValue(point)[0];
    double calculatedValue = _pEvaluatedData->GetData(index)[0];

    switch (iGroupId) {
      case 0:
        oValue = realValue;
        break;
      case 1:
        oValue = calculatedValue;
        break;
      case 2:
        oValue = GUAbs(realValue - calculatedValue);
        break;
      default:
        return false;
    }
    return true;
  }

protected:
  IMMG402FuncTemporal<double, SpaceDim, double, 1>* _pRealFunc;
};

#endif

