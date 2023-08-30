#ifndef MMG402OperatorSplittingScheme_H
#define MMG402OperatorSplittingScheme_H

#include "MMG402.h"
#include "MMG402\libs\MExp\IMESymbolOperators.h"

#include "MMG402\libs\GUtils\GUAssert.h"
#include "MMG402\libs\GUtils\GUNullableType.h"

#include "MMG402\MMG402Expression\IMMG402OperatorUnary.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataGrid.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverDataStorageTemporal.h"

#include <set>

#define MMG402SolverOperatorRegistry_MMG402OperatorSplittingScheme 607

namespace MMG402Solver
{
  namespace Temporal
  {
    const std::string MMG402OperatorExtensionNameTemporalImplementator1 = "TemporalImplementator";
    const std::string MMG402OperatorExtensionVarSplittingSchemeOperatorData = "SplittingSchemeOperatorData";

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402SplittingSchemeSolverFacrory
    {
    public:
      virtual MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateInstance() const = 0;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, int ReducedSpaceDim>
    struct MMG402OperatorSplittingSchemeData {
      std::set<int> reduceOnDims;
      MExp::MESymbol equationExpression;
      IMMG402SplittingSchemeSolverFacrory<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pInnerSolverFactory;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, int ReducedSpaceDim>
    class MMG402OperatorSplittingSchemeImplementation : public IMESymbolOperatorUnary
    {
    public:
      enum { DataDimAtCompile  = DataDim  };
      enum { SpaceDimAtCompile = SpaceDim };

      MMG402OperatorSplittingSchemeImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput)
      {
        _pInput = ipMasterInput;
      }

      virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol& iSybmol) const
      {
        GUAssert(ipData->GetType() == MMG402SymbolDataType_Grid);
        GUAssertT((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pGrid = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) (ipData.GetPointer());

        MMG402OperatorSplittingSchemeData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>* pSpData = (MMG402OperatorSplittingSchemeData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>*)iSybmol.GetExtensionObject(MMG402OperatorExtensionVarSplittingSchemeOperatorData);

        std::vector<MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, unsigned int>*> reducedIndexes;
        std::vector<MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>*> reducedPoints;

        MMG402Domain::MMG402DomainSpaceDimReductor<SpaceScalarType, SpaceDim, ReducedSpaceDim>* pReducedDomain = new MMG402Domain::MMG402DomainSpaceDimReductor<SpaceScalarType, SpaceDim, ReducedSpaceDim>(_pInput->pGrid->GetDomain());
        reducedPoints.push_back(pReducedDomain);
        MMG402Grid::MMG402GridIndexerSpaceDimReductor<SpaceDim, ReducedSpaceDim>* pReducedIndexr = new MMG402Grid::MMG402GridIndexerSpaceDimReductor<SpaceDim, ReducedSpaceDim>(_pInput->pGridIterator);
        reducedIndexes.push_back(pReducedIndexr);
        MMG402Grid::MMG402GridSpaceDimReductor<SpaceScalarType, SpaceDim, ReducedSpaceDim>* pReducedGrid = new MMG402Grid::MMG402GridSpaceDimReductor<SpaceScalarType, SpaceDim, ReducedSpaceDim>(_pInput->pGrid, pReducedIndexr, pReducedDomain);
        reducedIndexes.push_back(pReducedGrid);

        // inner sheme initial data
        MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pDataStorage = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>();
        MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* priviosData = _pInput->pData->Pop(0)->pData;
        MMG402NumericData::MMG402NumDataSpaceDimReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>* pDataReductor = new MMG402NumericData::MMG402NumDataSpaceDimReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>(priviosData, pReducedGrid);
        reducedIndexes.push_back(pDataReductor);
        pDataStorage->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>(pDataReductor, _pInput->pData->Pop(0)->t));

        MMG402Solver::Temporal::IMMG402SolverTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pSolver = pSpData->pInnerSolverFactory->CreateInstance();
        MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim> ti;

        ti.dt =  _pInput->dt;
        ti.pData = pDataStorage;
        ti.targetSymbol = _pInput->targetSymbol;
        ti.equationExpression = pSpData->equationExpression;
        ti.pGrid = pReducedGrid;
        ti.pFuncStorage = _pInput->pFuncStorage;

        MExp::IMESymbolData* pTargetSymbolData = (MExp::IMESymbolData*) _pInput->symbolTable.GetData(ti.targetSymbol.GetSymbolName());
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTargetSymbolDataGrid = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) (pTargetSymbolData);
        MMG402NumericData::MMG402NumDataSpaceDimReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>* pTargetSymbolDataGridReduction = new MMG402NumericData::MMG402NumDataSpaceDimReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>(pTargetSymbolDataGrid->GetGridData(), pReducedGrid);
        reducedIndexes.push_back(pTargetSymbolDataGridReduction);
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pReducedSymbolDataGrid = new MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>(pTargetSymbolDataGridReduction);
        ti.symbolTable.SetData(ti.targetSymbol.GetSymbolName(), pReducedSymbolDataGrid);


        MMG402Domain::MMG402DomainPoint<GUNullableType<unsigned int>, SpaceDim> reductionIndexTemplate;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          reductionIndexTemplate(i) =  pSpData->reduceOnDims.find(i) == pSpData->reduceOnDims.end() ? 0 : GUNullableType<unsigned int>();
        }

