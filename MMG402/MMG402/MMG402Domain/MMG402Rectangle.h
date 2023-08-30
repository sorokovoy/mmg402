#ifndef MMG402Rectangle_H
#define MMG402Rectangle_H

#include "MMG402\MMG402Domain\MMG402DomainPoint.h"

namespace MMG402Domain
{
  template <typename SpaceScalarType, int SpaceDim>
  class MMG402Rectangle
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };
    MMG402Rectangle(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iLow, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iUp)
      : _low(iLow), _up(iUp)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        GUAssert(_low[i] < _up[i]);
      }
    }

    bool IsContains(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        if (_low[i] > iPoint[i] || iPoint[i] > _up[i]) {
          return false;
        }
      }
      return true;
    }
    const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& GetUpPoint() const
    {
      return _up;
    }
    const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& GetLowPoint() const
    {
      return _low;
    }

  protected:
    const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> _low;
    const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> _up;
  };


} // MMG402Domain
#endif
