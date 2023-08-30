#ifndef IMMG402Matrix_H
#define IMMG402Matrix_H

#include "MMG402\libs\GUtils\GUAssert.h"

namespace MMG402Matrix
{

#define IMMG402MatrixImplementBinaryOpearatorVector(OP) \
    GUAssert(rows() == iVal.rows());           \
    GUAssert(cols() == iVal.cols());           \
    for(int i = 0; i < rows(); i++) {          \
      for(int j = 0; j < cols(); j++) {        \
        Value(i, j) OP iVal.Value(i, j);       \
      }                                        \
    }                                          \

#define IMMG402MatrixImplementBinaryOpearatorScalar(OP) \
    for(int i = 0; i < rows(); i++) {          \
      for(int j = 0; j < cols(); j++) {        \
        Value(i, j) OP iVal;                   \
      }                                        \
    }                                          \

  enum MMG402MatrixType {
    MMG402MatrixTypeUndefined = 0,
    MMG402MatrixTypeDensy = 1,
    MMG402MatrixTypeSparce = 2,
    MMG402MatrixType3Diagonal = 3
  };

  template<typename T>
  class IMMG402Matrix
  {
  public:
    virtual ~IMMG402Matrix() {};

    virtual int rows() const = 0;
    virtual int cols() const = 0;

    virtual const T& Value(int r, int c) const = 0;
    virtual T& Value(int r, int c) = 0;

    virtual const T& operator()(int r, int c) const = 0;
    virtual T& operator()(int r, int c) = 0;

    virtual IMMG402Matrix<T>* CreateCopy() const = 0;

    virtual MMG402MatrixType GetType() const = 0;

    virtual void RowMult(int iRowId, const T& iVal) {
      for (int j = 0; j < cols(); j++) {
        Value(iRowId, j) *=  iVal;
      }
    }

    virtual IMMG402Matrix<T>& operator - ()
    {
      for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < cols(); j++) {
          Value(i, j) = - Value(i, j);
        }
      }
      return *this;
    }

    // vector operations
    virtual void operator -= (const IMMG402Matrix<T>& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorVector( -= );
    }

    virtual void operator += (const IMMG402Matrix<T>& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorVector( += );
    }

    virtual void operator *= (const IMMG402Matrix<T>& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorVector( *= );
    }

    virtual void operator /= (const IMMG402Matrix<T>& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorVector( /= );
    }

    // scalar operations
    virtual void operator -= (const T& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorScalar( -= );
    }

    virtual void operator += (const T& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorScalar( += );
    }

    virtual void operator *= (const T& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorScalar( *= );
    }

    virtual void operator /= (const T& iVal)
    {
      IMMG402MatrixImplementBinaryOpearatorScalar( /= );
    }
  };

} // end namespace MMG402Matrix
#endif
