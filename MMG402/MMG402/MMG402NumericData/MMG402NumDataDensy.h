#ifndef MMG402NumDataDensy_H
#define MMG402NumDataDensy_H

#include "MMG402\libs\GUtils\GUPointerStorage.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

namespace MMG402NumericData
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402NumDataDensy : public IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>, private GUPointerStorage
  {
  public:
    MMG402NumDataDensy(const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* ipMasterGrid)
      : IMMG402NumData(ipMasterGrid)
    {
      _pData = new IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::EqNumDataType[_pGrid->GetIndexer()->GetIteratorMaxValue()];
      for (unsigned int i = 0; i < _pGrid->GetIndexer()->GetIteratorMaxValue(); i++) {
        for (unsigned int j = 0; j < DataDimAtCompile; j++) {
          _pData[i][j] = 0;
        }
      }
    }

    virtual ~MMG402NumDataDensy()
    {
      // from GUPointerStorage
      OnDerivedDestructorCall();
    }

    virtual EqNumDataType& GetData(typename const MMG402GridIndex& iIndex)
    {
      unsigned int iterator;
      if (!_pGrid->GetIndexer()->IndexToIterator(iIndex, iterator)) {
        throw new std::invalid_argument("Argumeout out of range");
      } else {
        return _pData[iterator];
      }
    }

    virtual const EqNumDataType& GetData(typename const MMG402GridIndex& iIndex) const
    {
      unsigned int iterator;
      if (!_pGrid->GetIndexer()->IndexToIterator(iIndex, iterator)) {
        throw new std::invalid_argument("Argumeout out of range");
      } else {
        return _pData[iterator];
      }
    }

    virtual bool IsHasData(typename const MMG402GridIndex& iIndex) const
    {
      return _pGrid->GetIndexer()->IsValidIndex(iIndex);
    }

    virtual IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateCopy() const
    {
      MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pRes = new MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_pGrid);
      for (unsigned int i = 0; i < _pGrid->GetIndexer()->GetIteratorMaxValue(); i++) {
        for (unsigned int j = 0; j < DataDimAtCompile; j++) {
          pRes->_pData[i][j] = _pData[i][j];
        }
      }
      return pRes;
    }

  protected:
    typename IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::EqNumDataType* _pData;

  private:
    // from GUPointerStorage
    virtual void DeleteData()
    {
      delete _pData;
    }
  };
} // end namespace MMG402NumericData
#endif

