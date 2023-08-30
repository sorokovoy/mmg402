// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Unit test for general objects units
//
//===================================================================
//  MAR 2013  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#include "MMG402TestMExp.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolFunc.h"
#include "MMG402\libs\MExp\MESymbolArifmetic.h"
#include "MMG402\libs\MExp\MESymbolDerivative.h"
#include "MMG402\libs\MExp\MESymbolPrinterArifmetic.h"

#include "MMG402\libs\MExp\MESymbolTable.h"
#include "MMG402\libs\MExp\MECalculatorDeep.h"

#include "MMG402\libs\GUtils\GURandom.h"

#include "MMG402TestingOperatorSymbolicElevator.h"
#include "MMG402TestingRegistryFucntion.h"
#include "MMG402TestingRegistryEquationOperatorsExplicitTemporal.h"

using namespace MExp;

#define DefineSymbols \
  MESymbol sx("x"); MESymbol sy("y"); MESymbol sz("z"); \

#define ValidateExp(symbol, analyticalEq) {                           \
  double x, y, z, t;                                                  \
  MESymbolTable symbt;                                                \
  symbt.SetContantParser(new MEArifmeticConstantParser<double>());    \
  MESymbolNumData<double> xv(2);                                      \
  MESymbolNumData<double> yv(3);                                      \
  MESymbolNumData<double> zv(4);                                      \
  MESymbolNumData<double> tv(4);                                      \
  symbt.SetData("x", &xv);                                            \
  symbt.SetData("y", &yv);                                            \
  symbt.SetData("z", &zv);                                            \
  symbt.SetData("t", &tv);                                            \
  symbt.AddBinaryOperator(new MESymbolOperatorSum<double>());         \
  symbt.AddBinaryOperator(new MESymbolOperatorProd<double>());        \
  symbt.AddBinaryOperator(new MESymbolOperatorDevide<double>());      \
  symbt.AddUnaryOperator(new MESymbolOperatorUnaryInverse<double>()); \
  symbt.AddUnaryOperator(new MESymbolOperatorUnaryCos<double>());     \
  symbt.AddUnaryOperator(new MESymbolOperatorUnarySin<double>());     \
  symbt.AddUnaryOperator(new MESymbolOperatorUnaryExp<double>());     \
  MECalculatorDeep c;      \
  c.SetExpression(symbol); \
  c.SetSymbolTable(symbt); \
  c.Validate(true);        \
  for(int pc = 0; pc < 100; pc++) { \
    x = GURand::U01(); y = GURand::U01(); z = GURand::U01(); t = GURand::U01(); \
    xv.Value() = x; yv.Value() = y; zv.Value() = z; tv.Value() = t;             \
    double dv = analyticalEq;                                \
    MESymbolNumData<double>* pData;                          \
    pData = (MESymbolNumData<double>*)c.Evaluate();          \
    double rv = pData->Value();                              \
    ASSERT_DOUBLE_EQUALS(dv, rv);                            \
  } }                                                        \


bool MMG402TestMExp::Run()
{
  RUN_TMMG402T(TestMExp());
  return true;
}

bool MMG402TestMExp::TestMExp()
{
  ASSERT_TRUE(TestElevation());
  ASSERT_TRUE(TestDerivaty());
  ASSERT_TRUE(TestPrinting());
  ASSERT_TRUE(TestSymbolicDerivator());
  return true;
}

bool MMG402TestMExp::TestElevation()
{
  DefineSymbols;
  ValidateExp(sx + sy + sz, x + y + z);
  ValidateExp(60 + sx * sx + sy / sz + sz * sx, 60 + x * x + y / z + z * x);
  ValidateExp(MESin(sx) + MECos(sy) + MEExp(sz), sin(x) + cos(y) + exp(z));
  return true;
}

class Component
{
public:
  MESymbol operator() (int iType, int v1)
  {
    return MESymbol("eeeddd11111");
  }

  MESymbol operator[] (const MESymbol& iSymb) const
  {
    return MESymbol(iSymb, 10);
  }
};


