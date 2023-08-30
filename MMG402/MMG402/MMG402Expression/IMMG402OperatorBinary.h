#ifndef IMMG402OperatorBinary_H
#define IMMG402OperatorBinary_H

#include "MMG402\MMG402Expression\IMMG402Operator.h"

namespace MMG402Expression
{

  class MExp::IMESymbolOperatorBinary;

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402OperatorBinary : public IMMG402Operator
  {
  public:
    virtual MExp::IMESymbolOperatorBinary* CreateImplementation() const = 0;
    virtual MMG402OperatorArgumentType GetArgumentType() const
    {
      return MMG402OperatorArgumentType_Binary;
    }
  };

} // end MMG402Expression
#endif // IMMG402OperatorBinary_H
