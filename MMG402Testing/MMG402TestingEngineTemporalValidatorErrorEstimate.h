#ifndef MMG402TestingEngineTemporalValidatorErrorEstimate_H
#define MMG402TestingEngineTemporalValidatorErrorEstimate_H

#include "MMG402\libs\GUtils\GUMathUtils.h"
#include "MMG402\libs\ObjRegistry.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"
#include "MMG402\MMG402Grid\IMMG402Grid.h"

#include "IMMG402TestingEngineTemporalResultValidator.h"

using namespace MMG402NumericData;

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
struct MMG402TestingEngineTemporalValidatorErrorEstimateBaseData {
  typename MMG402NumData _maxError;
  typename MMG402NumData _avError;
  typename MMG402NumData _avValue;
  unsigned int _pointCount;
};

#define MMG402TestingEngineTemporalValidatorErrorEstimateBase_PrintSingleBufferSize 1024

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalValidatorErrorEstimateBase : public IMMG402TestingEngineTemporalResultValidator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual void Print(std::ostream& out) const = 0;
  virtual int  GetId() const = 0;

protected:
  virtual void OnInit()
  {
    ResetValues();
  }

  virtual void OnLayerValidationFinish()
  {}

  enum { DataDimAtCompile = DataDim };
  virtual void ProcessValuePair(typename const MMG402GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue)
  {
    for (int i = 0; i < DataDimAtCompile; i++) {
      //printf(" <%E %E> ", iCurrentValue(i) , iTargetValue(i));
      double err = iCurrentValue(i) - iTargetValue(i);
      _currentData._maxError(i) = GUMax(_currentData._maxError(i), GUAbs(err));

      _currentData._avError(i) = (_currentData._pointCount * _currentData._avError(i) + GUAbs(err));
      _currentData._avValue(i) = (_currentData._pointCount * _currentData._avValue(i) + GUAbs(iCurrentValue(i)));

      _currentData._pointCount++;
      _currentData._avError(i) = _currentData._avError(i) / _currentData._pointCount;
      _currentData._avValue(i) = _currentData._avValue(i) / _currentData._pointCount;
    }
  }

  virtual bool AnalyzeResults()
  {
    return true;
  }

  void ResetValues()
  {
    _currentData._pointCount = 0;
    _currentData._maxError = MMG402NumData(-1.00E10);
    _currentData._avError = MMG402NumData(0);
    _currentData._avValue = MMG402NumData(0);
  }

  void PrintSingle(const MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal, char* ioBuf) const
  {
    sprintf_s(ioBuf, MMG402TestingEngineTemporalValidatorErrorEstimateBase_PrintSingleBufferSize, "Max: %10s Av %10s RelMax %10s RelAv %10s", ToString(iVal._maxError, 2).c_str(), ToString(iVal._avError, 2).c_str(), ToString(iVal._maxError / iVal._avValue, 2).c_str(), ToString(iVal._avError / iVal._avValue, 2).c_str());
  }

  MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim> _currentData;
};


#define MMG402TestingEngineValidatorRegistry_ErrorEstimateLastLayer 10
template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalValidatorErrorEstimateLastLayer : public MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual void ProcessValuePair(typename const MMG402GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue)
  {
    MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::ProcessValuePair(iIndex, iPoint, iCurrentValue, iTargetValue);
  }

  virtual bool OnLayerValidationStart()
  {
    return _currentLayer == _totalLayers;
  }

  virtual void Print(std::ostream& out) const
  {
    out << "Last layer error estimates" << std::endl;
    char buf[MMG402TestingEngineTemporalValidatorErrorEstimateBase_PrintSingleBufferSize];
    PrintSingle(_currentData, buf);
    out << buf << std::endl;
  }

  virtual int GetId() const
  {
    return MMG402TestingEngineValidatorRegistry_ErrorEstimateLastLayer;
  }

  const MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>&  GetRawData() const
  {
    return _currentData;
  }
};
REGISTER_DERIVED_ID(IMMG402TestingEngineTemporalResultPrinter, MMG402TestingEngineTemporalValidatorErrorEstimateLastLayer, MMG402TestingEngineValidatorRegistry_ErrorEstimateLastLayer);

#define MMG402TestingEngineValidatorRegistry_ErrorEstimateFirstLayer 11
template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalValidatorErrorEstimateFirstLayer : public MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual void ProcessValuePair(typename const MMG402GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue)
  {
    MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::ProcessValuePair(iIndex, iPoint, iCurrentValue, iTargetValue);
  }

  virtual bool OnLayerValidationStart()
  {
    return _currentLayer == 0;
  }

  virtual void Print(std::ostream& out) const
  {
    out << "First layer error estimates" << std::endl;
    char buf[MMG402TestingEngineTemporalValidatorErrorEstimateBase_PrintSingleBufferSize];
    PrintSingle(_currentData, buf);
    out << buf << std::endl;
  }

  virtual int GetId() const
  {
    return MMG402TestingEngineValidatorRegistry_ErrorEstimateFirstLayer;
  }

  const MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>&  GetRawData() const
  {
    return _currentData;
  }
};
REGISTER_DERIVED_ID(IMMG402TestingEngineTemporalResultPrinter, MMG402TestingEngineTemporalValidatorErrorEstimateFirstLayer, MMG402TestingEngineValidatorRegistry_ErrorEstimateFirstLayer);


#include <vector>
#define MMG402TestingEngineValidatorRegistry_ErrorEstimateOnAllLayers 12
template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402TestingEngineTemporalValidatorErrorEstimateOnAllLayers : public MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
public:
  virtual bool OnLayerValidationStart()
  {
    ResetValues();
    return true;
  }

  virtual void OnLayerValidationFinish()
  {
    _allData.push_back(_currentData);
  }

  virtual void ProcessValuePair(typename const MMG402GridIndex& iIndex, typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>& iPoint, typename const MMG402NumData& iCurrentValue, typename const MMG402NumData& iTargetValue)
  {
    MMG402TestingEngineTemporalValidatorErrorEstimateBase<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::ProcessValuePair(iIndex, iPoint, iCurrentValue, iTargetValue);
  }

  virtual void Print(std::ostream& out) const
  {
    out << "All layers error estimate" << std::endl;
    for (unsigned int i = 0; i < _allData.size(); i++) {
      char buf[MMG402TestingEngineTemporalValidatorErrorEstimateBase_PrintSingleBufferSize];
      PrintSingle(_allData[i], buf);
      out << "(" <<  i << ") " << buf << std::endl;
    }
  }

  virtual int GetId() const
  {
    return MMG402TestingEngineValidatorRegistry_ErrorEstimateOnAllLayers;
  }

  const std::vector<MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>>&  GetRawData() const
  {
    return _allData;
  }

private:
  std::vector<MMG402TestingEngineTemporalValidatorErrorEstimateBaseData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>> _allData;
};
REGISTER_DERIVED_ID(IMMG402TestingEngineTemporalResultPrinter, MMG402TestingEngineTemporalValidatorErrorEstimateOnAllLayers, MMG402TestingEngineValidatorRegistry_ErrorEstimateOnAllLayers);

#endif
