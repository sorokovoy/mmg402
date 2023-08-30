
#include "MMG402.h"
#include "MMG402Examples.h"

#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzerExpGrid.h"
#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzerExpScalar.h"

#include "MMG402\MMG402NumericData\IMMG402NumDataOperator.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialAverage.h"
#include "MMG402IO\MMG402IOGroup2DCImage.h"


void PrintGrid1(const MMG402NumericData::IMMG402NumData<double, 2, double, 1>* ipGrid)
{
  MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = ipGrid->GetMasterGrid()->GetIndexer()->GetIndexMaxValue();
  for (unsigned int i = 0; i < index(0); i++) {
    for (unsigned int j = 0; j < index(1); j++) {
      MMG402Grid::IMMG402GridIndexer<2>::GridIndex curr;
      curr(0) = i;
      curr(1) = j;
      printf(" %5.2E ", ipGrid->GetData(curr)(0));
    }
    printf("\n");
  }
  printf("\n");
}


void PrintGrid2(const MMG402NumericData::IMMG402NumData<double, 2, bool, 1>* ipGrid)
{
  MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = ipGrid->GetMasterGrid()->GetIndexer()->GetIndexMaxValue();
  for (unsigned int i = 0; i < index(0); i++) {
    for (unsigned int j = 0; j < index(1); j++) {
      MMG402Grid::IMMG402GridIndexer<2>::GridIndex curr;
      curr(0) = i;
      curr(1) = j;
      printf(" %d ", ipGrid->GetData(curr)(0));
    }
    printf("\n");
  }
  printf("\n");
}

template <typename SpaceScalarType, int SpaceDim>
class MMG402NumDataBool : public MMG402NumericData::IMMG402NumDataOperator<SpaceScalarType, SpaceDim, bool, 1>
{
protected:
  virtual int GetMaskDeepth() const
  {
    return 1;
  }


  virtual bool BeginPointProcess(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, bool, 1>* ipData, typename const MMG402GridIndex& iBaseIndex)
  {
    currVal = ipData->GetData(iBaseIndex)(0);
    if(currVal) return false;
    totalCounnt = 0;
    positiveCount = 0;
    negativeCount = 0;
    return true;
  }

  virtual void PointProcess(const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, bool, 1>* ipData, typename const MMG402GridIndex& iBaseIndex, typename const MMG402GridIndex& iNearIndex)
  {
    if (ipData->GetData(iNearIndex)(0)) {
      positiveCount ++;
    } else {
      negativeCount ++;
    }
    totalCounnt++;
  }

  virtual typename MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, bool, 1>::EqNumDataType FinishPointProcess()
  {
    MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, bool, 1>::EqNumDataType res;
    res(0) = currVal ? (positiveCount * 3 > totalCounnt) : (!(negativeCount * 3 > totalCounnt));
    return res;
  }

  bool currVal; 

  int totalCounnt;
  int positiveCount;
  int negativeCount;
};

template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
class MMG402NumericDataAverageCalculation : public MMG402NumericData::IMMG402NumDataOperator<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
{
protected:
  virtual int GetMaskDeepth() const
  {
    return 1;
  }

  virtual bool BeginPointProcess(typename const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex)
  {
    sum = MMG402NumData(0);
    count = 0;
    return true;
  }

  virtual void PointProcess(typename const MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipData, typename const MMG402GridIndex& iBaseIndex, typename const MMG402GridIndex& iNearIndex)
  {
    sum += ipData->GetData(iNearIndex);
    count++;
  }

  virtual typename MMG402NumData FinishPointProcess()
  {
    return sum / (DataScalarType)count;
  }

  typename MMG402NumData sum;
  int count;
};


class BonesFileNameParcer : public MMG402IO::IMMG402IOGroup2DCImageFileNameParcer
{
public:
  virtual double Parce(const char* ipFileName) const
  {
    int h, min;
    sscanf(ipFileName, "%2d%2d", &h, &min);
    return (double)(h * 60 * 60 + min * 60) / 1000;
  }
};

