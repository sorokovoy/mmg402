#ifndef IMECalculator_H
#define IMECalculator_H

#include "MMG402\libs\MExp\IMESymbolData.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolTable.h"

#include "MMG402\libs\GUtils\GUAutoPtr.h"
#include "MMG402\libs\GUtils\GUAssert.h"

#include <sstream>

namespace MExp
{

#define IMECalculatorValidatorAssert(exp, Message)  \
   if(!(exp)) {                                     \
      GUAssertT(exp, Message);                      \
      if(iShowException) {                          \
        std::stringstream ss; ss << Message;        \
        throw new std::exception(ss.str().c_str()); \
      } else {                                      \
        return false;                               \
      }                                             \
   }                                                \

  class IMECalculator
  {
  public:
    virtual ~IMECalculator() {};

    virtual void SetExpression(const MESymbol& iExp)
    {
      _expression = iExp;
    }
    virtual void SetSymbolTable(const MESymbolTable& iTable)
    {
      _table = iTable;
    }

    virtual bool Validate(bool iShowException = false) const
    {
      return ValidateExp(_expression, iShowException);
    }

    virtual IMESymbolData* Evaluate() const = 0;

  protected:
    virtual bool ValidateExp(const MESymbol& iSymb, bool iShowException) const
    {
      if (iSymb.GetSymbolType() == MESymbolType_Constant) {
        IMECalculatorValidatorAssert(_table.GetContantParser() != 0, "Expression contains constant '" << iSymb.GetSymbolName() << "' while _table.pConstantParser == NULL");
        IMESymbolData* pVal = _table.GetContantParser()->Parse(iSymb.GetSymbolName());
        IMECalculatorValidatorAssert(pVal != 0, "Unable parce constant '" << iSymb.GetSymbolName() << "'");
        delete pVal;
      } else if (iSymb.GetSymbolType() == MESymbolType_Symbol) {
        IMECalculatorValidatorAssert(_table.GetData(iSymb.GetSymbolName()), "Value " << iSymb.GetSymbolName() << " not found in symbol table");
        IMECalculatorValidatorAssert(iSymb.GetArguments().size() == 0, "Symbol " << iSymb.GetSymbolName() << " has arguments.");
      } else {
        bool find = false;
        if (QueryUnaryOperator(iSymb)) {
          IMECalculatorValidatorAssert(iSymb.GetArguments().size() == 1, "Operator " << iSymb.GetSymbolType() << " has " << iSymb.GetArguments().size() << " arguments require 1.");
          find = true;
        }

        if (QueryBinaryOperator(iSymb)) {
          IMECalculatorValidatorAssert(iSymb.GetArguments().size() >= 2, "Operator " << iSymb.GetSymbolType() << " has " << iSymb.GetArguments().size() << " arguments require at least 2.");
          IMECalculatorValidatorAssert(!find, "WOW!!!! Operator " << iSymb.GetSymbolType() << " is unary and binary at one time!!");
          find = true;
        }

        IMECalculatorValidatorAssert(find, "Operator " << iSymb.GetSymbolType() << " not found in symbol table");
        for (unsigned int i = 0; i < iSymb.GetArguments().size(); i++) {
          if (!ValidateExp( iSymb.GetArguments()[i], iShowException)) return false;
        }
      }
      return true;
    }

    virtual const IMESymbolOperatorUnary* QueryUnaryOperator(const MESymbol& iSymb) const
    {
      return _table.GetUnaryOperator(iSymb.GetSymbolType());
    }

    virtual const IMESymbolOperatorBinary* QueryBinaryOperator(const MESymbol& iSymb) const
    {
      return _table.GetBinaryOperator(iSymb.GetSymbolType());
    }


    MESymbol _expression;
    mutable MESymbolTable _table;

    IMECalculator() {};

  private:
    IMECalculator(const IMECalculator&) {};
  };

} // end namespace MExp

#endif // IMECalculator_H