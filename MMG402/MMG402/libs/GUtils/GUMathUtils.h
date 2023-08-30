// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Math utilitiGU
//
//===================================================================
//  OCT 2012  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUMathUtils_H
#define GUMathUtils_H

#include <math.h>
#include <time.h>

#define TOLERANCE       1.00E-006
#define ANGLE_TOLERANCE 1.00E-003

#define GUMax(a,b)    (((a) > (b)) ? (a) : (b))
#define GUMin(a,b)    (((a) < (b)) ? (a) : (b))
#define GUAbs(x)      ((x) < 0 ? -(x) : (x))
#define GUSqr(X)      ((X)*(X))
#define GUSqrt(X)     (X < 0 ? 0 : sqrt(X))

// constants
#define GUPi    3.14159265358979323846264338327950288419716939937510
#define GU2Pi   6.28318530717958647692528676655900576839433879875021
#define GUPiBy2 1.57079632679489661923132169163975144209858469968755
#define GUPiBy3 1.04719755119659774615421446109316762806572313312504
#define GUPiBy4 0.78539816339744830961566084581987572104929234984377
#define GUEVal  2.71828182845904523536028747135266249775724709369995

// GURadian is 180.0/GUPi
#define GURadian 57.29577951308232087679815481410517033240547246656432154916

#define GUBigNumber 1e100
#define GUBigFloatNumber 1.00E35f

#define GUIsInRange(x, iMin, iMax)  \
   ((iMin) <= (x) && (x) <= (iMax)) \

#define GUIsInRangeEx(x, iMin, iMax, iTolerance)  \
   ((iMin - iTolerance) <= (x) && (x) <= (iMax + iTolerance)) \

#define GUIsNAN(x)    ( _isnan(x) )
#define GUIsFinite(x) ( _finite((x)) != 0 )

#define GUTimerInit(TimerName)   clock_t TimerName = clock();
#define GUTimerReset(TimerName)  TimerName = clock();
#define GUTimerGetDiffInSec(TimerName)  ((double)(clock() - TimerName) / CLOCKS_PER_SEC)

template <typename T>
T GUPow(T x, unsigned int n)
{
  T r = 1;
  while (n) {
    if (n & 1) r *= x;
    n >>= 1;
    x = x * x;
  }
  return r;
}

template <class T>
inline void GUSwap(T& iValue1, T& iValue2)
{
  T tmp = iValue1;
  iValue1 = iValue2;
  iValue2 = tmp;
}

#endif // GUMathUtils_H
