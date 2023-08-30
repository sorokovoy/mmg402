
#ifndef DataSourceInterface_H
#define DataSourceInterface_H
#ifndef QDENSITYPLOT

class IQWT3DDataSourceDllAdapter;
extern IQWT3DDataSourceDllAdapter* pCore;

enum IDataSourceType {
  IDataSourceTypeUndefined = 0,
  IDataSourceType3DPointsScattered = 1,
  IDataSourceType3DPointsGrid = 2,
  IDataSourceType2DPointsGrid = 3
};

#define DataSourceFunc __declspec(dllexport)
extern "C"
{
  // Char buffer size should be more than 255
  DataSourceFunc int DataSourcePluginGetInterfaceVersion();
  DataSourceFunc int DataSourcePluginInitialize();

  // Data source controller insterface
  DataSourceFunc bool DataSourcePluginIsDataMoveAvalable();
  DataSourceFunc bool DataSourcePluginDataMove();

  DataSourceFunc bool DataSourcePluginIsDataBackAvalable();
  DataSourceFunc bool DataSourcePluginDataBack();

  // Data sources
  DataSourceFunc int  DataSourcePluginGetDataGroupsCount();
  DataSourceFunc bool DataSourcePluginGetDataGroupName(int iGroupId, char* oBuf);
  DataSourceFunc bool DataSourcePluginGetCoordName(int iGroupId, int iCoordID, char* oBuf);
  DataSourceFunc IDataSourceType DataSourcePluginDataSourceType(int iGroupId);

  // IDataSource2DPointsGrid
  DataSourceFunc bool DataSourcePlugin2DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, double& xmin, double& xmax, double& ymin, double& ymax);
  DataSourceFunc bool DataSourcePlugin2DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, double& oValue);

  // IDataSource3DPointsGrid
  DataSourceFunc bool DataSourcePlugin3DPointsGridGetRanges(int iGroupId, unsigned int& xCount, unsigned int& yCount, unsigned int& zCount, double& xmin, double& xmax, double& ymin, double& ymax, double& zmin, double& zmax);
  DataSourceFunc bool DataSourcePlugin3DPointsGridGetValue(int iGroupId, unsigned int xIdx, unsigned int yidx, unsigned int zidx, double& oValue);

  // DataSource3DPointsScattered
  DataSourceFunc bool DataSourcePlugin3DPointsScatteredGetPointCount(int iGroupId, int& oCount);
  DataSourceFunc bool DataSourcePlugin3DPointsScatteredResetPointIterator(int iGroupId);
  DataSourceFunc bool DataSourcePlugin3DPointsScatteredGetNextPoint(int iGroupId, float& ox, float& oy, float& oz, float& oval);
}

#endif
#endif

