#include "MMG402.h"

#include "MMG402SmoluhovskiDestructionOperator.h"
#include "MMG402SmoluhovskiProductionOperator.h"
#include "MMG402SmoluhovskiExactSolve.h"
#include "MMG402SmoluhovskiKore.h"
#include "MMG402SmoluhovskiExpGrid.h"

#include "MMG402TestingEngineTemporalValidatorErrorEstimate.h"
#include "MMG402TestingEngineTemporalValidatorL2.h"
#include "MMG402TestingEngineTemporalConvergenceTest.h"


#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzerExpScalar.h"
#include "MMG402\libs\GUtils\GURandom.h"

#include "MMG402IO\MMG402IOSingle1DCImage.h"

using namespace MExp;

void SmoluhovskiRunConvergenceTest(int iShemeId, MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pSolveFunc);
void SmoluhovskiRunCoreBackTest(MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, const std::vector<MMG402Func::IMMG402Func<double, 2, double, 1>*>& onBasis, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* ipInitialData, double iDmult);
void SmoluhovskiRunForvardTest(MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, std::string oFileName);
void SmoluhovskiRunRealDataBackTest();
void SmoluhovskiRunOperatorConvergenceTest(bool isProductionOperator, MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pSolveFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pOperatorSolveFuncFunc);
void SmoluhovskiRunCDROperatorConvergenceTest(int pointCount);

void main()
{
  SmoluhovskiRunCDROperatorConvergenceTest(200);
  return;

  SmoluhovskiRunCDROperatorConvergenceTest(10);
  SmoluhovskiRunCDROperatorConvergenceTest(20);
  SmoluhovskiRunCDROperatorConvergenceTest(40);
  SmoluhovskiRunCDROperatorConvergenceTest(100);
  SmoluhovskiRunCDROperatorConvergenceTest(1000);
  return;

  //SmoluhovskiRunRealDataBackTest();
 // SmoluhovskiRunForvardTest(0, "res2.bmp");
  //return;

  //if (1) {
   // SmoluhovskiRunForvardTest(0, "res.bmp");
   // return;
 // }

  /*
  int pointCount = 1000;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1.00E-010), MMG402Domain::MMG402DomainPoint<double, 1>(1.00E002)));
  MMG402Grid::MMG402SmoluhovskiExpGrid<double> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)), 10);
  for(int i = 0; i < pointCount; i++) {
    MMG402Domain::MMG402DomainPoint<double, 1> p;
    grid.IndexToCoord(i, p);
    std::cout << p  << " " << i << std::endl;
  }
  return;
  */

  //SmoluhovskiRunOperatorConvergenceTest(true, new SmoluhovskiKoreXmY  <double, double>(), new SmoluhovskiKoreXmYOperatorTestSolve1<double, double>(), new SmoluhovskiKoreXmYOperatorTestProduction1<double, double>());
  //SmoluhovskiRunOperatorConvergenceTest(false, new SmoluhovskiKoreXmY  <double, double>(), new SmoluhovskiKoreXmYOperatorTestSolve1<double, double>(), new SmoluhovskiKoreXmYOperatorTestDestruction1<double, double>());
 // return;

  //SmoluhovskiRunOperatorConvergenceTest(true, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1<double, double>(), new SmoluhovskiSolveKore1Production<double, double>());
  //SmoluhovskiRunOperatorConvergenceTest(false, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1<double, double>(), new SmoluhovskiSolveKore1Destruction<double, double>());

  

  
 // return;

  //SmoluhovskiRunConvergenceTest(3, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  //SmoluhovskiRunConvergenceTest(2, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  //return;
  
  SmoluhovskiRunConvergenceTest(0, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  SmoluhovskiRunConvergenceTest(0, new SmoluhovskiKoreXmY<double, double>(), new SmoluhovskiSolveKoreXmY<double, double>());
  SmoluhovskiRunConvergenceTest(0, new SmoluhovskiKoreXpY<double, double>(), new SmoluhovskiSolveKoreXpY<double, double>());

  
  SmoluhovskiRunConvergenceTest(2, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  SmoluhovskiRunConvergenceTest(2, new SmoluhovskiKoreXmY<double, double>(), new SmoluhovskiSolveKoreXmY<double, double>());
  SmoluhovskiRunConvergenceTest(2, new SmoluhovskiKoreXpY<double, double>(), new SmoluhovskiSolveKoreXpY<double, double>());

  SmoluhovskiRunConvergenceTest(1, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  SmoluhovskiRunConvergenceTest(1, new SmoluhovskiKoreXmY<double, double>(), new SmoluhovskiSolveKoreXmY<double, double>());
  SmoluhovskiRunConvergenceTest(1, new SmoluhovskiKoreXpY<double, double>(), new SmoluhovskiSolveKoreXpY<double, double>());

  SmoluhovskiRunConvergenceTest(3, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  SmoluhovskiRunConvergenceTest(3, new SmoluhovskiKoreXmY<double, double>(), new SmoluhovskiSolveKoreXmY<double, double>());
  SmoluhovskiRunConvergenceTest(3, new SmoluhovskiKoreXpY<double, double>(), new SmoluhovskiSolveKoreXpY<double, double>());

  SmoluhovskiRunConvergenceTest(4, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolveKore1  <double, double>());
  SmoluhovskiRunConvergenceTest(4, new SmoluhovskiKoreXmY<double, double>(), new SmoluhovskiSolveKoreXmY<double, double>());
  SmoluhovskiRunConvergenceTest(4, new SmoluhovskiKoreXpY<double, double>(), new SmoluhovskiSolveKoreXpY<double, double>());


  return;

  SmoluhovskiRunConvergenceTest(2, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolve3  <double, double>());
  SmoluhovskiRunConvergenceTest(3, new SmoluhovskiKore1  <double, double>(), new SmoluhovskiSolve3  <double, double>());
  return;

  if (1) {
    std::vector<MMG402Func::IMMG402Func<double, 2, double, 1>*> basis;
    basis.push_back(new SmoluhovskiKoreXmY<double, double>());
    basis.push_back(new SmoluhovskiKoreXpY<double, double>());
    //basis.push_back(new SmoluhovskiKoreExpXpY<double, double>());

    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-008);
    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-006);
    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-004);
    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-003);
    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-002);
    SmoluhovskiRunCoreBackTest(new SmoluhovskiKoreMixed<double, double>(), basis, new SmoluhovskiSolveKore1<double, double>(), 1.00E-001);
    return;
  }







  return;
}

