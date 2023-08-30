#ifndef IMMG402TestingEngineTemporalResultPrinter_H
#define IMMG402TestingEngineTemporalResultPrinter_H

#include <stdio.h>

class IMMG402TestingEngineTemporalResultPrinter
{
public:
  virtual void Print(std::ostream& out) const = 0;
  virtual int  GetId() const = 0;
};

#endif
