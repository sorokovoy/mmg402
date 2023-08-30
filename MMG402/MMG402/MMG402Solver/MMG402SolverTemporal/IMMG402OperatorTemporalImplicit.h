#ifndef IMMG402OperatorTemporalImplicit_H
#define IMMG402OperatorTemporalImplicit_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402SolverTemporalImplicitFlags.h"

#include "MMG402\MMG402Matrix\MMG402MatrixDensy.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSparce.h"
#include "MMG402\MMG402Matrix\MMG402Matrix3Diagonal.h"

namespace MMG402Solver
{
  namespace Temporal
  {
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    class IMMG402OperatorTemporalImplicitImplementation : public IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
    {
    public:
      IMMG402OperatorTemporalImplicitImplementation(const MMG402SolverInputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipGeneral)
        : IMMG402OperatorTemporalImplementation<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(ipGeneral)
      {
        pFlags = 0;
      }

    protected:
      MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* GetMatrixData(typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ioTargetData) const
      {
        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pMatrixData = 0;
        if (ioTargetData->IsHasMatrixData()) {
          pMatrixData = ioTargetData->GetMatrixData();
        } else {
          unsigned int n = _pInput->pGridIterator->GetIteratorMaxValue();
          switch (GetFlags().matrixType) {
            case MMG402Matrix::MMG402MatrixTypeDensy: {
              pMatrixData = new MMG402Matrix::MMG402MatrixDensy<typename MMG402NumData>(n, n + 1);
              break;
            }
            case MMG402Matrix::MMG402MatrixTypeSparce: {
              pMatrixData = new MMG402Matrix::MMG402MatrixSparce<typename MMG402NumData>(n, n + 1);
              break;
            }
            case MMG402Matrix::MMG402MatrixType3Diagonal: {
              pMatrixData = new MMG402Matrix::MMG402Matrix3Diagonal<typename MMG402NumData>(n, n + 1);
              break;
            }
            default: {
              throw std::exception("Matrix type not supported");
            }
          }
          ioTargetData->SetMatrixData(pMatrixData);
        }
        return pMatrixData;
      }

      const MMG402SolverTemporalImplicitFlags& GetFlags() const
      {
        if (pFlags == 0) {
          GUAssert((dynamic_cast<MMG402SolverTemporalImplicit<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>(_pInput->pSolver)) != 0);
          MMG402SolverTemporalImplicit<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolver = (MMG402SolverTemporalImplicit<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*) _pInput->pSolver;
          pFlags = &pSolver->GetSolverFlags();
        }
        return *pFlags;
      }

    private:
      mutable const MMG402SolverTemporalImplicitFlags* pFlags;
    };

  } // end namespace Temporal
}// end namespace MMG402Solver
#endif // MMG402OperatorImplicitDt_H
