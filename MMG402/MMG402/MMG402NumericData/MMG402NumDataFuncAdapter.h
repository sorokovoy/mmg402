#ifndef MMG402NumDataFuncAdapter_H
#define MMG402NumDataFuncAdapter_H

#include "MMG402\libs\GUtils\GUAssert.h"

#include "MMG402\MMG402Func\IMMG402Func.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402NumericData\MMG402NumDataDensy.h"

namespace MMG402NumericData
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402NumDataFuncAdapter : public IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402NumDataFuncAdapter(const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* ipMasterGrid, const MMG402Func::IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipFunc)
      : IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipMasterGrid),
        _pFunc(ipFunc)
    {}

    virtual EqNumDataType& GetData(typename const MMG402GridIndex& iIndex)
    {
      throw new std::invalid_argument("Operation forbidden");
    }

    virtual const EqNumDataType& GetData(typename const MMG402GridIndex& iIndex) const
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> point;
      if (!_pGrid->IndexToCoord(iIndex, point)) {
        throw new std::invalid_argument("Argumeout out of range");
      } else {
        // !todo fix this HACK
        val = _pFunc->GetValue(point);
        return val;
      }
    }

    virtual bool IsHasData(typename const MMG402GridIndex& iIndex) const
    {
      return _pGrid->GetIndexer()->IsValidIndex(iIndex);
    }

    virtual IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateCopy() const
    {
      MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pRes = new MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_pGrid);
      for (unsigned int i = 0; i < _pGrid->GetIndexer()->GetIteratorMaxValue(); i++) {
        MMG402GridIndex index;
        MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>  point;
        _pGrid->GetIndexer()->IteratorToIndex(i, index);
        _pGrid->IndexToCoord(index, point);

        pRes->GetData(index) = _pFunc->GetValue(point);
      }
      return pRes;
    }

  protected:
    mutable EqNumDataType val;
    const MMG402Func::IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pFunc;
  };

} // end namespace MMG402NumericData
#endif
