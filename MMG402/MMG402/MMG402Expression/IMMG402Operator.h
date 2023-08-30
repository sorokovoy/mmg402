#ifndef IMMG402Operator_H
#define IMMG402Operator_H

#include "MMG402/libs/ObjExtension.h"

namespace MMG402Expression
{
  enum MMG402OperatorArgumentType {
    MMG402OperatorArgumentType_Undefined = -1,
    MMG402OperatorArgumentType_Unary = 1,
    MMG402OperatorArgumentType_Binary = 2
  };

  enum MMG402OperatorSchemeType {
    MMG402OperatorSchemeType_Undefined = -1,
    MMG402OperatorSchemeType_ImplicitTemporal = 1,
    MMG402OperatorSchemeType_ExplicitTemporal = 2,
    MMG402OperatorSchemeType_TemporalUniversal = 3
  };

  class IMMG402Operator : public ObjExtension::ObjExtensionClass, public ObjExtension::ObjExtensionInfo
  {
  public:
    virtual ~IMMG402Operator() {};

    virtual MMG402OperatorArgumentType GetArgumentType() const = 0;
    virtual MMG402OperatorSchemeType   GetSchemeType()   const = 0;

  protected:
    IMMG402Operator() {};
    IMMG402Operator(const IMMG402Operator&) {};
  };

} // end MMG402Expression

#endif