void SmoluhovskiRunCoreBackTest(MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, const std::vector<MMG402Func::IMMG402Func<double, 2, double, 1>*>& onBasis, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* ipInitialData, double iDmult)
{
  int count =  100;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1.00E-008), MMG402Domain::MMG402DomainPoint<double, 1>(10)));
  MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(count)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MMG402SmoluhovskiDestructionOperator<double, double, 1> Dest;
  MMG402SmoluhovskiProductionOperator<double, double, 1>  Prod;

  MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1> Dt;

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  MESymbol eq = u * MMG402Expression::MMG402Exp(Dest(u, pCoreFunc) * dt) + 0.5 * Prod(u, pCoreFunc) * (1.00 - MMG402Expression::MMG402Exp(Dest(u, pCoreFunc)  * dt)) / Dest(u, pCoreFunc);
  ti.equationExpression = eq;

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();

  ipInitialData->SetTemporalValue(0);
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> initialData(&grid, ipInitialData);
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(initialData.CreateCopy(), 0));

  ti.pGrid = &grid;
  ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(count));

  ti.targetSymbol = u;
  ti.dt = 1.00E-005;

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1> solver;
  solver.Init(&ti);

  solver.MakeIteration();
  solver.MakeIteration();
  solver.MakeIteration();

  const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 1, double, 1>* pOut = solver.GetOutput();

  // add noise
  for (unsigned int layer = 0; layer < pOut->pData->GetCurrentStorageDeep(); layer++) {
    const MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>* lay = pOut->pData->Pop(pOut->pData->GetCurrentStorageDeep() - 1 - layer);
    // iterate on the grid
    const MMG402Grid::IMMG402Grid<double, 1>* pGrid = lay->pData->GetMasterGrid();
    const MMG402Grid::IMMG402GridIndexer<1>* indexer = pGrid->GetIndexer();
    for (unsigned int it = 0; it < indexer->GetIteratorMaxValue(); it++) {
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex curentIndex;
      GUAssertC(indexer->IteratorToIndex(it, curentIndex));
      lay->pData->GetData(curentIndex)[0] = GURand::N(0, lay->pData->GetData(curentIndex)[0] * iDmult) + lay->pData->GetData(curentIndex)[0];
    }
  }

  //MMG402TestingEngineTemporalValidatorErrorEstimateOnAllLayers<double, 1, double, 1> validator;
  //validator.PrepareData(pOut, *ipInitialData);
  //validator.Print(std::cout);


  MMG402BackwardSolver::MMG402RegressionAnalyzerInput<double, 1, double, 1> rinp;
  rinp.averageLayerCount = 1;

  for (unsigned int i = 0; i < onBasis.size(); i++) {
    rinp.dependencyVariables.push_back(0.5 * Prod(u, onBasis[i]) - u * Dest(u, onBasis[i]));
    //rinp.dependencyVariables.push_back(u * MMG402Expression::MMG402Exp(Dest(u, onBasis[i]) * dt) + 0.5 * Prod(u, onBasis[i]) * (1.00 - MMG402Expression::MMG402Exp(Dest(u, onBasis[i])  * dt)) / Dest(u, onBasis[i]));
  }

  rinp.equationExpression = (MESymbol(MMG402BackwardSolver::UT1) - u) / dt;
  rinp.dt = ti.dt;
  rinp.pData = solver.GetOutput()->pData;
  rinp.pFuncStorage = ti.pFuncStorage;
  rinp.pGrid = ti.pGrid;
  rinp.pGridIterator = ti.pGridIterator;
  rinp.targetSymbol = ti.targetSymbol;

  MMG402BackwardSolver::MMG402RegressionAnalyzerExpScalar<double, 1, double, 1> analyzer;
  analyzer.Init(&rinp);

  analyzer.MakeIteration();
  const MMG402BackwardSolver::MMG402RegressionAnalyzerExpScalarOutput<double, 1, double, 1>* pOut1 = analyzer.GetOutput();
  std::cout << iDmult << std::endl;
  std::cout << pOut1->dependencyCoefs;

  analyzer.MakeIteration();
  std::cout << pOut1->dependencyCoefs;

  analyzer.MakeIteration();
  std::cout << pOut1->dependencyCoefs;
  std::cout << std::endl;
}

