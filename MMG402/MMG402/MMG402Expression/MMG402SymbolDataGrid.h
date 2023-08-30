#ifndef MMG402SymbolDataGrid_H
#define MMG402SymbolDataGrid_H

#include "MMG402\libs\ObjRegistry.h"

#include "MMG402\libs\MExp\IMESymbolData.h"

#include "MMG402\libs\GUtils\GUPointerStorage.h"
#include "MMG402\MMG402Matrix\IMMG402Matrix.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

namespace MMG402Expression
{

#define MMG402SymbolDataType_Grid 101

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolDataGridDataCore
  {
  protected:
    MMG402SymbolDataGridDataCore()
    {
      _pProxyReference = 0;
      _pSimpleReference = 0;
      _pMatrix = 0;
    }

    void DeleteAll()
    {
      if (_pProxyReference != 0)  delete *_pProxyReference;
      if (_pSimpleReference != 0) delete _pSimpleReference;
      if (_pMatrix != 0) {
        delete _pMatrix;
      }
    }


    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>** _pProxyReference;
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pSimpleReference;
    typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* _pMatrix;

  };

#define ImplementBinaryMMG402SymbolDataGridOperationVector(OP) \
    if(IsHasGridData() && iVal.IsHasGridData()) {            \
      *(GetGridData()) OP *iVal.GetGridData();               \
    } else if(!IsHasGridData() && iVal.IsHasGridData()) {    \
      _pSimpleReference = iVal.GetGridData()->CreateCopy();  \
    }                                                        \
    if(IsHasMatrixData() && iVal.IsHasMatrixData()) {        \
      *(GetMatrixData()) OP *iVal.GetMatrixData();           \
    } else if(!IsHasMatrixData() && iVal.IsHasMatrixData()) {\
      _pMatrix = iVal.GetMatrixData()->CreateCopy();         \
    }                                                        \

#define ImplementBinaryMMG402SymbolDataGridOperationScalar(OP) \
    if(IsHasGridData()) {                                    \
      *GetGridData() OP iVal;                                \
    }                                                        \
    if(IsHasMatrixData()) {                                  \
      *GetMatrixData() OP iVal;                              \
    }                                                        \


  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402SymbolDataGrid : public IMESymbolData,
    protected MMG402SymbolDataGridDataCore<SpaceScalarType, SpaceDim, DataScalarType, DataDim>,
    private GUPointerStorage
  {
  public:
    MMG402SymbolDataGrid()
    {};

    MMG402SymbolDataGrid(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>** ipProxyReference)
    {
      _pProxyReference = ipProxyReference;
    }

    MMG402SymbolDataGrid(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipSimpleReference)
    {
      _pSimpleReference = ipSimpleReference;
    }

    MMG402SymbolDataGrid(typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* ipMatrix)
    {
      _pMatrix = ipMatrix;
    }

    virtual int GetType() const
    {
      return MMG402SymbolDataType_Grid;
    }

    bool IsHasGridData() const
    {
      return _pSimpleReference != 0 || _pProxyReference != 0;
    }

    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetGridData() const
    {
      if (!IsHasGridData()) return 0;
      return _pSimpleReference != 0 ? _pSimpleReference : *_pProxyReference;
    }

    bool SetGridData(typename MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* cipGrid)
    {
      GUAssertV(!IsHasGridData(), false);
      _pSimpleReference = cipGrid;
      return false;
    }

    bool IsHasMatrixData() const
    {
      return _pMatrix != 0;
    }

    typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* GetMatrixData() const
    {
      return _pMatrix;
    }

    bool SetMatrixData(typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* cipMatrix)
    {
      GUAssertV(_pMatrix == 0, false);
      _pMatrix = cipMatrix;
      return false;
    }

    virtual MExp::IMESymbolData* CreateCopy() const
    {
      MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pNewInstance = new MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
      if (IsHasMatrixData()) {
        pNewInstance->_pMatrix = GetMatrixData()->CreateCopy();
      }

      if (IsHasGridData()) {
        pNewInstance->_pSimpleReference = GetGridData()->CreateCopy();
      }
      return pNewInstance;
    }

    virtual ~MMG402SymbolDataGrid()
    {
      // from GUPointerStorage
      OnDerivedDestructorCall();
    }

    // vector operations
    void operator -= (const MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {

      ImplementBinaryMMG402SymbolDataGridOperationVector( -= );
    }

    void operator += (const MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      ImplementBinaryMMG402SymbolDataGridOperationVector( += );
    }

    void operator *= (const MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      GUAssert(!(IsHasMatrixData() && iVal.IsHasMatrixData()));
      // cross multiplication
      if((IsHasMatrixData() && iVal.IsHasGridData()) || (iVal.IsHasMatrixData() && IsHasGridData())) {
        MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pGridData = IsHasGridData() ? GetGridData() : iVal.GetGridData();
        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pMaxtrixData = IsHasMatrixData() ? GetMatrixData() : iVal.GetMatrixData();
        MMG402GridIndex index;
        const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = pGridData->GetMasterGrid()->GetIndexer();
        for(unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {
          pIndexer->IteratorToIndex(i, index);
          MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> val = pGridData->GetData(index);
          pMaxtrixData->RowMult(i, val);

         // for(unsigned int j = 0; j < pIndexer->GetIteratorMaxValue(); j++) {
         //   pMaxtrixData->Value(i, j) *= val;
         // }

          if(pMaxtrixData->cols() == pIndexer->GetIteratorMaxValue()) {
            pMaxtrixData->Value(i, pIndexer->GetIteratorMaxValue()) *= val;
          }
        }
      }
      ImplementBinaryMMG402SymbolDataGridOperationVector( *= );
    }

    void operator /= (const MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      GUAssert(!(IsHasMatrixData() && iVal.IsHasMatrixData()));

      // implement cross multiplication?
      GUAssert(!((IsHasMatrixData() && iVal.IsHasGridData()) || (iVal.IsHasMatrixData() && IsHasGridData())));
      ImplementBinaryMMG402SymbolDataGridOperationVector( /= );
    }

    // scalar operations
    void operator -= (typename const MMG402NumData& iVal)
    {
      ImplementBinaryMMG402SymbolDataGridOperationScalar( -= );
    }

    void operator += (typename const MMG402NumData& iVal)
    {
      ImplementBinaryMMG402SymbolDataGridOperationScalar( += );
    }

    void operator *= (typename const MMG402NumData& iVal)
    {
      ImplementBinaryMMG402SymbolDataGridOperationScalar( *= );
    }

    void operator /= (typename const MMG402NumData& iVal)
    {
      ImplementBinaryMMG402SymbolDataGridOperationScalar( /= );
    }

  protected:
    // for GUPointerStorage
    virtual void DeleteData()
    {
      MMG402SymbolDataGridDataCore::DeleteAll();
    }
  };

  REGISTER_DERIVED_ID(MExp::IMESymbolData, MMG402SymbolDataGrid, MMG402SymbolDataType_Grid); // validate ID

} // end MMG402Expression
#endif // MMG402SymbolDataGrid_H
