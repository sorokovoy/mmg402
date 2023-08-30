#ifndef MMG402TestingOperatorSymbolicElevator_H
#define MMG402TestingOperatorSymbolicElevator_H

#include "MMG402\libs\MExp\MECalculatorDeep.h"

#include "MMG402TestingOperatorSymbolicArifmetic.h"
#include "MMG402TestingOperatorSymbolicDx.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitDt.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitDt.h"

#include "MMG402TestingRegistryFucntion.h"

using namespace MExp;
using namespace MMG402Solver::Temporal;

class MMG402TestingOperatorSymbolicElevator
{
public:
  static MMG402TestingOperatorSymbolicElevator& GetInstance()
  {
    static MMG402TestingOperatorSymbolicElevator instance;
    return instance;
  }

  MESymbol EvaluateDfOperator(MESymbol func, MESymbol dfOperator)
  {
    MECalculatorDeep cd;
    cd.SetExpression(dfOperator);

    MESymbolTable t;
    t.SetContantParser(new MMG402TestingOperatorSymbolicConstant());
    t.AddUnaryOperator(new MMG402TestingOperatorUnaryInverse());
    t.AddUnaryOperator(new MMG402TestingOperatorUnarySin());
    t.AddUnaryOperator(new MMG402TestingOperatorUnaryCos());
    t.AddUnaryOperator(new MMG402TestingOperatorUnaryExp());

    t.AddBinaryOperator(new MMG402TestingOperatorDevide());
    t.AddBinaryOperator(new MMG402TestingOperatorProd());
    t.AddBinaryOperator(new MMG402TestingOperatorSum());

    const std::vector<MESymbol>& vars = MMG402TestingRegistryFucntion::GetInstance().GetVars();

    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD1, vars, 1));
    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorExplicitSpartialD2, vars, 2));
    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorExplicitDt,   vars, 1));

    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD1, vars, 1));
    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorImplicitSpartialD2, vars, 2));
    t.AddUnaryOperator(new MMG402TestingOperatorSymbolicDX(MMG402SolverOperatorRegistry_MMG402OperatorImplicitDt,   vars, 1));

    t.SetData("u", new MMG402TestingOperatorSymbolicData(func));
    t.SetData("f", new MMG402TestingOperatorSymbolicData(MESymbol("0", true)));

    for (unsigned int i = 0; i <  vars.size(); i++) {
      t.SetData(vars[i].GetSymbolName(), new MMG402TestingOperatorSymbolicData(vars[i]));
    }

    cd.SetSymbolTable(t);
    IMESymbolData* pRes = cd.Evaluate();

    MESymbol result = ((MMG402TestingOperatorSymbolicData*)pRes)->GetData();
    delete pRes;
    return result;
  }
};

#endif // MMG402TestingOperatorSymbolicElevator_H
