// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Unit test for general objects units
//
//===================================================================
//  MAR 2013  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef MMG402TestMExp_H
#define MMG402TestMExp_H

#include "MMG402UnitTest.h"

class MMG402TestMExp : public MMG402UnitTest
{
public:
  virtual bool Run();

private:
  bool TestMExp();

  bool TestDerivaty();
  bool TestElevation();
  bool TestPrinting();

  bool TestSymbolicDerivator();

  bool TestSymbolicDerivator1();
  bool TestSymbolicDerivator2();
  bool TestSymbolicDerivator3();
  bool TestSymbolicDerivator4();
};

#endif // TestGeneralObjects_H
