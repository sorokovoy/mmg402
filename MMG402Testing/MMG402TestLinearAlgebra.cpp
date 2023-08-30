// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Tests and example of using linear algebra lib.
// Now using Eigen: http://eigen.tuxfamily.org/index.php?title=Main_Page
//
//===================================================================
//  AUG 2013  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#include "MMG402TestLinearAlgebra.h"


#include "MMG402/libs/Eigen/Core"
#include "MMG402/libs/Eigen/LU"
#include "MMG402/libs/Eigen/SVD"

#include "MMG402/MMG402Matrix/MMG402MatrixSparce.h"
#include "MMG402/MMG402Matrix/MMG402Matrix3Diagonal.h"
#include "MMG402/MMG402Matrix/MMG402MatrixSolver.h"

using namespace Eigen;
using namespace std;

bool MMG402TestLinearAlgebra::Run()
{
  RUN_TMMG402T(TestEIGEN());
  RUN_TMMG402T(TestMMG402Matrix());
  RUN_TMMG402T(TestMMG402MatrixSolve());
  return true;
}

bool MMG402TestLinearAlgebra::TestEIGEN()
{
  ASSERT_TRUE(TestLinearSolverLU());
  ASSERT_TRUE(TestLinearSolverSVD());
  ASSERT_TRUE(TestMatrixValueCalculation());
  return true;
}

bool MMG402TestLinearAlgebra::TestLinearSolverSVD()
{
  MatrixXf A = MatrixXf::Random(20, 20);
  A(0, 0) = 0;
  VectorXf b = VectorXf::Random(20);
  Eigen::JacobiSVD<Eigen::MatrixXf> svd(A, Eigen::ComputeThinU | Eigen::ComputeThinV);
  VectorXf x = svd.solve(b);
  double err = (A * x - b).norm();
  ASSERT_TRUE(err < 0.1);
  return true;
}

bool MMG402TestLinearAlgebra::TestLinearSolverLU()
{
  MatrixXf A = MatrixXf::Random(20, 20);
  A(0, 0) = 0;
  VectorXf b = VectorXf::Random(20);
  VectorXf x = A.lu().solve(b);
  double err = (A * x - b).norm();
  ASSERT_TRUE(err < 0.1);
  return true;
}

bool MMG402TestLinearAlgebra::TestMatrixValueCalculation()
{
  MatrixXf A = MatrixXf::Random(20, 20);
  double det = A.determinant();
// cout << "det is " << det << endl;

  JacobiSVD<MatrixXf> svd(A);
  double cond = svd.singularValues()(0) / svd.singularValues()(svd.singularValues().size() - 1);
//  cout << "cond is " << cond << endl;

  return true;
}


bool MMG402TestLinearAlgebra::TestMMG402Matrix()
{
  ASSERT_TRUE(TestMMG402SparceMatrix());
  ASSERT_TRUE(TestMMG4023DiagonalMatrix());
  return true;
}

