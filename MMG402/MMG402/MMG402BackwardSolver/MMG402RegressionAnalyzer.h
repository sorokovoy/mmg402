#ifndef MMG402RegressionAnalyzer_H
#define MMG402RegressionAnalyzer_H

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MECalculatorDeep.h"

#include "MMG402\MMG402Matrix\MMG402MatrixDensy.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSolver.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverTemporalUniversal.h"

#include <list>

namespace MMG402BackwardSolver
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  struct MMG402RegressionAnalyzerInput  : MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
    MMG402RegressionAnalyzerInput()
    {
      pDataFocus = 0;
    }

    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, bool, 1>* pDataFocus;

    std::vector<MExp::MESymbol> dependencyVariables;
    unsigned int averageLayerCount;
  };



// name of symbol for targetSymbol at t + 1
  static const std::string UT1 = "U T+1";

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402RegressionAnalyzer : public MMG402Solver::Temporal::MMG402SolverTemporalUniversal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402RegressionAnalyzer()
    {
      _lastTValue = -1.00E010;
    }

    virtual ~MMG402RegressionAnalyzer()
    {}

    virtual void Init(MMG402RegressionAnalyzerInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
    {
      _pInput = ipInput;
      _pInput->symbolTable.SetData(UT1, new MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(&_nextDataProxyReference));
      _lastTValue = _pInput->pData->Pop(_pInput->pData->GetCurrentStorageDeep() - 1)->t;
      IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::Init(ipInput);
    }

    // from IMMG402SolverTemporal
    virtual bool Validate(bool iShowException = false) const
    {
      GUAssertV(IMMG402SolverTemporal::Validate(iShowException), false);
      for (unsigned int i = 0; i < _pInput->dependencyVariables.size(); i++) {
        GUAssertV(IMMG402SolverTemporal::ValidateExp(_pInput->dependencyVariables[i], iShowException), false);
      }
      return true;
    }

    virtual bool MakeIteration()
    {
      // find minimal tvalue > _lastTValue
      _currentDataProxyReference = 0;
      _nextDataProxyReference = 0;
      for (unsigned int i = 0; i < _pInput->pData->GetCurrentStorageDeep(); i++) {
        MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pCurrDataItem = _pInput->pData->Pop(_pInput->pData->GetCurrentStorageDeep() - 1 - i);
        GUAssertC(pCurrDataItem);
        if (pCurrDataItem->t > _lastTValue) {
          MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pNextDataItem = _pInput->pData->Pop(_pInput->pData->GetCurrentStorageDeep() - 1 - i + 1);
          if (pNextDataItem == 0) continue;
          _nextDataProxyReference = pNextDataItem->pData;

          // set current delta t value
          _deltaT->Value() = pCurrDataItem->t - _lastTValue;
          _lastTValue = pCurrDataItem->t;
          _currentDataProxyReference = pCurrDataItem->pData;
          break;
        }
      }
      if (_currentDataProxyReference == 0 || _nextDataProxyReference == 0) return false;
      _pInput->pFuncStorage->SetTemporalValue(_lastTValue);

      MExp::IMESymbolData* pRightExp = IMMG402SolverTemporal::Evaluate();
      std::vector<MExp::IMESymbolData*> variables;
      for (unsigned int i = 0; i < _pInput->dependencyVariables.size(); i++) {
        variables.push_back(EvaluateExp(_pInput->dependencyVariables[i]).ExtractPtr());
      }
      bool res = ProcessData(variables, pRightExp);
      delete pRightExp;
      for (unsigned int i = 0; i < variables.size(); i++)
        delete variables[i];
      return res;
    }

    virtual const MMG402RegressionAnalyzerInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*  GetInput()  const
    {
      return _pInput;
    }

  protected:
    virtual void SubExpressionsOperateReplace(const MESymbol& iSource, const MESymbol& iTarget)
    {
      for (unsigned int i = 0; i < _pInput->dependencyVariables.size(); i++) {
        _pInput->dependencyVariables[i] =  _pInput->dependencyVariables[i].Replace(iSource, iTarget);
      }
    }

    virtual bool ProcessData(std::vector<MExp::IMESymbolData*>& iVariables, MExp::IMESymbolData* iTargetExp)
    {
      GUAssertV((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(iTargetExp) != 0), false);
      for (unsigned int i = 0; i < iVariables.size(); i++) {
        GUAssertV((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(iVariables[i]) != 0), false);
      }

      std::vector<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*> variables;
      for (unsigned int i = 0; i < iVariables.size(); i++) {
        MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData = ((MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) iVariables[i])->GetGridData();
        GUAssertV(pData, false);
        GUAssertV(!((MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) iVariables[i])->IsHasMatrixData(), false);
        variables.push_back(pData);
      }

      MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTargetExp = ((MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) iTargetExp)->GetGridData();
      GUAssertV(pTargetExp, false);
      GUAssertV(!((MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) iTargetExp)->IsHasMatrixData(), false);
      return ProcessDataFocus(variables, pTargetExp);
    }

    virtual bool ProcessDataFocus(std::vector<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>& iVariables, MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* iTargetExp)
    {
      if (_pInput->pDataFocus == 0) {
        return ProcessData(iVariables, iTargetExp);
      }

      for (unsigned int i = 0; i < iVariables.size(); i++) {
        ProcessDataFocus(iVariables[i]);
      }
      ProcessDataFocus(iTargetExp);
      return ProcessData(iVariables, iTargetExp);
    }

    virtual void ProcessDataFocus(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData)
    {
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIndexer = _pInput->pGridIterator;
      GUAssertR(ipData->GetMasterGrid()->GetIndexer()->GetIteratorMaxValue() == pGridIndexer->GetIteratorMaxValue());
      GUAssertR(_pInput->pDataFocus->GetMasterGrid()->GetIndexer()->GetIteratorMaxValue() == pGridIndexer->GetIteratorMaxValue());

      for (unsigned int i = 0; i < pGridIndexer->GetIteratorMaxValue(); i++) {
        MMG402GridIndex index = pGridIndexer->IteratorToIndex(i);
        if (!_pInput->pDataFocus->GetData(index)(0)) {
          ipData->GetData(index) = 0;
        }
      }
    }

    virtual bool ProcessData(std::vector<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>& iVariables, MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* iTargetExp) = 0;
    virtual bool EvaluateSolution() = 0;

    MMG402RegressionAnalyzerInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*  _pInput;
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _nextDataProxyReference;

    double _lastTValue;
  };

}; // end namespace MMG402BackvardSolve
#endif // MMG402RegressionAnalyzer_H
