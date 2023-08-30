// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Tests and example of using linear algebra lib.
// Now using Eigen: http://eigen.tuxfamily.org/index.php?title=Main_Page
//
//===================================================================
//  AUG 2013  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef MMG402TestLinearAlgebra_H
#define MMG402TestLinearAlgebra_H

#include "MMG402UnitTest.h"

class MMG402TestLinearAlgebra : public MMG402UnitTest
{
public:
  virtual bool Run();

private:
  bool TestEIGEN();
  bool TestLinearSolverLU();
  bool TestLinearSolverSVD();
  bool TestMatrixValueCalculation();


  bool TestMMG402Matrix();
  bool TestMMG402SparceMatrix();
  bool TestMMG4023DiagonalMatrix();

  bool TestMMG402MatrixSolve();
  bool TestMMG4023DiagonalMatrixSolve1();
  bool TestMMG4023DiagonalMatrixSolve2();
  bool TestMMG4023DiagonalMatrixSolve3();
  bool TestMMG4023DiagonalMatrixSolve4();
  bool TestMMG4023DiagonalMatrixSolve5();
  bool TestMMG4023DiagonalMatrixSolve6();
  bool TestMMG4023DiagonalMatrixSolve7();
  bool TestMMG4023DiagonalMatrixSolve8();
  bool TestMMG4023DiagonalMatrixSolve9();
  bool TestMMG4023DiagonalMatrixSolve10();
  bool TestMMG4023DiagonalMatrixSolve11();
};

#endif // MMG402TestLinearAlgebra_H
