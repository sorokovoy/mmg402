#ifndef MMG402SmoluhovskiExactSolve_H
#define MMG402SmoluhovskiExactSolve_H

#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolveKore1 : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolveKore1()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    double NT = 2.0 / (2.00  + 1.00 * t);
    return NT * NT * exp(-NT * x);
  }
  double t;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolveKore1Production : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolveKore1Production()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
    t = 0;
  }

  virtual void SetTemporalValue(double iVal)
  {
  //  t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return 8.0 * exp(- 2*x / (2 + t)) * x / GUPow(2 + t, 4);
  }
  double t;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolveKore1Destruction : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolveKore1Destruction()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
    t = 0;
  }

  virtual void SetTemporalValue(double iVal)
  {
   // t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    return 2.0 / (2.00  + 1.00 * t);
  }
  double t;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolveKoreXpY : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolveKoreXpY()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    double v = pow(x, -1.50) * exp(-t) * exp( - exp(-2.0 * t) * x / 2.0);
    return v / (sqrt(GU2Pi));
  }
  double t;
};


template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolveKoreXmY : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolveKoreXmY()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    if (t > 1.00E-009) {
      double T;
      if (t <= 1) {
        T = 1 + t;
      } else {
        T = 2 * sqrt(t);
      }

      double integralXArg = 2 * x * sqrt(t);
      double IX = _j1( integralXArg);  // _j1 bessel function furst kind

      double v1 = exp(-T * x ) * IX;
      return v1 / (x * x * sqrt(t));
    } else {
      return exp(-x) / x;
    }
  }
  double t;
};


// destruction 0 production 1
// eq du/dt = u
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolve1: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolve1()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return exp(-t) * x;
  }
  double t;
};

// destruction 0 production u
// eq du/dt = -u^2
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolve2: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolve2()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return x / (1 + t * x);
  }
  double t;
};


// destruction u production u
// eq du/dt = u - u^2
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiSolve3: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiSolve3()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual  typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return x / ((1 - x) * exp(-t) + x);
  }
  double t;
};


template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreXmYOperatorTestSolve1: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiKoreXmYOperatorTestSolve1()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual  typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return exp(-x) / x;
  }
  double t;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreXmYOperatorTestProduction1: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiKoreXmYOperatorTestProduction1()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual  typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return exp(-x) * x / 2;
  }
  double t;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreXmYOperatorTestDestruction1: public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiKoreXmYOperatorTestDestruction1()
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {
    SetTemporalValue(0);
  }

  virtual void SetTemporalValue(double iVal)
  {
    t = iVal;
  }

  virtual  typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    return x;
  }
  double t;
};

#endif // MMG402SmoluhovskiExactSolve_H