void SmoluhovskiRunRealDataBackTest()
{
  int count = 375;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1 * 1.00E-006), MMG402Domain::MMG402DomainPoint<double, 1>(100 * 1.00E-006)));
  MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(count)));
  double dr; grid.GetGridStep(0, 0, dr, dr);

  MMG402IO::MMG402IOSingle1DCImage<double, double> imageIO("grad.bmp", 1.00E001, 1.00E002);
  MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>* pOut = imageIO.Read("data.bmp", &grid, 1.00E001);


  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1> D1;
  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1> D2;

  MMG402BackwardSolver::MMG402RegressionAnalyzerInput<double, 1, double, 1> rinp;
  rinp.averageLayerCount = pOut->GetCurrentStorageDeep();

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  MMG402SmoluhovskiDestructionOperator<double, double, 1> Dest;
  MMG402SmoluhovskiProductionOperator<double, double, 1>  Prod;

  MESymbol Ux("ux");
  MESymbol DUx("Dux");

  SmoluhovskiKoreKb<double, double>* pKb = new SmoluhovskiKoreKb<double, double>();
  rinp.dependencyVariables.push_back(0.5 * Prod(u, pKb) - u * Dest(u, pKb));
  //rinp.dependencyVariables.push_back(u * MMG402Expression::MMG402Exp(Dest(u, pKb) * dt) + 0.5 *  Prod(u, pKb) * (1.00 - MMG402Expression::MMG402Exp(Dest(u, pKb)  * dt)) / Dest(u, pKb));


  SmoluhovskiKoreKg<double, double>* pKg = new SmoluhovskiKoreKg<double, double>();
  rinp.dependencyVariables.push_back(0.5 * Prod(u, pKg) - u * Dest(u, pKg));
  //rinp.dependencyVariables.push_back(u * MMG402Expression::MMG402Exp(Dest(u, pKg) * dt) + 0.5 *  Prod(u, pKg) * (1.00 - MMG402Expression::MMG402Exp(Dest(u, pKg)  * dt)) / Dest(u, pKg));

  rinp.dependencyVariables.push_back(D1(Ux * u, 0, true));
  //rinp.dependencyVariables.push_back(D2(u, 0));

  rinp.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));

  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> UxFunc(&grid, new SmoluhovskiUFuncV2<double, double>());
  rinp.symbolTable.SetData(Ux.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(UxFunc.CreateCopy()));

  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> DUxFunc(&grid, new SmoluhovskiDiFunc<double, double>(dr));
  rinp.symbolTable.SetData(DUx.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(DUxFunc.CreateCopy()));


  rinp.equationExpression = (MESymbol(MMG402BackwardSolver::UT1) - u) / dt;
  rinp.pData = pOut;
  rinp.pGrid = &grid;
  rinp.pGridIterator = grid.GetIndexer();
  rinp.targetSymbol = u;

  MMG402BackwardSolver::MMG402RegressionAnalyzerExpScalar<double, 1, double, 1> analyzer;
  analyzer.Init(&rinp);


  for (unsigned int i = 0; i < pOut->GetCurrentStorageDeep(); i++) {
    analyzer.MakeIteration();
  }

  for (unsigned int i = 0; i < analyzer.GetOutput()->dependencyCoefs.size(); i++) {
    printf("%E \n", analyzer.GetOutput()->dependencyCoefs[i](0));
  }
}

