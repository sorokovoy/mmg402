#ifndef MMG402RegressionAnalyzerExpGrid_H
#define MMG402RegressionAnalyzerExpGrid_H

#include "MMG402\libs\GUtils\GUAutoPtr.h"
#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzer.h"

namespace MMG402BackwardSolver
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  struct MMG402RegressionAnalyzerExpGridOutput : MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim> {
    std::vector<GUAutoPtr<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>>> dependencyCoefs;
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402RegressionAnalyzerExpGrid : public MMG402BackwardSolver::MMG402RegressionAnalyzer<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402RegressionAnalyzerExpGrid()
    {
      _pMatrixSolver = 0;
    }

    virtual ~MMG402RegressionAnalyzerExpGrid()
    {
      delete _pMatrixSolver;
      std::list<typename MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>*>::iterator curr;
      for (curr = _matrixes.begin(); curr != _matrixes.end(); curr++) {
        DeleteMatrixGrid(*curr);
      }
    }

    virtual const MMG402RegressionAnalyzerExpGridOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetOutput() const
    {
      return _pOutput;
    }

    virtual void Init(MMG402RegressionAnalyzerInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
    {
      MMG402BackwardSolver::MMG402RegressionAnalyzer<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::Init(ipInput);

      _pOutput = new MMG402RegressionAnalyzerExpGridOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
      _pOutput->pData = _pInput->pData;
      _pOutput->pInput = _pInput;
      for (unsigned int i = 0; i < ipInput->dependencyVariables.size(); i++)
        _pOutput->dependencyCoefs.push_back(new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(_pInput->pGrid));

      _pMatrixSolver = new MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>();
    }

  protected:
    virtual bool ProcessData(std::vector<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>& iVariables, MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* iTargetExp)
    {
      // evaluate current average grid
      MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* pCurrentMatrixGrid = CreateMatrixGrid();
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pCurrentMatrix = pCurrentMatrixGrid->GetData(index)(0);
        for (unsigned int vi = 0; vi < iVariables.size(); vi++) {
          for (unsigned int vj = 0; vj < iVariables.size(); vj++) {
            (*pCurrentMatrix)(vi, vj) += iVariables[vi]->GetData(index) * iVariables[vj]->GetData(index);
          }
          (*pCurrentMatrix)(vi, iVariables.size()) += iVariables[vi]->GetData(index) * iTargetExp->GetData(index);
        }

      }
      _matrixes.push_back(pCurrentMatrixGrid);
      return EvaluateSolution();
    }

    virtual bool EvaluateSolution()
    {
      MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* pCurrentMatrixGrid = 0;
      std::list<typename MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>*>::iterator curr;
      if (_matrixes.size() == _pInput->averageLayerCount) {
        pCurrentMatrixGrid = _matrixes.front();
        _matrixes.pop_front();
        curr = _matrixes.begin();
      } else {
        pCurrentMatrixGrid = CreateMatrixGridCopy(_matrixes.front());
        curr = _matrixes.begin();
        curr ++;
      }

      // calculate sum
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      for (curr = curr; curr != _matrixes.end(); curr++) {
        for (unsigned int i = 0; i < n; i++) {
          MMG402GridIndex index = pIndexer->IteratorToIndex(i);
          (*(pCurrentMatrixGrid->GetData(index)(0))) += *((*curr)->GetData(index)(0));
        }
      }

      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> currentSolution = _pMatrixSolver->Solve(pCurrentMatrixGrid->GetData(index)(0));
        for (unsigned int v = 0; v < currentSolution.size(); v++) {
          _pOutput->dependencyCoefs[v]->GetData(index) = currentSolution[v];

        }
      }
      DeleteMatrixGrid(pCurrentMatrixGrid);
      return true;
    }

    MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* CreateMatrixGrid() const
    {
      MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* pRes = new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>(_pInput->pGrid);
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        pRes->GetData(index) = new MMG402Matrix::MMG402MatrixDensy<typename MMG402NumData>(_pInput->dependencyVariables.size(), _pInput->dependencyVariables.size() + 1);
      }
      return pRes;
    }

    MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* CreateMatrixGridCopy(const MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* ipCopyFrom) const
    {
      MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* pRes = new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>(_pInput->pGrid);
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        pRes->GetData(index) = ipCopyFrom->GetData(index)(0)->CreateCopy();
      }
      return pRes;
    }

    void DeleteMatrixGrid(const MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>* ipMatrixGrid)
    {
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        delete ipMatrixGrid->GetData(index)(0);
      }
      delete ipMatrixGrid;
    }

    std::list<typename MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, SpaceDim, typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*, 1>*> _matrixes;
    MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>* _pMatrixSolver;

    MMG402RegressionAnalyzerExpGridOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pOutput;
  };

}; // end namespace MMG402BackvardSolve
#endif // MMG402RegressionAnalyzerExpGrid_H
