#ifndef IMMG402NumData_H
#define IMMG402NumData_H

#include "MMG402\MMG402Grid\IMMG402Grid.h"

#include "MMG402\libs\GUtils\GUAssert.h"

#define MMG402NumData MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::EqNumDataType

namespace MMG402NumericData
{

#define ImplementIMMG402NumDataBinaryOperatorVector(OP)                                                                    \
    GUAssert(GetMasterGrid()->GetIndexer()->GetIndexMaxValue() == iVal.GetMasterGrid()->GetIndexer()->GetIndexMaxValue()); \
    GUAssert(GetDataDim()  == iVal.GetDataDim());                                                                          \
    GUAssert(GetSpaceDim() == iVal.GetSpaceDim());                                                                         \
    MMG402GridIndex index;                                                                                                 \
    const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = GetMasterGrid()->GetIndexer();                              \
    for(unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {                                                    \
      pIndexer->IteratorToIndex(i, index);                                                                                 \
      GetData(index) OP iVal.GetData(index);                                                                               \
    }                                                                                                                      \

#define ImplementIMMG402NumDataBinaryOperatorScalar(OP)                                                                    \
    MMG402GridIndex index;                                                                                                 \
    const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = GetMasterGrid()->GetIndexer();                              \
    for(unsigned int i = 0; i < pIndexer->GetIteratorMaxValue(); i++) {                                                    \
      pIndexer->IteratorToIndex(i, index);                                                                                 \
      GetData(index) OP iVal;                                                                                              \
    }                                                                                                                      \

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class IMMG402NumData
  {
  public:
    virtual ~IMMG402NumData() {};

    enum { DataDimAtCompile  = DataDim  };
    enum { SpaceDimAtCompile = SpaceDim };

    typedef MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> EqNumDataType;

    int GetSpaceDim() const
    {
      return SpaceDimAtCompile;
    }
    int GetDataDim()  const
    {
      return DataDimAtCompile;
    }

    virtual bool IsHasData(typename const MMG402GridIndex& iIndex) const = 0;

    virtual EqNumDataType& GetData(typename const MMG402GridIndex& iIndex) = 0;
    virtual const EqNumDataType& GetData(typename const MMG402GridIndex& iIndex) const = 0;

    virtual const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* GetMasterGrid() const
    {
      return _pGrid;
    }

    virtual IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* CreateCopy() const = 0;

    // vector operations
    void operator -= (const IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorVector( -= );
    }

    void operator += (const IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorVector( += );
    }

    void operator *= (const IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorVector( *= );
    }

    void operator /= (const IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorVector( /= );
    }

    // scalar operations
    void operator -= (const EqNumDataType& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorScalar( -= );
    }

    void operator += (const EqNumDataType& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorScalar( += );
    }

    void operator *= (const EqNumDataType& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorScalar( *= );
    }

    void operator /= (const EqNumDataType& iVal)
    {
      ImplementIMMG402NumDataBinaryOperatorScalar( /= );
    }

  protected:
    const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* _pGrid;

    IMMG402NumData(const MMG402Grid::IMMG402Grid<SpaceScalarType, SpaceDim>* ipMasterGrid) : _pGrid(ipMasterGrid) {};
    IMMG402NumData() {};

  private:
    IMMG402NumData(const IMMG402NumData&) {};
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  std::ostream& operator << (std::ostream& output, const std::vector<MMG402Domain::MMG402DomainPoint<SpaceScalarType, SpaceDim>>& iPoint)
  {
    for (unsigned int i = 0; i < iPoint.size(); i++) {
      output << "p[" << i << "] " << iPoint[i] << " ";
      if ( (i + 1) % 6 == 0) {
        output << std::endl;
      }
    }
    if (iPoint.size() % 6 != 0) {
      output << std::endl;
    }
    return output;
  }

} // end namespace MMG402NumericData
#endif
