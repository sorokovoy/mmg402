#ifndef MMG402SmoluhovskiKore_H
#define MMG402SmoluhovskiKore_H

#include "MMG402\libs\GUtils\GUMathUtils.h"
#include "MMG402\MMG402Expression\MMG402SymbolFunc.h"

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreBase : public MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, 1>
{
private:
  struct SmoluhovskiKoreBaseCasheKey {
    SmoluhovskiKoreBaseCasheKey(double ix, double iy)
      : x(ix), y(iy)
    {}
    double x;
    double y;
    bool operator < ( const SmoluhovskiKoreBaseCasheKey& iVal)
    {
      return ((x < iVal.x) || ( abs(x - iVal.x) < 1.00E-011 && y < iVal.y));
    }
  };
public:
  SmoluhovskiKoreBase() : MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, 1>(MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {}

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 2, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2>& iPoint) const
  {
    return EvalueteValue(iPoint[0], iPoint[1]);
  }
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const = 0;
};


template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKore1 : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return 1;
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreXpY : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return x + y;
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreXmY : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return x * y;
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreExpXpY : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return exp(-(x + y)) * x * y;
  }
};


template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreMixed : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    //return  3 * exp(-(x + y)) - 2 * (x + y);
    return  1 * x * y - 0 * (x + y) + 0 * exp(-(x + y)) * x * y;
  }
};



namespace MMG402SmoluhovskiGeneralCoreEq
{

#define SKG_Pi 3.14159265358979323846
#define SKG_ASMax(a,b)    (((a) > (b)) ? (a) : (b))
#define SKG_ASMin(a,b)    (((a) < (b)) ? (a) : (b))
#define SKG_F1 3200000000000      //{3.2*10^12/N} Source at the begining moment (amount of particles)
#define SKG_F2 0      //Sink at the begining moment 
//#define d 1  //[m] Diametre of a gas's molecule
#define SKG_Deff 1000 //[Kg/m^3] Effective density
#define SKG_Ox 0.0000000182 //[Pa*sec] Viscosity of oxegen
#define SKG_Pi 3.14159265358979323846
#define SKG_g 9.8 //[m/sec^2]
#define SKG_T 298 //[K] Temperature
#define SKG_K 0.0000000000000000000000138 // Bolzman's constant 
#define SKG_RGas 8.3144 //??????
#define SKG_dRk1 0.0000001 //meters
#define SKG_Tmax 3600 //[sec]
#define SKG_Ad 200 //[m^2] S of horizontal 
#define SKG_Ah 600 //[m^2] S ov vertical
#define SKG_deltaD 0.0001//[m]
#define SKG_V 2000 //[m^3]
#define SKG_D 1000 // 1.7 //[Kg/m^3] Density of particle 
//#define Rop 1.00E003
#define SKG_Rop SKG_D
#define SKG_M 0.1
#define SKG_lambda 6.86E-008
#define SKG_Mu 1.83E-006
#define SKG_deltaPi 1.00E-004
#define SKG_L 0.0000001
#define SKG_DP 1

  double E(double Ri, double Rk) //koefficient of effective of couple
  {
    return ( (0.5 * Ri * Ri) / ((Ri + Rk) * (Ri + Rk)) );
  }

  double Kn(double Ri)
  {
    return 0.0000001 / Ri ; //L=0.0000001
  }
  double Cn(double Ri)
  {
    return (1 + 1.246 * Kn(Ri) + 0.42 * Kn(Ri) * exp(-(0.87) / (Kn(Ri))));
  }
  double Us(double Ri)
  {
    return (2 * SKG_Deff * SKG_g * Ri * Ri * Cn(Ri) / (9 * SKG_Ox));
  }
  double Kg(double Ri, double Rk)
  {
    double fabsValue = fabs(Us(Ri) - Us(Rk));
    return (SKG_Pi * E(Ri, Rk) * (Ri + Rk) * (Ri + Rk) * (fabsValue));
  }

