
#ifndef IMMG402TestingEngineTemporalValidator_H
#define IMMG402TestingEngineTemporalValidator_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Domain\IMMG402Domain.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\GUtils\GUAssert.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverDataStorageTemporal.h"

#include "IMMG402TestingEngineTemporalResultPrinter.h"
#include "MMG402TestingSymbolFuncAdapter.h"

class MExp::MESymbol;

int c = 0;

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class IMMG402TestingEngineTemporalResultValidator : public IMMG402TestingEngineTemporalResultPrinter
{
public:
  virtual void PrepareData(const MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipOuput, const MExp::MESymbol& iTarget)
  {
    OnInit();
    MMG402TestingSymbolFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim> func(MMG402FuncStorageKey(MMG402FuncDestination_Special), iTarget);
    RunLayerValidation(ipOuput, func);
  }

  virtual void PrepareData(const MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipOuput, MMG402Func::IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iTargetFunc)
  {
    _pOuput = ipOuput;
    OnInit();
    RunLayerValidation(ipOuput, iTargetFunc);
  }

  virtual bool AnalyzeResults() = 0;

  virtual void Print(std::ostream& out) const = 0;
  virtual int  GetId() const = 0;

protected:
  virtual void RunLayerValidation(const MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipOuput, MMG402Func::IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& func)
  {
    _totalLayers = ipOuput->pData->GetCurrentStorageDeep() - 1;
    for (unsigned int layer = 0; layer < ipOuput->pData->GetCurrentStorageDeep(); layer++) {
      _currentLayer = layer;

      if (!OnLayerValidationStart()) continue;
      const MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* lay = ipOuput->pData->Pop(ipOuput->pData->GetCurrentStorageDeep() - 1 - layer);
      func.SetTemporalValue(lay->t);
      if(layer != _totalLayers) {
        _currentDt = GUAbs(ipOuput->pData->Pop(ipOuput->pData->GetCurrentStorageDeep() - 1 - (layer + 1))->t - lay->t);
      }

      // iterate on the grid
      const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* pGrid = lay->pData->GetMasterGrid();
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* indexer = pGrid->GetIndexer();

      FILE* f;

      char fbuf[20];
      sprintf(fbuf, "cmp%d.txt", c);
      c++;
      fopen_s(&f, fbuf, "w");
      fprintf(f, "exactValue, numericalValue \n");
      for (unsigned int it = 0; it < indexer->GetIteratorMaxValue(); it++) {
        MMG402GridIndex curentIndex;
        GUAssertC(indexer->IteratorToIndex(it, curentIndex));

        MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> domainPoint;
        GUAssertC(pGrid->IndexToCoord(curentIndex, domainPoint));

        MMG402NumData exactValue = func.GetValue(domainPoint);
        MMG402NumData numericalValue = lay->pData->GetData(curentIndex);
        fprintf(f, " %f %f \n", exactValue, numericalValue);
        ProcessValuePair(curentIndex, domainPoint, numericalValue, exactValue);
      }
      fclose(f);
      OnLayerValidationFinish();
    }
  }

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pOuput;

  unsigned int _currentLayer;
  unsigned int _totalLayers;
  double _currentDt;

  virtual void OnInit() {};

  virtual bool OnLayerValidationStart() = 0;
  virtual void OnLayerValidationFinish() = 0;
  virtual void ProcessValuePair(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue) = 0;
};

#endif
