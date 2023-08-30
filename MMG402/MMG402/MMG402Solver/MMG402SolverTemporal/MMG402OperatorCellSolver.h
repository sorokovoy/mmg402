#ifndef MMG402OperatorCellSolver_H
#define MMG402OperatorCellSolver_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"


#include "MMG402\MMG402NumericData\MMG402NumDataCellReductor.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

using namespace MExp;
#define MMG402SolverOperatorRegistry_MMG402OperatorCellSolver 605

namespace MMG402Solver
{
  namespace Temporal
  {
    const std::string MMG402OperatorCellSolverDataKey = "CellSolverInnerKey";

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, typename InnerSolverSpaceScalarType>
    struct MMG402OperatorCellSolverData {
      MMG402SolverInputTemporal<InnerSolverSpaceScalarType, 1, DataScalarType, 1>* pInnerSolver;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, typename InnerSolverSpaceScalarType>
    class MMG402OperatorCellSolverImplementation : public IMESymbolOperatorUnary
    {
    public:
      enum { DataDimAtCompile  = DataDim  };
      enum { SpaceDimAtCompile = SpaceDim };

      MMG402OperatorCellSolverImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput)
      {
        _pInput = ipMasterInput;
      }

      virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol& iSybmol) const
      {
        GUAssert(ipData->GetType() == MMG402SymbolDataType_Grid);
        GUAssertT((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pGrid = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) (ipData.GetPointer());

        MMG402OperatorCellSolverData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>* pSpData = (MMG402OperatorCellSolverData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>*)iSybmol.GetExtensionObject(MMG402OperatorCellSolverDataKey);

        MMG402NumericData::MMG402NumDataCellReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>* pDataReductor = new MMG402NumericData::MMG402NumDataCellReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>(pGrid->GetGridData(), pSpData->pInnerSolver->pGrid);

        int max = (int) _pInput->pGridIterator->GetIteratorMaxValue();
        MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex index;


        IMMG402SolverTemporal<InnerSolverSpaceScalarType, 1, DataScalarType, 1>* pCSolver = (IMMG402SolverTemporal<InnerSolverSpaceScalarType, 1, DataScalarType, 1>*) pSpData->pInnerSolver->pSolver;

        // prepare storage
        pCSolver->GetOutput()->pData->Clear();
        pCSolver->GetOutput()->pData->SetStorageMaxDeep(2);

        pCSolver->GetOutput()->pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<InnerSolverSpaceScalarType, 1, DataScalarType, 1>(pDataReductor, _pInput->pData->Pop(0)->t));
        pCSolver->SetDataProxyReference(pCSolver->GetOutput()->pData->Pop(0)->pData);

        int iterationCount = (int)(_pInput->dt / pCSolver->GetInput()->dt);
        if (iterationCount == 0) iterationCount++;

        for (int i = 0; i < max; i++) {
          _pInput->pGridIterator->IteratorToIndex((unsigned int)i, index);

          pDataReductor->SetCurrentCellIndex(index);
          for (int it = 0; it < iterationCount; it++) {
            pCSolver->MakeIteration();
          }

          MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>* pEvaluatedData = pCSolver->GetOutput()->pData->Pop(0)->pData;
          // extract data
          for (int j = 0; j < DataDimAtCompile; j++) {
            pGrid->GetGridData()->GetData(index)(j) = pEvaluatedData->GetData(j)(0);
          }

          pCSolver->GetOutput()->pData->Clear();

          pDataReductor = new MMG402NumericData::MMG402NumDataCellReductor<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>(pGrid->GetGridData(), pSpData->pInnerSolver->pGrid);
          pCSolver->GetOutput()->pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<InnerSolverSpaceScalarType, 1, DataScalarType, 1>(pDataReductor, _pInput->pData->Pop(0)->t));
          pCSolver->SetDataProxyReference(pCSolver->GetOutput()->pData->Pop(0)->pData);
        }
      }


      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorCellSolver;
      }

    protected:
      const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pInput;
    };


    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, typename InnerSolverSpaceScalarType>
    class MMG402OperatorCellSolver : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorCellSolverImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType()   const
      {
        return MMG402Expression::MMG402OperatorSchemeType_Undefined;
      }

      MESymbol operator () (const MESymbol& iSymb, MMG402SolverInputTemporal<InnerSolverSpaceScalarType, 1, DataScalarType, 1>* ipInnerSolver)
      {
        MESymbol result(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorCellSolver, "CellSolver");
        MMG402OperatorCellSolverData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>* pOperatorData = new MMG402OperatorCellSolverData<SpaceScalarType, SpaceDim, DataScalarType, DataDim, InnerSolverSpaceScalarType>();
        pOperatorData->pInnerSolver = ipInnerSolver;
        result.AddExtensionObject(pOperatorData, MMG402OperatorCellSolverDataKey);
        return PrepareSymbol(result);
      }
    };

    class MESymbolOperatorCellSolverPrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorCellSolver :
            out << " CellSolver ";
            return true;
        }
        return false;
      }
    };

    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorCellSolver, MMG402SolverOperatorRegistry_MMG402OperatorCellSolver);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorCellSolverPrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif // MMG402OperatorCellSolver_H