  double B(double Ri)
  {
    return (Cn(Ri) / (6 * SKG_Pi * SKG_Ox * Ri));
  }
  double Kb(double Ri, double Rk)
  {
    return (4 * SKG_Pi * SKG_K * SKG_T * (Ri + Rk) * (B(Ri) + B(Rk)));
  }
  double alphaD(double Ri)
  {
    return (SKG_K * SKG_T * B(Ri) * SKG_Ad / (SKG_deltaD * SKG_V));
  }
  double alphaS(double Ri)
  {
    return ((1.222222222222222222) * SKG_Pi * SKG_D * Ri * Ri * Ri * B(Ri) * SKG_Ah / SKG_V);
  }


  #define SKG2_Di 1.00E-005
  #define SKG2_Mi 1.00E-001

  // skorost perenosa
  double Ur(double ri)
  {
    return ((4 * SKG2_Di * SKG2_Mi) / (SKG_RGas * SKG_T * 1 * 2 * ri )) * 1.00E-004 * Cn(ri) * 1.00E-003;
  }
};


namespace MMG402SmoluhovskiGeneralCoreEq2
{
  #define SKG2_Pi 3.14159265358979323846
  #define SKG2_RGas 8.3144 //??????
  #define SKG2_g 9.8 //[m/sec^2]
  #define SKG2_K 0.0000000000000000000000138 // Bolzman's constant 

  double freeLenght = 6.53 * 1.00E-008; // длинна свободного пробега
  double nugas = 1.82 * 1.00E-005; // в€зкость газа
  double Di = 1.00E-005;  // диффузионный коэфициент 
  double Mi = 0.1;  // мол€рный коэффициент
  double T = 300; // temperatura
  double ro = 1; // плотность частиц
  double roEff = 1.00E003; // эффективна€ плотность сферической частицы
  double deltaP = 1.00E-004; // параметер определ€ющий суть процесса

  double Ad = 200; // вертикальна€ скорость осаждени€ частиц
  double As = 600; // горизонтальна€ скорость осаждени€ частиц
  double sigmaD = 1.00E-004; // толщина пограницчного сло€
  double Volume = 2000; // обьем в котором идет процесс

  double q(double ri, double rk)
  {
    return pow(GUPow(ri, 3) - GUPow(rk, 3), 1.00 / 3);
  }

  double w(double ri, double rk)
  {
    return GUPow(ri, 2) / GUPow(q(ri, rk), 2);
  }

  double Kn(double Ri) // число кнудеена
  {
    return freeLenght / Ri;
  }

  double Cn(double Ri) // эмпирический корректирующий коэфициент каннингема
  {
    return (1 + 1.246 * Kn(Ri) + 0.42 * Kn(Ri) * exp(-(0.87) / (Kn(Ri))));
  }

  double Eps(double ri, double rk) // эффективность столкновени€ цастиц
  {
    return 0.5 * GUPow(ri / (ri + rk), 2);
  }

  double Us(double ri) // скорость гравитационного осаждени€ частиц
  {
    return ((2.00 * roEff * SKG2_g) / (9.00 * nugas)) * GUPow(ri, 2) * Cn(ri);
  }

  double U(double ri) // скорость конденсационного роста частиц
  {
    return ((4.00 * Di * Mi) / (2 * SKG2_RGas *  T  * ri * ro)) * deltaP * Cn(ri);
  }

  double B(double Ri) // мобильность частиц радиуса r
  {
    return Cn(Ri) / (6 * SKG2_Pi * nugas * Ri);
  }

  double alphaD(double r)
  {
    return SKG2_K * T * B(r) * Ad / (sigmaD * Volume);
  }

  double alphaS(double r)
  {
    return (4.00 / 3.00) * SKG_Pi * ro * GUPow(r, 3) * SKG2_g * B(r) * As / Volume;
  }

  double Kg(double ri, double rk)
  {
    return SKG2_Pi * Eps(ri, rk) * GUPow((ri + rk), 2) * GUAbs(Us(ri) - Us(rk));
  }

