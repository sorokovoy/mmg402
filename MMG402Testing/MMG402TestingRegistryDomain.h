#ifndef MMG402TestingRegistryDomain_H
#define MMG402TestingRegistryDomain_H

#include "MMG402\MMG402Domain\IMMG402Domain.h"
#include "MMG402\MMG402Domain\MMG402RectDomain.h"

using namespace MMG402Domain;

class MMG402TestingRegistryDomain
{
public:
  static unsigned int GetDomainCount()
  {
    return 2;
  }

  template <typename SpaceScalarType, int SpaceDim>
  static IMMG402Domain<SpaceScalarType, SpaceDim>* CreateDomain(unsigned int iID)
  {
    enum { SpaceDimAtCompile = SpaceDim };
    switch (iID) {
      case 0: {
        Eigen::AlignedBox<SpaceScalarType, SpaceDim> bb;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          bb.min()[i] = -50;
          bb.max()[i] =  50;
        }
        return new MMG402RectDomain<double, SpaceDim>(bb);
      }
      case 1: {
        Eigen::AlignedBox<SpaceScalarType, SpaceDim> bb;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          bb.min()[i] = -1;
          bb.max()[i] =  1;
        }
        return new MMG402RectDomain<double, SpaceDim>(bb);
      }
    }
    return 0;
  }

private:
  MMG402TestingRegistryDomain() {};
  MMG402TestingRegistryDomain(const MMG402TestingRegistryDomain&) {};
};

#endif