void SmoluhovskiRunConvergenceTest(int iShemeId, MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pSolveFunc)
{
  int pointCount = 1000;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1.00E-20), MMG402Domain::MMG402DomainPoint<double, 1>(1.00E002)));
 // MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)));
  MMG402Grid::MMG402SmoluhovskiExpGrid<double> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)), 20);

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MMG402SmoluhovskiDestructionOperator<double, double, 1> Dest;
  MMG402SmoluhovskiProductionOperator<double, double, 1>  Prod;

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  ti.pGrid = &grid;
  ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount));

  ti.targetSymbol = u;

  MESymbol dest = Dest(u, pCoreFunc);
  MESymbol prod = Prod(u, pCoreFunc);
  ti.equationSubExpressions.push_back(dest);
  ti.equationSubExpressions.push_back(prod);
  switch (iShemeId) {
    case 0: {
      MESymbol psi_x0 = MMG402Expression::MMG402Exp(- dest * dt);
      ti.equationExpression = u * psi_x0 + psi_x0 * prod *  dt;
      ti.equationSubExpressions.push_back(psi_x0);
      printf("Simple cheme \n");
      break;
    }
    case 1: {
      ti.equationExpression = u * MMG402Expression::MMG402Exp(-dest * dt) + prod * (1.00 - MMG402Expression::MMG402Exp(-dest * dt)) / dest;
      printf("QSSA \n");
      break;
    }
    case 2: {
      MESymbol psi_x0 = MMG402Expression::MMG402Exp(- dest * dt);
      MESymbol uT = u * psi_x0 + psi_x0 * prod *  dt;
      MESymbol destT = Dest(uT, pCoreFunc);
      MESymbol prodT = Prod(uT, pCoreFunc);
      MESymbol psi_xT = MMG402Expression::MMG402Exp(- (destT + dest) * dt / 2);
      ti.equationExpression = u * psi_xT + (prod * psi_x0 + prodT * psi_xT) * dt / 2;
      ti.equationSubExpressions.push_back(psi_x0);
      ti.equationSubExpressions.push_back(uT);
      ti.equationSubExpressions.push_back(destT);
      ti.equationSubExpressions.push_back(prodT);
      ti.equationSubExpressions.push_back(psi_xT);
      printf("Simple cheme 2 Order \n");
      break;
    }
    case 3: {
      MESymbol psi_x0 = MMG402Expression::MMG402Exp(- dest * dt);
      MESymbol uT = u * psi_x0 + prod * (1.00 - psi_x0) / dest;
      MESymbol destT = Dest(uT, pCoreFunc);
      MESymbol prodT = Prod(uT, pCoreFunc);
      MESymbol psi_xT = MMG402Expression::MMG402Exp(- (destT + dest) * dt / 2);
      ti.equationExpression = u * psi_xT + (prod * psi_xT + prodT) * dt / 2;
      ti.equationSubExpressions.push_back(uT);
      ti.equationSubExpressions.push_back(destT);
      ti.equationSubExpressions.push_back(prodT);
      ti.equationSubExpressions.push_back(psi_xT);
      printf("QSSA 2 Order\n");
      break;
    }
    case 4: {
      MESymbol uTby2 = u + dt * (0.5 * prod - u * dest);

      ti.equationSubExpressions.push_back(uTby2);
      ti.equationExpression = u + dt * ((0.5 * prod - u * dest) + (0.5 * Prod(uTby2, pCoreFunc) - uTby2 * Dest(uTby2, pCoreFunc))) / 2;
      printf("Heun's method\n");
      break;
    }

  }

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> initialData(&grid, pSolveFunc);
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(initialData.CreateCopy(), 0));

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>* pSol = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>();
  pSol->Init(&ti);

  MMG402TestingEngineTemporalConvergenceTestInput<double, 1, double, 1> convergenceInput;
  convergenceInput.pSolverInput = &ti;
  convergenceInput.Tmin = 0;
  convergenceInput.Tmax = 6;
  convergenceInput.devisionBegin = 2;
  convergenceInput.devisionCount = 5;
  convergenceInput.pTargetFunc = pSolveFunc;
  convergenceInput.pValidator = new MMG402TestingEngineTemporalValidatorL2<double, 1, double, 1> ();
  //convergenceInput.pValidator = new MMG402TestingEngineTemporalValidatorErrorEstimateLastLayer<double, 1, double, 1> ();

  MMG402TestingEngineTemporalConvergenceTest<double, 1, double, 1>  test;
  test.RunTest(convergenceInput);
}

