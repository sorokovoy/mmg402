#ifndef IQWT3DDataSourceDllAdapterMMG402_H
#define IQWT3DDataSourceDllAdapterMMG402_H

#include "MMG402.h"
#include "IQWT3DDataSourceDllAdapter.h"

template<int SpaceDim, int DataDim>
class IQWT3DDataSourceDllAdapterMMG402 : public IQWT3DDataSourceDllAdapter
{
public:
  enum { SpaceDimAtCompile = SpaceDim };

  virtual int DataSourcePluginGetDataGroupsCount() = 0;
  virtual bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf) = 0;

  virtual bool DataSourcePluginGetValueByIndex(typename const MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex& iIndex, int iGroupId, double& oValue) = 0;

  virtual bool MakeIteration()
  {
    _pInput->pSolver->MakeIteration();
    _pEvaluatedData = ((MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, SpaceDim, double, DataDim>*)_pInput->pSolver->GetOutput())->pData->Pop(0)->pData;
    return true;
  }

  virtual bool DataSourcePluginGetCoordName(int iGroupId, int iCoordID, char* oBuf)
  {
    switch (iCoordID) {
      case 0: {
        sprintf_s(oBuf, 255, "X");
        return true;
      }
      case 1: {
        sprintf_s(oBuf, 255, "Y");
        return true;
      }
      case 2: {
        sprintf_s(oBuf, 255, "Z");
        return true;
      }
    }
    return true;
  }

  virtual bool DataSourcePlugin3DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, unsigned int zidx, double& oValue)
  {
    if (SpaceDimAtCompile != 3) return false;
    if (_pEvaluatedData == 0) return false;

    MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex index;
    index[0] = xIdx;
    index[1] = yidx;
    index[2] = zidx;

    if (!_pInput->pGrid->IsHasValue(index)) return false;
    return DataSourcePluginGetValueByIndex(index, iGroupId, oValue);
  }

  virtual bool DataSourcePlugin2DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, double& oValue)
  {
    if (SpaceDimAtCompile != 2) return false;
    if (_pEvaluatedData == 0) return false;

    MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex index;
    index[0] = xIdx;
    index[1] = yidx;

    if (!_pInput->pGrid->IsHasValue(index)) return false;
    return DataSourcePluginGetValueByIndex(index, iGroupId, oValue);
  }


  virtual IDataSourceType DataSourcePluginDataSourceType(int iGroupId)
  {
    if (SpaceDimAtCompile == 2) {
      return IDataSourceType2DPointsGrid;
    }
    if (SpaceDimAtCompile == 3) {
      return IDataSourceType3DPointsGrid;
    }
    return IDataSourceTypeUndefined;
  }

  // IDataSource2DPointsGrid
  virtual bool DataSourcePlugin2DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, double& xmin, double& xmax, double& ymin, double& ymax)
  {
    if (SpaceDimAtCompile != 2) return false;

    MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex size = _pInput->pGrid->GetIndexer()->GetIndexMaxValue();
    const MMG402Domain::MMG402Rectangle<double, SpaceDim>& bb = _pInput->pGrid->GetDomain()->GetBoundingBox();

    xCount = size[0];
    yCount = size[1];

    xmin = bb.GetLowPoint()[0];
    ymin = bb.GetLowPoint()[1];

    xmax = bb.GetUpPoint()[0];
    ymax = bb.GetUpPoint()[1];

    return true;
  }


  // IDataSource3DPointsGrid
  virtual bool DataSourcePlugin3DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, unsigned int& zCount, double& xmin, double& xmax, double& ymin, double& ymax, double& zmin, double& zmax)
  {
    if (SpaceDimAtCompile != 3) return false;

    MMG402Grid::IMMG402GridIndexer<SpaceDim>::GridIndex size = _pInput->pGrid->GetIndexer()->GetIndexMaxValue();
    const MMG402Domain::MMG402Rectangle<double, SpaceDim>& bb = _pInput->pGrid->GetDomain()->GetBoundingBox();

    xCount = size[0];
    yCount = size[1];
    zCount = size[2];

    xmin = bb.GetLowPoint()[0];
    ymin = bb.GetLowPoint()[1];
    zmin = bb.GetLowPoint()[2];

    xmax = bb.GetUpPoint()[0];
    ymax = bb.GetUpPoint()[1];
    zmax = bb.GetUpPoint()[2];
    return true;
  }

protected:
  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, SpaceDim, double, DataDim>* _pInput;
  const MMG402NumericData::IMMG402NumData<double, SpaceDim, double, DataDim>* _pEvaluatedData;
};


#endif
