#ifndef MMG402TestCommon_H
#define MMG402TestCommon_H

#include "MMG402UnitTest.h"

class MMG402TestCommon : public MMG402UnitTest
{
public:
  virtual bool Run();

private:
  bool TestObjRegistryStorage();
};

#endif