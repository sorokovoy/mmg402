
#ifndef IMMG402Solver_H
#define IMMG402Solver_H

#include "MMG402\libs\ObjExtension.h"

#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"
#include "MMG402\MMG402Solver\MMG402SolverInput.h"
#include "MMG402\MMG402Solver\MMG402SolverOutput.h"

namespace MMG402Solver
{

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402Solver : protected ObjExtension::ObjExtensionClass
  {
  public:
    enum { DataDimAtCompile  = DataDim  };
    enum { SpaceDimAtCompile = SpaceDim };

    IMMG402Solver()
    {
      _isInited = false;
    }

    virtual void Init(MMG402SolverInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
    {
      if (_isInited) {
        throw new std::exception("Alredy inited");
      }
      _isInited = true;
      ipInput->pSolver = this;
      if (ipInput->pGridIterator == 0) {
        ipInput->pGridIterator = ipInput->pGrid->GetIndexer();
      } else {
        GUAssert(ipInput->pGrid->GetIndexer()->GetIteratorMaxValue() == ipInput->pGridIterator->GetIteratorMaxValue());
      }
    }

    virtual bool MakeIteration() = 0;

    virtual const MMG402SolverInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*  GetInput()  const = 0;
    virtual const MMG402SolverOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetOutput() const = 0;

  private:
    bool _isInited;

  };

} // end namespace MMG402Solver
#endif
