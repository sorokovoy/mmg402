#ifndef MMG402TestingRegistryEquationOperatorsExplicitTemporal_H
#define MMG402TestingRegistryEquationOperatorsExplicitTemporal_H

#include "MMG402\libs\MExp\MESymbol.h"

#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD1.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitSpartialD2.h"
#include "MMG402\MMG402Solver\MMG402SolverTemporal\MMG402OperatorExplicitDt.h"

#include "MMG402TestingRegistryFucntion.h"
#include "MMG402\libs\GUtils\GUMathUtils.h"

using namespace MMG402Solver::Temporal;

#define CREATE_EXPLICIT_D1_D2_DT \
    MMG402OperatorExplicitSpartialD1<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* D1 = new MMG402OperatorExplicitSpartialD1<SpaceScalarType, SpaceDim, DataScalarType, DataDim>(); \
    MMG402OperatorExplicitSpartialD2<SpaceScalarType, SpaceDim, DataScalarType, DataDim>* D2 = new MMG402OperatorExplicitSpartialD2<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();      \
    MMG402OperatorExplicitDt<SpaceScalarType, SpaceDim, DataScalarType, DataDim>*   DT = new MMG402OperatorExplicitDt<SpaceScalarType, SpaceDim, DataScalarType, DataDim>();                    \

class MMG402TestingRegistryEquationOperatorsExplicitTemporal
{
public:
  static unsigned int GetCount()
  {
    return 4;
  }

  template <typename SpaceScalarType, int SpaceDim, typename DataScalarType, int DataDim>
  static MESymbol CreateOperator(unsigned int iID, const MESymbol& ipTargetSymbol, const MESymbol& ipRightPart)
  {
    std::vector<MESymbol> varCollection = MMG402TestingRegistryFucntion::GetInstance().GetVars();
    MESymbol res = ipRightPart;
    enum { SpaceDimAtCompile = SpaceDim };
    switch (iID) {
      case 0: {
        CREATE_EXPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i, true);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 1: {
        CREATE_EXPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D2)(ipTargetSymbol, i);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 2: {
        CREATE_EXPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i, true);
          res += (*D2)(ipTargetSymbol, i);
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      case 3: {
        CREATE_EXPLICIT_D1_D2_DT;
        for (int i = 0; i < SpaceDimAtCompile; i++) {
          res += (*D1)(ipTargetSymbol, i, true) * varCollection[i];
          res += (*D2)(ipTargetSymbol, i) * varCollection[ GUAbs(SpaceDimAtCompile - i) % varCollection.size()];
        }
        res += (*DT)(ipTargetSymbol);
        break;
      }
      default:
        {
          throw new std::exception("Not Implemented");
        }
    }
    return res;
  }

private:
  MMG402TestingRegistryEquationOperatorsExplicitTemporal() {};
  MMG402TestingRegistryEquationOperatorsExplicitTemporal(const MMG402TestingRegistryEquationOperatorsExplicitTemporal&) {};
};



#endif // MMG402TestingRegistryEquationOperatorsExplicitTemporal_H

