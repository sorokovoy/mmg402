
#ifndef MMG402GridUniform_H
#define MMG402GridUniform_H

#include "MMG402\MMG402Grid\IMMG402Grid.h"

namespace MMG402Grid
{

  template <typename SpaceScalarType, int SpaceDim>
  class MMG402GridUniform : public IMMG402Grid<SpaceScalarType, SpaceDim>
  {
  public:
    MMG402GridUniform(const MMG402Domain::IMMG402Domain<SpaceScalarType, SpaceDim>* ipDomain, const IMMG402GridIndexer<SpaceDim>* cipIndexer)
      : IMMG402Grid<SpaceScalarType, SpaceDim>(ipDomain),
        _pIndexer(cipIndexer)
    {
      const MMG402Domain::MMG402Rectangle<SpaceScalarType, SpaceDim>& bb = _pDomain->GetBoundingBox();
      for (int i = 0; i < IMMG402Grid<SpaceScalarType, SpaceDim>::SpaceDimAtCompile; i++) {
        _steps[i] = (bb.GetUpPoint()[i] - bb.GetLowPoint()[i]) / (_pIndexer->GetIndexMaxValue()[i] - 1);
      }

    }

    virtual const IMMG402GridIndexer<SpaceDim>* GetIndexer() const
    {
      return _pIndexer;
    }

    virtual bool GetGridStep(typename const MMG402GridIndex& iIndexAt, unsigned int iCoordIdx, SpaceScalarType& oToPositive, SpaceScalarType& oToNegative) const
    {
      oToNegative = _steps[iCoordIdx];
      oToPositive = _steps[iCoordIdx];
      return true;
    }

    virtual bool IsHasValue(typename const MMG402GridIndex& iIndex) const
    {
      for (int i = 0; i < _steps.SpaceDimAtCompile; i++) {
        if (iIndex[i] >= _pIndexer->GetIndexMaxValue()[i]) return false;
      }
      return true;
    }

    virtual bool IndexToCoord(typename const MMG402GridIndex& iIndex, typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& oPoint) const
    {
      const MMG402Domain::MMG402Rectangle<SpaceScalarType, SpaceDim>& bb = _pDomain->GetBoundingBox();
      for (int i = 0; i < _steps.SpaceDimAtCompile; i++) {
        if (iIndex[i] >= _pIndexer->GetIndexMaxValue()[i]) return false;
        oPoint[i] =  bb.GetLowPoint()[i] + iIndex[i] * _steps[i];
      }
      return true;
    }

  protected:
    typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> _steps;
    const IMMG402GridIndexer<SpaceDim>* _pIndexer;
  };


} // end namespace MMG402Grid
#endif
