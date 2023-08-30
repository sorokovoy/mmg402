#ifndef IMMG402SolverTemporal_H
#define IMMG402SolverTemporal_H

#include "MMG402\libs\MExp\IMECalculator.h"

#include "MMG402\MMG402Expression\MMG402SymbolFunc.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataScalar.h"

#include "MMG402\MMG402Func\MMG402FuncStorageKey.h"

#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Solver\IMMG402Solver.h"

#include "MMG402\MMG402Solver\MMG402SolverInput.h"
#include "MMG402\MMG402Solver\MMG402SolverOutput.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverDataStorageTemporal.h"

namespace MMG402Solver
{
  namespace Temporal
  {
    namespace Symbols
    {
      // current time symbol
      static const std::string t = "t";

      // name of symbol what you can use for take current delta T value in your expression
      static const std::string DeltaT = "DeltaT";

      // name of symbol what you can use for create zerowed symbol grid
      static const std::string ZeroGrid = "ZeroGrid";
    }

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402SolverTemporal;

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    struct MMG402SolverInputTemporal : public MMG402SolverInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim> 
    {
      MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData;
      double dt;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    struct MMG402SolverOutputTemporal : public MMG402SolverOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim> 
    {
      MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pInput;
      MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402SolverTemporal : public IMMG402Solver<SpaceScalarType, SpaceDim, DataScalarType, DataDim>, protected virtual IMECalculator
    {
    public:
      IMMG402SolverTemporal()
      {
        _deltaT = 0;
        _t = 0;
        _currentDataProxyReference = 0;
      }

      virtual ~IMMG402SolverTemporal()
      {
        delete _deltaT;
        delete _t;
        for (unsigned int i = 0; i < _subExpressionProxyReference.size(); i++)
          delete _subExpressionProxyReference[i];
      }

      virtual void Init(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
      {
        IMMG402Solver<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::Init(ipInput);
        _pInput = ipInput;
        _pOutput = new MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
        PrepareExpression(_pInput);
      }

      // from IMECalculator
      virtual bool Validate(bool iShowException = false) const
      {
        GUAssertV(IMECalculator::Validate(iShowException), false);
        for (unsigned int i = 0; i < _pInput->equationSubExpressions.size(); i++) {
          GUAssertV(ValidateExp( _pInput->equationSubExpressions[i], iShowException), false);
        }
        return true;
      }

      virtual bool MakeIteration() = 0;

      virtual const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*  GetInput()  const
      {
        return _pInput;
      }
      virtual const MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetOutput() const
      {
        _pOutput->pData = _pInput->pData;
        _pOutput->pInput = _pInput;
        return _pOutput;
      }

      void SetDataProxyReference(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipCurrentDataProxyReference)
      {
        _currentDataProxyReference = ipCurrentDataProxyReference;
      }

    protected:
      // from IMECalculator
      virtual MExp::IMESymbolData* Evaluate() const
      {
        // set current t value 
        _t->Value() = GetOutput()->pData->Pop(0)->t + _deltaT->Value();

        GUAssert(_currentDataProxyReference != 0);
        for (unsigned int i = 0; i < _pInput->equationSubExpressions.size(); i++) {
          delete *_subExpressionProxyReference[i];
          *_subExpressionProxyReference[i] = EvaluateExp(_pInput->equationSubExpressions[i]).ExtractPtr();
        }
        return EvaluateExp(_pInput->equationExpression).ExtractPtr();
      }

      virtual GUAutoPtr<MExp::IMESymbolData> EvaluateExp(const MExp::MESymbol& iSymb) const = 0;

      virtual const MExp::IMESymbolOperatorUnary* QueryUnaryOperator(const MExp::MESymbol& iSymb) const
      {
        const MExp::IMESymbolOperatorUnary* res = IMECalculator::QueryUnaryOperator(iSymb);
        if (res != 0) return res;
        void* pTemporalImpementator = iSymb.GetExtensionObject(MMG402OperatorExtensionNameTemporalImplementator);
        if (pTemporalImpementator != 0) {
          // GUAssert((dynamic_cast<IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>((MMG402Expression::IMMG402Operator*)pTemporalImpementator)));
          IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pImplementator = (IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)(pTemporalImpementator);
          _table.AddUnaryOperator(pImplementator->CreateImplementation(_pInput));
          return IMECalculator::QueryUnaryOperator(iSymb);
        }
        return 0;
      }

      virtual const MExp::IMESymbolOperatorBinary* QueryBinaryOperator(const MExp::MESymbol& iSymb) const
      {
        return IMECalculator::QueryBinaryOperator(iSymb);
      }

      void PrepareExpression(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
      {
        MMG402Expression::MMG402SymbolArifmeticAddAllSymbols<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_pInput->symbolTable);
        MMG402Expression::MMG402SymbolFuncAddAllSymbols<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_pInput->symbolTable);

        if (ipInput->pData->Pop(0) != 0) {
          _currentDataProxyReference = ipInput->pData->Pop(0)->pData;
        }

        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pCurrent = new MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(&_currentDataProxyReference);
        ipInput->symbolTable.SetData(ipInput->targetSymbol.GetSymbolName(), pCurrent);

        const std::set<MMG402Func::MMG402FuncStorageKey>& allFuncKeys = ipInput->pFuncStorage->GetKeys();
        for (std::set<MMG402Func::MMG402FuncStorageKey>::const_iterator fit = allFuncKeys.begin(); fit != allFuncKeys.end(); fit++) {
          ipInput->symbolTable.SetData(fit->FuncName, new MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(new MMG402NumericData::MMG402NumDataFuncAdapter<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipInput->pGrid, ipInput->pFuncStorage->GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(*fit))));
        }

        // add predefined symbols
        _deltaT = new MMG402Expression::MMG402SymbolDataScalar<double>(&_pInput->dt);
        ipInput->symbolTable.SetData(Symbols::DeltaT, _deltaT);

        _t = new MMG402Expression::MMG402SymbolDataScalar<double>(new double(0));
        ipInput->symbolTable.SetData(Symbols::t, _t);

        ipInput->symbolTable.SetData(Symbols::ZeroGrid, new MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipInput->pGrid)));

        SubExpressionsPrepare();

        SetSymbolTable(ipInput->symbolTable);
        for (unsigned int i = 0; i < _pInput->equationSubExpressions.size(); i++) {
          ValidateExp(_pInput->equationSubExpressions[i], false);
        }

        for (unsigned int i = 0; i < _pInput->equationSubExpressions.size(); i++)
          *_subExpressionProxyReference[i] = 0;
      }

