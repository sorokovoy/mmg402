#ifndef MMG402GridIndexerLinear_H
#define MMG402GridIndexerLinear_H

#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

namespace MMG402Grid
{

  template <int SpaceDim>
  class MMG402GridIndexerLinear : public IMMG402GridIndexer<SpaceDim>
  {
  public:
    MMG402GridIndexerLinear(const GridIndex& iMax)
      : _indexMax(iMax)
    {
      _iteratorMax = 1;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _iteratorMax *= _indexMax[i];
      }
    }

    virtual unsigned int GetIteratorMaxValue() const
    {
      return _iteratorMax;
    }
    virtual const GridIndex& GetIndexMaxValue() const
    {
      return _indexMax;
    }

    virtual bool IteratorToIndex(unsigned int iIteratorValue, GridIndex& oIndex) const
    {
      if (iIteratorValue >= _iteratorMax) return false;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        oIndex[i] = iIteratorValue % _indexMax[i];
        iIteratorValue = iIteratorValue / _indexMax[i];
      }
      return true;
    }

    virtual bool IndexToIterator(const GridIndex& iIndex, unsigned int& oIteratorValue) const
    {
      oIteratorValue = 0;
      for (int i = SpaceDimAtCompile - 1; i >= 0; i--) {
        if (iIndex[i] >= _indexMax[i]) return false;
        oIteratorValue += iIndex[i];
        if (i - 1 >= 0) {
          oIteratorValue *= _indexMax[i - 1];
        }
      }
      return true;
    }

    virtual bool IsValidIndex(const GridIndex& iIndex) const
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        if (iIndex[i] >= _indexMax[i]) return false;
      }
      return true;
    }

  protected:
    GridIndex _indexMax;
    unsigned int _iteratorMax;
  };

} // end namespace MMG402Grid
#endif
