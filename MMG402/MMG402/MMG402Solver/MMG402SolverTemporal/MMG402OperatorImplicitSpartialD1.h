#ifndef MMG402OperatorImplicitSpartialD1_H
#define MMG402OperatorImplicitSpartialD1_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporalUniversal.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"
#include "MMG402\libs\GUtils\GUMathUtils.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSparce.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {

#define MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1 512

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorImplicitSpartialD1Implementation : public IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorImplicitSpartialD1Implementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput)
        : IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipMasterInput)
      {}

      virtual bool Evaluate(typename const MMG402GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTmpData) const
      {
        int iVarIdx = *((int*)iSybmol.GetExtensionObject(MMG402OperatorExtensionVarIDx));

        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pMatrixData = GetMatrixData(ioTargetData);
        const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIndexer = _pInput->pGridIterator;
        unsigned int n = pGridIndexer->GetIteratorMaxValue();

        SpaceScalarType stepPos, stepNeg;
        ipGrid->GetGridStep(iWhere, iVarIdx, stepPos, stepNeg);
        SpaceScalarType h = stepNeg;

        unsigned int currentRow;
        GUAssert(pGridIndexer->IndexToIterator(iWhere, currentRow));
        if (!GetFlags().IsNoMatrixRecalculate) {
          pMatrixData->Value(currentRow, currentRow) += 1.00 / h;
        }

        MMG402GridIndex nearValueIndex = iWhere;
        nearValueIndex[iVarIdx]++;

        unsigned int col;
        if (pGridIndexer->IndexToIterator(nearValueIndex, col)) {
          if (!GetFlags().IsNoMatrixRecalculate) {
            pMatrixData->Value(currentRow, col) -= 1.00 / h;
          }
        } else {
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> dp;
          ipGrid->IndexToCoord(iWhere, dp);
          dp(iVarIdx) -= stepNeg;
          pMatrixData->Value(currentRow, n) += ipFunc->GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(dp) / h;
        }
        ioTargetData->GetGridData()->GetData(iWhere) = 0;
        return true;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1;
      }
    };


    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorImplicitSpartialD1 : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorImplicitSpartialD1Implementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb, int iVarIdx)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1, GUToString("D1(x%d)", iVarIdx)), iVarIdx);
      }

    protected:
      virtual MESymbol PrepareSymbol(MESymbol& iSymb, int iVarIdx)
      {
        MESymbol res = IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::PrepareSymbol(iSymb);
        res.AddExtensionObject(new int(iVarIdx), MMG402OperatorExtensionVarIDx);
        return res;
      }
    };

    class MESymbolOperatorImplicitSpartialD1Printer  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1 :
            out << " D1(" << iSymb.GetArguments()[0]  << "," << iSymb.GetSymbolName() << ")";
            return true;
        }
        return false;
      }
    };

    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorImplicitSpartialD1, MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorImplicitSpartialD1Printer);

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif // MMG402OperatorImplicitSpartialD1_H
