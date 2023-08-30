
#ifndef MMG402DomainSpaceDimReductor_H
#define MMG402DomainSpaceDimReductor_H

#include "MMG402\MMG402Domain\IMMG402Domain.h"

#include "MMG402\libs\GUtils\GUNullableType.h"

#include "MMG402\libs\MMG402IndexDimReductor.h"

namespace MMG402Domain
{

  template <typename SpaceScalarType, int SpaceDim, int ReducedSpaceDim>
  class MMG402DomainSpaceDimReductor : public IMMG402Domain<SpaceScalarType, ReducedSpaceDim>, public MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>
  {
  public:

    enum { ReducedDimAtCompile  = ReducedSpaceDim };

    MMG402DomainSpaceDimReductor(const IMMG402Domain<SpaceScalarType, SpaceDim>* ipSourceDomain)
    {
      _pSourceDomain = ipSourceDomain;
      _pReducedBoundingBox = 0;
    }

    virtual void SetReduction(const MMG402Domain::MMG402DomainPoint<GUNullableType<SpaceScalarType>, SpaceDim>& iReductionIndex)
    {
      MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>::SetReduction(iReductionIndex);
      delete _pReducedBoundingBox;
    }

    virtual const MMG402Rectangle<SpaceScalarType, ReducedSpaceDim>& GetBoundingBox() const
    {
      if (_pReducedBoundingBox != 0) return *_pReducedBoundingBox;
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim> upper;
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim> down;
      const MMG402Rectangle<SpaceScalarType, SpaceDim>& pSourceBoundingBox = _pSourceDomain->GetBoundingBox();

      for (unsigned int i = 0; i < _indexMapping.size(); i++) {
        upper[i] = pSourceBoundingBox.GetUpPoint()(_indexMapping[i]);
        down[i] = pSourceBoundingBox.GetLowPoint()(_indexMapping[i]);
      }
      _pReducedBoundingBox = new MMG402Rectangle<SpaceScalarType, ReducedSpaceDim>(down, upper);
      return *_pReducedBoundingBox;
    }

    virtual bool IsContains(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim>& iPoint) const
    {
      return _pSourceDomain->IsContains(GetOriginalIndex(iPoint));
    }

  protected:
    mutable MMG402Rectangle<SpaceScalarType, ReducedSpaceDim>* _pReducedBoundingBox;

    const IMMG402Domain<SpaceScalarType, SpaceDim>* _pSourceDomain;
  };

} // MMG402Domain

#endif // IMMG402Domain_H
