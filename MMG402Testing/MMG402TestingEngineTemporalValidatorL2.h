#ifndef MMG402TestingEngineValidatorRegistry_L2_H
#define MMG402TestingEngineValidatorRegistry_L2_H

#include "MMG402\libs\GUtils\GUMathUtils.h"
#include "MMG402\libs\ObjRegistry.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402Grid\IMMG402Grid.h"

#include "IMMG402TestingEngineTemporalResultValidator.h"

using namespace MMG402NumericData;

#define MMG402TestingEngineValidatorRegistry_L2 20

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalValidatorL2 : public IMMG402TestingEngineTemporalResultValidator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual void Print(std::ostream& out) const
  {
    out << "Value " << _intergalValue;
  }

  virtual int  GetId() const
  {
    return MMG402TestingEngineValidatorRegistry_L2;
  }

  typename MMG402NumData GetIntegralValue() const
  {
    return _intergalValue;
  }

protected:
  virtual void OnInit()
  {
    _intergalValue = 0;
    ResetValues();
  }

  virtual bool OnLayerValidationStart()
  {
    return true;
  }

  virtual void OnLayerValidationFinish()
  {
    std::cout << (_currentAverage) * _currentDt << std::endl;
    _intergalValue += (_currentAverage) * _currentDt;
    ResetValues();
  }

  enum { DataDimAtCompile = DataDim };
  virtual void ProcessValuePair(typename const MMG402GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue)
  {
    SpaceScalarType stepP, stepN;
    _pOuput->pInput->pGrid->GetGridStep(iIndex, 0, stepP, stepN);
    MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x;
    _pOuput->pInput->pGrid->IndexToCoord(iIndex, x);

    for (int i = 0; i < DataDimAtCompile; i++) {
      //double err = stepP * GUPow((iCurrentValue(i) - iTargetValue(i)) / GUMax(iTargetValue(i), 100.00), 2) * GUPow(x(0), 2);
      double err = stepP * GUPow(iCurrentValue(i) - iTargetValue(i), 2);// * GUPow(x(0), 2);
      _currentAverage(i) += GUAbs(err);
    }
  }

  virtual bool AnalyzeResults()
  {
    return true;
  }

  void ResetValues()
  {
    _currentAverage = 0;
    _currentAveragePointCount = 0;
  }

  typename MMG402NumData _currentAverage;
  unsigned int _currentAveragePointCount;

  typename MMG402NumData _intergalValue;
};

REGISTER_DERIVED_ID(IMMG402TestingEngineTemporalResultPrinter, MMG402TestingEngineTemporalValidatorL2, MMG402TestingEngineValidatorRegistry_L2);

#endif
