
#ifndef MMG402FuncSpaceDimReductor_H
#define MMG402FuncSpaceDimReductor_H

//nclude "MMG402\libs\MMG402IndexDimReductor.h"
#include "MMG402\libs\MMG402IndexDimReductor.h"

#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"
#include "MMG402\libs\GUtils\GUNullableType.h"

namespace MMG402Func
{
  class IMMG402FuncSpaceDimReductorDescriptor
  {
  public:
    virtual int GetOridginalDim() const = 0;
    virtual int GetReducedlDim() const = 0;
    virtual std::vector<bool> GetIsReductedSpaceInfo() const = 0;
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim, int ReducedSpaceDim>
  class MMG402FuncSpaceDimReductor : public IMMG402FuncTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>,
    public MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>,
    public IMMG402FuncSpaceDimReductorDescriptor
  {
  public:
    MMG402FuncSpaceDimReductor(IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipFunc)
      : IMMG402FuncTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>(ipFunc->GetDescription().GetKey())
    {
      _pSourceFunc = ipFunc;
      if (ipFunc->GetType() == MMG402Func::IMMG402FuncType_Temporal) {
        _pTemporalInterface = dynamic_cast<IMMG402FuncTemporal<SpaceScalarType, ReducedSpaceDim, DataScalarType, DataDim>*> (ipFunc);
      }
    }

    virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, ReducedSpaceDim>& iPoint) const
    {
      return _pSourceFunc->GetValue(GetOriginalIndex(iPoint));
    }

    virtual void SetTemporalValue(double iVal)
    {
      if (_pTemporalInterface != 0)
        _pTemporalInterface->SetTemporalValue(iVal);
    }

    virtual IMMG402FuncType GetType() const
    {
      return _pSourceFunc->GetType();
    }

    virtual int GetOridginalDim() const
    {
      return SpaceDimAtCompile;
    }

    virtual int GetReducedlDim() const
    {
      return ReducedSpaceDim;
    }

    virtual std::vector<bool> GetIsReductedSpaceInfo() const
    {
      std::vector<bool> res;
      for(unsigned int i = 0; i < SpaceDim; i++) {
        res.push_back(_reductionIndex[i].HasValue());
      }
      return res;
    }

  protected:
    const IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pSourceFunc;
    IMMG402FuncTemporalInterface* _pTemporalInterface;
  };

} // end namespace MMG402Func
#endif // IMMG402Func_H
