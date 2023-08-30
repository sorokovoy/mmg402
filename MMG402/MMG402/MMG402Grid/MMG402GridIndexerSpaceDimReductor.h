#ifndef MMG402GridIndexerSpaceDimReductor_H
#define MMG402GridIndexerSpaceDimReductor_H

#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"
#include "MMG402\MMG402Grid\MMG402GridIndexerLinear.h"

#include "MMG402\libs\MMG402IndexDimReductor.h"

#define MMG402GridIndex MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex

namespace MMG402Grid
{
  template <int SpaceDim, int ReducedSpaceDim>
  class MMG402GridIndexerSpaceDimReductor : public IMMG402GridIndexer<ReducedSpaceDim>, public MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, unsigned int>
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };
    enum { ReducedSpaceDimAtCompile = ReducedSpaceDim };

    MMG402GridIndexerSpaceDimReductor(const IMMG402GridIndexer<SpaceDim>* ipSourceGridIndexer)
    {
      _pSourceGridIndexer = ipSourceGridIndexer;
      _isIndexMaxRecalculationRequire = true;
      _isIteratorMaxRecalculationRequire = true;
      _isInnerIndexerRecalculationRequire = true;
      _pInnnerIndexer = 0;
    }

    virtual void SetReduction(const MMG402Domain::MMG402DomainPoint<GUNullableType<unsigned int>, SpaceDim>& iReductionIndex)
    {
      std::vector<unsigned int> priviosIndexMapping = _indexMapping;
      MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, unsigned int>::SetReduction(iReductionIndex);
      if (priviosIndexMapping != _indexMapping) {
        _isIndexMaxRecalculationRequire = true;
        _isIteratorMaxRecalculationRequire = true;
        _isInnerIndexerRecalculationRequire = true;
      }
    }

    virtual unsigned int GetIteratorMaxValue() const
    {
      if (!_isIteratorMaxRecalculationRequire) return _iteratorMaxValue;
      MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex maxIndex = GetIndexMaxValue();
      _iteratorMaxValue = 1;
      for (int i = 0; i < ReducedSpaceDimAtCompile; i++) {
        _iteratorMaxValue *= maxIndex(i);
      }
      _isIteratorMaxRecalculationRequire = false;
      return _iteratorMaxValue;
    }

    virtual const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& GetIndexMaxValue() const
    {
      if (!_isIndexMaxRecalculationRequire) return _indexMax;
      MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex sourceMaxIndex = _pSourceGridIndexer->GetIndexMaxValue();
      for (int i = 0; i < ReducedSpaceDimAtCompile; i++) {
        _indexMax(i) = sourceMaxIndex(_indexMapping[i]);
      }
      return _indexMax;
    }

    virtual bool IteratorToIndex(unsigned int iIteratorValue, MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& oIndex) const
    {
      PrepareInnerIndexer();
      return _pInnnerIndexer->IteratorToIndex(iIteratorValue, oIndex);
    }

    virtual bool IndexToIterator(const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex, unsigned int& oIteratorValue) const
    {
      PrepareInnerIndexer();
      return _pInnnerIndexer->IndexToIterator(iIndex, oIteratorValue);
    }

    virtual bool IsValidIndex(const MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex& iIndex) const
    {
      PrepareInnerIndexer();
      return _pInnnerIndexer->IsValidIndex(iIndex);
    }

  protected:
    void PrepareInnerIndexer() const
    {
      if (!_isInnerIndexerRecalculationRequire) return;

      delete _pInnnerIndexer;
      _pInnnerIndexer = new MMG402Grid::MMG402GridIndexerLinear<ReducedSpaceDim>(GetIndexMaxValue());
      _isInnerIndexerRecalculationRequire = false;
    }

    const IMMG402GridIndexer<SpaceDim>* _pSourceGridIndexer;

    mutable bool _isInnerIndexerRecalculationRequire;
    mutable MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>* _pInnnerIndexer;

    mutable bool _isIndexMaxRecalculationRequire;
    mutable MMG402Grid::IMMG402GridIndexer<ReducedSpaceDim>::GridIndex _indexMax;

    mutable bool _isIteratorMaxRecalculationRequire;
    mutable unsigned int _iteratorMaxValue;
  };


} // end namespace MMG402Grid
#endif // MMG402GridIndexerSpaceDimReductor_H