bool MMG402TestLinearAlgebra::TestMMG402SparceMatrix()
{
  MMG402Matrix::MMG402MatrixSparce<int> arr1(10, 10);
  MMG402Matrix::MMG402MatrixSparce<int> arr2(10, 10);
  arr1(0, 0) = 0;
  arr1(1, 1) = 1;
  arr1(2, 2) = 3;
  arr1(7, 3) = 3;

  arr2(0, 0) = 1;
  arr2(1, 1) = 3;
  arr2(4, 8) = -1;
  arr2(2, 3) = 3;

  arr1 += arr2;

  ASSERT_EQUALS(arr1(0, 0), 1);
  ASSERT_EQUALS(arr1(1, 1), 4);

  ASSERT_EQUALS(arr1(2, 2), 3);
  ASSERT_EQUALS(arr1(2, 3), 3);

  ASSERT_EQUALS(arr1(7, 3), 3);
  ASSERT_EQUALS(arr1(7, 7), 0);

  arr1.RowMult(2, 2);
  ASSERT_EQUALS(arr1(2, 2), 6);
  ASSERT_EQUALS(arr1(2, 3), 6);
  ASSERT_EQUALS(arr1(2, 0), 0);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrix()
{
  MMG402Matrix::MMG402Matrix3Diagonal<int> m1(10, 11);
  m1.GetRawDataDiagonal();
  m1.GetRawDataRightPart();

  m1(0, 0) = 2;
  m1(1, 0) = 3;
  m1(1, 1) = 4;

  bool isHasException = false;
  try {
    m1(5, 9) = 43;
  } catch (...) {
    isHasException = true;
  }
  ASSERT_TRUE(isHasException);

  m1(3, 10) = 4;

  MMG402Matrix::MMG402Matrix3Diagonal<int> m2(10, 11);

  m2(3, 10) = 2;
  m2(4, 10) = 4;
  m2(0, 0) = 2;

  m1 += m2;

  ASSERT_EQUALS(m1(0, 0), 4);
  ASSERT_EQUALS(m1(1, 0), 3);
  ASSERT_EQUALS(m1(1, 1), 4);
  ASSERT_EQUALS(m1(3, 10), 6);
  ASSERT_EQUALS(m1(4, 10), 4);
  ASSERT_EQUALS(m1(3, 3), 0);


  m1.RowMult(1, 2);
  ASSERT_EQUALS(m1(1, 0), 6);
  ASSERT_EQUALS(m1(1, 1), 8);
  ASSERT_EQUALS(m1(3, 10), 6);
  ASSERT_EQUALS(m1(4, 10), 4);
  ASSERT_EQUALS(m1(3, 3), 0);

  m1.RowMult(3, 2);
  ASSERT_EQUALS(m1(1, 0), 6);
  ASSERT_EQUALS(m1(1, 1), 8);
  ASSERT_EQUALS(m1(3, 10), 12);
  ASSERT_EQUALS(m1(4, 10), 4);
  ASSERT_EQUALS(m1(3, 3), 0);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG402MatrixSolve()
{
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve7());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve8());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve9());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve10());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve11());
  
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve1());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve2());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve3());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve4());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve5());
  ASSERT_TRUE(TestMMG4023DiagonalMatrixSolve6());

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve1()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(3, 4);
  m(0, 0) = 1;
  m(0, 1) = 1;
  m(1, 0) = 1;
  m(1, 1) = 1;
  m(1, 2) = 1;
  m(2, 1) = 1;
  m(2, 2) = 1;

  m(0, 3) = 1;
  m(1, 3) = 1;
  m(2, 3) = 1;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 0);
  ASSERT_DOUBLE_EQUALS(res[1](0), 1);
  ASSERT_DOUBLE_EQUALS(res[2](0), 0);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve2()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 1;
  m(0, 1) = 1;
  m(1, 0) = 1;
  m(1, 1) = 1;
  m(1, 2) = 1;
  m(2, 1) = 1;
  m(2, 2) = 1;
  m(2, 3) = 1;
  m(3, 2) = 1;
  m(3, 3) = 1;
  m(0, 4) = 1;
  m(1, 4) = 1;
  m(2, 4) = 1;
  m(3, 4) = 1;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 1);
  ASSERT_DOUBLE_EQUALS(res[1](0), 0);
  ASSERT_DOUBLE_EQUALS(res[2](0), 0);
  ASSERT_DOUBLE_EQUALS(res[3](0), 1);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve4()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 1;
  m(0, 1) = 1;

  m(1, 0) = 2;
  m(1, 1) = 2;
  m(1, 2) = 4;

  m(2, 1) = 2;
  m(2, 2) = 4;
  m(2, 3) = 4;

  m(3, 2) = 1;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 4);
  ASSERT_DOUBLE_EQUALS(res[1](0), -3);
  ASSERT_DOUBLE_EQUALS(res[2](0), 0);
  ASSERT_DOUBLE_EQUALS(res[3](0), 2);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve5()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 1;

  m(1, 0) = 2;
  m(1, 1) = 2;
  m(1, 2) = 4;

  m(2, 1) = 0;
  m(2, 2) = 1;
  m(2, 3) = 4;

  m(3, 2) = 1;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 4);
  ASSERT_DOUBLE_EQUALS(res[1](0), -7);
  ASSERT_DOUBLE_EQUALS(res[2](0), 2);
  ASSERT_DOUBLE_EQUALS(res[3](0), 0);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve6()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 1;

  m(1, 0) = 2;
  m(1, 1) = 2;
  m(1, 2) = 0;

  m(2, 1) = 0;
  m(2, 2) = 0;
  m(2, 3) = 4;

  m(3, 2) = 1;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 0);
  ASSERT_DOUBLE_EQUALS(res[1](0), 1);
  ASSERT_DOUBLE_EQUALS(res[2](0), 3.0/2);
  ASSERT_DOUBLE_EQUALS(res[3](0), 1.0/2);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve7()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 1;
  m(0, 1) = 0;

  m(1, 0) = 0;
  m(1, 1) = 1;
  m(1, 2) = 0;

  m(2, 1) = 0;
  m(2, 2) = 1;
  m(2, 3) = 0;

  m(3, 2) = 0;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 1;
  m(2, 4) = 1;
  m(3, 4) = 1;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 1);
  ASSERT_DOUBLE_EQUALS(res[1](0), 1);
  ASSERT_DOUBLE_EQUALS(res[2](0), 1);
  ASSERT_DOUBLE_EQUALS(res[3](0), 1);

  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve8()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 0;

  m(1, 0) = 0;
  m(1, 1) = 0;
  m(1, 2) = 4;

  m(2, 1) = 0;
  m(2, 2) = 1;
  m(2, 3) = 4;

  m(3, 2) = 1;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_EQUALS(res.size(), 0);
  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve9()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 0;

  m(1, 0) = 0;
  m(1, 1) = 0;
  m(1, 2) = 4;

  m(2, 1) = 1;
  m(2, 2) = 0;
  m(2, 3) = 4;

  m(3, 2) = 0;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);

  ASSERT_DOUBLE_EQUALS(res[0](0), 0.5);
  ASSERT_DOUBLE_EQUALS(res[1](0), -6);
  ASSERT_DOUBLE_EQUALS(res[2](0), 0.5);
  ASSERT_DOUBLE_EQUALS(res[3](0), 2);
  
  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve10()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 0;

  m(1, 0) = 0;
  m(1, 1) = 0;
  m(1, 2) = 4;

  m(2, 1) = 1;
  m(2, 2) = 0;
  m(2, 3) = 0;

  m(3, 2) = 0;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);

  ASSERT_DOUBLE_EQUALS(res[0](0), 0.5);
  ASSERT_DOUBLE_EQUALS(res[1](0), 2);
  ASSERT_DOUBLE_EQUALS(res[2](0), 0.5);
  ASSERT_DOUBLE_EQUALS(res[3](0), 2);
  
  return true;
}

bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve11()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(4, 5);
  m(0, 0) = 2;
  m(0, 1) = 0;

  m(1, 0) = 0;
  m(1, 1) = 1;
  m(1, 2) = 4;

  m(2, 1) = 0;
  m(2, 2) = 1;
  m(2, 3) = 0;

  m(3, 2) = 0;
  m(3, 3) = 1;

  m(0, 4) = 1;
  m(1, 4) = 2;
  m(2, 4) = 2;
  m(3, 4) = 2;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);

  ASSERT_DOUBLE_EQUALS(res[0](0), 0.5);
  ASSERT_DOUBLE_EQUALS(res[1](0), -6);
  ASSERT_DOUBLE_EQUALS(res[2](0), 2);
  ASSERT_DOUBLE_EQUALS(res[3](0), 2);
  
  return true;
}


bool MMG402TestLinearAlgebra::TestMMG4023DiagonalMatrixSolve3()
{
  MMG402Matrix::MMG402MatrixSolver<double, 1> sol;
  MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<double, 1>> m(6, 7);
  m(0, 0) = 2;
  m(0, 1) = 3;

  m(1, 0) = 7;
  m(1, 1) = 8;
  m(1, 2) = 5;

  m(2, 1) = 8;
  m(2, 2) = 5;
  m(2, 3) = 0;

  m(3, 2) = 2;
  m(3, 3) = 1;
  m(3, 4) = 1;

  m(4, 3) = 9;
  m(4, 4) = 5;
  m(4, 5) = 2;

  m(5, 4) = 4;
  m(5, 5) = 4;

  m(0, 6) = 1;
  m(1, 6) = 1;
  m(2, 6) = 1;
  m(3, 6) = 1;
  m(4, 6) = 1;
  m(5, 6) = 1;

  std::vector<MMG402Domain::MMG402DomainPoint<double, 1>> res = sol.Solve(&m);
  ASSERT_DOUBLE_EQUALS(res[0](0), 0.00);
  ASSERT_DOUBLE_EQUALS(res[1](0),  1.00 / 3);
  ASSERT_DOUBLE_EQUALS(res[2](0), -1.00 / 3);
  ASSERT_DOUBLE_EQUALS(res[3](0), -0.750);
  ASSERT_DOUBLE_EQUALS(res[4](0), 29.0 / 12);
  ASSERT_DOUBLE_EQUALS(res[5](0), -13.0 / 6);

  return true;
}