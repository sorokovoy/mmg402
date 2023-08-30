
#include "QWT3DDataSourceDll.h"
#include "MMG402\libs\GUtils\GUAssert.h"

void EmulateDataSourceUsage()
{
  int groupId = 0;
  int iterationCount = 50;
  int v;
  v = DataSourcePluginGetInterfaceVersion();
  v = DataSourcePluginInitialize();

  DataSourcePluginIsDataMoveAvalable();
  DataSourcePluginIsDataBackAvalable();

  v = DataSourcePluginGetDataGroupsCount();
  char buf[255];
  DataSourcePluginGetDataGroupName(groupId, buf);

  DataSourcePluginGetCoordName(groupId, 0, buf);
  DataSourcePluginGetCoordName(groupId, 1, buf);
  DataSourcePluginGetCoordName(groupId, 2, buf);


  for (int itc = 0; itc < iterationCount; itc++) {
    switch (DataSourcePluginDataSourceType(groupId)) {
      case IDataSourceType2DPointsGrid: {
        unsigned int xc, yc;
        double xmin, ymin, xmax, ymax;
        DataSourcePlugin2DPointsGridGetRanges(groupId, xc, yc, xmin, xmax, ymin, ymax);
        for (unsigned int x = 0; x < xc; x++) {
          for (unsigned int y = 0; y < yc; y++) {
            double v;
            GUAssert(DataSourcePlugin2DPointsGridGetValue(groupId, x, y, v));
          }
        }
        break;
      }
      case IDataSourceType3DPointsGrid: {
        unsigned int xc, yc, zc;
        double xmin, ymin, zmin, xmax, ymax, zmax;
        DataSourcePlugin3DPointsGridGetRanges(groupId, xc, yc, zc, xmin, xmax, ymin, ymax, zmin, zmax);

        for (unsigned int x = 0; x < xc; x++) {
          for (unsigned int y = 0; y < yc; y++) {
            for (unsigned int z = 0; z < zc; z++) {
              double v;
              GUAssert(DataSourcePlugin3DPointsGridGetValue(groupId, x, y, z, v));
            }
          }
        }
        break;
      }
      case IDataSourceType3DPointsScattered: {
        int pointCount;
        DataSourcePlugin3DPointsScatteredGetPointCount(groupId, pointCount);
        DataSourcePlugin3DPointsScatteredResetPointIterator(groupId);
        float x, y, z, v;
        while (pointCount > 0) {
          GUAssert(DataSourcePlugin3DPointsScatteredGetNextPoint(0, x, y, z, v));
          pointCount--;
        }
        break;
      }
    }

    if (!DataSourcePluginIsDataMoveAvalable()) {
      break;
    } else {
      DataSourcePluginDataMove();
    }
  }
}

void main()
{
  EmulateDataSourceUsage();
}