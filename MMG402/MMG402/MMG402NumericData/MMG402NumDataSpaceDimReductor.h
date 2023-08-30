#ifndef MMG402NumDataSpaceDimReductor_H
#define MMG402NumDataSpaceDimReductor_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

#include "MMG402\libs\GUtils\GUNullableType.h"

#include "MMG402\libs\MMG402IndexDimReductor.h"

namespace MMG402NumericData
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, int ReducedSpaceDim>
  class MMG402NumDataSpaceDimReductor : public MMG402NumericData::IMMG402NumData<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>,
    public MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, unsigned int>
  {
  public:
    enum { ReducedDimAtCompile  = ReducedSpaceDim };
    enum { SpaceDimAtCompile = SpaceDim };

    MMG402NumDataSpaceDimReductor(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipSourceData, const MMG402Grid::IMMG402Grid<SpaceScalarType, ReducedSpaceDim>* ipReducedGrid)
    {
      _pSourceData = ipSourceData;
      _pReducedGrid = ipReducedGrid;
    }

    virtual bool IsHasData(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex) const
    {
      return _pSourceData->IsHasData(GetOriginalIndex(iIndex));
    }

    virtual MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>& GetData(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex)
    {
      return _pSourceData->GetData(GetOriginalIndex(iIndex));
    }

    virtual const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>& GetData(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex) const
    {
      return _pSourceData->GetData(GetOriginalIndex(iIndex));
    }

    virtual const MMG402Grid::IMMG402Grid<SpaceScalarType, ReducedSpaceDim>* GetMasterGrid() const
    {
      return _pReducedGrid;
    }

    virtual IMMG402NumData<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* CreateCopy() const
    {
      MMG402NumDataDensy<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>* pDataClone = new MMG402NumDataDensy<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>(_pReducedGrid);
      for (unsigned int i = 0; i < _pReducedGrid->GetIndexer()->GetIteratorMaxValue(); i++) {
        MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex index = _pReducedGrid->GetIndexer()->IteratorToIndex(i);
        pDataClone->GetData(index) = _pSourceData->GetData(GetOriginalIndex(index));
      }
      return pDataClone;
    }

  protected:
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pSourceData;
    const MMG402Grid::IMMG402Grid<SpaceScalarType, ReducedSpaceDim>* _pReducedGrid;
  };

} // end namespace MMG402NumericData
#endif