bool MMG402TestMExp::TestDerivaty()
{
  Component c;
  MESymbol d("ddd");
  MESymbol x1 = c[d];
  MESymbol x2 = c(22, 4);


  DefineSymbols;
  ValidateExp(Df(sx + sy + sz, sx), 1);
  ValidateExp(Df(sx + sy + sz, sy), 1);
  ValidateExp(Df(sx + sy + sz, sz), 1);
  ValidateExp(Df(sx + sy, sz), 0);
  ValidateExp(Df(MESin(sx + sy), sz), 0);
  ValidateExp(Df(MESin(sx + sy + sz), sz), cos(x + y + z));
  ValidateExp(Df(MECos(sx + sy + sz), sz), -sin(x + y + z));
  ValidateExp(Df(MEExp(MECos(sx + sy + sz)), sz), -exp((double)(cos(x + y + z)))*sin(x + y + z));
  ValidateExp(Df(sx * sx + sy * sy, sx), x * 2);
  return true;
}

#include <sstream>

bool MMG402TestMExp::TestPrinting()
{
  DefineSymbols;

  std::stringstream ss1;
  ss1 << sx + sy + sz;
  ASSERT_TRUE(ss1.str() == "(x+y+z)");

  std::stringstream ss2;
  ss2 << MESin(sx + sy);
  ASSERT_TRUE(ss2.str() == " sin((x+y))");

  return true;
}

bool MMG402TestMExp::TestSymbolicDerivator()
{
  ASSERT_TRUE(TestSymbolicDerivator1());
  ASSERT_TRUE(TestSymbolicDerivator2());
  ASSERT_TRUE(TestSymbolicDerivator3());
  ASSERT_TRUE(TestSymbolicDerivator4());
  return true;
}

bool MMG402TestMExp::TestSymbolicDerivator1()
{
  MESymbol func = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(2, 2);
  MESymbol dOperator = MMG402TestingRegistryEquationOperatorsExplicitTemporal::CreateOperator<double, 2, double, 1>(0, MESymbol("u"), MESymbol("f"));
  MESymbol dRes =  MMG402TestingOperatorSymbolicElevator::GetInstance().EvaluateDfOperator(func, dOperator);
  ValidateExp(dRes, 2 * x + 1 + exp(y) + 2 * t);
  return true;
}

bool MMG402TestMExp::TestSymbolicDerivator2()
{
  MESymbol func = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(2, 2);
  MESymbol dOperator = MMG402TestingRegistryEquationOperatorsExplicitTemporal::CreateOperator<double, 2, double, 1>(1, MESymbol("u"), MESymbol("f"));
  MESymbol dRes =  MMG402TestingOperatorSymbolicElevator::GetInstance().EvaluateDfOperator(func, dOperator);
  ValidateExp(dRes, 2 + exp(y) + 2 * t);
  return true;
}

bool MMG402TestMExp::TestSymbolicDerivator3()
{
  MESymbol func = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(3, 2);
  MESymbol dOperator = MMG402TestingRegistryEquationOperatorsExplicitTemporal::CreateOperator<double, 3, double, 1>(0, MESymbol("u"), MESymbol("f"));
  MESymbol dRes =  MMG402TestingOperatorSymbolicElevator::GetInstance().EvaluateDfOperator(func, dOperator);
  ValidateExp(dRes,  2 * x + 1 + exp(y) + 2 * t + 2 * z);
  return true;
}

bool MMG402TestMExp::TestSymbolicDerivator4()
{
  MESymbol func = MMG402TestingRegistryFucntion::GetInstance().GetTFunction(3, 2);
  MESymbol dOperator = MMG402TestingRegistryEquationOperatorsExplicitTemporal::CreateOperator<double, 3, double, 1>(3, MESymbol("u"), MESymbol("f"));
  MESymbol dRes =  MMG402TestingOperatorSymbolicElevator::GetInstance().EvaluateDfOperator(func, dOperator);
  ValidateExp(dRes, 2 * x * t + 2 * z + (1 + exp(y))*x + exp(y)*y + 2 * z * y + 2 * x + 2 * t);
  return true;
}

