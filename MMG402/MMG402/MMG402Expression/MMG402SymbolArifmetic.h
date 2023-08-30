#ifndef MMG402SymbolArifmetic_H
#define MMG402SymbolArifmetic_H

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\IMESymbolOperators.h"

#include "MMG402\MMG402Expression\MMG402SymbolDataScalar.h"
#include "MMG402\MMG402Expression\MMG402SymbolDataGrid.h"

#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\GUtils\GUAssert.h"
#include "MMG402\libs\GUtils\GUStringUtils.h"

namespace MMG402Expression
{

  template <typename DataScalarType>
  class MMG402SymbolConstantParser : public MExp::IMESymbolConstantParser
  {
  public:
    virtual MExp::IMESymbolData* Parse(const MExp::MESymbol& iName) const
    {
      return new MMG402SymbolDataScalar<DataScalarType>(GUParse<DataScalarType>(iName.GetSymbolName()));
    }
  };


  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorUnaryInverse : public MExp::IMESymbolOperatorUnary
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
                pData->GetData(index)[j] = -(pData->GetData(index)[j]);
              }
            }
          }
          if (pDataGrid->IsHasMatrixData()) {
            *pDataGrid->GetMatrixData() = -*pDataGrid->GetMatrixData();
          }
          return;
        }
        case MMG402SymbolDataType_Scalar: {
          GUAssertT(dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(ipData.GetPointer()), "Invalid symbol data type");
          ((MMG402SymbolDataScalar<DataScalarType>*)ipData.GetPointer())->Value() = -(((MMG402SymbolDataScalar<DataScalarType>*) ipData.GetPointer())->Value());
          return;
        }
        default: {
          GUAssertT(false, "Unable evaluate unary inverce (" << MESymbolType_UnaryInverce << ") for type " << ipData->GetType());
        }
      }
    }
    virtual int  GetType() const
    {
      return MESymbolType_UnaryInverce;
    }
  };

#define ImplementBinaryOperation(OP)                                                                         \
    MExp::IMESymbolData* pLeft = ipLeft.GetPointer();                                                              \
    MExp::IMESymbolData* pRight = ipRight.GetPointer();                                                            \
    if(ipLeft->GetType() == MMG402SymbolDataType_Grid && ipRight->GetType() == MMG402SymbolDataType_Grid) {                                                    \
      GUAssertT((dynamic_cast<MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipLeft.GetPointer())), "Invalid symbol data type");   \
      GUAssertT((dynamic_cast<MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipRight.GetPointer())), "Invalid symbol data type");  \
                                                                                                                                                               \
      MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pLeftData = ((MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)ipLeft.GetPointer());  \
      MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pRightData = ((MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)ipRight.GetPointer());\
      *pLeftData OP *pRightData;                                                                              \
      return ipLeft;                                                                                          \
    }                                                                                                         \
                                                                                                              \
    if(ipLeft->GetType() == MMG402SymbolDataType_Scalar && ipRight->GetType() == MMG402SymbolDataType_Scalar) { \
       GUAssertT((dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(pLeft)),  "Invalid symbol data type");  \
       GUAssertT((dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(pRight)), "Invalid symbol data type");  \
      ((MMG402SymbolDataScalar<DataScalarType>*)pLeft)->Value() OP (((MMG402SymbolDataScalar<DataScalarType>*)pRight)->Value());\
      return ipLeft;                                                                                        \
    }                                                                                                       \
                                                                                                            \
    if(ipLeft->GetType() == MMG402SymbolDataType_Scalar && ipRight->GetType() == MMG402SymbolDataType_Grid) {   \
      GUAutoPtr<IMESymbolData> ipLeftClone = ipLeft;                                                        \
      ipLeft = ipRight;                                                                                     \
      ipRight = ipLeftClone;                                                                                \
      pLeft = ipLeft.GetPointer();                                                                          \
      pRight = ipRight.GetPointer();                                                                        \
    }                                                                                                       \
                                                                                                            \
    if(ipLeft->GetType() == MMG402SymbolDataType_Grid && ipRight->GetType() == MMG402SymbolDataType_Scalar) { \
      GUAssertT((dynamic_cast<MMG402SymbolDataScalar<DataScalarType>*>(pRight)), "Invalid symbol data type"); \
      GUAssertT((dynamic_cast<MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(ipLeft.GetPointer())), "Invalid symbol data type"); \
      MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pLeftData = ((MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*)ipLeft.GetPointer());\
      DataScalarType scalar = ((MMG402SymbolDataScalar<DataScalarType>*)pRight)->Value();                   \
      *pLeftData OP scalar;                                                                                 \
      return ipLeft;                                                                                        \
    }                                                                                                       \

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorSum : public IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<MExp::IMESymbolData> Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipLeft, GUAutoPtr<MExp::IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      ImplementBinaryOperation( += );
      GUAssertT(false, "Unable evaluate SUM (" << MESymbolType_Sum << ") for type " << pLeft->GetType() << " and " << pRight->GetType());
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Sum;
    }
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorProd : public MExp::IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<MExp::IMESymbolData> Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipLeft, GUAutoPtr<MExp::IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      ImplementBinaryOperation( *= );
      GUAssertT(false, "Unable evaluate PROD (" << MESymbolType_Sum << ") for type " << pLeft->GetType() << " and " << pRight->GetType());
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Prod;
    }
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolOperatorDevide : public MExp::IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<MExp::IMESymbolData> Evaluate(GUAutoPtr<MExp::IMESymbolData>& ipLeft, GUAutoPtr<MExp::IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      GUAssertV(!(ipLeft->GetType() == MMG402SymbolDataType_Scalar && ipRight->GetType() == MMG402SymbolDataType_Grid), ipLeft);
      ImplementBinaryOperation( /= );
      GUAssertT(false, "Unable evaluate PROD (" << MESymbolType_Sum << ") for type " << pLeft->GetType() << " and " << pRight->GetType());
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Devide;
    }
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  void MMG402SymbolArifmeticAddAllSymbols(MExp::MESymbolTable& iTable)
  {
    iTable.SetContantParser(new MMG402SymbolConstantParser<DataScalarType>());
    iTable.AddUnaryOperator(new MMG402SymbolOperatorUnaryInverse<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());

    iTable.AddBinaryOperator(new MMG402SymbolOperatorSum<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
    iTable.AddBinaryOperator(new MMG402SymbolOperatorProd<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
    iTable.AddBinaryOperator(new MMG402SymbolOperatorDevide<SpaceScalarType, SpaceDim, DataScalarType, DataDim>());
  }

}; // end MMG402Expression
#endif // MMG402SymbolArifmetic_H