  double Kb(double ri, double rk)
  {
    return 4 * SKG2_Pi * SKG2_K * T * (ri + rk) * (B(ri) + B(rk));
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiAlphaDAlphaS: public MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiAlphaDAlphaS() : MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>(MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {}

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double r = iPoint[0];
    return MMG402SmoluhovskiGeneralCoreEq2::alphaD(r) + MMG402SmoluhovskiGeneralCoreEq2::alphaS(r);
  }
};

// перенос
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiUFunc: public MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiUFunc() : MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>(MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {}

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double r = iPoint[0];
    return MMG402SmoluhovskiGeneralCoreEq2::U(r);
  }
};


// diffusion
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiDiFunc: public SmoluhovskiUFunc<SpaceScalarType, DataScalarType>
{
public:
  SmoluhovskiDiFunc(double dr)
  {
    DeltaR = dr;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType _value = SmoluhovskiUFunc<SpaceScalarType, DataScalarType>::GetValue(iPoint);
    _value = _value * DeltaR / 5.00;
    return _value;
  }

private:
  double DeltaR;
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreKb : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return MMG402SmoluhovskiGeneralCoreEq2::Kb(x, y);
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreKg : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return MMG402SmoluhovskiGeneralCoreEq2::Kg(x, y);
  }
};

template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiKoreKgKb : public SmoluhovskiKoreBase<SpaceScalarType, DataScalarType>
{
public:
  SmoluhovskiKoreKgKb(double imKb, double imKg)
  {
    mKb = imKb;
    mKg = imKg;
  }

  SmoluhovskiKoreKgKb()
  {
    mKb = 1;
    mKg = 1;
  }

protected:
  virtual DataScalarType EvalueteValue(DataScalarType x, DataScalarType y) const
  {
    return  ( mKb * MMG402SmoluhovskiGeneralCoreEq::Kb(x, y) + mKg * MMG402SmoluhovskiGeneralCoreEq::Kg(x, y));
  }

  double mKb;
  double mKg;
};

namespace MMG402SmoluhovskiVaporone
{

#define Vaporone_sigm 35
#define Vaporone_T 298
#define Vaporone_Pi 3.14159265358979323846
#define Vaporone_K 0.0000000000000000000000138
#define Vaporone_Mw 300
#define Vaporone_Rop 1//[g/cm^3]
#define Vaporone_Lj 6.53E-006
#define Vaporone_S 1.0379
#define Vaporone_Cv 1.5496E-10//[g/cm^3]
#define Vaporone_C 1.6083E-010//[g/cm^3]

  //double S=C/Cv;

  double U(double x)
  {
    return x + 1;
  }
  double Kn(double Ri)
  {
    return 0.0000001 / Ri ; //L=0.0000001
  }
  double gam(double Ri)
  {
    return (4 / 3) * Vaporone_Pi * pow(Ri, 3) ; //??? particle's molecular volume
  }
  double Ke(double Ri)
  {
    return (4 * Vaporone_sigm * gam(Ri)) / (2 * Ri * Vaporone_K * Vaporone_T) ;
  }

  double Vj(double Ri)
  {
    return sqrt((3 * Ri * Vaporone_T) / Vaporone_Mw) ; // mean molecular speed
  }

  double Psi(double Ri)

  {
//Ri=3.00E-007
    //C=6.387E+007
    double tmp_deg = 1 / 3;
    double tmp_pow = pow((3 / 4) * Vaporone_Pi * Vaporone_Rop, tmp_deg);
    double tmp_rat = (1.333 * Kn(Ri) + 0.71) / (1 + 1 / Kn(Ri));
    double Dgj = tmp_deg * Vj(Ri) * Vaporone_Lj;
    double rat = 1 / (1 + tmp_rat);
    return 4 * Vaporone_Pi * tmp_pow * Dgj * pow(Ri, tmp_deg) * Vaporone_Cv * (Vaporone_S - exp(Ke(Ri))) * rat;
  }

};

// перенос
template <typename SpaceScalarType, typename DataScalarType>
class SmoluhovskiUFuncV2: public MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  SmoluhovskiUFuncV2() : MMG402Func::IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>(MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special))
  {}

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double r = iPoint[0];
    return MMG402SmoluhovskiVaporone::Psi(r);
  }
};



#endif
