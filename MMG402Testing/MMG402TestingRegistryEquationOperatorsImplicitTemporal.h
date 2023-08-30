#ifndef MMG402TestingRegistryEquationOperatorsImplicitTemporal_H
#define MMG402TestingRegistryEquationOperatorsImplicitTemporal_H

#include "MMG402\libs\MExp\MESymbol.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorImplicitDt.h"

#include "MMG402TestingRegistryFucntion.h"
#include "MMG402\libs\GUtils\GUMathUtils.h"

using namespace MMG402Solver::Temporal;

#define CREATE_IMPLICIT_D1_D2_DT \
    MMG402OperatorImplicitSpartialD1<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* D1 = new MMG402OperatorImplicitSpartialD1<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();        \
    MMG402OperatorImplicitSpartialD2<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* D2 = new MMG402OperatorImplicitSpartialD2<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();        \
    MMG402OperatorImplicitDt<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*   DT = new MMG402OperatorImplicitDt<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();          \

class MMG402TestingRegistryEquationOperatorsImplicitTemporal
{
public:
  static unsigned int GetCount()
  {
    return 6;
  }

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  static MESymbol CreateOperator(unsigned int iID, const MESymbol& ipTargetSymbol, const MESymbol& ipRightPart)
  {
    std::vector<MESymbol> varCollection = MMG402TestingRegistryFucntion::GetInstance().GetVars();
    MESymbol res = ipRightPart;
    enum { SpaceDimAtCompile = SpaceDim };
    switch (iID) {
      case 0: {
        CREATE_IMPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 1: {
        CREATE_IMPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D2)(ipTargetSymbol, i);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 2: {
        CREATE_IMPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i);
          res += (*D2)(ipTargetSymbol, i);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 3: {
        CREATE_IMPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i) * varCollection[i];
          res += (*D2)(ipTargetSymbol, i) * varCollection[ GUAbs(SpaceDimAtCompile - i) % varCollection.size()];
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 4: {
        CREATE_IMPLICIT_D1_D2_DT;
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 5: {
        CREATE_IMPLICIT_D1_D2_DT;
        res += (*DT)(ipTargetSymbol);
        res += (*D2)(ipTargetSymbol, 0);
        break;
      }
      default:{
        throw new std::exception("Not implemented");
      }
    }
    return res;
  }

private:
  MMG402TestingRegistryEquationOperatorsImplicitTemporal() {};
  MMG402TestingRegistryEquationOperatorsImplicitTemporal(const MMG402TestingRegistryEquationOperatorsImplicitTemporal&) {};
};



#endif // MMG402TestingRegistryEquationOperatorsImplicitTemporal_H

