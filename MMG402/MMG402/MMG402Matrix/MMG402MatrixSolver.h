#ifndef MMG402MatrixSolver_H
#define MMG402MatrixSolver_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402Matrix\MMG402MatrixDensy.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSparce.h"
#include "MMG402\MMG402Matrix\MMG402Matrix3Diagonal.h"

#include "MMG402/libs/Eigen/Core"
#include "MMG402/libs/Eigen/Dense"
#include "MMG402/libs/Eigen/SparseCore"
#include "MMG402/libs/Eigen/SparseLU"
#include "MMG402/libs/Eigen/SparseQR"
#include "MMG402/libs/GUtils/GUMathUtils.h"

namespace MMG402Matrix
{

  enum MMG402MatrixSolverSparceMatrixSolveMethod
  {
    MMG402MatrixSolverSparceMatrixSolveMethodEigenSparceLU,
    MMG402MatrixSolverSparceMatrixSolveMethodEigenSparceQR
  };

  struct MMG402MatrixSolverFlags 
  {
    MMG402MatrixSolverFlags()
    {
      IsNoSparceMatrixChange = false;
      sparceMethod = MMG402MatrixSolverSparceMatrixSolveMethodEigenSparceQR;
    }

    bool IsNoSparceMatrixChange;
    MMG402MatrixSolverSparceMatrixSolveMethod sparceMethod;
  };

  template <typename DataScalarType, int DataDim>
  class MMG402MatrixSolver
  {
  public:
    MMG402MatrixSolver()
    {
      for (unsigned int dim = 0; dim < DataDimAtCompile; dim++) {
        pStoredSparceMatrix[dim] = 0;
      }
    }

    virtual ~MMG402MatrixSolver()
    {
      for (unsigned int dim = 0; dim < DataDimAtCompile; dim++) {
        delete pStoredSparceMatrix[dim];
      }
    }
    enum { DataDimAtCompile  = DataDim  };
    MMG402MatrixSolverFlags& GetSolverFlags()
    {
      return _flags;
    }

    std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> Solve(IMMG402Matrix<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pInput)
    {
      GUAssert(pInput->rows() + 1 == pInput->cols());
      switch (pInput->GetType()) {
        case MMG402Matrix::MMG402MatrixTypeDensy: {
          MMG402MatrixDensy<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pAsDency = dynamic_cast<MMG402MatrixDensy<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>*>(pInput);
          GUAssertB(pAsDency);
          return SolveAsDency(pAsDency);
        }
        case MMG402Matrix::MMG402MatrixTypeSparce: {
          MMG402MatrixSparce<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pAsSparce = dynamic_cast<MMG402MatrixSparce<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>*>(pInput);
          GUAssertB(pAsSparce);
          return SolveAsSparce(pAsSparce);
        }
        case MMG402Matrix::MMG402MatrixType3Diagonal: {
          MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pAs3Diagonal = dynamic_cast<MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>*>(pInput);
          GUAssertB(pAs3Diagonal);
          return SolveAs3Diagonal(pAs3Diagonal);
        }
        default: {
          throw std::exception("Matrix type not supported");
        }
      }
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> result(0);
      return result;
    }

  protected:
    std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> SolveAsSparce(MMG402MatrixSparce<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pInput)
    {
      unsigned int n = pInput->rows();
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> result(n);

      const std::map<MMG402MatrixSparceKey, MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>& rawData = pInput->GetRawData();
      for (unsigned int dim = 0; dim < DataDimAtCompile; dim++) {
        Eigen::SparseMatrix<DataScalarType>* pCurrentMatrix = 0;
        if (GetSolverFlags().IsNoSparceMatrixChange && pStoredSparceMatrix[dim] != 0) {
          pCurrentMatrix = pStoredSparceMatrix[dim];
        } else {
          pCurrentMatrix = new Eigen::SparseMatrix<DataScalarType>(n, n);
          for (std::map<MMG402MatrixSparceKey, MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>::const_iterator it = rawData.begin(); it != rawData.end(); it++) {
            if (it->first.c != n) { // original matrix size is NxN+1
              pCurrentMatrix->insert(it->first.r, it->first.c) = it->second(dim);
            }
          }
          pCurrentMatrix->makeCompressed();
          if(GetSolverFlags().IsNoSparceMatrixChange) {
            pStoredSparceMatrix[dim] = pCurrentMatrix;
          }
        }

        Eigen::Matrix < DataScalarType, -1, 1 > * pEigVector = new Eigen::Matrix < DataScalarType, -1, 1 > (n);
        for (unsigned int i = 0; i < n; i++) {
          (*pEigVector)(i) = (*pInput)(i, n)[dim];
        }

        //std::cout << Eigen::MatrixXd(*pCurrentMatrix) << std::endl;
        //std::cout << *pEigVector << std::endl;
        // select sparce solver

        Eigen::Matrix < DataScalarType, -1, 1 > res;
        switch (GetSolverFlags().sparceMethod)
        {
        case MMG402MatrixSolverSparceMatrixSolveMethodEigenSparceLU:
          {
            Eigen::SparseLU<Eigen::SparseMatrix<DataScalarType>> spSolver;
            spSolver.analyzePattern(*pCurrentMatrix);
            spSolver.factorize(*pCurrentMatrix);
            res = spSolver.solve(*pEigVector);
            break;
          }
        case MMG402MatrixSolverSparceMatrixSolveMethodEigenSparceQR:
          {
            Eigen::SparseQR<Eigen::SparseMatrix<DataScalarType>, Eigen::COLAMDOrdering<int>> spSolver;
            spSolver.analyzePattern(*pCurrentMatrix);
            spSolver.factorize(*pCurrentMatrix);
            res = spSolver.solve(*pEigVector);
            break;
          }
        default:
          {
            throw new std::exception("Not implemented");
          }
        }

        delete pEigVector;

        //double err =  (Eigen::MatrixXd(spMatrix) * res   - (*pEigVector)).norm();

        for (unsigned int i = 0; i < n; i++) {
          result[i][dim] = res[i];
        }

        if(!GetSolverFlags().IsNoSparceMatrixChange) {
          delete pCurrentMatrix;
        }
      }
      return result;
    }

