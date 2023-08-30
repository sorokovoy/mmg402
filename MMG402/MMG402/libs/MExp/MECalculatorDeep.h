
#ifndef MECalculatorDeep_H
#define MECalculatorDeep_H

#include "MMG402\libs\MExp\IMESymbolData.h"
#include "MMG402\libs\MExp\IMECalculator.h"
#include "MMG402\libs\MExp\MESymbol.h"

#include "MMG402\libs\GUtils\GUAutoPtr.h"

namespace MExp
{

  class MECalculatorDeep : public IMECalculator
  {
  public:
    virtual IMESymbolData* Evaluate() const
    {
      return EvaluateExp(_expression).ExtractPtr();
    }

  protected:
    virtual GUAutoPtr<IMESymbolData> EvaluateExp(const MESymbol& iSymb) const
    {
      if (iSymb.GetSymbolType() == MESymbolType_Constant) {
        return _table.GetContantParser()->Parse(iSymb.GetSymbolName());
      } else if (iSymb.GetSymbolType() == MESymbolType_Symbol) {
        return _table.GetData(iSymb.GetSymbolName())->CreateCopy();
      } else {
        const IMESymbolOperatorUnary* pUnaryOp = QueryUnaryOperator(iSymb);
        if (pUnaryOp != 0) {
          GUAutoPtr<IMESymbolData> pData = EvaluateExp(iSymb.GetArguments()[0]);
          pUnaryOp->Evaluate(pData, iSymb);
          return pData;
        }

        const IMESymbolOperatorBinary* pBinaryOp = QueryBinaryOperator(iSymb);
        if (pBinaryOp != 0) {
          GUAutoPtr<IMESymbolData> pLeft = EvaluateExp(iSymb.GetArguments()[0]);
          for (unsigned int i = 1; i < iSymb.GetArguments().size(); i++) {
            GUAutoPtr<IMESymbolData> pRight = EvaluateExp(iSymb.GetArguments()[i]);
            pLeft = pBinaryOp->Evaluate(pLeft, pRight, iSymb);
          }
          return pLeft;
        }
      }
      std::stringstream ss;
      ss << "Operator " << iSymb.GetSymbolType() << " not found in symbol table. Use validator before evaluation.";
      throw std::exception(ss.str().c_str());
    }
  };

} // end namespace MExp

#endif // MECalculatorDeep_H