      virtual void SubExpressionsPrepare()
      {
        for (unsigned int i = 0; i < _pInput->equationSubExpressions.size(); i++) {
          // check is sub expression is expression
          GUAssertC(_pInput->equationSubExpressions[i].GetSymbolType() != MESymbolType_Zero && _pInput->equationSubExpressions[i].GetSymbolType() != MESymbolType_Constant && _pInput->equationSubExpressions[i].GetSymbolType() != MESymbolType_Symbol);

          // create sub expression result
          IMESymbolData** pCurrProxyReference = new IMESymbolData*((IMESymbolData*)(1));
          _subExpressionProxyReference.push_back(pCurrProxyReference);

          // replace sub expression
          char subExpressionName[32];
          sprintf_s(subExpressionName, 32, "Exp_%d", i);
          MESymbol subExp(subExpressionName);
          for (unsigned int j = i + 1; j < _pInput->equationSubExpressions.size(); j++) {
            _pInput->equationSubExpressions[j] = _pInput->equationSubExpressions[j].Replace(_pInput->equationSubExpressions[i], subExp);
          }
          _pInput->equationExpression = _pInput->equationExpression.Replace(_pInput->equationSubExpressions[i], subExp);
          SubExpressionsOperateReplace(_pInput->equationSubExpressions[i], subExp);

          _pInput->symbolTable.SetData(subExpressionName, pCurrProxyReference);
        }
      }

      virtual void SubExpressionsOperateReplace(const MExp::MESymbol& iSource, const MExp::MESymbol& iTarget)
      {}

      MMG402Expression::MMG402SymbolDataScalar<double>* _deltaT;
      MMG402Expression::MMG402SymbolDataScalar<double>* _t;

      MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _currentDataProxyReference;
      std::vector<MExp::IMESymbolData**> _subExpressionProxyReference;


      MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*  _pInput;
      MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pOutput;
    };

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif // IMMG402SolverTemporal
