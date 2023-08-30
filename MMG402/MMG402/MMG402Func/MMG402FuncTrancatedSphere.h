#ifndef MMG402FuncTrancatedSphere_H
#define MMG402FuncTrancatedSphere_H

#include "MMG402\libs\GUtils\GUMathUtils.h"

#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"

namespace MMG402Func
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402FuncTrancatedSphere : public IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402FuncTrancatedSphere(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iCenter, SpaceScalarType iRadius, typename const MMG402NumData& iVal, const MMG402FuncStorageKey& iKey)
      : IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey)
    {
      _center = iCenter;
      _sqRadius = GUSqr(iRadius);
      _value = iVal;
      _zeroValue =  MMG402NumData();
      for (int i = 0; i < _zeroValue.SpaceDimAtCompile; i++) {
        _zeroValue[i] = 0;
      }
    }

    virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const
    {
      enum { SpaceDimAtCompile = SpaceDim };
      SpaceScalarType d = 0;
      bool hasNegative = false;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        d += GUSqr(iPoint[i] - _center[i]);
        if (iPoint[i] < 0)
          hasNegative = true;
      }

      if (d > _sqRadius || !hasNegative) {
        return _zeroValue;
      } else {
        return _value;
      }
    }

    virtual void  SetTemporalValue(double iVal)
    {}

  protected:
    typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> _center;
    SpaceScalarType _sqRadius;
    typename MMG402NumData _value;
    typename MMG402NumData _zeroValue;
  };

} // end namespace MMG402Func
#endif
