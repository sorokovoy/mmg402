
#ifndef MESymbolArifmetic_H
#define MESymbolArifmetic_H

#include "MMG402\libs\MExp\IMESymbolData.h"
#include "MMG402\libs\MExp\IMESymbolOperators.h"

#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolFunc.h"
#include "MMG402\libs\MExp\MESymbolTable.h"

#include "MMG402\libs\GUtils\GUStringUtils.h"

#include <math.h>

namespace MExp
{

  template<typename T>
  class MESymbolNumData : public IMESymbolData
  {
  public:
    MESymbolNumData(const T& iVal) : _data(iVal) {};

    virtual int GetType() const
    {
      return 1;
    }

    virtual IMESymbolData* CreateCopy() const
    {
      return new MESymbolNumData(*this);
    }

    virtual T& Value()
    {
      return _data;
    }

    virtual const T& Value() const
    {
      return _data;
    }

  protected:
    T _data;
  };

  template<typename T>
  class MEArifmeticConstantParser : public IMESymbolConstantParser
  {
  public:
    virtual IMESymbolData* Parse(const MExp::MESymbol& iName) const
    {
      return new MESymbolNumData<T>(GUParse<T>(iName.GetSymbolName()));
    }
  };

  template<typename T>
  class MESymbolOperatorUnaryInverse : public IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipData.GetPointer())->Value() = -((MESymbolNumData<T>*) ipData.GetPointer())->Value();
    }
    virtual int  GetType() const
    {
      return MESymbolType_UnaryInverce;
    }
  };

  template<typename T>
  class MESymbolOperatorUnarySin : public IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipData.GetPointer())->Value() = sin(((MESymbolNumData<T>*) ipData.GetPointer())->Value());
    }
    virtual int  GetType() const
    {
      return MESymbolType_FuncSymbolSin;
    }
  };

  template<typename T>
  class MESymbolOperatorUnaryCos : public IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipData.GetPointer())->Value() = cos(((MESymbolNumData<T>*) ipData.GetPointer())->Value());
    }
    virtual int  GetType() const
    {
      return MESymbolType_FuncSymbolCos;
    }
  };

  template<typename T>
  class MESymbolOperatorUnaryExp : public IMESymbolOperatorUnary
  {
  public:
    virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipData.GetPointer())->Value() = (T)exp(((MESymbolNumData<T>*) ipData.GetPointer())->Value());
    }
    virtual int  GetType() const
    {
      return MESymbolType_FuncSymbolExp;
    }
  };

  template<typename T>
  class MESymbolOperatorSum : public IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipLeft.GetPointer())->Value() = ((MESymbolNumData<T>*) ipLeft.GetPointer())->Value() + ((MESymbolNumData<T>*) ipRight.GetPointer())->Value();
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Sum;
    }
  };

  template<typename T>
  class MESymbolOperatorProd : public IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipLeft.GetPointer())->Value() = ((MESymbolNumData<T>*) ipLeft.GetPointer())->Value() * ((MESymbolNumData<T>*) ipRight.GetPointer())->Value();
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Prod;
    }
  };

  template<typename T>
  class MESymbolOperatorDevide : public IMESymbolOperatorBinary
  {
  public:
    virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
    {
      ((MESymbolNumData<T>*)ipLeft.GetPointer())->Value() = ((MESymbolNumData<T>*) ipLeft.GetPointer())->Value() / ((MESymbolNumData<T>*) ipRight.GetPointer())->Value();
      return ipLeft;
    }
    virtual int  GetType() const
    {
      return MESymbolType_Devide;
    }
  };

} // end MExp namespace

#endif // MESymbolArifmetic