void SmoluhovskiRunOperatorConvergenceTest(bool isProductionOperator, MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pSolveFunc, MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pOperatorSolveFuncFunc)
{
  int pointCount = 1000;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(1.00E-080), MMG402Domain::MMG402DomainPoint<double, 1>(1.00E002)));
  //MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)));
  MMG402Grid::MMG402SmoluhovskiExpGrid<double> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)), 60);

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MMG402SmoluhovskiDestructionOperator<double, double, 1> Dest;
  MMG402SmoluhovskiProductionOperator<double, double, 1>  Prod;

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  ti.pGrid = &grid;
  ti.targetSymbol = u;

  if(isProductionOperator) {
    ti.equationExpression = Prod(u, pCoreFunc);
  } else {
    ti.equationExpression = Dest(u, pCoreFunc);
  }

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> initialData(&grid, pSolveFunc);
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(initialData.CreateCopy(), 0));

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>* pSol = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>();
  pSol->Init(&ti);

  MMG402TestingEngineTemporalConvergenceTestInput<double, 1, double, 1> convergenceInput;
  convergenceInput.pSolverInput = &ti;
  convergenceInput.Tmin = 0;
  convergenceInput.Tmax = 60;
  convergenceInput.devisionBegin = 1;
  convergenceInput.devisionCount = 1;
  convergenceInput.pTargetFunc = pOperatorSolveFuncFunc;
  convergenceInput.pValidator = new MMG402TestingEngineTemporalValidatorErrorEstimateLastLayer<double, 1, double, 1> ();


  MMG402TestingEngineTemporalConvergenceTest<double, 1, double, 1> test;
  test.RunTest(convergenceInput);
}

#include "MMG402OperatorCDR.h"

template <typename SpaceScalarType, typename DataScalarType>
class BurgersEqExactSolution : public MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>
{
public:
  BurgersEqExactSolution(MMG402Func::MMG402FuncStorageKey iKey, double iMu)
    : MMG402Func::IMMG402FuncTemporal<SpaceScalarType, 1, DataScalarType, 1>(iKey)
  {
    _mu = iMu;
  }

