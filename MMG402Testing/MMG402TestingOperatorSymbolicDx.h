#ifndef MMG402TestingOperatorSymbolicDx_H
#define MMG402TestingOperatorSymbolicDx_H

#include "MMG402TestingOperatorSymbolic.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"

class MMG402TestingOperatorSymbolicDX : public MMG402TestingOperatorSymbolic
{
public:
  MMG402TestingOperatorSymbolicDX(int iMasterOperatorType, std::vector<MESymbol> iVarCollection, int iDCont)
    : MMG402TestingOperatorSymbolic(iMasterOperatorType, iVarCollection),
      _dCount(iDCont)
  {}

  virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol& iSymbol) const
  {
    METestingOperatorUnaryCheck;
    int varIdx = 0;
    void* pVarIdx = iSymbol.GetExtensionObject(MMG402Solver::Temporal::MMG402OperatorExtensionVarIDx);
    if (pVarIdx != 0) { // if not parsable it is DT operator
      varIdx = *((int*)pVarIdx);
      varIdx ++;
    }
    GUAssert(varIdx < (int) _varCollection.size());
    MESymbol& data = ((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData();
    for (int i = 0; i < _dCount; i++) {
      data = Df(data, _varCollection[varIdx]);
    }
  }
protected:
  const int _dCount;
};

#endif