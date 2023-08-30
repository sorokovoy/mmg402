#ifndef MMG402OperatorExplicitDt_H
#define MMG402OperatorExplicitDt_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {

#define MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt 503

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitDtImplementation : public IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorExplicitDtImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral)
        : IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral)
      {}

      virtual bool Evaluate(typename const MMG402GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pTmpData) const
      {
        const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* prData = ipPriviosData->Pop(0)->pData;
        MMG402NumData& result = ioTargetData->GetGridData()->GetData(iWhere);
        result = prData->GetData(iWhere) - _pInput->dt * result;
        return true;
      }

      virtual int  GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt;
      }
    };


    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitDt : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorExplicitDtImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType   GetSchemeType()   const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt, "DT"));
      }
    };

    class MESymbolOperatorExplicitSpartialDTPrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt :
            out << " DT(" << iSymb.GetArguments()[0] << ")";
            return true;
        }
        return false;
      }
    };

    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorExplicitDt, MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSpartialDTPrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif // MMG402OperatorExplicitDt_H
