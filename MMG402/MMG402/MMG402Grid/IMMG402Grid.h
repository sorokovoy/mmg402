
#ifndef IMMG402Grid_H
#define IMMG402Grid_H

#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"
#include "MMG402\MMG402Domain\IMMG402Domain.h"

#include "MMG402\libs\GUtils\GUMathUtils.h"

namespace MMG402Grid
{
  template <typename SpaceScalarType, int SpaceDim>
  class IMMG402Grid
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };

    virtual ~IMMG402Grid() {};

    virtual const IMMG402GridIndexer<SpaceDim>* GetIndexer() const = 0;
    virtual const MMG402Domain::IMMG402Domain<SpaceScalarType, SpaceDim>* GetDomain() const
    {
      return _pDomain;
    }

    virtual bool IsHasValue(typename const MMG402GridIndex& iIndex) const = 0;
    virtual bool IndexToCoord(typename const MMG402GridIndex& iIndex, typename MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& oPoint) const = 0;

    virtual bool GetGridStep(typename const MMG402GridIndex& iIndexAt, unsigned int iCoordIdx, SpaceScalarType& oToPositive, SpaceScalarType& oToNegative) const
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> pCenter;
      GUAssertV(IndexToCoord(iIndexAt, pCenter), false);

      MMG402GridIndex positive = iIndexAt;
      positive(iCoordIdx)++;
      if(IsHasValue(positive)) {
        MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> pPositive;
        GUAssert(IndexToCoord(positive, pPositive));
        oToPositive = GUAbs(pPositive(iCoordIdx) - pCenter(iCoordIdx));
      } else {
        oToPositive = 0;
      }

     MMG402GridIndex negative = iIndexAt;
      negative(iCoordIdx)--;
      if(IsHasValue(negative)) {
        MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> pNegative;
        GUAssert(IndexToCoord(negative, pNegative));
        oToNegative = GUAbs(pNegative(iCoordIdx) - pCenter(iCoordIdx));
      } else {
        oToNegative = 0;
      }
      return true;
    }

  protected:
    IMMG402Grid(const MMG402Domain::IMMG402Domain<SpaceScalarType, SpaceDim>* ipDomain) : _pDomain(ipDomain) {};
    const MMG402Domain::IMMG402Domain<SpaceScalarType, SpaceDim>* _pDomain;

  private:
    IMMG402Grid() {};
    IMMG402Grid(const IMMG402Grid&) {};
  };

} // end namespace MMG402Grid
#endif // IMMG402Grid
