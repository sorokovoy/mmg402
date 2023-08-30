#ifndef MMG402SolverDataStorageTemporal_H
#define MMG402SolverDataStorageTemporal_H

#include <list>

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402Solver\IMMG402SolverDataStorage.h"

namespace MMG402Solver
{
  namespace Temporal
  {

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    struct MMG402SolverDataStorageTemporalDataItem {
      MMG402SolverDataStorageTemporalDataItem()
      {
        pData = 0;
        t = 0;
      }
      MMG402SolverDataStorageTemporalDataItem(MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, double iT)
      {
        pData = ipData;
        t = iT;
      }

      ~MMG402SolverDataStorageTemporalDataItem()
      {
        delete pData;
      }

      MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData;
      double t;
    };

    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class MMG402SolverDataStorageTemporal : public IMMG402SolverDataStorage<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      MMG402SolverDataStorageTemporal()
      {
        _maxDeep = 5;
      }

      ~MMG402SolverDataStorageTemporal()
      {
        Clear();
      }

      virtual MMG402SolverDataStorageType GetType() const
      {
        return MMG402SolverDataStorageType_Temporal;
      }

      virtual void Clear()
      {
        for (std::list<MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>::const_iterator it = _storage.begin(); it != _storage.end(); it++) {
          delete *it;
        }
        _storage.clear();
      }

      virtual const MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* Pop(unsigned int iDeep) const
      {
        if (iDeep >= GetCurrentStorageDeep()) return 0;

        std::list<MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>::const_iterator it = _storage.begin();
        while (iDeep > 0) {
          it++;
          iDeep--;
        }
        return *it;
      }

      virtual MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* Pop(unsigned int iDeep)
      {
        if (iDeep >= GetCurrentStorageDeep()) return 0;
        std::list<MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>::iterator it = _storage.begin();
        while (iDeep > 0) {
          it++;
          iDeep--;
        }
        return *it;
      }

      virtual void EraseFirst()
      {
        if (_storage.size() == 0) return;
        delete *_storage.begin();
        _storage.pop_front();
      }

      virtual void Push(MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData)
      {
        _storage.push_front(ipData);
        if (_storage.size() > _maxDeep) {
          delete _storage.back();
          _storage.pop_back();
        }
      }

      virtual void SetStorageMaxDeep(unsigned int iSize)
      {
        _maxDeep = iSize;
      }

      virtual unsigned int GetStorageMaxDeep() const
      {
        return _maxDeep;
      }

      virtual unsigned int GetCurrentStorageDeep() const
      {
        return _storage.size();
      }

    protected:
      std::list<MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*> _storage;
      unsigned int _maxDeep;
    };

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif
