
#include "MMG402.h"

int main()
{
  MMG402Solver::Temporal::MMG402OperatorSplittingScheme<double, 3, double, 3, 1> Red;
  MMG402Solver::Temporal::MMG402OperatorCellSolver<double, 3, double, 3, double> Red1;
  //Red(MESymbol("M"), 0, new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 3>());
}