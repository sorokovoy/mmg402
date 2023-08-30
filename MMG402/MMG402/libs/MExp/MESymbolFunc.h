
#ifndef MESymbolFunc_H
#define MESymbolFunc_H

#include "MMG402\libs\MExp\IMESymbolPrinter.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolDerivative.h"

namespace MExp
{

#define MESymbolType_FuncSymbolSin 14
#define MESymbolType_FuncSymbolCos 15
#define MESymbolType_FuncSymbolExp 16

  class MESin : public MESymbol
  {
  public:
    MESin(const MESymbol& iArg)
      : MESymbol(0, MESymbolType_FuncSymbolSin)
    {
      _arguments.push_back(iArg);
    }
  };
  REGISTER_DERIVED_ID(MESymbol, MESin, MESymbolType_FuncSymbolSin);

  class MECos : public MESymbol
  {
  public:
    MECos(const MESymbol& iArg)
      : MESymbol(0, MESymbolType_FuncSymbolCos)
    {
      _arguments.push_back(iArg);
    }
  };
  REGISTER_DERIVED_ID(MESymbol, MECos, MESymbolType_FuncSymbolCos);

  class MEExp : public MESymbol
  {
  public:
    MEExp(const MESymbol& iArg)
      : MESymbol(0, MESymbolType_FuncSymbolExp)
    {
      _arguments.push_back(iArg);
    }
  };
  REGISTER_DERIVED_ID(MESymbol, MEExp, MESymbolType_FuncSymbolExp);

  class MESymbolFuncPrinter : public IMESymbolPrinter
  {
  public:
    virtual bool Print(std::ostream& out, const MESymbol& iSymb)
    {
      switch (iSymb.GetSymbolType()) {
        case MESymbolType_FuncSymbolSin :
          out << " sin(" << iSymb.GetArguments()[0]  << ")";
          return true;
        case MESymbolType_FuncSymbolCos :
          out << " cos(" << iSymb.GetArguments()[0]  << ")";
          return true;
        case MESymbolType_FuncSymbolExp :
          out << " exp(" << iSymb.GetArguments()[0]  << ")";
          return true;
      }
      return false;
    }
  };

  REGISTER_OBJECT(IMESymbolPrinter, MESymbolFuncPrinter);

  class MESymbolFuncDerivator : public IMESymbolDerivator
  {
  public:
    virtual bool Evaluate(const MESymbol& iValue, const MESymbol& iDf, MESymbol& oResult)
    {
      switch (iValue.GetSymbolType()) {
        case MESymbolType_FuncSymbolSin: {
          oResult = MECos(iValue.GetArguments()[0]) * Df(iValue.GetArguments()[0], iDf);
          return true;
        }

        case MESymbolType_FuncSymbolCos: {
          oResult = -MESin(iValue.GetArguments()[0]) * Df(iValue.GetArguments()[0], iDf);
          return true;
        }

        case MESymbolType_FuncSymbolExp: {
          oResult = MEExp(iValue.GetArguments()[0]) * Df(iValue.GetArguments()[0], iDf);
          return true;
        }
      }
      return false;
    }
  };

  REGISTER_OBJECT(IMESymbolDerivator, MESymbolFuncDerivator);


} // end MExp namespace
#endif //MESymbolFunc_H