        // assign current reduction index template
        for (unsigned int i = 0; i < reducedIndexes.size(); i++) {
          reducedIndexes[i]->SetReduction(reductionIndexTemplate);
        }

        pSolver->Init(&ti);

        MMG402GridIndex sourceMaxIndex = _pInput->pGrid->GetIndexer()->GetIndexMaxValue();
        MMG402Domain::MMG402DomainPoint<GUNullableType<unsigned int>, SpaceDim> reductionIndex = reductionIndexTemplate;
        while (true) {
          // get reduction point
          MMG402GridIndex currentReductionCoords;
          for (int i = 0; i < SpaceDimAtCompile; i++) {
            currentReductionCoords(i) = reductionIndex(i).HasValue() ? reductionIndex(i).GetValue() : 0;
          }
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> currentReductionPoint;
          GUAssert(_pInput->pGrid->IndexToCoord(currentReductionCoords, currentReductionPoint));

          MMG402Domain::MMG402DomainPoint<GUNullableType<SpaceScalarType>, SpaceDim> reductionPoint;
          for (int i = 0; i < SpaceDimAtCompile; i++) {
            reductionPoint(i) = reductionIndex(i).HasValue() ? currentReductionPoint(i) : GUNullableType<SpaceScalarType>();
          }

          // assign current reduction index
          for (unsigned int i = 0; i < reducedIndexes.size(); i++) {
            reducedIndexes[i]->SetReduction(reductionIndex);
          }

          ti.pFuncStorage->SetReduction<SpaceScalarType, SpaceDim, ReducedSpaceDim> (reductionPoint);
          for (unsigned int i = 0; i < reducedPoints.size(); i++) {
            reducedPoints[i]->SetReduction(reductionPoint);
          }

          pSolver->MakeIteration();

          // extract data
          MMG402NumericData::IMMG402NumData<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pCalculatedData = pSolver->GetOutput()->pData->Pop(0)->pData;
          for (unsigned int i = 0; i < pReducedIndexr->GetIteratorMaxValue(); i++) {
            MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex index;
            GUAssert(pReducedIndexr->IteratorToIndex(i, index));
            pGrid->GetGridData()->GetData(pReducedIndexr->GetOriginalIndex(index)) = pCalculatedData->GetData(index);
          }

          // prepare for next iteration
          pSolver->GetOutput()->pData->EraseFirst();
          pSolver->SetDataProxyReference(pSolver->GetOutput()->pData->Pop(0)->pData);

          // increment index;
          int dimIndex = 0;
          while (dimIndex < SpaceDimAtCompile) {
            if (!reductionIndex(dimIndex).HasValue()) {
              dimIndex++;
              continue;
            }
            reductionIndex(dimIndex).GetValue()++;
            if (reductionIndex(dimIndex).GetValue() == sourceMaxIndex(dimIndex)) {
              reductionIndex(dimIndex).GetValue() = 0;
              dimIndex++;
            } else {
              break;
            }
          }
          // reach max index -> exit loop
          if (dimIndex == SpaceDimAtCompile) {
            break;
          }
        }

        // delete created objects
        delete pDataStorage;
        delete pReducedGrid;
        delete pReducedIndexr;
        delete pReducedDomain;
        delete pReducedSymbolDataGrid;

        delete pSolver;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorSplittingScheme;
      }

    protected:
      const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pInput;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, int ReducedSpaceDim>
    class MMG402OperatorSplittingScheme : public MMG402Expression::IMMG402OperatorUnary<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput) const
      {
        return new MMG402OperatorSplittingSchemeImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>(ipMasterInput);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_Undefined;
      }

      MExp::MESymbol operator() (MExp::MESymbol& iSymb, MMG402Domain::MMG402DomainPoint<int, ReducedSpaceDim> iReduceOnDims, IMMG402SplittingSchemeSolverFacrory<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* ipSolverFactory)
      {
        MESymbol res = MESymbol(MESymbol(Symbols::ZeroGrid) , MMG402SolverOperatorRegistry_MMG402OperatorSplittingScheme, GUToString("Split on %s", GUToString(iReduceOnDims).c_str()));
        MMG402OperatorSplittingSchemeData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>* pData = new MMG402OperatorSplittingSchemeData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, ReducedSpaceDim>();
        for (int i = 0; i < ReducedSpaceDim; i++) {
          pData->reduceOnDims.insert(iReduceOnDims(i));
        }
        pData->pInnerSolverFactory = ipSolverFactory;
        pData->equationExpression = iSymb;
        res.AddExtensionObject(pData, MMG402OperatorExtensionVarSplittingSchemeOperatorData);
        res.AddExtensionObject(this, MMG402OperatorExtensionNameTemporalImplementator1);

        return res;
      }
    };

  } // end namespace Temporal
}// end namespace MMG402Solver


#endif // MMG402OperatorSplittingScheme_H