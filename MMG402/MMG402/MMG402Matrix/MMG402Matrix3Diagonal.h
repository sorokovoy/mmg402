#ifndef MMG402Matrix3Diagonal_H
#define MMG402Matrix3Diagonal_H

#include "IMMG402Matrix.h"

#include <map>
#include <vector>

namespace MMG402Matrix
{

#define GUMMG402Matrix3DiagonalImplementBinaryOpearatorVector(OP) \
      if(iVal.GetType() != MMG402MatrixType3Diagonal) {           \
        throw new std::exception("Arifmetic operations allowed only for 3 diagonal matrix"); \
      }                                                                                      \
      if(iVal.cols() != cols() || iVal.rows() != rows()) {                                   \
        throw new std::exception("Size of matricex must mutch");                             \
      }                                                                                      \
      MMG402Matrix3Diagonal<T>& rightPart = (MMG402Matrix3Diagonal<T>&)iVal;                 \
      const T** rightRawData = rightPart.GetRawDataDiagonal();                               \
      for(int  i = 0; i < 3; i++) {                                                          \
        for(int j = 0; j < _size; j++) {                                                     \
          _pData[i][j] OP rightRawData[i][j];                                                \
        }                                                                                    \
      }                                                                                      \
      if(IsHasRightPart()) {                                                                 \
        const T* pRightRightPartRawData = rightPart.GetRawDataRightPart();                   \
        for(int j = 0; j < _size; j++) {                                                     \
          _pRightPart[j] OP pRightRightPartRawData[j];                                       \
        }                                                                                    \
      }                                                                                      \

#define GUMMG402Matrix3DiagonalImplementBinaryOpearatorScalar(OP) \
      for(int  i = 0; i < 3; i++) {       \
        for(int j = 0; j < _size; j++) {  \
          _pData[i][j] OP iVal;           \
        }                                 \
      }                                   \
      if(IsHasRightPart()) {              \
        for(int j = 0; j < _size; j++) {  \
          _pRightPart[j] OP iVal;         \
        }                                 \
      }                                   \


  template<typename T>
  class MMG402Matrix3Diagonal : public IMMG402Matrix<T>
  {
  public:
    MMG402Matrix3Diagonal(int iRows, int iCols)
      : _size(iRows)
    {
      if (iCols != iRows && iCols != iRows + 1) {
        throw new std::exception("3 diagonal matrix should be square or square with right part");
      }
      for (int i = 0; i < 3; i++) {
        _pData[i] = new T[_size];
        for (int j = 0; j < _size; j++) {
          _pData[i][j] = 0;
        }
      }
      _pRightPart = iCols == iRows + 1 ? new T[_size] : 0;
      if (IsHasRightPart()) {
        for (int j = 0; j < _size; j++) {
          _pRightPart[j] = 0;
        }
      }
    }

    virtual ~MMG402Matrix3Diagonal()
    {
      for (int i = 0; i < 3; i++) {
        delete _pData[i];
      }
      delete _pRightPart;
    }

    bool IsHasRightPart() const
    {
      return _pRightPart != 0;
    }

    bool CheckIndexes(int r, int c) const
    {
      return (r < _size && c < _size + IsHasRightPart() ? 1 : 0)  // check array bound
             && ((abs(r - c) <= 1) || (IsHasRightPart() && c == _size)); // check 3 linerizl structure
    }

    virtual int rows() const
    {
      return _size;
    }

    virtual int cols() const
    {
      return _size + (IsHasRightPart() ? 1 : 0);
    }

    virtual MMG402MatrixType GetType() const
    {
      return MMG402MatrixType3Diagonal;
    }

    virtual const T& Value(int r, int c) const
    {
      return GetValue(r, c);
    }

    virtual T& Value(int r, int c)
    {
      return GetValue(r, c);
    }

    virtual const T& operator()(int r, int c) const
    {
      return Value(r, c);
    }

    virtual T& operator()(int r, int c)
    {
      return Value(r, c);
    }

    virtual T& GetValue(int r, int c) const
    {
      if (!CheckIndexes(r, c)) {
        throw new std::exception("Invalid indexes in 3 diagonal matrix");
      }
      if (c == _size) {
        return _pRightPart[r];
      } else {
        return _pData[r - c + 1][r - (r - c == 1 ? 1 : 0)];
      }
    }

    virtual void RowMult(int iRowId, const T& iVal) {
      _pData[0][iRowId] *= iVal;
      _pData[1][iRowId] *= iVal;
      if(iRowId != 0) {
        _pData[2][iRowId - 1] *= iVal;
      }
      _pRightPart[iRowId] *= iVal;
    }

    virtual IMMG402Matrix<T>* CreateCopy() const
    {
      MMG402Matrix3Diagonal<T>* pNewInstance = new MMG402Matrix3Diagonal<T>(rows(), cols());
      for (int i = 0; i < 3; i++) {
        memcpy(pNewInstance->_pData[i], _pData[i], sizeof(T) * _size);
      }
      if (IsHasRightPart()) {
        memcpy(pNewInstance->_pRightPart, _pRightPart, sizeof(T) * _size);
      }
      return pNewInstance;
    }

    virtual IMMG402Matrix<T>& operator - ()
    {
      for (int i = 0; i < 3; i++) {
        for (int j = 0; j < _size; j++) {
          _pData[i][j] = -_pData[i][j];
        }
      }

      if (IsHasRightPart()) {
        for (int j = 0; j < _size; j++) {
          _pRightPart[j] = -_pRightPart[j];
        }
      }
      return *this;
    }

    // vector operations
    virtual void operator -= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorVector( -= );
    }

    virtual void operator += (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorVector( += );
    }

    virtual void operator *= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorVector( *= );
    }

    virtual void operator /= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorVector( /= );
    }

    // scalar operations
    virtual void operator -= (const T& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorScalar( -= );
    }

    virtual void operator += (const T& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorScalar( += );
    }

    virtual void operator *= (const T& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorScalar( *= );
    }

    virtual void operator /= (const T& iVal)
    {
      GUMMG402Matrix3DiagonalImplementBinaryOpearatorScalar( /= );
    }

    const T** GetRawDataDiagonal() const
    {
      return (const T**)_pData;
    }

    const T* GetRawDataRightPart() const
    {
      return _pRightPart;
    }

  protected:
    const int _size;

    T* _pData[3];
    T* _pRightPart;

  private:
    MMG402Matrix3Diagonal()
      : _rows(0), _cols(0)
    {}
  };

} // end namespace MMG402Matrix
#endif