  virtual void SetTemporalValue(double iVal) 
  {
    _t = iVal;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>::EqNumDataType GetValue(typename const MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1>& iPoint) const
  {
    double x = iPoint[0];
    double zm = _t * (1 + sqrt(_t) * exp( GUSqr(x) / (4 * _mu * _t)));
    return x / zm;
  }
protected:
  double _t;
  double _mu;
};


void SmoluhovskiRunCDROperatorConvergenceTest(int pointCount)
{
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(MMG402Domain::MMG402DomainPoint<double, 1>(0.1), MMG402Domain::MMG402DomainPoint<double, 1>(1.00)));
  MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  ti.pGrid = &grid;
  ti.targetSymbol = u;

  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 1, double, 1>& D1 = *(new MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 1, double, 1>());
  MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 1, double, 1>& D2 = *(new MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 1, double, 1>());
  MMG402Solver::Temporal::MMG402OperatorImplicitDt<double, 1, double, 1>& DT = *(new MMG402Solver::Temporal::MMG402OperatorImplicitDt<double, 1, double, 1>());


  //MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1>& D1 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1>());
  //MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1>& D2 = *(new MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1>());
  //MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1>& DT = *(new MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1>());
  
  MESymbol UVal("U");
  double mu = 0.1;
  MMG402Solver::Temporal::MMG402OperatorCDR<double, double, 1, 1>& CDR = *(new MMG402Solver::Temporal::MMG402OperatorCDR<double, double, 1, 1>());
  ti.equationExpression = CDR(u);
  //ti.equationExpression = DT(u) + UVal * D1(u, 0, false) - mu * D2(u, 0);
  //ti.equationExpression = DT(u) + UVal * D1(u, 0) - mu * D2(u, 0);

  
  MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pBoundaryDataFunc = new BurgersEqExactSolution<double, double>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData), mu);
  pBoundaryDataFunc->SetTemporalValue(0.3);
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(pBoundaryDataFunc);

  MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pRigtPartFunc = new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(0), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f"));
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(pRigtPartFunc);

  MMG402Func::MMG402FuncConstant<double, 1, double, 1>* pMuFunc = new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(mu), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "Mu"));
  //MMG402Func::MMG402FuncConstant<double, 1, double, 1>* pUFunc = new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "U"));
  MMG402Func::IMMG402FuncTemporal<double, 1, double, 1>* pUFunc = new BurgersEqExactSolution<double, double>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "U"), mu);

  ti.pFuncStorage->SetFunc<double, 1, double, 1>(pMuFunc);
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(pUFunc);

  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> initialData(&grid, pBoundaryDataFunc);
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(initialData.CreateCopy(), 0));

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>* pSol = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>();
  pSol->Init(&ti);

  MMG402TestingEngineTemporalConvergenceTestInput<double, 1, double, 1> convergenceInput;
  convergenceInput.pSolverInput = &ti;
  convergenceInput.Tmin = 0.3;
  convergenceInput.Tmax = 2.0;
  convergenceInput.devisionBegin = 100;
  convergenceInput.devisionCount = 1;
  convergenceInput.pTargetFunc = pBoundaryDataFunc;
  convergenceInput.pValidator = new MMG402TestingEngineTemporalValidatorL2<double, 1, double, 1> ();
  //convergenceInput.pValidator = new MMG402TestingEngineTemporalValidatorErrorEstimateOnAllLayers<double, 1, double, 1> ();


  printf("Points %d \n", pointCount);
  MMG402TestingEngineTemporalConvergenceTest<double, 1, double, 1> test;
  test.RunTest(convergenceInput);
}

