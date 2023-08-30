#ifndef MMG402FuncStorage_H
#define MMG402FuncStorage_H

#include <map>
#include <set>
#include <vector>

#include "MMG402\MMG402Func\IMMG402Func.h"
#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"
#include "MMG402\MMG402Func\MMG402FuncSpaceDimReductor.h"
#include "MMG402\MMG402Func\MMG402FuncStorageKey.h"

namespace MMG402Func
{
  struct MMG402FuncStorageGenericAgrumentKey {
    int SpaceScalarType;
    int SpaceDim;
    int DataScalarType;
    int DataDim;
  };

  inline bool operator < (const MMG402FuncStorageGenericAgrumentKey& iV1, const MMG402FuncStorageGenericAgrumentKey& iV2)
  {
    return (iV1.DataDim < iV2.DataDim) ||
           (iV1.DataDim == iV2.DataDim && iV1.DataScalarType <  iV2.DataScalarType) ||
           (iV1.DataDim == iV2.DataDim && iV1.DataScalarType == iV2.DataScalarType && iV1.SpaceDim <  iV2.SpaceDim) ||
           (iV1.DataDim == iV2.DataDim && iV1.DataScalarType == iV2.DataScalarType && iV1.SpaceDim == iV2.SpaceDim && iV1.SpaceScalarType < iV2.SpaceScalarType);
  }

  class MMG402FuncStorage
  {
  public:
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetFunc(const MMG402FuncStorageKey& iKey) const
    {
      return (IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) GetFunc(GenerateGenericArgumentKey<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(), iKey);
    }

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    void SetFunc(IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipFunc)
    {
      const MMG402FuncStorageKey& iKey = ipFunc->GetDescription().GetKey();
      GUAssert((GetFunc<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey) == 0));

      if (ipFunc->GetType() == IMMG402FuncType_Temporal) {
        GUAssert(dynamic_cast<IMMG402FuncTemporalInterface*>(ipFunc));
        _temporalParts.push_back(dynamic_cast<IMMG402FuncTemporalInterface*>(ipFunc));
      }

      IMMG402FuncSpaceDimReductorDescriptor* pReductionPart = dynamic_cast<IMMG402FuncSpaceDimReductorDescriptor*>(ipFunc);
      if (pReductionPart != 0) {
        _reductionParts.push_back(pReductionPart);
      }

      SetFunc(ipFunc, GenerateGenericArgumentKey<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(), iKey);
    }

    void SetTemporalValue(double iVal)
    {
      for (unsigned int i = 0; i < _temporalParts.size(); i++) {
        _temporalParts[i]->SetTemporalValue(iVal);
      }
    }

    template <typename SpaceScalarType, int SpaceDim, int ReducedSpaceDim>
    void SetReduction(const MMG402Domain::MMG402DomainPoint<GUNullableType<SpaceScalarType>, SpaceDim>& iReductionIndex)
    {
      for (unsigned int i = 0; i < _reductionParts.size(); i++) {
        if (_reductionParts[i]->GetOridginalDim() != SpaceDim || _reductionParts[i]->GetReducedlDim() != ReducedSpaceDim) {
          continue;
        }
        MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>* pAsIndexPart = dynamic_cast<MMG402Common::MMG402IndexDimReductor<SpaceDim, ReducedSpaceDim, SpaceScalarType>*>(_reductionParts[i]);
        GUAssertC(pAsIndexPart);
        pAsIndexPart->SetReduction(iReductionIndex);
      }
    }

    const std::set<MMG402FuncStorageKey>& GetKeys() const
    {
      return _allKeys;
    }

  protected:
    std::set<MMG402FuncStorageKey> _allKeys;
    std::map<MMG402FuncStorageGenericAgrumentKey, std::map<MMG402FuncStorageKey, void*>> _storage;
    std::vector<IMMG402FuncTemporalInterface*> _temporalParts;
    std::vector<IMMG402FuncSpaceDimReductorDescriptor*> _reductionParts;

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    MMG402FuncStorageGenericAgrumentKey GenerateGenericArgumentKey() const
    {
      MMG402FuncStorageGenericAgrumentKey result;
      enum { DataDimAtCompile = DataDim };
      enum { SpaceDimAtCompile = SpaceDim };
      result.DataDim = DataDimAtCompile;
      result.DataScalarType = typeid(DataScalarType).hash_code();
      result.SpaceDim = SpaceDimAtCompile;
      result.SpaceScalarType = typeid(SpaceScalarType).hash_code();
      return result;
    }

    virtual void SetFunc(void* ipFunc, const MMG402FuncStorageGenericAgrumentKey& iGenericKey, const MMG402FuncStorageKey& iFuncKey)
    {
      _storage[iGenericKey][iFuncKey] = ipFunc;
      _allKeys.insert(iFuncKey);
    }

    void* GetFunc(const MMG402FuncStorageGenericAgrumentKey& iGenericKey, const MMG402FuncStorageKey& iKey) const
    {
      std::map<MMG402FuncStorageGenericAgrumentKey, std::map<MMG402FuncStorageKey, void*>>::const_iterator it1 = _storage.find(iGenericKey);
      if (it1 != _storage.end()) {
        std::map<MMG402FuncStorageKey, void*>::const_iterator it2 = it1->second.find(iKey);
        if (it2 != it1->second.end()) {
          return (it2->second);
        }
      }
      return 0;
    }
  };
} // end namespace MMG402Func
#endif
