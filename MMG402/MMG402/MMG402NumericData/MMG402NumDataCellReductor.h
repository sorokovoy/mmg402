#ifndef MMG402NumDataCellReductor_H
#define MMG402NumDataCellReductor_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

#include "MMG402\libs\GUtils\GUNullableType.h"

#include "MMG402\libs\MMG402IndexDimReductor.h"

namespace MMG402NumericData
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, typename InnerSolverSpaceScalarType>
  class MMG402NumDataCellReductor : public MMG402NumericData::IMMG402NumData<InnerSolverSpaceScalarType, 1, DataScalarType, 1>
  {
  public:
    MMG402NumDataCellReductor(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipSourceData, const MMG402Grid::IMMG402Grid<InnerSolverSpaceScalarType, 1>* ipInnerSolverGrid)
    {
      _pSourceData = ipSourceData;
      _pInnerSolverGrid = ipInnerSolverGrid;
    }

    void SetCurrentCellIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& iIndex)
    {
      _currentCellIndex = iIndex;
      GUAssert(_pSourceData->IsHasData(iIndex));
    }

    virtual bool IsHasData(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iIndex) const
    {
      return (iIndex(0) < DataDim);
    }

    virtual MMG402Domain::MMG402DomainPoint<DataScalarType, 1>& GetData(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iIndex)
    {
      _currentValue(0) = _pSourceData->GetData(_currentCellIndex)(iIndex(0));
      return _currentValue;
    }

    virtual const MMG402Domain::MMG402DomainPoint<DataScalarType, 1>& GetData(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iIndex) const
    {
      _currentValue(0) = _pSourceData->GetData(_currentCellIndex)(iIndex(0));
      return _currentValue;
    }

    virtual const MMG402Grid::IMMG402Grid<InnerSolverSpaceScalarType, 1>* GetMasterGrid() const
    {
      return _pInnerSolverGrid;
    }

    virtual IMMG402NumData<InnerSolverSpaceScalarType, 1, DataScalarType, 1>* CreateCopy() const
    {
      MMG402NumDataDensy<InnerSolverSpaceScalarType, 1, DataScalarType, 1>* pDataClone = new MMG402NumDataDensy<InnerSolverSpaceScalarType, 1, DataScalarType, 1>(_pInnerSolverGrid);
      for (unsigned int i = 0; i < _pInnerSolverGrid->GetIndexer()->GetIteratorMaxValue(); i++) {
        pDataClone->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(i)) = _pSourceData->GetData(_currentCellIndex)(i);
      }
      return pDataClone;
    }

  protected:
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pSourceData;
    const MMG402Grid::IMMG402Grid<InnerSolverSpaceScalarType, 1>* _pInnerSolverGrid;

    typename MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex _currentCellIndex;

    mutable typename MMG402Domain::MMG402DomainPoint<DataScalarType, 1> _currentValue;
  };

} // end namespace MMG402NumericData
#endif
