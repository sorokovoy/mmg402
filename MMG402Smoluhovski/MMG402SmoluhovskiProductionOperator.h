#ifndef MMG402SmoluhovskiProductionOperator_H
#define MMG402SmoluhovskiProductionOperator_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporal.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402SolverTemporal.h"

#include "MMG402\MMG402NumericData\IMMG402NumData.h"


#include "MMG402\MMG402Func\IMMG402Func.h"
#include "MMG402\MMG402Grid\IMMG402GridIndexer.h"


#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"

#include "MMG402\libs\ttmath\ttmath.h"

using namespace MExp;

#define MMG402SolverOperatorRegistry_MMG402SmoluhovskiProductionOperator 601

template <typename SpaceScalarType, typename DataScalarType, int DataDim>
class MMG402SmoluhovskiProductionOperatorImplementation : public MMG402Solver::Temporal::IMMG402OperatorTemporalImplementation<SpaceScalarType, 1, DataScalarType, DataDim>
{
public:
  MMG402SmoluhovskiProductionOperatorImplementation(const MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipGeneral)
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

   // MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType& result = ioTargetData->GetGridData()->GetData(iWhere);
     //= MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType(0);


    ttmath::Big<12,152> result = 0;

    const MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>* prData = pTmpData->GetGridData();

    const MMG402Grid::IMMG402GridIndexer<1>* pIndexer = ipGrid->GetIndexer();

    unsigned int i;
    pIndexer->IndexToIterator(iWhere, i);

    for (unsigned int j = 0; (int)j < (int)i; j++) {
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex i_ind   = pIndexer->IteratorToIndex(i);
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex i_sub_j = pIndexer->IteratorToIndex(i - j);
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex j_ind   = pIndexer->IteratorToIndex(j);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_i_sub_j;
      ipGrid->IndexToCoord(i_sub_j, x_i_sub_j);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_j;
      ipGrid->IndexToCoord(j_ind, x_j);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_i;
      ipGrid->IndexToCoord(i_ind, x_i);

      SpaceScalarType toPositive, toNegative;
      ipGrid->GetGridStep(j_ind, 0, toPositive, toNegative);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_0;
      ipGrid->IndexToCoord(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(0), x_0);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> p;
      if(0) {
        DataScalarType qRkRi = pow(GUPow(x_i(0), 3) - GUPow(x_j(0), 3), 1.00 / 3);

        unsigned int qInd = (unsigned int) ((qRkRi - x_0(0)) / toPositive);
        if(qInd >= i) continue;

        MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_q;
        ipGrid->IndexToCoord(pIndexer->IteratorToIndex(qInd), x_q);

        //DataScalarType omega = pow(GUPow(x_i_sub_j[0], 3) + GUPow(x_i[0], 3), 2.00/3) / (GUPow(x_i_sub_j[0], 2));
        DataScalarType omega =  (GUPow(x_i[0], 2)) / pow(GUPow(x_i[0], 3) + GUPow(x_j[0], 3), 2.00/3);

        p[0] = x_q[0];
        p[1] = x_j[0];
        result += (_pKFunc->GetValue(p) * prData->GetData(pIndexer->IteratorToIndex(qInd)) * prData->GetData(j_ind) * toPositive * omega)(0);
      } else {
        //p[0] = x_i_sub_j[0];
        p[0] = x_i[0] - x_j[0];
        p[1] = x_j[0];


        MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> i_sub_j_xVal;
        ipGrid->IndexToCoord(i_sub_j, i_sub_j_xVal);
        bool sign = i_sub_j_xVal[0] > p[0];


        while (true) {
          i_sub_j[0] -= sign ? 1 : -1;
          if(!ipGrid->IndexToCoord(i_sub_j, i_sub_j_xVal)) {
            i_sub_j[0] += sign ? 1 : -1;
            break;
          }
          bool sign1 = i_sub_j_xVal[0] > p[0];
          if(sign1 != sign) {
            break;
          }
        }


        //result += _pKFunc->GetValue(p) * prData->GetData(bestIdx) * prData->GetData(j_ind) * toPositive;
        result += (_pKFunc->GetValue(p) * prData->GetData(i_sub_j) * prData->GetData(j_ind) * toPositive)(0);
      }
      continue;

      MMG402Grid::IMMG402GridIndexer<1>::GridIndex i_sub_j1 = pIndexer->IteratorToIndex(i - j - 1);
      MMG402Grid::IMMG402GridIndexer<1>::GridIndex j1_ind   = pIndexer->IteratorToIndex(j + 1);
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_i_sub_j1;
      ipGrid->IndexToCoord(i_sub_j1, x_i_sub_j1);

      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> x_j1;
      ipGrid->IndexToCoord(j1_ind, x_j1);
      MMG402Domain::MMG402DomainPoint<SpaceScalarType, 2> p1;
      p[0] = x_i_sub_j1[0];
      p[1] = x_j1[0];

      result += ((_pKFunc->GetValue(p) * prData->GetData(i_sub_j) * prData->GetData(j_ind) + _pKFunc->GetValue(p1) * prData->GetData(i_sub_j1) * prData->GetData(j1_ind)) * toPositive / 2.00)(0);
    }
    result = result / 2;
    ioTargetData->GetGridData()->GetData(iWhere)(0) = result.ToDouble();
    //MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType(0) = 

    return true;
  }

  virtual int GetType() const
  {
    return MMG402SolverOperatorRegistry_MMG402SmoluhovskiProductionOperator;
  }

protected:
  MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* _pKFunc;
};

template <typename SpaceScalarType, typename DataScalarType, int DataDim>
class MMG402SmoluhovskiProductionOperator : public MMG402Solver::Temporal::IMMG402OperatorTemporal<SpaceScalarType, 1, DataScalarType, DataDim>
{
public:
  virtual MExp::IMESymbolOperatorUnary* CreateImplementation(MMG402Solver::Temporal::MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipGeneral) const
  {
    return new MMG402SmoluhovskiProductionOperatorImplementation<SpaceScalarType, DataScalarType, DataDim>(ipGeneral);
  }

  virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
  {
    return MMG402Expression::MMG402OperatorSchemeType_ExplicitTemporal;
  }

  MESymbol operator () (const MESymbol& iSymb, MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* ipKFunc)
  {
    return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402SmoluhovskiProductionOperator, "Int[j=0..i](K(xi - xj, xj) n(xi-xj) n(xj))"), ipKFunc);
  }

protected:
  virtual MESymbol PrepareSymbol(MESymbol& iSymb, MMG402Func::IMMG402Func<SpaceScalarType, 2, DataScalarType, DataDim>* ipKFunc)
  {
    MESymbol res =  MMG402Solver::Temporal::IMMG402OperatorTemporal<SpaceScalarType, 1, DataScalarType, DataDim>::PrepareSymbol(iSymb);
    res.AddExtensionObject(ipKFunc, "KFunc");
    return res;
  }
};


class MESymbolOperatorExplicitSmoluhovskiProductionOperatorPrinter  : public IMESymbolPrinter
{
public:
  virtual bool Print(std::ostream& out, const MESymbol& iSymb)
  {
    switch (iSymb.GetSymbolType()) {
      case MMG402SolverOperatorRegistry_MMG402SmoluhovskiProductionOperator :
        out <<  iSymb.GetSymbolName() << " n -> " << iSymb.GetArguments()[0];
        return true;
    }
    return false;
  }
};


REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402SmoluhovskiProductionOperator, MMG402SolverOperatorRegistry_MMG402SmoluhovskiProductionOperator);
REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorExplicitSmoluhovskiProductionOperatorPrinter);

#endif