    std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> SolveAsDency(MMG402Matrix::MMG402MatrixDensy<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pInput)
    {
      unsigned int n = pInput->rows();
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> result(n);

      Eigen::Matrix < DataScalarType, -1, -1 > * pEigMat = new Eigen::Matrix < DataScalarType, -1, -1 > (n, n);
      Eigen::Matrix < DataScalarType, -1, 1 > * pEigVector = new Eigen::Matrix < DataScalarType, -1, 1 > (n);
      for (unsigned int dim = 0; dim < DataDimAtCompile; dim++) {
        bool isHasNonZero = false;
        for (unsigned int i = 0; i < n; i++) {
          for (unsigned int j = 0; j < n; j++) {
            (*pEigMat)(i, j) = (*pInput)(i, j)[dim];
            if (GUAbs((*pInput)(i, j)[dim]) > TOLERANCE) {
              isHasNonZero = true;
            }
          }
          (*pEigVector)(i) = (*pInput)(i, n)[dim];
        }

        Eigen::Matrix < DataScalarType, -1, 1 > res = pEigMat->colPivHouseholderQr().solve(*pEigVector);

        for (unsigned int i = 0; i < n; i++) {
          result[i][dim] = isHasNonZero ? res[i] : 0;
        }
      }
      delete pEigMat;
      delete pEigVector;
      return result;
    }

    std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> SolveAs3Diagonal(MMG402Matrix::MMG402Matrix3Diagonal<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>* pInput)
    {
      unsigned int n = pInput->rows();
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> result(n);
      const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>** pRawData = pInput->GetRawDataDiagonal();
      if(!SolveAs3Diagonal(pRawData[0], pRawData[1], pRawData[2], pInput->GetRawDataRightPart(), result, pInput->rows())) {
        return std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>(); 
      }
      return result;
    }

    bool SolveAs3Diagonal(const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>* c, // upper diagonal
                          const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>* b, // diagonal
                          const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>* a, // lower diagonal
                          const MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>* d, // right part
                          std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>>& f, int N)
    {
      // Create the temporary vectors
      // Note that this is inefficient as it is possible to call
      // this function many times. A better implementation would
      // pass these temporary matrices by non-const reference to
      // save excess allocation and deallocation
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> a_star(N, 0);
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> b_star(N, 0);
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> c_star(N, 0);
      std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> d_star(N, 0);

      a_star[0] = 0;
      b_star[0] = b[0];
      c_star[0] = c[0];
      d_star[0] = d[0];

      for (unsigned int dim = 0; dim < DataDimAtCompile; dim++) {

      for (int i = 1; i < N; i++) {
        if(GUAbs(b_star[i - 1][dim]) > TOLERANCE) {
          a_star[i][dim] = 0;
          b_star[i][dim] = b[i][dim] - c_star[i - 1][dim] * a[i - 1][dim] / b_star[i - 1][dim];
          c_star[i][dim] = c[i][dim];
          d_star[i][dim] = d[i][dim] - d_star[i - 1][dim] * a[i - 1][dim] / b_star[i - 1][dim];
        } else {
          if(GUAbs(c_star[i - 1][dim]) > TOLERANCE) { // (cond 1) 
            b_star[i][dim] = 0;
            a_star[i][dim] = a[i - 1][dim];
            c_star[i][dim] = c[i][dim];
            d_star[i][dim] = d[i][dim] - d_star[i - 1][dim] * b[i][dim] / c_star[i - 1][dim];
            if(i + 1 < N) {
              a_star[i+1][dim] = 0;
              d_star[i+1][dim] = d[i + 1][dim] - d_star[i - 1][dim] * a[i - 1 + 1][dim] / c_star[i - 1][dim];
              b_star[i+1][dim] = b[i+1][dim];
              c_star[i+1][dim] = c[i+1][dim];
              i++;
              continue;
            }
          } else {
            GUAssert(false);
          }
        }
      }
       
      for(int i = N - 1; i >= 0; i--) {
        if(GUAbs(b_star[i][dim]) > TOLERANCE) {
          f[i][dim] = d_star[i][dim] / b_star[i][dim];
          if(i != 0) {
            d_star[i - 1][dim] -= d_star[i][dim] * c_star[i - 1][dim] / b_star[i][dim];
          }
        } else {
          if(GUAbs(a_star[i][dim]) > TOLERANCE) {
            if(i != 0) {
              f[i - 1][dim] = d_star[i][dim] / a_star[i][dim];
              GUAssert(GUAbs(c_star[i - 1][dim]) > TOLERANCE); // by (cond 1) 
              f[i][dim] = d_star[i - 1][dim] / c_star[i - 1][dim];
              i--;
              if(i != 0) {
                d_star[i - 1][dim] -= f[i][dim] *  c_star[i - 1][dim];
              }
              continue;
            }
          }

          if(GUAbs(d_star[i][dim]) < TOLERANCE) {
            f[i][dim] = 0;
          } else {
            return false; // no solution
          }
        }
      }
      }

      return true;
    }

    Eigen::SparseMatrix<DataScalarType>* pStoredSparceMatrix[DataDim];
    MMG402MatrixSolverFlags _flags;
  };

} // end namespace MMG402Matrix
#endif

