#ifndef MMG402SmoluhovskiDestructionOperator_H
#define MMG402SmoluhovskiDestructionOperator_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"

#include "MMG402\MMG402Func\IMMG402Func.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

using namespace MExp;

#define MMG402SolverOperatorRegistry_MMG402SmoluhovskiDestructionOperator 602

template <typename SpaceScalarType, typename DataScalarType, int DataDim>
class MMG402SmoluhovskiDestructionOperatorImplementation : public MMG402Solver::Temporal::IMMG402OperatorTemporalImplementation<SpaceScalarType, 1, DataScalarType, DataDim>
{
public:
  MMG402SmoluhovskiDestructionOperatorImplementation(const MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipGeneral)
    : IMMG402OperatorTemporalImplementation<SpaceScalarType, 1, DataScalarType, DataDim>(ipGeneral)
  {
     _requireTmpDataClone = true;
  }

  virtual bool Evaluate(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iWhere,
                        typename const MMG402Grid::IMMG402Grid<SpaceScalarType, 1>*       ipGrid,
                        typename const MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipPriviosData,
                        typename const MMG402Func::MMG402FuncStorage* ipFunc,
                        const MESymbol& iSybmol,
                        typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, 1, DataScalarType, DataDim>* ioTargetData,
                        typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, 1, DataScalarType, DataDim>* pTmpData) const
  {
    MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* _pKFunc = (MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>*)iSybmol.GetExtensionObject("KFunc");

    //MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType& result = ioTargetData->GetGridData()->GetData(iWhere);
    //result = MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType(0);

    ttmath::Big<12,152> result = 0;

    const MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>* prData = pTmpData->GetGridData();

    const MMG402Grid::IMMG402GridIndexer<1>* pIndexer = ipGrid->GetIndexer();

    unsigned int N = pIndexer->GetIteratorMaxValue();
    unsigned int i;
    pIndexer->IndexToIterator(iWhere, i);

    for (unsigned int j = 0; j < N - 1; j++) {
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex j_ind;
      pIndexer->IteratorToIndex(j, j_ind);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_i;
      ipGrid->IndexToCoord(iWhere, x_i);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_j;
      ipGrid->IndexToCoord(j_ind , x_j);

      SpaceScalarType toPositive, toNegative;
      ipGrid->GetGridStep(j_ind, 0, toPositive, toNegative);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> p;
      p[0] = x_i[0];
      p[1] = x_j[0];

    //  result += _pKFunc->GetValue(p) * toPositive * prData->GetData(j_ind);
    //  continue;

      MMG402Grid::IMMG402GridIndexer<1>::GridIndex j1_ind;
      pIndexer->IteratorToIndex(j + 1, j1_ind);
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_j1;
      ipGrid->IndexToCoord(j1_ind , x_j1);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> p1;
      p1[0] = x_i[0];
      p1[1] = x_j1[0];

      result += ( (_pKFunc->GetValue(p) * prData->GetData(j_ind) + _pKFunc->GetValue(p1) * prData->GetData(j1_ind)) * toPositive / 2.00)(0);
    }

    for (int i = 0; i < DataDim; i++) {
      if (result < 1.00E-007) result = 1.00E-007;
    }
    ioTargetData->GetGridData()->GetData(iWhere)(0) = result.ToDouble();
    return true;
  }

  virtual int GetType() const
  {
    return MMG402SolverOperatorRegistry_MMG402SmoluhovskiDestructionOperator;
  }

protected:
  MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* _pKFunc;
};

template <typename SpaceScalarType, typename DataScalarType, int DataDim>
class MMG402SmoluhovskiDestructionOperator : public MMG402Solver::Temporal::IMMG402OperatorTemporal<SpaceScalarType, 1, DataScalarType, DataDim>
{
public:
  virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipGeneral) const
  {
    return new MMG402SmoluhovskiDestructionOperatorImplementation<SpaceScalarType, DataScalarType, DataDim>(ipGeneral);
  }

  virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
  {
    return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
  }

  MESymbol operator () (const MESymbol& iSymb, MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* ipKFunc)
  {
    return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402SmoluhovskiDestructionOperator, "Int[j=0..N](K(xi, xj) n(xj))"), ipKFunc);
  }

protected:
  virtual MESymbol PrepareSymbol(MESymbol& iSymb, MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* ipKFunc)
  {
    MESymbol res =  MMG402Solver::Temporal::IMMG402OperatorTemporal<SpaceScalarType, 1, DataScalarType, DataDim>::PrepareSymbol(iSymb);
    res.AddExtensionObject(ipKFunc, "KFunc");
    return res;
  }
};


class MESymbolOperatorExplicitSmoluhovskiDestructionOperatorPrinter  : public IMESymbolPrinter
{
public:
  virtual bool Print(std::ostream& out, const MESymbol& iSymb)
  {
    switch (iSymb.GetSymbolType()) {
      case MMG402SolverOperatorRegistry_MMG402SmoluhovskiDestructionOperator :
        out <<  iSymb.GetSymbolName() << " n -> " << iSymb.GetArguments()[0];
        return true;
    }
    return false;
  }
};


REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402SmoluhovskiDestructionOperator, MMG402SolverOperatorRegistry_MMG402SmoluhovskiDestructionOperator);
REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSmoluhovskiDestructionOperatorPrinter);

#endif
