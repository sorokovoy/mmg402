#ifndef MMG402TestingEngineTemporalTimeMesher_H
#define MMG402TestingEngineTemporalTimeMesher_H

#include "IMMG402TestingEngineTemporalResultPrinter.h"

#include "MMG402\libs\GUtils\GUMathUtils.h"
#include <vector>
#include <time.h>

#define MMG402TestingEngineValidatorRegistry_TimeMesher 9

class MMG402TestingEngineTemporalTimeMesher : public IMMG402TestingEngineTemporalResultPrinter
{
public:
  void Set()
  {
    _t = clock();
  }

  void Loop()
  {
    double t = (double)(clock() - _t) / CLOCKS_PER_SEC;
    ProcessNewTime(t);
  }

  virtual void Print(std::ostream& out) const
  {
    double max = -1.00E010;
    double min =  1.00E010;
    double sum = 0;
    for (unsigned int i = 0; i < _timings.size(); i++) {
      max = GUMax(max, _timings[i]);
      min = GUMin(min, _timings[i]);
      sum += _timings[i];
    }
    char buf[1024];
    sprintf_s(buf, 1024, "Timings (sec). av:%6.2f min:%6.2f max:%6.2f \n" , sum / _timings.size(), min , max);
    out << buf;
  }

  virtual int  GetId() const
  {
    return MMG402TestingEngineValidatorRegistry_TimeMesher;
  }
private:

  void ProcessNewTime(double t)
  {
    _timings.push_back(t);
  }

  clock_t _t;
  std::vector<double> _timings;
};

#include "MMG402\libs\ObjRegistry.h"
REGISTER_DERIVED_ID(IMMG402TestingEngineTemporalResultPrinter, MMG402TestingEngineTemporalTimeMesher, MMG402TestingEngineValidatorRegistry_TimeMesher);

#endif
