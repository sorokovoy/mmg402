#ifndef MMG402SolverTemporalUniversal_H
#define MMG402SolverTemporalUniversal_H

#include "MMG402\libs\MExp\IMECalculator.h"
#include "MMG402\libs\GUtils\GUAssert.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverTemporalUniversalFlags.h"

#include "MMG402\MMG402Expression\IMMG402OperatorUnary.h"
#include "MMG402\MMG402Expression\IMMG402OperatorBinary.h"
#include "MMG402\MMG402Expression\MMG402SymbolArifmetic.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataGrid.h"

#include "MMG402\MMG402NumericData\MMG402NumDataFuncAdapter.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSolver.h"


namespace MMG402Solver
{
  namespace Temporal
  {
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402SolverTemporalUniversal : public IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402SolverTemporalUniversal()
      {
        pMatrixSolver = 0;
      }

      MMG402SolverTemporalUniversal(const MMG402SolverTemporalUniversalFlags& iFlags)
      {
        pMatrixSolver = 0;
        _flags = iFlags;
      }

      virtual ~MMG402SolverTemporalUniversal()
      {
        delete pMatrixSolver;
      }

      void SetSolverFlags(const MMG402SolverTemporalUniversalFlags& iFlags)
      {
        _flags = iFlags;
      }

      const MMG402SolverTemporalUniversalFlags& GetSolverFlags() const
      {
        return _flags;
      }

      virtual bool MakeIteration()
      {
        _pInput->pFuncStorage->SetTemporalValue(_pInput->pData->Pop(0)->t + _pInput->dt);

        bool setNoMatrixRecalculate = false;
        if (GetSolverFlags().IsNoMatrixRecalculate && pStoredMatrix[0] == 0) {
          setNoMatrixRecalculate = true;
          _flags.IsNoMatrixRecalculate = false;
        }
        IMESymbolData* pNewDataR = Evaluate();
        if (setNoMatrixRecalculate) {
          _flags.IsNoMatrixRecalculate = true;
        }

        GUAssert((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(pNewDataR) != 0));
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pNewData = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)(pNewDataR);

        // prepare matrix
        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pMatrixData = pNewData->GetMatrixData();

        // in case no implicit operators used
        if (pMatrixData != 0) {
          const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIndexer = _pInput->pGridIterator;
          unsigned int n = pGridIndexer->GetIteratorMaxValue();

          for (unsigned int i = 0; i < n; i++) {
            MMG402GridIndex index;
            GUAssertV(pGridIndexer->IteratorToIndex(i, index), false);
            pMatrixData->Value(i, n) -= pNewData->GetGridData()->GetData(index);
          }

          // solve matrix
          SolveMatrix(pNewData);
        }

        // extract result
        _currentDataProxyReference = pNewData->GetGridData()->CreateCopy();
        _pInput->pData->Push(new MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_currentDataProxyReference, _pInput->pData->Pop(0)->t + _pInput->dt));

        delete pNewDataR;
        return true;
      }

    protected:

      MMG402SolverTemporalUniversalFlags _flags;

      // IMECalculator
      virtual IMESymbolData* Evaluate() const
      {
        return IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::Evaluate();
      }

      virtual GUAutoPtr<IMESymbolData> EvaluateExp(const MESymbol& iSymb) const
      {
        if (iSymb.GetSymbolType() == MESymbolType_Constant) {
          return _table.GetContantParser()->Parse(iSymb);
        } else if (iSymb.GetSymbolType() == MESymbolType_Symbol) {
          return _table.GetData(iSymb.GetSymbolName())->CreateCopy();
        } else {
          const IMESymbolOperatorUnary* pUnaryOp = QueryUnaryOperator(iSymb);
          if (pUnaryOp != 0) {
            GUAutoPtr<IMESymbolData> pData = EvaluateExp(iSymb.GetArguments()[0]);
            pUnaryOp->Evaluate(pData, iSymb);
            return pData;
          }

          const IMESymbolOperatorBinary* pBinaryOp = QueryBinaryOperator(iSymb);
          if (pBinaryOp != 0) {
            GUAutoPtr<IMESymbolData> pLeft = 0;
            const IMESymbolOperatorUnary* pDtOperator = 0;
            MESymbol DtSymbol;

            for (unsigned int i = 0; i < iSymb.GetArguments().size(); i++) {
              // DT operator elevated last
              // !TODO fix it
              if (iSymb.GetArguments()[i].GetSymbolType() == MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt) {
                GUAssert(pDtOperator == 0);
                GUAssert(iSymb.GetArguments()[i].GetArguments().size() == 1);
                //GUAssert(iSymb.GetArguments()[i].GetArguments()[0] == _pInput->targetSymbol);

                // DT operator allowed only on targetSymbol variable
                DtSymbol = iSymb.GetArguments()[i];
                pDtOperator = QueryUnaryOperator(DtSymbol);
                continue;
              }

              if (pLeft == 0) {
                pLeft = EvaluateExp(iSymb.GetArguments()[i]);
                continue;
              }

              GUAutoPtr<IMESymbolData> pRight = EvaluateExp(iSymb.GetArguments()[i]);
              pLeft = pBinaryOp->Evaluate(pLeft, pRight, iSymb);
            }
            if (pDtOperator)
              pDtOperator->Evaluate(pLeft, DtSymbol);
            return pLeft;
          }
        }
        std::stringstream ss;
        ss << "Operator " << iSymb.GetSymbolType() << " not found in symbol table. Use validator before evaluation.";
        throw std::exception(ss.str().c_str());
      }


      MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>* pMatrixSolver;
      Eigen::SparseMatrix<DataScalarType>* pStoredMatrix[DataDim];

      void SolveMatrix(MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData)
      {
        const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIndexer = _pInput->pGridIterator;
        unsigned int n = pGridIndexer->GetIteratorMaxValue();
        if (pMatrixSolver == 0) {
          // create solver and set solver flags
          pMatrixSolver = new MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>();
          pMatrixSolver->GetSolverFlags().IsNoSparceMatrixChange = GetSolverFlags().IsNoMatrixRecalculate;
        }
        std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> res = pMatrixSolver->Solve(ipData->GetMatrixData());
        for (unsigned int i = 0; i < n; i++) {
          MMG402GridIndex index;
          GUAssert(pGridIndexer->IteratorToIndex(i, index));
          ipData->GetGridData()->GetData(index) = res[i];
        }
      }

    };
  } // end namespace Temporal
}// end namespace MMG402Solver
#endif
