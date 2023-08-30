#ifndef MMG402TestingOperatorSymbolicArifmetic_H
#define MMG402TestingOperatorSymbolicArifmetic_H

#include "MMG402\libs\MExp\IMESymbolOperators.h"
#include "MMG402\libs\MExp\MESymbolFunc.h"

#include "MMG402TestingOperatorSymbolicData.h"
#include "MMG402\libs\GUtils\GUAssert.h"

class MMG402TestingOperatorSymbolicConstant : public IMESymbolConstantParser
{
public:
  virtual IMESymbolData* Parse(const MExp::MESymbol& iName) const
  {
    return new MMG402TestingOperatorSymbolicData(MESymbol(iName.GetSymbolName(), true));
  }
};

#define METestingOperatorUnaryCheck \
    GUAssert(ipData->GetType() == MMG402TestingOperatorSymbolicDataTypeID);                                         \
    GUAssertT((dynamic_cast<MMG402TestingOperatorSymbolicData*>(ipData.GetPointer())), "Invalid symbol data type"); \

#define METestingOperatorBinaryCheck \
    GUAssert(ipLeft->GetType() == MMG402TestingOperatorSymbolicDataTypeID);                                         \
    GUAssertT((dynamic_cast<MMG402TestingOperatorSymbolicData*>(ipLeft.GetPointer())), "Invalid symbol data type"); \
    GUAssert(ipRight->GetType() == MMG402TestingOperatorSymbolicDataTypeID);                                        \
    GUAssertT((dynamic_cast<MMG402TestingOperatorSymbolicData*>(ipRight.GetPointer())), "Invalid symbol data type");\

class MMG402TestingOperatorUnaryInverse : public IMESymbolOperatorUnary
{
public:
  virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
  {
    METestingOperatorUnaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData() = -((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData();
  }
  virtual int  GetType() const
  {
    return MESymbolType_UnaryInverce;
  }
};

class MMG402TestingOperatorUnarySin : public IMESymbolOperatorUnary
{
public:
  virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
  {
    METestingOperatorUnaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData() = MESin(((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData());
  }
  virtual int  GetType() const
  {
    return MESymbolType_FuncSymbolSin;
  }
};

class MMG402TestingOperatorUnaryCos : public IMESymbolOperatorUnary
{
public:
  virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
  {
    METestingOperatorUnaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData() = MECos(((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData());
  }
  virtual int  GetType() const
  {
    return MESymbolType_FuncSymbolCos;
  }
};

class MMG402TestingOperatorUnaryExp : public IMESymbolOperatorUnary
{
public:
  virtual void Evaluate(GUAutoPtr<IMESymbolData>& ipData, const MExp::MESymbol&) const
  {
    METestingOperatorUnaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData() = MEExp(((MMG402TestingOperatorSymbolicData*)ipData.GetPointer())->GetData());
  }
  virtual int  GetType() const
  {
    return MESymbolType_FuncSymbolExp;
  }
};

class MMG402TestingOperatorSum : public IMESymbolOperatorBinary
{
public:
  virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
  {
    METestingOperatorBinaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipLeft.GetPointer())->GetData() = ((MMG402TestingOperatorSymbolicData*) ipLeft.GetPointer())->GetData() + ((MMG402TestingOperatorSymbolicData*) ipRight.GetPointer())->GetData();
    return ipLeft;
  }
  virtual int  GetType() const
  {
    return MESymbolType_Sum;
  }
};

class MMG402TestingOperatorProd : public IMESymbolOperatorBinary
{
public:
  virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
  {
    METestingOperatorBinaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipLeft.GetPointer())->GetData() = ((MMG402TestingOperatorSymbolicData*) ipLeft.GetPointer())->GetData() * ((MMG402TestingOperatorSymbolicData*) ipRight.GetPointer())->GetData();
    return ipLeft;
  }
  virtual int  GetType() const
  {
    return MESymbolType_Prod;
  }
};

class MMG402TestingOperatorDevide : public IMESymbolOperatorBinary
{
public:
  virtual GUAutoPtr<IMESymbolData> Evaluate(GUAutoPtr<IMESymbolData>& ipLeft, GUAutoPtr<IMESymbolData>& ipRight, const MExp::MESymbol&) const
  {
    METestingOperatorBinaryCheck;
    ((MMG402TestingOperatorSymbolicData*)ipLeft.GetPointer())->GetData() = ((MMG402TestingOperatorSymbolicData*) ipLeft.GetPointer())->GetData() / ((MMG402TestingOperatorSymbolicData*) ipRight.GetPointer())->GetData();
    return ipLeft;
  }
  virtual int  GetType() const
  {
    return MESymbolType_Devide;
  }
};

#endif
