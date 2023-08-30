#ifndef MMG402SymbolFunc_H
#define MMG402SymbolFunc_H

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\IMESymbolOperators.h"

#include "MMG402\MMG402Expression\MMG402SymbolDataScalar.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataGrid.h"

#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\GUtils\GUAssert.h"
#include "MMG402\libs\GUtils\GUStringUtils.h"

namespace MMG402Expression
{

  // ========= MMG402Exp
  #define MMG402SymbolOperatorUnaryExpId 33
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorUnaryExp : public MExp::IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol&) const
    {
      switch (ipData->GetType()) {
        case MMG402SymbolDataType_Grid: {
          GUAssertT((dynamic_cast<MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
          MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pDataGrid = (MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)ipData.GetPointer();
          if (pDataGrid->IsHasGridData()) {
            MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData = (pDataGrid)->GetGridData();
            MMG402GridIndex index;
            const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = pData->GetMasterGrid()->GetIndexer();
            for (unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {
              pIndexer->IteratorToIndex(i, index);
              for (int j = 0; j < pData->GetDataDim(); j++) {
                pData->GetData(index)[j] = exp(pData->GetData(index)[j]);
              }
            }
          }
          if (pDataGrid->IsHasMatrixData()) {
            throw new std::exception("Not Implemented");
          }
          return;
        }
        case MMG402SymbolDataType_Scalar: {
          GUAssertT(dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(ipData.GetPointer()), "Invalid symbol data type");
          ((MMG402SymbolDataScalar<DataScalarType>*)ipData.GetPointer())->Value() = exp(((MMG402SymbolDataScalar<DataScalarType>*) ipData.GetPointer())->Value());
          return;
        }
        default: {
          GUAssertT(false, "Unable evaluate Exp (" << MMG402SymbolOperatorUnaryExpId << ") for type " << ipData->GetType());
        }
      }
    }
    virtual int GetType() const
    {
      return MMG402SymbolOperatorUnaryExpId;
    }
  };

  class MMG402Exp : public MExp::MESymbol
  {
  public:
    MMG402Exp(const MExp::MESymbol& iArg)
      : MESymbol(0, MMG402SymbolOperatorUnaryExpId)
    {
      _arguments.push_back(iArg);
    }
  };
  REGISTER_DERIVED_ID(MExp::MESymbol, MMG402Exp, MMG402SymbolOperatorUnaryExpId);


  // ========= MMG402Sg
  #define MMG402SymbolOperatorSignumId 34
  const std::string IsPositiveExtensionInfo = "IsToPositive";
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorSignum: public MExp::IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipData, const MExp::MESymbol& iSymb) const
    {
      bool isPositive = *((bool*)iSymb.GetExtensionObject(IsPositiveExtensionInfo));
      switch (ipData->GetType()) {
        case MMG402SymbolDataType_Grid: {
          GUAssertT((dynamic_cast<MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipData.GetPointer())), "Invalid symbol data type");
          MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pDataGrid = (MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)ipData.GetPointer();
          if (pDataGrid->IsHasGridData()) {
            MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData = (pDataGrid)->GetGridData();
            MMG402GridIndex index;
            const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = pData->GetMasterGrid()->GetIndexer();
            for (unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {
              pIndexer->IteratorToIndex(i, index);
              for (int j = 0; j < pData->GetDataDim(); j++) {
                pData->GetData(index)[j] = EvaluateSG(pData->GetData(index)[j], isPositive);
              }
            }
          }
          return;
        }
        case MMG402SymbolDataType_Scalar: {
          GUAssertT(dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(ipData.GetPointer()), "Invalid symbol data type");
          ((MMG402SymbolDataScalar<DataScalarType>*)ipData.GetPointer())->Value() = EvaluateSG(((MMG402SymbolDataScalar<DataScalarType>*) ipData.GetPointer())->Value(), isPositive);
          return;
        }
        default: {
          GUAssertT(false, "Unable evaluate Exp (" << MMG402SymbolOperatorSignumId << ") for type " << ipData->GetType());
        }
      }
    }
    virtual int GetType() const
    {
      return MMG402SymbolOperatorSignumId;
    }
  private:
    DataScalarType EvaluateSG(DataScalarType& iVal, bool isPositive) const
    {
      if((iVal > 0 && isPositive) ||
         (iVal < 0 && !isPositive)) {
        return iVal;
      } else {
        return 0;
      }
    }
  };

  class MMG402Sg : public MExp::MESymbol
  {
  public:
    MMG402Sg(const MExp::MESymbol& iArg, bool IsAllowPositive)
      : MESymbol(0, MMG402SymbolOperatorSignumId)
    {
      AddExtensionObject(new bool(IsAllowPositive), IsPositiveExtensionInfo);
      _arguments.push_back(iArg);
    }
  };

  REGISTER_DERIVED_ID(MExp::MESymbol, MMG402Sg, MMG402SymbolOperatorSignumId);


  // ========= staff
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  void MMG402SymbolFuncAddAllSymbols(MExp::MESymbolTable& iTable)
  {
    iTable.AddUnaryOperator(new MMG402SymbolOperatorUnaryExp<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
    iTable.AddUnaryOperator(new MMG402SymbolOperatorSignum<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
  }

  class MMG402SymbolFuncPrinter : public MExp::IMESymbolPrinter
  {
  public:
    virtual bool Print(std::ostream& out, const MExp::MESymbol& iSymb)
    {
      switch (iSymb.GetSymbolType()) {
        case MMG402SymbolOperatorUnaryExpId : 
          {
            out << " Exp(" << iSymb.GetArguments()[0]  << ")";
            return true;
          }
        case MMG402SymbolOperatorSignumId : 
          {
            out << " Sg(" << iSymb.GetArguments()[0]  << ")";
            return true;
          }
      }
      return false;
    }
  };

  REGISTER_OBJECT(MExp::IMESymbolPrinter, MMG402SymbolFuncPrinter);

} // end MMG402Expression
#endif // MMG402SymbolArifmetic_H
