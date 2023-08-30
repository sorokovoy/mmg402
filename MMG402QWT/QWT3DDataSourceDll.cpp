#include "QWT3DDataSourceDll.h"
#include "IQWT3DDataSourceDllAdapter.h"

IQWT3DDataSourceDllAdapter* pCore = 0;

int DataSourcePluginGetInterfaceVersion()
{
  return 2;
}

bool lastMoveResult = true;
// Data source controller insterface
bool DataSourcePluginIsDataMoveAvalable()
{
  return lastMoveResult;
}

bool DataSourcePluginDataMove()
{
  lastMoveResult = pCore->MakeIteration();
  return true;
}

bool DataSourcePluginIsDataBackAvalable()
{
  return false;
}

bool DataSourcePluginDataBack()
{
  return false;
}

// Data sources
int  DataSourcePluginGetDataGroupsCount()
{
  return pCore->DataSourcePluginGetDataGroupsCount();
}

bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf)
{
  return pCore->DataSourcePluginGetDataGroupName(iGroupId, oBuf);
}

bool DataSourcePluginGetCoordName(int iGroupId, int iCoordID, char* oBuf)
{
  return pCore->DataSourcePluginGetCoordName(iGroupId, iCoordID, oBuf);
}

IDataSourceType DataSourcePluginDataSourceType(int iGroupId)
{
  return pCore->DataSourcePluginDataSourceType(iGroupId);
}

// IDataSource2DPointsGrid
bool DataSourcePlugin2DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, double& xmin, double& xmax, double& ymin, double& ymax)
{
  return pCore->DataSourcePlugin2DPointsGridGetRanges(iGroupId, xCount, yCount, xmin, xmax, ymin, ymax);
}

bool DataSourcePlugin2DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, double& oValue)
{
  return pCore->DataSourcePlugin2DPointsGridGetValue(iGroupId, xIdx, yidx, oValue);
}

// IDataSource3DPointsGrid
bool DataSourcePlugin3DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, unsigned int& zCount, double& xmin, double& xmax, double& ymin, double& ymax, double& zmin, double& zmax)
{
  return pCore->DataSourcePlugin3DPointsGridGetRanges(iGroupId, xCount, yCount, zCount, xmin, xmax, ymin, ymax, zmin, zmax);
}

bool DataSourcePlugin3DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, unsigned int zidx, double& oValue)
{
  return pCore->DataSourcePlugin3DPointsGridGetValue(iGroupId, xIdx, yidx, zidx, oValue);
}

// DataSource3DPointsScattered
bool DataSourcePlugin3DPointsScatteredGetPointCount(int iGroupId, int& oCount)
{
  return false;
}

bool DataSourcePlugin3DPointsScatteredResetPointIterator(int iGroupId)
{
  return false;
}

bool DataSourcePlugin3DPointsScatteredGetNextPoint(int iGroupId, float& ox, float& oy, float& oz, float& oval)
{
  return false;
}
