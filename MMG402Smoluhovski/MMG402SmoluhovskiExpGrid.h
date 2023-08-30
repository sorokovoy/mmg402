
#ifndef MMG402SmoluhovskiExpGrid_H
#define MMG402SmoluhovskiExpGrid_H

#include "MMG402\MMG402Grid\IMMG402Grid.h"

namespace MMG402Grid
{

  template <typename SpaceScalarType>
  class MMG402SmoluhovskiExpGrid : public IMMG402Grid<SpaceScalarType, 1>
  {
  public:
    MMG402SmoluhovskiExpGrid(const MMG402Domain::IMMG402Domain<SpaceScalarType, 1>* ipDomain, const IMMG402GridIndexer<1>* cipIndexer, double iDeg)
      : IMMG402Grid<SpaceScalarType, 1>(ipDomain),
        _pIndexer(cipIndexer)
    {
      _deg = iDeg;
      _mVal = 1.00;
      _mVal = f01(1);
    }

    virtual const IMMG402GridIndexer<1>* GetIndexer() const
    {
      return _pIndexer;
    }

    virtual bool IsHasValue(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iIndex) const
    {
      for (int i = 0; i < _steps.SpaceDimAtCompile; i++) {
        if (iIndex[i] >= _pIndexer->GetIndexMaxValue()[i]) return false;
      }
      return true;
    }

    virtual bool IndexToCoord(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iIndex, typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& oPoint) const
    {
      const MMG402Domain::MMG402Rectangle<SpaceScalarType, 1>& bb = _pDomain->GetBoundingBox();
      for (int i = 0; i < _steps.SpaceDimAtCompile; i++) {
        if (iIndex[i] >= _pIndexer->GetIndexMaxValue()[i]) return false;
        oPoint[i] = bb.GetLowPoint()[i] + f01(((double) iIndex[i] / (_pIndexer->GetIndexMaxValue()[i] - 1))) * (bb.GetUpPoint()[i] - bb.GetLowPoint()[i]);
      }
      return true;
    }

  protected:
    double f01(double x) const
    {
      return (exp(_deg * x) - 1)/ _mVal;
    }

    double _deg;
    double _mVal;

    typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> _steps;
    const IMMG402GridIndexer<1>* _pIndexer;
  };


} // end namespace MMG402Grid
#endif
