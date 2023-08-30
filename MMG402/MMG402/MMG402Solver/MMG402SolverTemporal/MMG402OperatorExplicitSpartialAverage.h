#ifndef MMG402OperatorExplicitSpartialAverage_H
#define MMG402OperatorExplicitSpartialAverage_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

#include "MMG402\MMG402NumericData\IMMG402NumDataOperator.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402NumericDataDiffCalculation : public MMG402NumericData::IMMG402NumDataOperator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    protected:
      virtual int GetMaskDeepth() const
      {
        return 1;
      }

      virtual bool BeginPointProcess(typename const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex)
      {
        sum = MMG402NumData(0);
        centerV = ipData->GetData(iBaseIndex)(0);
        count = 0;
        return true;
      }

      virtual void PointProcess(typename const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex, typename const MMG402GridIndex& iNearIndex)
      {
        if (iBaseIndex != iNearIndex && ipData->GetData(iNearIndex)(0) > centerV) {
          sum += ipData->GetData(iNearIndex);
          count++;
        } 
      }

      virtual typename MMG402NumData FinishPointProcess()
      {
        sum -= centerV * count;
        if(count != 0) {
          return sum / (DataScalarType)count;
        } else {
          return sum;
        }
      }

      typename MMG402NumData sum;
      double centerV;
      int count;
    };

#define MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialAverage 514

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialAverageImplementation : public IMESymbolOperatorUnary
    {
    public:
      enum { DataDimAtCompile  = DataDim  };
      enum { SpaceDimAtCompile = SpaceDim };

      MMG402OperatorExplicitSpartialAverageImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipMasterInput)
      {
        _pInput = ipMasterInput;
      }

      virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol& iSybmol) const
      {
        GUAssert(ipData->GetType() == MMG402SymbolDataType_Grid);
        GUAssertT((dynamic_cast<MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
        MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pGrid = (MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) (ipData.GetPointer());

        MMG402NumericDataDiffCalculation<SpaceScalarType, SpaceDim, DataScalarType, DataDim> processor;
        MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pNewData = processor.Process(pGrid->GetGridData());

        MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex index;

        int max = (int) _pInput->pGridIterator->GetIteratorMaxValue();
        for (int i = 0; i < max; i++) {
          _pInput->pGridIterator->IteratorToIndex((unsigned int)i, index);
          pGrid->GetGridData()->GetData(index) = pNewData->GetData(index);
        }
        delete pNewData;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialAverage;
      }

    protected:
      const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pInput;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402OperatorExplicitSpartialAverage : public IMMG402OperatorTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorExplicitSpartialAverageImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialAverage, "Average"));
      }
    };


    class MESymbolOperatorExplicitSpartialAveragePrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialAverage :
            out << " Average(" << iSymb.GetArguments()[0]  << "," << iSymb.GetSymbolName() << ")";
            return true;
        }
        return false;
      }
    };


    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorExplicitSpartialAverage, MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialAverage);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSpartialAveragePrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif // MMG402OperatorExplicitSpartialAverage_H
