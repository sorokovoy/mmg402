#ifndef MMG402FuncDescription_H
#define MMG402FuncDescription_H

#include <string>
#include "MMG402\MMG402Func\MMG402FuncStorageKey.h"

namespace MMG402Func
{

  class MMG402FuncDescription
  {
  public:
    template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
    static MMG402FuncDescription ConstructInstance(const MMG402FuncStorageKey& iKey, int iFuncType)
    {
      MMG402FuncDescription instance(iKey);
      enum { SpaceDimAtCompile = SpaceDim };
      enum { DataDimAtCompile = DataDim };
      instance._dataScalarType = typeid(DataScalarType).name();
      instance._spaceScalarType = typeid(SpaceScalarType).name();

      instance._spaceDimAtCompile = SpaceDimAtCompile;
      instance._dataDimAtCompile = DataDimAtCompile;
      instance._funcType = iFuncType;
      return instance;
    }

    int GetFuncType() const
    {
      return _funcType;
    }

    std::string GetSpaceScalarType() const
    {
      return _spaceScalarType;
    }
    int GetSpaceDim() const
    {
      return _spaceDimAtCompile;
    }

    std::string GetDataScalarType() const
    {
      return _dataScalarType;
    }
    int GetDataDim() const
    {
      return _dataDimAtCompile;
    }

    const MMG402FuncStorageKey& GetKey() const
    {
      return _key;
    }

    bool operator < (const MMG402FuncDescription& iVal) const
    {
      if (iVal._key <  _key) return true;
      if (iVal._key == _key && iVal._spaceDimAtCompile <  _spaceDimAtCompile) return true;
      if (iVal._key == _key && iVal._spaceDimAtCompile == _spaceDimAtCompile && iVal._dataDimAtCompile <  _dataDimAtCompile) return true;
      if (iVal._key == _key && iVal._spaceDimAtCompile == _spaceDimAtCompile && iVal._dataDimAtCompile == _dataDimAtCompile && iVal._spaceScalarType <  _spaceScalarType) return true;
      if (iVal._key == _key && iVal._spaceDimAtCompile == _spaceDimAtCompile && iVal._dataDimAtCompile == _dataDimAtCompile && iVal._spaceScalarType == _spaceScalarType && iVal._dataScalarType <  _dataScalarType) return true;
      if (iVal._key == _key && iVal._spaceDimAtCompile == _spaceDimAtCompile && iVal._dataDimAtCompile == _dataDimAtCompile && iVal._spaceScalarType == _spaceScalarType && iVal._dataScalarType == _dataScalarType && iVal._funcType < _funcType) return true;
      return false;
    }

    bool operator == (const MMG402FuncDescription& iVal) const
    {
      return (iVal._key == _key && iVal._spaceDimAtCompile == _spaceDimAtCompile && iVal._dataDimAtCompile == _dataDimAtCompile && iVal._spaceScalarType == _spaceScalarType && iVal._dataScalarType == _dataScalarType && iVal._funcType == _funcType);
    }

  protected:
    const MMG402FuncStorageKey _key;
    int _spaceDimAtCompile;
    int _dataDimAtCompile;
    int _funcType;
    std::string _spaceScalarType;
    std::string _dataScalarType;

  private:
    MMG402FuncDescription( const MMG402FuncStorageKey& iKey) : _key(iKey) {};
  };

} // end namespace MMG402Func
#endif
