#ifndef MMG402RegressionAnalyzerExpScalar_H
#define MMG402RegressionAnalyzerExpScalar_H

#include "MMG402\MMG402BackwardSolver\MMG402RegressionAnalyzer.h"

namespace MMG402BackwardSolver
{
  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  struct MMG402RegressionAnalyzerExpScalarOutput : MMG402Solver::Temporal::MMG402SolverOutputTemporal<SpaceScalarType, SpaceDim, DataScalarType, DataDim> {
    std::vector<MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim>> dependencyCoefs;
  };

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  class MMG402RegressionAnalyzerExpScalar : public MMG402BackwardSolver::MMG402RegressionAnalyzer<SpaceScalarType, SpaceDim, DataScalarType, DataDim>
  {
  public:
    MMG402RegressionAnalyzerExpScalar()
    {
      _pMatrixSolver = 0;
    }

    virtual ~MMG402RegressionAnalyzerExpScalar()
    {
      delete _pMatrixSolver;
    }

    virtual const MMG402RegressionAnalyzerExpScalarOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* GetOutput() const
    {
      return _pOutput;
    }

    virtual void Init(MMG402RegressionAnalyzerInput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* ipInput)
    {
      MMG402BackwardSolver::MMG402RegressionAnalyzer<SpaceScalarType, SpaceDim, DataScalarType, DataDim>::Init(ipInput);

      _pOutput = new MMG402RegressionAnalyzerExpScalarOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();
      _pOutput->pData = _pInput->pData;
      _pOutput->pInput = _pInput;
      for (unsigned int i = 0; i < ipInput->dependencyVariables.size(); i++)
        _pOutput->dependencyCoefs.push_back(MMG402NumData(0));
      _pMatrixSolver = new MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>();
    }

  protected:
    virtual bool ProcessData(std::vector<MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*>& iVariables, MMG402NumericData::IMMG402NumData<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* iTargetExp)
    {
      // evaluate current average matrix;
      MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pCurrentMatrix = new MMG402Matrix::MMG402MatrixDensy<typename MMG402NumData>(_pInput->dependencyVariables.size(), _pInput->dependencyVariables.size() + 1);
      const MMG402Grid::IMMG402GridIndexer<SpaceDim>* pIndexer = _pInput->pGridIterator;
      unsigned int n = pIndexer->GetIteratorMaxValue();
      unsigned int total = 0;
      for (unsigned int i = 0; i < n; i++) {
        MMG402GridIndex index = pIndexer->IteratorToIndex(i);
        if ((_pInput->pDataFocus != 0) && (!_pInput->pDataFocus->GetData(index)(0))) {
          continue;
        }
        total ++;

        for (unsigned int vi = 0; vi < iVariables.size(); vi++) {
          for (unsigned int vj = 0; vj < iVariables.size(); vj++) {
            (*pCurrentMatrix)(vi, vj) += iVariables[vi]->GetData(index) * iVariables[vj]->GetData(index);
          }
          (*pCurrentMatrix)(vi, iVariables.size()) += iVariables[vi]->GetData(index) * iTargetExp->GetData(index);
        }
      }
      GUAssert(total != 0);
      (*pCurrentMatrix) /= total;
      _matrixes.push_back(pCurrentMatrix);
      return EvaluateSolution();
    }

    virtual bool EvaluateSolution()
    {
      MMG402Matrix::IMMG402Matrix<typename MMG402NumData>* pCurrentMatrix = 0;
      std::list<typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*>::iterator curr;
      if (_matrixes.size() == _pInput->averageLayerCount) {
        pCurrentMatrix = _matrixes.front();
        _matrixes.pop_front();
        curr = _matrixes.begin();
      } else {
        pCurrentMatrix = _matrixes.front()->CreateCopy();
        curr = _matrixes.begin();
        curr ++;
      }

      for (curr = curr; curr != _matrixes.end(); curr++) {
        (*pCurrentMatrix) += *(*curr);
      }
      _pOutput->dependencyCoefs = _pMatrixSolver->Solve(pCurrentMatrix);
      delete pCurrentMatrix;
      return true;
    }

    std::list<typename MMG402Matrix::IMMG402Matrix<typename MMG402NumData>*> _matrixes;
    MMG402Matrix::MMG402MatrixSolver<DataScalarType, DataDim>* _pMatrixSolver;

    MMG402RegressionAnalyzerExpScalarOutput<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* _pOutput;
  };

}; // end namespace MMG402BackvardSolve
#endif // MMG402RegressionAnalyzerExpScalar_H
