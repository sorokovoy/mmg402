#ifndef MMG402OperatorImplicitDt_H
#define MMG402OperatorImplicitDt_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporalUniversal.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {

#define MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt 511

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorImplicitDtImplementation : public IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorImplicitDtImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral)
        : IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral)
      {}

      virtual bool Evaluate(typename const MMG402GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTmpData) const
      {
        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pMatrixData = GetMatrixData(ioTargetData);
        const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIndexer = _pInput->pGridIterator;
        unsigned int n = pGridIndexer->GetIteratorMaxValue();

        unsigned int currentRow;
        GUAssert(pGridIndexer->IndexToIterator(iWhere, currentRow));
        if (!GetFlags().IsNoMatrixRecalculate) {
          pMatrixData->Value(currentRow, currentRow) += 1.00 / _pInput->dt;
        }

        const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* prData = ioTargetData->GetGridData();
        pMatrixData->Value(currentRow, n) += ioTargetData->GetGridData()->GetData(iWhere) / _pInput->dt;
        ioTargetData->GetGridData()->GetData(iWhere) = 0;
        return true;
      }

      virtual int  GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt;
      }
    };


    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorImplicitDt : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorImplicitDtImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt, "DT"));
      }
    };

    class MESymbolOperatorImplicitSpartialDTPrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt :
            out << " DT(" << iSymb.GetArguments()[0] << ")";
            return true;
        }
        return false;
      }
    };

    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorImplicitDt, MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorImplicitSpartialDTPrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif // MMG402OperatorImplicitDt_H
