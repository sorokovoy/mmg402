#ifndef MESymbolDerivative_H
#define MESymbolDerivative_H

#include "MMG402\libs\ObjRegistry.h"

#include "MMG402\libs\MExp\MESymbol.h"

namespace MExp
{

  inline MESymbol Df(const MESymbol& iValue, const MESymbol& iDf);

  class IMESymbolDerivator
  {
  public:
    virtual bool Evaluate(const MESymbol& iValue, const MESymbol& iDf, MESymbol& oResult) = 0;
  };

  class MESymbolDerivator : public IMESymbolDerivator
  {
  public:
    virtual bool Evaluate(const MESymbol& iValue, const MESymbol& iDf, MESymbol& oResult)
    {
      switch (iValue.GetSymbolType()) {
        case MESymbolType_Constant: {
          oResult = MESymbol("0", true);
          return true;
        }

        case MESymbolType_Symbol: {
          oResult = (iDf.GetSymbolName() == iValue.GetSymbolName()) ? MESymbol("1", true) : MESymbol("0", true);
          return true;
        }

        case MESymbolType_Sum: {
          MESymbol res(0, MESymbolType_Sum);
          for (unsigned int i = 0; i < iValue.GetArguments().size(); i++) {
            MESymbol s = Df(iValue.GetArguments()[i], iDf);
            if (s.GetSymbolType() != MESymbolType_Zero) {
              res._arguments.push_back(s);
            }
          }
          if (res._arguments.size() == 1) {
            oResult = res._arguments[0];
          }
          if (res._arguments.size() == 0) {
            oResult =  MESymbol("0", true);
          }
          oResult = res;
          return true;
        }

        case MESymbolType_Prod: {
          MESymbol res(0, MESymbolType_Sum);
          for (unsigned int i = 0; i < iValue._arguments.size(); i++) {
            MESymbol s = Df(iValue._arguments[i], iDf);
            if (s.GetSymbolType() != MESymbolType_Zero) {
              MESymbol prod(0, MESymbolType_Prod);
              for (unsigned int j = 0; j < iValue._arguments.size(); j++)
                prod._arguments.push_back(j == i ? s : iValue._arguments[j]);
              res._arguments.push_back(prod);
            }
          }
          if (res._arguments.size() == 1) {
            oResult = res._arguments[0];
          }
          if (res._arguments.size() == 0) {
            oResult =  MESymbol("0", true);
          }
          oResult = res;
          return true;
        }

        case MESymbolType_Devide: {
          oResult = (Df(iValue._arguments[0], iDf) * iValue._arguments[1] - iValue._arguments[0] * Df(iValue._arguments[1], iDf)) / (iValue._arguments[1] * iValue._arguments[1]);
          return true;
        }

        case MESymbolType_UnaryInverce: {
          oResult = -Df(iValue._arguments[0], iDf);
          return true;
        }

        case MESymbolType_Zero: {
          oResult = MESymbol();
          return true;
        }
      }
      return false;
    }
  };

  REGISTER_OBJECT(IMESymbolDerivator, MESymbolDerivator);

  inline MESymbol Df(const MESymbol& iValue, const MESymbol& iDf)
  {
    if (iDf.GetSymbolType() != MESymbolType_Symbol)
      throw new std::invalid_argument("Only by simple variable differentiation available");

    MESymbol res;
    const std::map<std::string, ObjRegistry::ObjectRegistryInfo<IMESymbolDerivator>>& inst = ObjRegistry::RegistryEntry<IMESymbolDerivator>::GetRegistry();
    for (std::map<std::string, ObjRegistry::ObjectRegistryInfo<IMESymbolDerivator>>::const_iterator it = inst.cbegin(); it != inst.cend(); it++) {
      if (it->second.pInstance->Evaluate(iValue, iDf, res)) return res;
    }

    throw new std::invalid_argument("Symbol type not supported");
  };

} // end MExp namespace
#endif // MESymbolDerivative_H
