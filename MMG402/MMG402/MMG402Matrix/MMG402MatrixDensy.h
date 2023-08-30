#ifndef MMG402MatrixDensy_H
#define MMG402MatrixDensy_H

#include "IMMG402Matrix.h"

namespace MMG402Matrix
{

  template<typename T>
  class MMG402MatrixDensy : public IMMG402Matrix<T>
  {
  public:
    MMG402MatrixDensy(int iRows, int iCols)
      : _rows(iRows), _cols(iCols)
    {
      _values = new T[_rows * _cols];
    }

    virtual int rows() const
    {
      return _rows;
    }
    virtual int cols() const
    {
      return _cols;
    }

    virtual MMG402MatrixType GetType() const
    {
      return MMG402MatrixTypeDensy;
    }

    virtual const T& Value(int r, int c) const
    {
      GUAssert(r < _rows && c < _cols);
      return _values[r * _cols + c];
    }

    virtual T& Value(int r, int c)
    {
      GUAssert(r < _rows && c < _cols);
      return _values[r * _cols + c];
    }

    virtual const T& operator()(int r, int c) const
    {
      return Value(r, c);
    }

    virtual T& operator()(int r, int c)
    {
      return Value(r, c);
    }

    virtual IMMG402Matrix<T>* CreateCopy() const
    {
      MMG402MatrixDensy<T>* pNewInstance = new MMG402MatrixDensy<T>(rows(), cols());
      for (int i = 0; i < rows() * cols(); i++) {
        pNewInstance->_values[i] = _values[i];
      }
      return pNewInstance;
    }

    virtual IMMG402Matrix<T>& operator - ()
    {
      for (int i = 0; i < rows() * cols(); i++) {
        _values[i] = -_values[i];
      }
      return *this;
    }

    const T* GetRawData() const
    {
      return _values;
    }

  protected:
    const int _rows;
    const int _cols;

    T* _values;

  private:
    MMG402MatrixDensy()
      : _rows(0), _cols(0), _values(0)
    {}
  };

}; // end namespace MMG402Matrix
#endif
