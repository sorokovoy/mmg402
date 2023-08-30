#ifndef IQWT3DDataSourceDllAdapter_H
#define IQWT3DDataSourceDllAdapter_H

#include "QWT3DDataSourceDll.h"

class IQWT3DDataSourceDllAdapter
{
public:
  virtual bool Initialize() = 0;
  virtual bool MakeIteration() = 0;

  virtual int DataSourcePluginGetDataGroupsCount() = 0;
  virtual bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf) = 0;
  virtual bool DataSourcePluginGetCoordName(int iGroupId, int iCoordID, char* oBuf) = 0;
  virtual IDataSourceType DataSourcePluginDataSourceType(int iGroupId) = 0;

  // IDataSource2DPointsGrid
  virtual bool DataSourcePlugin2DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, double& xmin, double& xmax, double& ymin, double& ymax) = 0;
  virtual bool DataSourcePlugin2DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, double& oValue) = 0;

  // IDataSource3DPointsGrid
  virtual bool DataSourcePlugin3DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, unsigned int& zCount, double& xmin, double& xmax, double& ymin, double& ymax, double& zmin, double& zmax) = 0;
  virtual bool DataSourcePlugin3DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, unsigned int zidx, double& oValue) = 0;
};

#endif
