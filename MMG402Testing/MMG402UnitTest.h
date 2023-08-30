// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Base class and utilities for create unit tests
//
//===================================================================
//  OCT 2012  Creation:                       Sorokovoy Alexey, NSU
//===================================================================


#ifndef MMG402UnitTest_H
#define MMG402UnitTest_H

#include <time.h>
#include <iostream>
#include <iomanip>

bool RunAllUnitTest(void);

// abstract class for creating unit test
class MMG402UnitTest
{
public:
  virtual ~MMG402UnitTest() {};
  virtual bool Run() = 0;
  int GetFailedTestsNb()
  {
    return _failedTests;
  }
  int GetAllTestsNb()
  {
    return _allTests;
  }
  double GetTotalTime()
  {
    return _totalTime;
  }
protected:
  MMG402UnitTest() : _retCode(true), _failedTests(0), _allTests(0), _totalTime(0) {}
  bool _retCode;
  int _failedTests;
  int _allTests;
  double _totalTime, _start, _time;
};

#define GetCurrTime ((double)clock()/CLOCKS_PER_SEC)

#define RUN_TMMG402T(func) \
  _start =  GetCurrTime; \
  std::cout <<  std::setw(45) << std::left << #func; \
  _retCode = func; \
  _time = (GetCurrTime - _start); \
  std::cout <<" Time = "<< std::setprecision (3) << std::fixed << std::setw(6) << _time << " -> "; \
  if (_retCode != true) { \
     std::cout << "  FAIL" << std::endl; \
    _retCode = false; \
    _failedTests++; \
  } \
  else { \
    std::cout << " OK" << std::endl; \
  } \
  _totalTime += _time; \
  _allTests++;

#define ASSERT_OK(arg) \
  if ((arg) != true) { \
    std::cout << "  Error:" << #arg << " is not OK" << std::endl; \
    return false; \
  }

#define ASSERT_TRUE(arg) \
  if (!(arg)) { \
    std::cout << "  Error:" << #arg << " is FALSE (must be TRUE)" << std::endl; \
    return false; \
  }

#define ASSERT_FALSE(arg) \
  if (arg) { \
    std::cout << "  Error:" << #arg << " is TRUE (must be FALSE)" << std::endl; \
    return false; \
  }

#define ASSERT_EQUALS(arg1, arg2) \
  if ((arg1) != (arg2)) { \
    std::cout << "  Error:" << arg1 << " is not equal " << arg2 << std::endl; \
    return false; \
  }

#define EQUAL_TOLERANCE 1e-6
#define ASSERT_DOUBLE_EQUALS(arg1, arg2) \
  if (!(fabs((arg1) - (arg2)) < EQUAL_TOLERANCE)) { \
    std::cout << "  Error:" << arg1 << " is not equal " << arg2 << std::endl; \
    return false; \
  }

#define ASSERT_DOUBLE_EQUALS_T(arg1, arg2, iTolerance) \
  if (!(fabs((arg1) - (arg2)) < iTolerance)) { \
    std::cout << "  Error:" << arg1 << " is not equal " << arg2 << std::endl; \
    return false; \
  }

#endif /* AUnitTest_H */
