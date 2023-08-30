#ifndef MMG402OperatorExplicitSpartialD1_H
#define MMG402OperatorExplicitSpartialD1_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {

#define MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1 501
    const std::string MMG402OperatorExplicitSpartialD1ForwardExtensionInfoId = "IsForward";

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialD1Implementation : public IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorExplicitSpartialD1Implementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral)
        : IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral)
      {
        _requireTmpDataClone = true;
      }

      virtual bool Evaluate(typename const MMG402GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTmpData) const
      {
        MMG402NumData& result = ioTargetData->GetGridData()->GetData(iWhere);
        int iVarIdx = *((int*)iSybmol.GetExtensionObject(MMG402OperatorExtensionVarIDx));
        bool isForward = *((bool*)iSybmol.GetExtensionObject(MMG402OperatorExplicitSpartialD1ForwardExtensionInfoId));

        const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* prData = pTmpData->GetGridData();

        MMG402GridIndex nearValueIndex = iWhere;
        //MMG402GridIndex near;
        //near = iWhere;
        int sign = isForward ? 1 : -1;
        nearValueIndex[iVarIdx] += sign;

        SpaceScalarType stepPos, stepNeg;
        ipGrid->GetGridStep(iWhere, iVarIdx, stepPos, stepNeg);

        MMG402NumData vVal;
        if (prData->IsHasData(nearValueIndex)) {
          vVal = prData->GetData(nearValueIndex);
        } else {
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> dp;
          ipGrid->IndexToCoord(iWhere, dp);
          dp(iVarIdx) += isForward ? stepPos : -stepNeg;
          vVal = ipFunc->GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(dp);
        }

        MMG402NumData vCurr = prData->GetData(iWhere);
        result = (vVal - vCurr) / (DataScalarType)(((DataScalarType)(isForward ? stepPos : stepNeg)) * sign);
        return true;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1;
      }
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialD1 : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorExplicitSpartialD1Implementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb, int iVarIdx, bool iIsForward)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1, GUToString("D%s(x%d)", iIsForward ? "+" : "-", iVarIdx)), iVarIdx, iIsForward);
      }

    protected:
      virtual MESymbol PrepareSymbol(MESymbol& iSymb, int iVarIdx, bool iIsForward)
      {
        MESymbol res = IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::PrepareSymbol(iSymb);
        res.AddExtensionObject(new int(iVarIdx), MMG402OperatorExtensionVarIDx);
        res.AddExtensionObject(new bool(iIsForward), MMG402OperatorExplicitSpartialD1ForwardExtensionInfoId);
        return res;
      }
    };


    class MESymbolOperatorExplicitSpartialD1Printer  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1 :
            out << " D1(" << iSymb.GetArguments()[0]  << "," << iSymb.GetSymbolName() << ")";
            return true;
        }
        return false;
      }
    };


    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorExplicitSpartialD1, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSpartialD1Printer);

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif // MMG402OperatorExplicitSpartialD1_H
