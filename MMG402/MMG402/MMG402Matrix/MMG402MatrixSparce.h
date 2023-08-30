#ifndef MMG402MatrixSparce_H
#define MMG402MatrixSparce_H

#include "IMMG402Matrix.h"

#include <map>
#include <vector>
#include <algorithm>

namespace MMG402Matrix
{

#define GUMMG402MatrixSparceImplementBinaryOpearatorVector(OP) \
    GUAssert(rows() == iVal.rows());                        \
    GUAssert(cols() == iVal.cols());                        \
    const MMG402MatrixSparce<T>* asSparce = dynamic_cast<const MMG402MatrixSparce<T>*>(&iVal); \
    if(asSparce != 0) {                                                                  \
      std::map<MMG402MatrixSparceKey, T>::iterator in  = _values.begin();                   \
      std::map<MMG402MatrixSparceKey, T>::const_iterator out = asSparce->_values.begin();   \
      std::vector<std::pair<MMG402MatrixSparceKey, T>> additional;                          \
      while(true) {                                                                      \
        if(out == asSparce->_values.end()) {                                             \
          break;                                                                         \
        }                                                                                \
        if(in == _values.end()) {                                                        \
          additional.push_back(*out);                                                    \
          out++;                                                                         \
          continue;                                                                      \
        }                                                                                \
        int cmp = keyComparator(in->first, out->first);                                  \
        if(cmp == 0) {                                                                   \
          in->second OP out->second;                                                     \
          in++; out++;                                                                   \
          continue;                                                                      \
        }                                                                                \
        if(cmp < 0) {                                                                    \
          in++;                                                                          \
        } else {                                                                         \
          additional.push_back(*out);                                                    \
          out++;                                                                         \
        }                                                                                \
      }                                                                                  \
      _values.insert(additional.begin(), additional.end());                              \
    } else {                                                                             \
      T defaultValue(0);                         \
      for(int i = 0; i < rows(); i++) {          \
        for(int j = 0; j < cols(); j++) {        \
          const T& v = iVal.Value(i, j);         \
          if(v != defaultValue) {                \
            Value(i, j) OP iVal.Value(i, j);     \
          }                                      \
        }                                        \
      }                                          \
    }                                            \

#define GUMMG402MatrixSparceImplementBinaryOpearatorScalar(OP) \
    T defaultValue(0);                         \
    if(iVal == defaultValue) return;           \
    for(int i = 0; i < rows(); i++) {          \
      for(int j = 0; j < cols(); j++) {        \
          Value(i, j) OP iVal;                 \
      }                                        \
    }                                          \

  struct MMG402MatrixSparceKey {
    MMG402MatrixSparceKey(int ir, int ic)
      : r(ir), c(ic)
    {}

    int r;
    int c;
    bool operator < (const MMG402MatrixSparceKey& iV) const
    {
      return (r < iV.r) || (r == iV.r && c < iV.c);
    }
  };

  inline int keyComparator(const MMG402MatrixSparceKey& k1, const MMG402MatrixSparceKey& k2)
  {
    if (k1 < k2) return -1;
    if (k2 < k1) return  1;
    return 0;
  }

  template<class T>
  int compare_first(T a, T b)
  {
    if (a < b) return -1;
    if (a == b) return 0;
    return 1;
  }

  template<typename T>
  class MMG402MatrixSparce : public IMMG402Matrix<T>
  {
  public:
    MMG402MatrixSparce(int iRows, int iCols)
      : _rows(iRows), _cols(iCols)
    {}

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
      return MMG402MatrixTypeSparce;
    }

    virtual const T& Value(int r, int c) const
    {
      std::map<MMG402MatrixSparceKey, T>::const_iterator it = _values.find(MMG402MatrixSparceKey(r, c));
      return it != _values.end() ? it->second : _defaultValue; // default value should be 0
    }

    virtual void RowMult(int iRowId, const T& iVal) {
      std::map<MMG402MatrixSparceKey, T>::iterator low = _values.lower_bound(MMG402MatrixSparceKey(iRowId, 0));
      std::map<MMG402MatrixSparceKey, T>::iterator up = _values.upper_bound(MMG402MatrixSparceKey(iRowId, INT_MAX));

      for(std::map<MMG402MatrixSparceKey, T>::iterator it = low; it != up; it++) {
        it->second *= iVal;
      }
    }

    virtual T& Value(int r, int c)
    {
      return _values[MMG402MatrixSparceKey(r, c)];
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
      MMG402MatrixSparce<T>* pNewInstance = new MMG402MatrixSparce<T>(rows(), cols());
      pNewInstance->_values = _values;
      return pNewInstance;
    }

    virtual IMMG402Matrix<T>& operator - ()
    {
      for (std::map<MMG402MatrixSparceKey, T>::iterator it = _values.begin(); it != _values.end(); it++) {
        it->second = -it->second;
      }
      return *this;
    }

    // vector operations
    virtual void operator -= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorVector( -= );
    }

    virtual void operator += (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorVector( += );
    }

    virtual void operator *= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorVector( *= );
    }

    virtual void operator /= (const IMMG402Matrix<T>& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorVector( /= );
    }

    // scalar operations
    virtual void operator -= (const T& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorScalar( -= );
    }

    virtual void operator += (const T& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorScalar( += );
    }

    virtual void operator *= (const T& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorScalar( *= );
    }

    virtual void operator /= (const T& iVal)
    {
      GUMMG402MatrixSparceImplementBinaryOpearatorScalar( /= );
    }

    const std::map<MMG402MatrixSparceKey, T>& GetRawData() const
    {
      return _values;
    }

  protected:
    const int _rows;
    const int _cols;
    T _defaultValue;

    std::map<MMG402MatrixSparceKey, T> _values;

  private:
    MMG402MatrixSparce()
      : _rows(0), _cols(0)
    {}
  };

} // end namespace MMG402Matrix
#endif
