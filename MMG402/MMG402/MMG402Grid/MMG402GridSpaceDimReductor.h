
#ifndef MMG402GridSpaceDimReductor_H
#define MMG402GridSpaceDimReductor_H

#include "MMG402\MMG402Grid\IMMG402Grid.h"

#include "MMG402\libs\MMG402IndexDimReductor.h"

namespace MMG402Grid
{
  template <typename SpaceScalarType, int SpaceDim, int ReducedSpaceDim>
  class MMG402GridSpaceDimReductor : public IMMG402Grid<SpaceScalarType, ReducedSpaceDim>, public MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, unsigned int>
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };

    MMG402GridSpaceDimReductor(const IMMG402Grid<SpaceScalarType, SpaceDim>* ipSourceGrid, IMMG402GridIndexer<ReducedSpaceDim>* ipReducedIndexer, MMG402Domain::IMMG402Domain<SpaceScalarType, ReducedSpaceDim>* ipReducedDomain)
      : IMMG402Grid<SpaceScalarType, ReducedSpaceDim>(ipReducedDomain)
    {
      _pSourceGrid = ipSourceGrid;
      _pReducedIndexer = ipReducedIndexer;
    }

    virtual const IMMG402GridIndexer<ReducedSpaceDim>* GetIndexer() const
    {
      return _pReducedIndexer;
    }

    virtual bool IsHasValue(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex) const
    {
      return _pSourceGrid->IsHasValue(GetOriginalIndex(iIndex));
    }

    virtual bool GetGridStep(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndexAt, unsigned int iCoordIdx, SpaceScalarType& oToPositive, SpaceScalarType& oToNegative) const
    {
      return _pSourceGrid->GetGridStep(GetOriginalIndex(iIndexAt), _indexMapping[iCoordIdx], oToPositive, oToNegative);
    }

    virtual bool IndexToCoord(typename const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex, typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim>& oPoint) const
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> sourcePoint;
      bool res = _pSourceGrid->IndexToCoord(GetOriginalIndex(iIndex), sourcePoint);
      for (unsigned int i = 0; i < _indexMapping.size(); i++) {
        oPoint(i) = sourcePoint(_indexMapping[i]);
      }
      return res;
    }

  protected:
    const IMMG402Grid<SpaceScalarType, SpaceDim>* _pSourceGrid;
    IMMG402GridIndexer<ReducedSpaceDim>* _pReducedIndexer;
  };
} // end namespace MMG402Grid
#endif // MMG402GridSpaceDimReductor_H
