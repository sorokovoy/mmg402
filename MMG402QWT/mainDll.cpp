
#include "QWT3DDataSourceDll.h"
#include "QWT3DDataSourceDllAdapterMMG402SolverTest.h"
#include "QWT3DDataSourceDllAdapterMMG402CDREq.h"
#include "QWT3DDataSourceDllAdapterMMG402CDREq2.h"
#include "QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq.h"
#include "QWT3DDataSourceDllAdapterMMG402SimpleSolve.h"

int DataSourcePluginInitialize()
{
  //pCore = new QWT3DDataSourceDllAdapterMMG402SolverTest<2>();
  pCore = new QWT3DDataSourceDllAdapterMMG402CDREq<2>();
  //pCore = new QWT3DDataSourceDllAdapterMMG402CDREq2<2>();
 // pCore = new QWT3DDataSourceDllAdapterMMG402SimpleSolve<2>();
 // pCore = new QWT3DDataSourceDllAdapterMMG402CDRSmoluhovskiEq<3, 10>();
  pCore->Initialize();
  return 0;
}
