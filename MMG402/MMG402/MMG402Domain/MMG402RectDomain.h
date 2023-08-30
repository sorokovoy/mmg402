
#ifndef MMG402RectDomain_H
#define MMG402RectDomain_H

#include "MMG402\MMG402Domain\IMMG402Domain.h"

namespace MMG402Domain
{

  template <typename SpaceScalarType, int SpaceDim>
  class MMG402RectDomain : public IMMG402Domain<SpaceScalarType, SpaceDim>
  {
  public:
    MMG402RectDomain(const MMG402Rectangle<SpaceScalarType, SpaceDim>& iSourceRect)
      : _base(iSourceRect)
    {}

    virtual const MMG402Rectangle<SpaceScalarType, SpaceDim>& GetBoundingBox() const
    {
      return _base;
    }

    virtual bool  IsContains(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const
    {
      return _base.IsContains(iPoint);
    }

  protected:
    const MMG402Rectangle<SpaceScalarType, SpaceDim> _base;
  };

} // MMG402Domain
#endif