void SmoluhovskiRunForvardTest(MMG402Func::IMMG402Func<double, 2, double, 1>* pCoreFunc, std::string oFileName)
{
  double domainMin;
  double domainMax;

  double valMin;
  double valMax;

  double dt1;

  // common
  {
    domainMin = 1.00E-010;
    domainMax = 1.00E-007;

    valMin = 1.00E004;
    valMax = 1.00E007;

    dt1 = 5.00E004;
  }



  //viruses
  {
    domainMin = 0.001 * 1.00E-006;
    domainMax = 0.1 * 1.00E-006;

    valMin = 0.00E002;
    valMax = 1.00E002;

    dt1 = 5.00E004;
  }
  
  // pollen
  {
    domainMin = 1 * 1.00E-006;
    domainMax = 40 * 1.00E-006;

    valMin = 1.00E001;
    valMax = 1.00E003;

    dt1 = 5.00E004;
  }

  //bakteria
  {
    domainMin = 0.1 * 1.00E-006;
    domainMax = 10 * 1.00E-006;

    valMin = 1.00E002;
    valMax = 1.00E004;

    dt1 = 1.00E004;
  }

  valMin = valMin * MMG402SmoluhovskiGeneralCoreEq2::Volume;
  valMax = valMax * MMG402SmoluhovskiGeneralCoreEq2::Volume;


  int pointCount = 375;
  MMG402Domain::MMG402RectDomain<double, 1> domain(MMG402Domain::MMG402Rectangle<double, 1>(domainMin, domainMax));
  MMG402Grid::MMG402GridUniform<double, 1> grid(&domain, new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount)));

  MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 1, double, 1> ti;

  MMG402SmoluhovskiDestructionOperator<double, double, 1> Dest;
  MMG402SmoluhovskiProductionOperator<double, double, 1>  Prod;

  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 1, double, 1> D1;
  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 1, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 1, double, 1> DT;

  //MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD1<double, 1, double, 1> D1;
 // MMG402Solver::Temporal::MMG402OperatorImplicitSpartialD2<double, 1, double, 1> D2;
 // MMG402Solver::Temporal::MMG402OperatorImplicitDt<double, 1, double, 1> DT;

  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  ti.pGrid = &grid;
  ti.pGridIterator = new MMG402Grid::MMG402GridIndexerLinear<1>(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(pointCount));

  ti.targetSymbol = u;

  pCoreFunc = new SmoluhovskiKoreKgKb<double, double>(1, 1);

  MESymbol dest = Dest(u, pCoreFunc);
  MESymbol prod = Prod(u, pCoreFunc);
  ti.equationSubExpressions.push_back(dest);
  ti.equationSubExpressions.push_back(prod);

  // equalation expression
  MESymbol psi_x0 = MMG402Expression::MMG402Exp(-dest * dt);
 // ti.equationSubExpressions.push_back(psi_x0);
  //ti.equationExpression = u * psi_x0 + psi_x0 * 0.5 * prod *  dt; // + D1(u, 0) + D2(u, 0);
  //ti.equationExpression = u * psi_x0 + 0.5 * prod * (1.00 - psi_x0) / dest;
  MESymbol smolyhovski = u * psi_x0 + 0.5 * prod * (1.00 - psi_x0) / dest;
  ti.equationSubExpressions.push_back(smolyhovski);

  MESymbol Ux("ux");
  MESymbol DUx("Dux");
  MESymbol AlphaDS("AlphaDS");


  /*

  MESymbol convection = DT(smolyhovski) + 1.00E-010 * D1(Ux * smolyhovski, 0) + 1.00E-010 * DUx * D2(smolyhovski, 0); //  + D2(u, 0);
  ti.equationSubExpressions.push_back(convection);

  MESymbol raspad = DT(convection) - AlphaDS * convection; //  + D2(u, 0);
  ti.equationExpression = raspad;
  //ti.equationSubExpressions.push_back(raspad);
  //ti.equationExpression = raspad;
  */

  //MESymbol convection = DT(u) - 1.00E-011 * D1(Ux * u, 0) + 1.00E-011 * DUx * D2(u, 0); //  + D2(u, 0);
  MESymbol convection = DT(u) - D1(Ux * u, 0, false) + DUx * D2(u, 0); //  + D2(u, 0);
  ti.equationSubExpressions.push_back(convection);

  MESymbol raspad = DT(u) + AlphaDS * u; //  + D2(u, 0);
  ti.equationSubExpressions.push_back(raspad);

// ti.equationExpression = DT(smolyhovski) + 1.00E-013 * D2(smolyhovski, 0) - 2.00E-007 * D1(smolyhovski, 0); //  + D2(u, 0);
  ti.equationExpression = 0.33333333 * raspad + 0.33333333 * smolyhovski + 0.33333333 * convection;
  //ti.equationExpression = 0.5 * raspad + 0.5 * smolyhovski;
 // ti.equationExpression = smolyhovski;
  //ti.equationExpression = DT(u) - AlphaDS * u;


  ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 1, double, 1>();

  MMG402NumericData::IMMG402NumData<double, 1, double, 1>* pInitialData = new MMG402NumericData::MMG402NumDataDensy<double, 1, double, 1>(ti.pGrid);
  unsigned int max = ti.pGrid->GetIndexer()->GetIteratorMaxValue();

  FILE* f = fopen("2.txt", "w");
  for (unsigned int i = 0; i < max ; i++) {
    double val;

    if(((0.1 * max < (double) i)  &&  ((double) i < 0.3 * max))) {
      double x = (abs((double) i - 0.2 * max) * 10.0 / max);
      val = valMin + (valMax - valMin) * exp(-6 * x * x);
     // val = valMax;
    } else {
      val = valMin;
    }
    val = val;
    pInitialData->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(i)) = val;
    fprintf(f, "%f\n", val);
  }
  fclose(f);
  ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 1, double, 1>(pInitialData, 0));

  MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>* pSol = new MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 1, double, 1>();
  ti.pFuncStorage->SetFunc<double, 1, double, 1>(new MMG402Func::MMG402FuncConstant<double, 1, double, 1>(MMG402NumericData::IMMG402NumData<double, 1, double, 1>::EqNumDataType(valMin), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));

  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> UxFunc(&grid, new SmoluhovskiUFunc<double, double>());
  const MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1>& cUxFunc = UxFunc;
  ti.symbolTable.SetData(Ux.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(UxFunc.CreateCopy()));

  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> AlphaDSF(&grid, new SmoluhovskiAlphaDAlphaS<double, double>());
  ti.symbolTable.SetData(AlphaDS.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(AlphaDSF.CreateCopy()));

  double dr; grid.GetGridStep(0, 0, dr, dr);
  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> DUxFunc(&grid, new SmoluhovskiDiFunc<double, double>(dr));
  ti.symbolTable.SetData(DUx.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(UxFunc.CreateCopy()));


//  MMG402NumericData::MMG402NumDataFuncAdapter<double, 1, double, 1> DUxFunc(&grid, new MMG402FuncConstant<double, 1, double,1>(uxMaxVal * uxMaxVal / 2, MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "any")));
 // ti.symbolTable.SetData(DUx.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 1, double, 1>(DUxFunc.CreateCopy()));


  pSol->Init(&ti);

  ti.dt = dt1;

  int totalIterations = 300;
  ti.pData->SetStorageMaxDeep(totalIterations);
  for (int i = 0; i < totalIterations; i++) {
    pSol->MakeIteration();
    printf(" %d " , i);
  }


  unsigned int reqIndex = 0;
  MMG402Domain::MMG402DomainPoint<double, 1> p;
  double mind = 1.00E010;
  double targetD =  10.0E-006;
  for (int i = 0; i < pointCount; i++) {
    grid.IndexToCoord(i, p);

    if (abs(p(0) -  targetD) < mind) {
      mind = abs(p(0) - targetD);
      reqIndex = i;
    }
  }


  FILE* f1 = fopen("1.txt", "w");
  for (int i = 0; i < totalIterations; i++) {
    fprintf(f1, "%f \n", ti.pData->Pop(totalIterations - i - 1)->pData->GetData(reqIndex)(0));
  }
  fclose(f1);

  MMG402IO::MMG402IOSingle1DCImage<double, double> writer("grad.bmp", valMin, valMax);

  /*
  unsigned int totalTimeLayers = pSol->GetOutput()->pData->GetCurrentStorageDeep();
  unsigned int valuesPerLayer  = pSol->GetOutput()->pData->Pop(0)->pData->GetMasterGrid()->GetIndexer()->GetIteratorMaxValue();

  for (unsigned int t = 0; t < totalTimeLayers; t++) {
    MMG402NumericData::IMMG402NumData<double, 1, double, 1>* pCurrData = pSol->GetOutput()->pData->Pop(totalTimeLayers - 1 - t)->pData;
    for (unsigned int r = 0; r < valuesPerLayer; r++) {
      MMG402Domain::MMG402DomainPoint<double, 1> oPoint;
      grid.IndexToCoord(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(r), oPoint);
      pCurrData->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(r))(0) = pCurrData->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(r))(0) * oPoint(0);
    }
  }

  */

  writer.Write(pSol->GetOutput()->pData, oFileName.c_str());
}

