#ifndef MMG402SolverTemporalUniversalFlags_H
#define MMG402SolverTemporalUniversalFlags_H

#include "MMG402/MMG402Matrix/IMMG402Matrix.h"

namespace MMG402Solver
{
  namespace Temporal
  {
    struct MMG402SolverTemporalUniversalFlags {
      MMG402SolverTemporalUniversalFlags()
      {
        IsNoMatrixRecalculate = false;
        matrixType = MMG402Matrix::MMG402MatrixTypeSparce;
      }

      bool IsNoMatrixRecalculate;
      MMG402Matrix::MMG402MatrixType matrixType;
    };

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif