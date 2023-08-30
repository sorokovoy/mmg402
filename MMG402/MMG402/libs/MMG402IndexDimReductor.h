#ifndef MMG402IndexDimReductor_H
#define MMG402IndexDimReductor_H

#include "MMG402\MMG402Domain\MMG402DomainPoint.h"

#include "MMG402\libs\GUtils\GUNullableType.h"

namespace MMG402Common
{
  template <int SpaceDim, int ReducedSpaceDim, typename SpaceScalarType>
  class MMG402IndexDimReductor
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };
    enum { ReducedDimAtCompile = ReducedSpaceDim };
    virtual void SetReduction(const MMG402Domain::MMG402DomainPoint<GUNullableType<SpaceScalarType>, SpaceDim>& iReductionIndex)
    {
      _indexMapping.clear();
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        if (!iReductionIndex(i).HasValue()) {
          _indexMapping.push_back(i);
        } else {
          _indexTempalte(i) = iReductionIndex(i).GetValue();
        }
      }

      if (_indexMapping.size() != ReducedDimAtCompile) {
        throw new std::exception("Invalid reduction index");
      }
      _reductionIndex = iReductionIndex;
    }

    virtual typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> GetOriginalIndex(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim> iIndex) const
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> index = _indexTempalte;
      for (int i = 0; i < ReducedDimAtCompile; i++) {
        index(_indexMapping[i]) = iIndex(i);
      }
      return index;
    }

  protected:
    MMG402Domain::MMG402DomainPoint<GUNullableType<SpaceScalarType>, SpaceDim> _reductionIndex;
    typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> _indexTempalte;
    std::vector<unsigned int> _indexMapping;
  };
}
#endif MMG402IndexDimReductor_H