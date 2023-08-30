#ifndef MMG402FuncConstant_H
#define MMG402FuncConstant_H

#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"

namespace MMG402Func
{

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402FuncConstant : public IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402FuncConstant(typename const MMG402NumData& iVal, const MMG402FuncStorageKey& iKey)
      : IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey)
    {
      _value = iVal;
    }

    virtual void SetTemporalValue(double iVal)
    {}

    virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const
    {
      return _value;
    }

  protected:
    typename MMG402NumData _value;
  };

} // end namespace MMG402Func
#endif
