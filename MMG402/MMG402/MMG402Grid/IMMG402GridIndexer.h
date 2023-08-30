#ifndef IMMG402GridIndexer_H
#define IMMG402GridIndexer_H

#include "MMG402\libs\GUtils\GUAssert.h"
#include "MMG402\MMG402Domain\MMG402DomainPoint.h"

#define MMG402GridIndex MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex

namespace MMG402Grid
{
  template <int SpaceDim>
  class IMMG402GridIndexer
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };

    typedef MMG402Domain::MMG402DomainPoint<unsigned int, SpaceDim> GridIndex;

    virtual unsigned int GetIteratorMaxValue() const = 0;
    virtual const GridIndex& GetIndexMaxValue() const = 0;
    virtual bool IteratorToIndex(unsigned int iIteratorValue, GridIndex& oIndex) const = 0;

    virtual GridIndex IteratorToIndex(unsigned int iIteratorValue) const
    {
      GridIndex result;
      GUAssert(IteratorToIndex(iIteratorValue, result));
      return result;
    }

    virtual bool IndexToIterator(const GridIndex& iIndex, unsigned int& oIteratorValue) const = 0;
    virtual unsigned int IndexToIterator(const GridIndex& iIndex) const
    {
      unsigned int result;
      GUAssert(IndexToIterator(iIndex, result));
      return result;
    }

    virtual bool IsValidIndex(const GridIndex& iIndex) const = 0;
  };


} // end namespace MMG402Grid
#endif
