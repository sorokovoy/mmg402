
#ifndef MMG402DomainPoint_H
#define MMG402DomainPoint_H

#include "MMG402\libs\GUtils\GUAssert.h"
#include <stdio.h>
#include <iomanip>
#include <vector>

namespace MMG402Domain
{
  template <typename SpaceScalarType, int SpaceDim>
  class MMG402DomainPoint
  {
  public:
    enum { SpaceDimAtCompile = SpaceDim };
    MMG402DomainPoint()
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] = 0;
      }
    }

    MMG402DomainPoint(const SpaceScalarType& iV1)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] = iV1;
      }
    }

    MMG402DomainPoint(const SpaceScalarType& iV1, const SpaceScalarType& iV2)
    {
      GUAssert(SpaceDimAtCompile == 2);
      _data[0] = iV1;
      _data[1] = iV2;
    }

    MMG402DomainPoint(const SpaceScalarType& iV1, const SpaceScalarType& iV2, const SpaceScalarType& iV3)
    {
      GUAssert(SpaceDimAtCompile == 3);
      data[0] = iV1;
      _data[1] = iV2;
      _data[2] = iV3;
    }

    MMG402DomainPoint(const SpaceScalarType& iV1, const SpaceScalarType& iV2, const SpaceScalarType& iV3, const SpaceScalarType& iV4)
    {
      GUAssert(SpaceDimAtCompile == 4);
      _data[0] = iV1;
      _data[1] = iV2;
      _data[2] = iV3;
      _data[3] = iV4;
    }

    MMG402DomainPoint(const SpaceScalarType& iV1, const SpaceScalarType& iV2, const SpaceScalarType& iV3, const SpaceScalarType& iV4, const SpaceScalarType& iV5)
    {
      GUAssert(SpaceDimAtCompile == 5);
      _data[0] = iV1;
      _data[1] = iV2;
      _data[2] = iV3;
      _data[3] = iV4;
      _data[4] = iV5;
    }

    MMG402DomainPoint(const SpaceScalarType& iV1, const SpaceScalarType& iV2, const SpaceScalarType& iV3, const SpaceScalarType& iV4, const SpaceScalarType& iV5, const SpaceScalarType& iV6)
    {
      GUAssert(SpaceDimAtCompile == 6);
      _data[0] = iV1;
      _data[1] = iV2;
      _data[2] = iV3;
      _data[3] = iV4;
      _data[4] = iV5;
      _data[5] = iV6;
    }


    MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> operator ()(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iv2)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> result;
      result[0] = iv1[0];
      result[1] = iv2[0];
      return result;
    }

    SpaceScalarType& operator() (int iIndex)
    {
      GUAssert(iIndex < SpaceDimAtCompile);
      return _data[iIndex];
    }

    SpaceScalarType& operator[] (int iIndex)
    {
      GUAssert(iIndex < SpaceDimAtCompile);
      return _data[iIndex];
    }

    const SpaceScalarType& operator() (int iIndex) const
    {
      GUAssert(iIndex < SpaceDimAtCompile);
      return _data[iIndex];
    }

    const SpaceScalarType& operator[] (int iIndex) const
    {
      GUAssert(iIndex < SpaceDimAtCompile);
      return _data[iIndex];
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& operator = (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] = iVal._data[i];
      }
      return *this;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& operator = (const SpaceScalarType& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] = iVal;
      }
      return *this;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator -()
    {
      MMG402DomainPoint result;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] = -_data[i];
      }
      return result;
    }

    void operator -= (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] -= iVal._data[i];
      }
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator - (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] -= iVal._data[i];
      }
      return result;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator - (SpaceScalarType iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] -= iVal;
      }
      return result;
    }

    void operator += (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] += iVal._data[i];
      }
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator + (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] += iVal._data[i];
      }
      return result;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator + (SpaceScalarType iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] += iVal;
      }
      return result;
    }

    void operator *= (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] *= iVal._data[i];
      }
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator * (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] *= iVal._data[i];
      }
      return result;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator * (SpaceScalarType iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] *= iVal;
      }
      return result;
    }


    void operator /= (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        _data[i] /= iVal._data[i];
      }
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator / (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] /= iVal._data[i];
      }
      return result;
    }

    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator / (SpaceScalarType iVal)
    {
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result = *this;
      for (int i = 0; i < SpaceDimAtCompile; i++) {
        result[i] /= iVal;
      }
      return result;
    }

    SpaceScalarType (& GetData())[SpaceDim] { return _data; }

  protected:
    SpaceScalarType _data[SpaceDim];
  };




  template <typename SpaceScalarType, int SpaceDim>
  std::ostream& operator << (std::ostream& output, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint)
  {
    if (iPoint.SpaceDimAtCompile == 1) {
      output << iPoint(0);
      return output;
    }

    output << "(";
    for (unsigned int i = 0; i < iPoint.SpaceDimAtCompile; i++) {
      output << iPoint(i);
      if (i != iPoint.SpaceDimAtCompile - 1) {
        output << ", ";
      } else {
        output << ")";
      }
    }
    return output;
  }

  template <typename SpaceScalarType, int SpaceDim>
  std::ostream& operator << (std::ostream& output, const std::vector<MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>>& iPoint)
  {
    for (unsigned int i = 0; i < iPoint.size(); i++) {
      output << "p[" << i << "] " << iPoint[i] << " ";
      if ( (i + 1) % 6 == 0) {
        output << std::endl;
      }
    }
    if (iPoint.size() % 6 != 0) {
      output << std::endl;
    }
    return output;
  }

  template <typename SpaceScalarType, int SpaceDim>
  std::string ToString(const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, int iPrecusion = -1)
  {
    std::stringstream res;
    if (iPrecusion >= 0)
      res << std::setprecision(iPrecusion);
    res << iPoint;
    return res.str();
  }


  template <typename SpaceScalarType, int SpaceDim>
  bool operator == (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal2)
  {
    for (int i = 0; i < iVal1.SpaceDimAtCompile; i++) {
      if (iVal1[i] != iVal2[i]) return false;
    }
    return true;
  }

  template <typename SpaceScalarType, int SpaceDim>
  bool operator != (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iVal2)
  {
    for (int i = 0; i < iVal1.SpaceDimAtCompile; i++) {
      if (iVal1[i] != iVal2[i]) return true;
    }
    return false;
  }

