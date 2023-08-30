#ifndef MMG402SolverInput_H
#define MMG402SolverInput_H

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolTable.h"

#include "MMG402\MMG402Grid\IMMG402Grid.h"

#include "MMG402\MMG402Solver\IMMG402SolverDataStorage.h"

#include "MMG402\MMG402Expression\IMMG402Operator.h"

#include "MMG402\MMG402Func\MMG402FuncStorage.h"

#include <set>
#include <vector>

namespace MMG402Solver
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402Solver;

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  struct MMG402SolverInput {
    // store io data
    MMG402Solver::IMMG402SolverDataStorage<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pData;

    // store numerical grid
    const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* pGrid;

    // store order of grid sells iterator
    const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pGridIterator;

    // store funcs used in cheme (right part for example)
    MMG402Func::MMG402FuncStorage* pFuncStorage;

    // sub expressions of targetSymbol equationExpression can be empty
    std::vector<MExp::MESymbol> equationSubExpressions;

    // main equation expression. generate result.
    MExp::MESymbol equationExpression;

    // symbol solving realte to
    MExp::MESymbol targetSymbol;

    // store known sybol representation
    MExp::MESymbolTable symbolTable;

    // attached to solver. Setted automatic on initialization
    MMG402Solver::IMMG402Solver<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* pSolver;

    // constructor set default values
    MMG402SolverInput()
    {
      pSolver = 0;
      pGrid = 0;
      pData = 0;
      pGridIterator = 0;
      pFuncStorage = new MMG402Func::MMG402FuncStorage();
    }
  };

}; // end namespace MMG402Solver
#endif
