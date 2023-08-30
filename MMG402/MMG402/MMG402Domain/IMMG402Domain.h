
#ifndef IMMG402Domain_H
#define IMMG402Domain_H

#include "MMG402\MMG402Domain\MMG402DomainPoint.h"
#include "MMG402\MMG402Domain\MMG402Rectangle.h"

namespace MMG402Domain
{

  template <typename SpaceScalarType, int SpaceDim>
  class IMMG402Domain
  {
  public:
    virtual const MMG402Rectangle<SpaceScalarType, SpaceDim>& GetBoundingBox() const = 0;
    virtual bool  IsContains(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const = 0;
  };

} // MMG402Domain

#endif // IMMG402Domain_H
