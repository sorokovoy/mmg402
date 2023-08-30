#ifndef MMG402OperatorExplicitSpartialD1Center_H
#define MMG402OperatorExplicitSpartialD1Center_H

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

#define MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1Center 504

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialD1CenterImplementation : public IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorExplicitSpartialD1CenterImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral)
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

        const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* prData = pTmpData->GetGridData();

        MMG402GridIndex nearValueIndex = iWhere;

        SpaceScalarType stepPos, stepNeg;
        ipGrid->GetGridStep(iWhere, iVarIdx, stepPos, stepNeg);

        nearValueIndex[iVarIdx]++;
        MMG402NumData vPlus;
        if (prData->IsHasData(nearValueIndex)) {
          vPlus = prData->GetData(nearValueIndex);
        } else {
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>  dp;
          ipGrid->IndexToCoord(iWhere, dp);
          dp(iVarIdx) += stepPos;
          vPlus = ipFunc->GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(dp);
          // return true;
        }

        nearValueIndex[iVarIdx] = nearValueIndex[iVarIdx] - 2;
        MMG402NumData vMinus;
        if (prData->IsHasData(nearValueIndex)) {
          vMinus = prData->GetData(nearValueIndex);
        } else {
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>  dp;
          ipGrid->IndexToCoord(iWhere, dp);
          dp(iVarIdx) -= stepNeg;
          vMinus = ipFunc->GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(dp);
          //return true;
        }

        MMG402NumData vCurr = prData->GetData(iWhere);
        result = (vPlus - vMinus) / ((stepPos + stepNeg) / 2);
        return true;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1Center;
      }

    protected:
      bool _isForvard;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialD1Center : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorExplicitSpartialD1CenterImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb, int iVarIdx)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1Center, GUToString("D(x%d)", iVarIdx)), iVarIdx);
      }

    protected:
      virtual MESymbol PrepareSymbol(MESymbol& iSymb, int iVarIdx)
      {
        MESymbol res = IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::PrepareSymbol(iSymb);
        res.AddExtensionObject(new int(iVarIdx), MMG402OperatorExtensionVarIDx);
        return res;
      }
    };


    class MESymbolOperatorExplicitSpartialD1CenterPrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1Center :
            out << " D1(" << iSymb.GetArguments()[0]  << "," << iSymb.GetSymbolName() << ")";
            return true;
        }
        return false;
      }
    };


    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorExplicitSpartialD1Center, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1Center);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSpartialD1CenterPrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif // MMG402OperatorExplicitSpartialD1Center_H
