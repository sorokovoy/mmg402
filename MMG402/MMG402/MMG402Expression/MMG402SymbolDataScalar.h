#ifndef MMG402SymbolDataScalar_H
#define MMG402SymbolDataScalar_H

#include "MMG402\libs\MExp\MESymbolArifmetic.h"

namespace MMG402Expression
{
#define MMG402SymbolDataType_Scalar 102

  template <typename DataScalarType>
  class MMG402SymbolDataScalar: public MExp::MESymbolNumData<DataScalarType>
  {
  public:
    MMG402SymbolDataScalar(DataScalarType iVal)
      : MExp::MESymbolNumData<DataScalarType>(iVal)
    {
      _pReference = 0;
    };
    MMG402SymbolDataScalar(DataScalarType* ipReference)
      : MExp::MESymbolNumData<DataScalarType>(*ipReference)
    {
      _pReference = ipReference;
    };

    virtual MExp::IMESymbolData* CreateCopy() const
    {
      return new MMG402SymbolDataScalar<DataScalarType>(*this);
    }

    virtual DataScalarType& Value()
    {
      return _pReference == 0 ? MExp::MESymbolNumData<DataScalarType>::Value() : *_pReference;
    }

    virtual const DataScalarType& Value() const
    {
      return _pReference == 0 ? MExp::MESymbolNumData<DataScalarType>::Value() : *_pReference;
    }

    virtual int GetType() const
    {
      return MMG402SymbolDataType_Scalar;
    }
  protected:
    DataScalarType* _pReference;
  };

  REGISTER_DERIVED_ID(MExp::IMESymbolData, MMG402SymbolDataScalar, MMG402SymbolDataType_Scalar); // validate ID

} // end MMG402Expression
#endif //MMG402SymbolDataScalar_H