void MMG402BonesDemo()
{
  MMG402Domain::IMMG402Domain<double, 2>* domain = new MMG402Domain::MMG402RectDomain<double, 2>(MMG402Domain::MMG402Rectangle<double, 2>(MMG402Domain::MMG402DomainPoint<double, 2>(-5.0), MMG402Domain::MMG402DomainPoint<double, 2>(5.0)));
  MMG402Grid::IMMG402Grid<double, 2>* grid = new MMG402Grid::MMG402GridUniform<double, 2>(domain,  new MMG402Grid::MMG402GridIndexerLinear<2>(MMG402Grid::IMMG402GridIndexer<2>::GridIndex(256, 256)));

  MMG402IO::MMG402IOGroup2DCImage<double, double> d("D:\\Projects\\data\\bonesgrad.bmp", 0, 1);
  MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>* pOut = d.Read("D:\\Projects\\data\\bones", grid, new BonesFileNameParcer());


  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialAverage<double, 1, double, 1> f;

  // create and fill cell selector
  MMG402NumericData::IMMG402NumData<double, 2, bool, 1>* pCellSelcetor = new MMG402NumericData::MMG402NumDataDensy<double, 2, bool, 1>(grid);
  for (unsigned int i = 0; i < grid->GetIndexer()->GetIteratorMaxValue(); i++) {
    MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = grid->GetIndexer()->IteratorToIndex(i);
    bool val = pOut->Pop(pOut->GetCurrentStorageDeep() - 1)->pData->GetData(index)(0) > 0.1 ? false : true;
    pCellSelcetor->GetData(index)(0) = val;
  }
  //PrintGrid2(pCellSelcetor);

  MMG402NumDataBool<double, 2> domainProcessor;
  pCellSelcetor = domainProcessor.Process(pCellSelcetor);
  //PrintGrid2(pCellSelcetor);


  MMG402NumericDataAverageCalculation<double, 2, double, 1> avProcessor;

  // preprocess data
  for (unsigned int i = 0; i < pOut->GetStorageMaxDeep(); i++) {
    pOut->Pop(i)->pData = avProcessor.Process(pOut->Pop(i)->pData);
    for (unsigned int j = 0; j < grid->GetIndexer()->GetIteratorMaxValue(); j++) {
      MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = grid->GetIndexer()->IteratorToIndex(j);
      if (!pCellSelcetor->GetData(index)(0)) {
        pOut->Pop(i)->pData->GetData(index) = 1;
      }
    }
  }


  //PrintGrid2(pCellSelcetor);
  //PrintGrid1(pOut->Pop(1)->pData);
  //PrintGrid1(pOut->Pop(2)->pData);


  // define model
  MESymbol u("u");
  MESymbol dt(MMG402Solver::Temporal::Symbols::DeltaT);

  MMG402BackwardSolver::MMG402RegressionAnalyzerInput<double, 2, double, 1> rinp;
  rinp.averageLayerCount = 100;

  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 2, double, 1> D2;
  MMG402Solver::Temporal::MMG402OperatorExplicitSpartialAverage<double, 2, double, 1> AV;
  //rinp.dependencyVariables.push_back(D1(u, 0) + D1(u, 1));
  //rinp.dependencyVariables.push_back(D2(u, 0) + D2(u, 1));
  rinp.dependencyVariables.push_back(AV(u));

  MMG402Func::MMG402FuncStorage* pFuncStorage = new MMG402Func::MMG402FuncStorage();
  pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));

  rinp.equationExpression = (MESymbol(MMG402BackwardSolver::UT1) - u) / dt;
  rinp.pData = pOut;
  rinp.pFuncStorage = pFuncStorage;
  rinp.pGrid = grid;
  rinp.targetSymbol = u;
  rinp.pDataFocus = pCellSelcetor;

  GUAutoPtr<MMG402NumericData::IMMG402NumData<double, 2, double, 1>> muData;

  if (0) {
    rinp.pDataFocus = 0;
    MMG402BackwardSolver::MMG402RegressionAnalyzerExpGrid<double, 2, double, 1> analyzer;
    analyzer.Init(&rinp);

    analyzer.MakeIteration();
    const MMG402BackwardSolver::MMG402RegressionAnalyzerExpGridOutput<double, 2, double, 1>* pOut1 = analyzer.GetOutput();

    for (unsigned int i = 0; i < pOut->GetCurrentStorageDeep() - 2; i++) {
      analyzer.MakeIteration();
    }
    muData = avProcessor.Process(pOut1->dependencyCoefs[0]);
    PrintGrid1(pOut1->dependencyCoefs[0].GetPointer());
  } else {

    MMG402BackwardSolver::MMG402RegressionAnalyzerExpScalar<double, 2, double, 1> analyzer;
    analyzer.Init(&rinp);

    analyzer.MakeIteration();
    const MMG402BackwardSolver::MMG402RegressionAnalyzerExpScalarOutput<double, 2, double, 1>* pOut1 = analyzer.GetOutput();

    for (unsigned int i = 0; i < pOut->GetCurrentStorageDeep() - 2; i++) {
      analyzer.MakeIteration();
    }
    printf("Coef %f \n ", pOut1->dependencyCoefs[0]);

    muData = new MMG402NumericData::MMG402NumDataDensy<double, 2, double, 1>(grid);
    for (unsigned int i = 0; i < grid->GetIndexer()->GetIteratorMaxValue(); i++) {
      MMG402Grid::IMMG402GridIndexer<2>::GridIndex index = grid->GetIndexer()->IteratorToIndex(i);
      muData->GetData(index)(0) = (pCellSelcetor->GetData(index)(0)) ? (pOut1->dependencyCoefs[0](0)) : 0;
    }
    //PrintGrid1(muData);
  }

  // check as forvard task
  {
    printf("Calculate forvard \n");
    MMG402Solver::Temporal::MMG402SolverInputTemporal<double, 2, double, 1> ti;

    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD1<double, 2, double, 1> D1;
    MMG402Solver::Temporal::MMG402OperatorExplicitSpartialD2<double, 2, double, 1> D2;
    MMG402Solver::Temporal::MMG402OperatorExplicitDt<double, 2, double, 1> DT;
    MMG402Solver::Temporal::MMG402SolverTemporalUniversal<double, 2, double, 1> solver;

    MESymbol u("u");
    MESymbol mu("mu");

    //MESymbol eq = DT(u) + mu * (D2(u, 0) + D2(u, 1));
    MESymbol eq = DT(u) - mu * (AV(u));

    ti.pData = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<double, 2, double, 1>();
    ti.pData->Push(new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<double, 2, double, 1>(pOut->Pop( pOut->GetCurrentStorageDeep() - 1)->pData->CreateCopy(), 0));

    ti.pGrid = grid;
    ti.equationExpression = eq;
    ti.targetSymbol = u;
    ti.dt = 0.3;

    ti.symbolTable.SetData(mu.GetSymbolName(), new MMG402Expression::MMG402SymbolDataGrid<double, 2, double, 1>(muData.GetPointer()->CreateCopy()));

    ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(1), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_BoundaryData)));
    ti.pFuncStorage->SetFunc<double, 2, double, 1>(new MMG402Func::MMG402FuncConstant<double, 2, double, 1>(MMG402NumericData::IMMG402NumData<double, 2, double, 1>::EqNumDataType(), MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_RightAdditionalFunction, "f")));


    solver.Init(&ti);
    solver.GetOutput()->pData->SetStorageMaxDeep(120);


    // make iterations
    for (int i = 0; i < 40; i++) {
      solver.MakeIteration();
      //PrintGrid1(solver.GetOutput()->pData->Pop(0)->pData);
    }

    const MMG402Solver::Temporal::MMG402SolverOutputTemporal<double, 2, double, 1>* pOut = solver.GetOutput();
    d.Write(pOut->pData, "D:\\Projects\\data\\eval1");

    printf("Done \n");
  }
  //PrintGrid1(pOut1->dependencyCoefs[1]);
}
