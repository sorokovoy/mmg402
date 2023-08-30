#ifndef IMMG402OperatorTemporal_H
#define IMMG402OperatorTemporal_H

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\IMESymbolOperators.h"

#include "MMG402\libs\GUtils\GUAssert.h"

#include "MMG402\MMG402Expression\IMMG402OperatorUnary.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataGrid.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverDataStorageTemporal.h"

namespace MMG402Solver
{
  namespace Temporal
  {
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402OperatorTemporalImplementation : public IMESymbolOperatorUnary
    {
    public:
      enum { DataDimAtCompile  = DataDim  };
      enum { SpaceDimAtCompile = SpaceDim };

      IMMG402OperatorTemporalImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput)
      {
        _pInput = ipMasterInput;
        _requireTmpDataClone = false;
      }

      virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol& iSybmol) const
      {
        PrepareForEvaluate();

        GUAssert(ipData->GetType() == MMG402SymbolDataType_Grid);
        GUAssertT((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pGrid = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) (ipData.GetPointer());

        int max = (int) _pInput->pGridIterator->GetIteratorMaxValue();
        MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex index;

        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ptmpDataClone = 0;
        if(_requireTmpDataClone) {
          ptmpDataClone = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)pGrid->CreateCopy();
        }

        #pragma omp parallel for
        for (int i = 0; i < max; i++) {
          _pInput->pGridIterator->IteratorToIndex((unsigned int)i, index);
          Evaluate(index, _pInput->pGrid, _pInput->pData, _pInput->pFuncStorage, iSybmol, pGrid, ptmpDataClone);
        }
        delete ptmpDataClone;
      }

    protected:
      virtual bool Evaluate(typename const MMG402GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTmpData) const = 0;

      virtual void PrepareForEvaluate() const {};

      const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pInput;
      bool _requireTmpDataClone;
    };

    const std::string MMG402OperatorExtensionNameTemporalImplementator = "TemporalImplementator";
    const std::string MMG402OperatorExtensionVarIDx = "varidx";

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402OperatorTemporal : public MMG402Expression::IMMG402OperatorUnary<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput) const = 0;

    protected:
      virtual MExp::MESymbol PrepareSymbol(MExp::MESymbol& iSymb)
      {
        iSymb.AddExtensionObject(this, MMG402OperatorExtensionNameTemporalImplementator);
        return iSymb;
      }
    };

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif
