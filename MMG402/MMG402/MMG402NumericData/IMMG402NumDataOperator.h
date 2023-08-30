#ifndef IMMG402NumDataOperator_H
#define IMMG402NumDataOperator_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

namespace MMG402NumericData
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402NumDataOperator
  {
  public:
    virtual void Process(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* opRes)
    {
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = ipData->GetMasterGrid()->GetIndexer();
      for (unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {
        typename MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        opRes->GetData(index) = Process(ipData, index);
      }
    }

    virtual MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* Process(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData)
    {
      IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pRes = ipData->CreateCopy();
      Process(ipData,  pRes);
      return pRes;
    }

  protected:
    virtual typename MMG402NumData Process(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iIndex)
    {
      if (!BeginPointProcess(ipData, iIndex)) {
        return ipData->GetData(iIndex);
      }
      MMG402GridIndex index;
      int incIndex[SpaceDim];

      unsigned int maskDeepth = GetMaskDeepth();

      for (unsigned int i = 0; i < SpaceDim; i++) {
        incIndex[i] = -(int)maskDeepth;
      }
      MMG402GridIndex max = ipData->GetMasterGrid()->GetIndexer()->GetIndexMaxValue();
      while (true) {
        index = iIndex;
        bool setIndexOk = true;
        for (unsigned int i = 0; i < SpaceDim; i++) {
          index(i) += incIndex[i];
          if (index(i) >= max(i)) {
            setIndexOk = false;
          }
        }

        // index set ok
        if (setIndexOk) {
          PointProcess(ipData, iIndex, index);
        }

        int incrIdx = 0;
        // increment index;
        while (true) {
          incIndex[incrIdx]++;
          if (incIndex[incrIdx] == maskDeepth + 1) {
            incIndex[incrIdx] = -(int)maskDeepth;
            incrIdx ++;
            if (incrIdx == SpaceDim) {
              break;
            }
          } else {
            break;
          }
        }

        // incremintation fail. End loop.
        if (incrIdx == SpaceDim) {
          break;
        }
      }
      return FinishPointProcess();
    }

    virtual int GetMaskTotalPoints() const
    {
      return GUPow(GetMaskDeepth() * 2 + 1, SpaceDim);
    }

    virtual int GetMaskDeepth() const = 0;

    virtual bool BeginPointProcess(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex) = 0;
    virtual void PointProcess(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex, typename const MMG402GridIndex& iNearIndex) = 0;
    virtual typename MMG402NumData FinishPointProcess() = 0;
  };
} // end namespace MMG402NumericData
#endif
