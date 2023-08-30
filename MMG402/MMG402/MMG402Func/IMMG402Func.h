
#ifndef IMMG402Func_H
#define IMMG402Func_H

#include "MMG402\MMG402Domain\IMMG402Domain.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

#include "MMG402\MMG402Func\MMG402FuncDescription.h"

namespace MMG402Func
{
  enum IMMG402FuncType {
    IMMG402FuncType_Undefined,
    IMMG402FuncType_Simple,
    IMMG402FuncType_Temporal
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402Func
  {
  public:
    IMMG402Func(const MMG402FuncStorageKey& iKey)
      : _description(MMG402FuncDescription::ConstructInstance<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey, IMMG402FuncType_Simple))
    {}

    virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const = 0;
    virtual const MMG402FuncDescription& GetDescription() const
    {
      return _description;
    }

    virtual IMMG402FuncType GetType() const
    {
      return IMMG402FuncType_Simple;
    }

  protected:
    const MMG402FuncDescription _description;
    IMMG402Func(const MMG402FuncStorageKey& iKey, IMMG402FuncType iType)
      : _description(MMG402FuncDescription::ConstructInstance<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey, iType))
    {}

  private:
    IMMG402Func()
      : _description(MMG402FuncDescription::ConstructInstance<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Undefined, "Undefined"), IMMG402FuncType_Undefined))
    {};
    IMMG402Func(const IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>&)
      : _description(MMG402FuncDescription::ConstructInstance<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(MMG402FuncStorageKey(MMG402FuncDestination_Undefined, "Undefined"), IMMG402FuncType_Undefined))
    {};
  };

} // end namespace MMG402Func
#endif // IMMG402Func_H