// ===================================== vector arg =====================================
  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator + (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] + iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator - (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] - iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator * (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] * iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator / (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] / iv2[i];
    }
    return result;
  }


// ===================================== scalar arg  Right =====================================
  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator + (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const SpaceScalarType& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] + iv2;
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator - (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const SpaceScalarType& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] - iv2;
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator * (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const SpaceScalarType& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] * iv2;
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator / (const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv1, const SpaceScalarType& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv1.SpaceDimAtCompile; i++) {
      result[i] = iv1[i] / iv2;
    }
    return result;
  }

// ===================================== scalar arg  Left =====================================
  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator + (const SpaceScalarType& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv2.SpaceDimAtCompile; i++) {
      result[i] = iv1 + iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator - (const SpaceScalarType& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv2.SpaceDimAtCompile; i++) {
      result[i] = iv1 - iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator * (const SpaceScalarType& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv2.SpaceDimAtCompile; i++) {
      result[i] = iv1 * iv2[i];
    }
    return result;
  }

  template <typename SpaceScalarType, int SpaceDim>
  MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> operator / (const SpaceScalarType& iv1, const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iv2)
  {
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim> result;
    for (int i = 0; i < iv2.SpaceDimAtCompile; i++) {
      result[i] = iv1 / iv2[i];
    }
    return result;
  }

}; // end namespace MMG402Domain
#endif
