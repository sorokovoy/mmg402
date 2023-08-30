#ifndef IMESymbolOperators_H
#define IMESymbolOperators_H

#include "MMG402\libs\GUtils\GUAutoPtr.h"

namespace MExp
{
  class IMESymbolData;
  class MESymbol;

  class IMESymbolConstantParser
  {
  public:
    // always should know how parce symbol "0" and "1"
    virtual IMESymbolData* Parse(const MESymbol& iSybmol) const = 0;
  };

  class IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MESymbol& iSybmol) const = 0;
    virtual int  GetType() const = 0;
  };

  class IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MESymbol& iSybmol) const = 0;
    virtual int  GetType() const = 0;
  };

} // end namespace MExp

#endif