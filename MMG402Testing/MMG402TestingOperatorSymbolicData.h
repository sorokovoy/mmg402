#ifndef MMG402TestingOperatorSymbolicData_H
#define MMG402TestingOperatorSymbolicData_H

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\IMESymbolData.h"

using namespace MExp;

#define MMG402TestingOperatorSymbolicDataTypeID 1008

class MMG402TestingOperatorSymbolicData : public IMESymbolData
{
public:
  MMG402TestingOperatorSymbolicData(const MESymbol& iSourceSymbol)
    : _data(iSourceSymbol)
  {}

  virtual int GetType() const
  {
    return MMG402TestingOperatorSymbolicDataTypeID;
  }

  MESymbol& GetData()
  {
    return _data;
  }

  virtual IMESymbolData* CreateCopy() const
  {
    return new MMG402TestingOperatorSymbolicData(_data);
  }

protected:
  MESymbol _data;
};

REGISTER_DERIVED_ID(IMESymbolData, MMG402TestingOperatorSymbolicData, MMG402TestingOperatorSymbolicDataTypeID);

#endif // MMG402TestingOperatorSymbolicData_H
