#ifndef IMMG402FuncTemporal_H
#define IMMG402FuncTemporal_H

#include "MMG402\MMG402Func\IMMG402Func.h"

namespace MMG402Func
{
  class IMMG402FuncTemporalInterface
  {
  public:
    virtual void SetTemporalValue(double iVal) = 0;
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402FuncTemporal : public IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>, public IMMG402FuncTemporalInterface
  {
  public:
    IMMG402FuncTemporal(const MMG402FuncStorageKey& iKey)
      : IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey, IMMG402FuncType_Temporal)
    {}

    virtual IMMG402FuncType GetType() const
    {
      return IMMG402FuncType_Temporal;
    }

    virtual void  SetTemporalValue(double iVal) = 0;
    virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const = 0;
  };
} // end namespace MMG402Func
#endif //IMMG402FuncTemporal_H
