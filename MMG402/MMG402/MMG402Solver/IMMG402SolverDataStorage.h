#ifndef IMMG402SolverDataStorage_H
#define IMMG402SolverDataStorage_H

namespace MMG402Solver
{

  enum MMG402SolverDataStorageType {
    MMG402SolverDataStorageType_Temporal = 1
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402SolverDataStorage
  {
  public:
    virtual MMG402SolverDataStorageType GetType() const = 0;
  };

} // end namespace MMG402Solver
#endif // IMMG402SolverDataStorage
