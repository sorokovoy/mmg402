#ifndef IMMG402OperatorUnary_H
#define IMMG402OperatorUnary_H

#include "MMG402\MMG402Expression\IMMG402Operator.h"

namespace MMG402Expression
{

  class MExp::IMESymbolOperatorUnary;

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402OperatorUnary : public IMMG402Operator
  {
  public:
    virtual MMG402OperatorArgumentType GetArgumentType() const
    {
      return MMG402OperatorArgumentType_Unary;
    }
  };

} // end MMG402Expression
#endif // IMMG402OperatorUnary_H
