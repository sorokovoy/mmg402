// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Define different random generators
//
//===================================================================
//  DEC 2012  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef GURandom_H
#define GURandom_H

#include <math.h>

#define GURandBaseMax 2147483647

#define GURandRSeedB 123456789
#define GURandA 1103515245
#define GURandC 12345

class GURand
{
public:
  static int BaseRand(void)
  {
    static int GURandRSeed(GURandRSeedB);
    GURandRSeed = GURandRSeed * GURandA + GURandC;
    return (GURandRSeed > 0 ? GURandRSeed : -GURandRSeed) % GURandBaseMax;
  }

  static double N(double E, double Sigma)
  {
    double alpha1, alpha2, alpha3;
    double w1, w2, ksi;
    do {
      alpha1 = U01();
      alpha2 = U01();
      w1 = 2 * alpha1 - 1;
      w2 = 2 * alpha2 - 1;
    } while (w1 * w1 + w2 * w2 <= 1);
    alpha3 = U(0.01, 1);

    w1 = w1 / sqrt(w1 * w1 + w2 * w2);
    w2 = w2 / sqrt(w1 * w1 + w2 * w2);

    ksi = sqrt(-2 * log(alpha3)) * w1;
    return ( Sigma * ksi + E);
  }

  static double U(double a, double b)
  {
    return U01() * ( b - a ) + a;
  }

  static double U01(void)
  {
    double randVal = (double)BaseRand() / GURandBaseMax;
    return randVal;
  }

  //return [0, max)
  static unsigned int RandInt(unsigned int max)
  {
    return BaseRand() % max;
  }

  static bool RandBool(void)
  {
    int v = RandInt(3);
    for (int i = 0; i < v; i++) {
      BaseRand();
    }
    return BaseRand() % 2 == 0;
  }
};

#endif // AIRandom_H
