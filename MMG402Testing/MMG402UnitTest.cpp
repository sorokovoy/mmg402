// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Base class and utilities for create unit tests
//
//===================================================================
//  OCT 2012  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#include <vector>
#include "MMG402UnitTest.h"

#include "MMG402TestMExp.h"
#include "MMG402TestLinearAlgebra.h"
#include "MMG402TestCommon.h"


bool RunAllUnitTest(void)
{
  std::vector<MMG402UnitTest*> tests;

  // Add new tests here
  tests.push_back(new MMG402TestCommon());
  tests.push_back(new MMG402TestLinearAlgebra());
  tests.push_back(new MMG402TestMExp());

  // Run all tests
  int totalTests = 0;
  int failedTests = 0;
  double totalTime = 0;

  for (unsigned int i = 0; i < tests.size(); i++) {
    MMG402UnitTest* test = tests[i];
    test->Run();
    totalTests += test->GetAllTestsNb();
    failedTests += test->GetFailedTestsNb();
    totalTime += test->GetTotalTime();
    delete test;
  }

  std::cout << "Total tests:\t" << totalTests << std::endl;
  std::cout << "Failed tests:\t" << failedTests << std::endl;
  std::cout << "Total time:\t" << totalTime << std::endl;

  return true;
}