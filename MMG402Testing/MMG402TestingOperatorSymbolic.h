#ifndef MMG402TestingOperatorSymbolic_H
#define MMG402TestingOperatorSymbolic_H

#include "MMG402TestingOperatorSymbolicArifmetic.h"
#include "MMG402\libs\MExp\MESymbolDerivative.h"

class MMG402TestingOperatorSymbolic : public IMESymbolOperatorUnary
{
public:
  MMG402TestingOperatorSymbolic(int iMasterOperatorType, std::vector<MESymbol> iVarCollection)
    : _masterOperatorType(iMasterOperatorType),
      _varCollection(iVarCollection)
  {}

  virtual int GetType() const
  {
    return _masterOperatorType;
  }

protected:
  const int _masterOperatorType;
  const std::vector<MESymbol> _varCollection;

private:
  MMG402TestingOperatorSymbolic() : _masterOperatorType(0), _varCollection(std::vector<MESymbol>()) {};
  MMG402TestingOperatorSymbolic(const MMG402TestingOperatorSymbolic&) : _masterOperatorType(0), _varCollection(std::vector<MESymbol>()) {};
};

#endif
