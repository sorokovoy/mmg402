#ifndef MMG402SolverOutput_H
#define MMG402SolverOutput_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402Solver\MMG402SolverInput.h"

namespace MMG402Solver
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  struct MMG402SolverOutput {
    MMG402SolverInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pInput;
    IMMG402SolverDataStorage<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData;

    MMG402SolverOutput()
    {
      pInput = 0;
      pData  = 0;
    }
  };
} // end namespace MMG402Solver
#endif
