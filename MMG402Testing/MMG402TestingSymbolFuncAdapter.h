#ifndef MMG402TestingSymbolFuncAdapter_H
#define MMG402TestingSymbolFuncAdapter_H

#include "MMG402\MMG402Func\IMMG402Func.h"
#include "MMG402\MMG402Func\IMMG402FuncTemporal.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"
#include <vector>
#include "libs/exprtk.hpp"

#include "MMG402\libs\GUtils\GUAssert.h"

using namespace MMG402Func;

#define SPACE_VARIABLMMG402_COUNT 4

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingSymbolFuncAdapter : public IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  MMG402TestingSymbolFuncAdapter(const MMG402FuncStorageKey& iKey, const std::vector<MExp::MESymbol>& iVectorFunc)
    : IMMG402Func<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey)
  {
    Prepare(iVectorFunc);
  }

  MMG402TestingSymbolFuncAdapter(const MMG402FuncStorageKey& iKey, const MExp::MESymbol& iSymb)
    : IMMG402FuncTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(iKey)
  {
    std::vector<MExp::MESymbol> vectorFunc;
    vectorFunc.push_back(iSymb);
    Prepare(vectorFunc);
  }

  virtual typename MMG402NumData GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint) const
  {
    // set var vals
    for (int i = 0; i < SpaceDimAtCompile; i++) {
      _vars[i] = iPoint(i);
    }

    for (int i = 0; i < DataDimAtCompile; i++) {
      _lastResult(i) = _vectorFunc[i].value();
    }
    return _lastResult;
  }

  virtual void SetTemporalValue(double iVal)
  {
    _vars[SPACE_VARIABLMMG402_COUNT] = (SpaceScalarType)iVal;
  }

  virtual IMMG402FuncType GetType() const
  {
    return IMMG402FuncType_Temporal;
  }

protected:
  virtual void InitSymbolTable(exprtk::symbol_table<SpaceScalarType>& table)
  {
    table.add_variable("x", _vars[0]);
    table.add_variable("y", _vars[1]);
    table.add_variable("z", _vars[2]);
    table.add_variable("v", _vars[3]);
    table.add_variable("t", _vars[4]);
    for (int i = 0; i < SPACE_VARIABLMMG402_COUNT + 1; i++) {
      _vars[i] = 0;
    }
  }

  void Prepare(std::vector<MExp::MESymbol> iVectorFunc)
  {
    GUAssert(iVectorFunc.size() == DataDimAtCompile);
    GUAssert(SpaceDimAtCompile < 5);

    exprtk::symbol_table<SpaceScalarType> symbTable;
    exprtk::parser<SpaceScalarType> parser;

    InitSymbolTable(symbTable);
    for (unsigned int i = 0 ; i < iVectorFunc.size(); i++) {
      exprtk::expression<SpaceScalarType> exp;
      exp.register_symbol_table(symbTable);
      GUAssert(parser.compile(ToString(iVectorFunc[i]), exp));
      _vectorFunc.push_back(exp);
    }
  }

  enum { SpaceDimAtCompile = SpaceDim };
  enum { DataDimAtCompile = DataDim };

  mutable typename MMG402NumData _lastResult;
  mutable SpaceScalarType _vars[SPACE_VARIABLMMG402_COUNT + 1];

  std::vector<exprtk::expression<SpaceScalarType>> _vectorFunc;
};

#pragma warning(default:4996)

#endif //MMG402TestingSymbolFuncAdapter
