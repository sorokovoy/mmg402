#ifndef MMG402TestingRegistryGrid_H
#define MMG402TestingRegistryGrid_H

#include "MMG402\MMG402Grid\IMMG402Grid.h"
#include "MMG402\MMG402Grid\MMG402GridIndexerLinear.h"
#include "MMG402\MMG402Grid\MMG402GridUniform.h"

using namespace MMG402Grid;

class MMG402TestingRegistryGrid
{
public:
  static unsigned int GetGridCount()
  {
    return 3;
  }

  template <typename SpaceScalarType, int SpaceDim>
  static IMMG402Grid<SpaceScalarType, SpaceDim>* CreateGrid(unsigned int iID, IMMG402Domain<SpaceScalarType, SpaceDim>* ipDomain)
  {
    enum { SpaceDimAtCompile = SpaceDim };
    switch (iID) {
      case 0: {
        Eigen::Matrix<unsigned int, SpaceDim, 1> gridSize;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          gridSize[i] = 30;
        }
        return new MMG402GridUniform<double, SpaceDim>(ipDomain, new MMG402GridIndexerLinear(gridSize));
      }
      case 1: {
        Eigen::Matrix<unsigned int, SpaceDim, 1> gridSize;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          gridSize[i] = 50;
        }
        return new MMG402GridUniform<double, SpaceDim>(ipDomain, new MMG402GridIndexerLinear(gridSize));
      }
      case 2: {
        Eigen::Matrix<unsigned int, SpaceDim, 1> gridSize;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          gridSize[i] = 100;
        }
        return new MMG402GridUniform<double, SpaceDim>(ipDomain, new MMG402GridIndexerLinear(gridSize));
      }
    }
    return 0;
  }


private:
  MMG402TestingRegistryGrid() {};
  MMG402TestingRegistryGrid(const MMG402TestingRegistryGrid&) {};
};

#endif
