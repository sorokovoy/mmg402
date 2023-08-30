#ifndef MMG402CDROperator_H
#define MMG402CDROperator_H

#include "MMG402\MMG402Solver\MMG402SolverTemporal\IMMG402OperatorTemporalUniversal.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\libs\MExp\IMESymbolPrinter.h"
#include "MMG402\libs\GUtils\GUMathUtils.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSparce.h"

using namespace MExp;

namespace MMG402Solver
{
  namespace Temporal
  {


    #define MMG402SolverOperatorRegistry_MMG402OperatorCDR 549

    /*
    #define E 2.7182818284590452353602874713526624977

    template <typename T>
    T Power(T iVal, double deg)
    {
      if(GUAbs(iVal - E) < 1.00E-005) {
        return exp(deg);
      }
      double o = pow(iVal, deg);
      return o;
    }
    */


    template <typename SpaceScalarType, typename DataScalarType, int DataDim, int OriginalSpaceDim>
    class MMG402OperatorCDRImplementation : public IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, 1, DataScalarType, DataDim>
    {
    public:
      MMG402OperatorCDRImplementation(const MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipMasterInput)
        : IMMG402OperatorTemporalUniversalImplementation<SpaceScalarType, 1, DataScalarType, DataDim>(ipMasterInput)
      {
        _requireTmpDataClone = false;
      }


      virtual bool Evaluate(typename const MMG402Grid::IMMG402GridIndexer<1>::GridIndex& iWhere,
                            typename const MMG402Grid::IMMG402Grid<SpaceScalarType, 1>*       ipGrid,
                            typename const MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipPriviosData,
                            typename const MMG402Func::MMG402FuncStorage* ipFunc,
                            const MExp::MESymbol& iSybmol,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, 1, DataScalarType, DataDim>* ioTargetData,
                            typename MMG402Expression::MMG402SymbolDataGrid<SpaceScalarType, 1, DataScalarType, DataDim>* pTmpData) const
      {
        MMG402Matrix::IMMG402Matrix<typename MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, DataDim>::EqNumDataType>* pMatrixData = GetMatrixData(ioTargetData);
        const MMG402Grid::IMMG402GridIndexer<1>* pGridIndexer = _pInput->pGridIterator;
        unsigned int n = pGridIndexer->GetIteratorMaxValue();

        SpaceScalarType stepPos, stepNeg;
        ipGrid->GetGridStep(iWhere, 0, stepPos, stepNeg);

        unsigned int currentRow;
        GUAssert(pGridIndexer->IndexToIterator(iWhere, currentRow));

        MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> currX;
        ipGrid->IndexToCoord(iWhere, currX);

        DataScalarType muVal = _pMuFunc->GetValue(currX)(0);
        DataScalarType muPlus12 = _pMuFunc->GetValue(currX(0) + stepPos/2)(0);
        DataScalarType muMinus12 = _pMuFunc->GetValue(currX(0) - stepNeg/2)(0);

        DataScalarType uVal = _pUFunc->GetValue(currX)(_uComponent);
        DataScalarType uPlus12 = _pUFunc->GetValue(currX(0) + stepPos/2)(_uComponent);
        DataScalarType uMinus12 = _pUFunc->GetValue(currX(0) - stepNeg/2)(_uComponent);


        DataScalarType gamma = 1.00; // OriginalSpaceDim

        DataScalarType d = 0;
        DataScalarType dVave = d + gamma / _deltaTVal;

        DataScalarType fim1 = 0;
        DataScalarType fi = 0;
        DataScalarType fip1 = 0;

        MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> fVaveim1 = fim1 + pTmpData->GetGridData()->GetData(iWhere) * (gamma / _deltaTVal);
        MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> fVaveip1 = fip1 + pTmpData->GetGridData()->GetData(iWhere) * (gamma / _deltaTVal);
        MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> fVavei = fi + pTmpData->GetGridData()->GetData(iWhere) * (gamma / _deltaTVal);
        if(iWhere(0) == 0) {
          MMG402Grid::IMMG402GridIndexer<1>::GridIndex ip1 = iWhere;
          ip1(0)++;
          fVaveip1 = fip1 + pTmpData->GetGridData()->GetData(ip1) * (gamma / _deltaTVal);

          MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> boundx = currX;
          boundx(0) -= stepNeg;
          MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> boud = ipFunc->GetFunc<SpaceScalarType, 1, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(boundx);
          fVaveim1 = fim1 + boud * (gamma / _deltaTVal);
        } else if(iWhere(0) == n - 1) {
          MMG402Grid::IMMG402GridIndexer<1>::GridIndex im1 = iWhere;
          im1(0)--;
          fVaveim1 = fim1 + pTmpData->GetGridData()->GetData(im1) * (gamma / _deltaTVal);

          MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> boundx = currX;
          boundx(0) += stepNeg;
          MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> boud = ipFunc->GetFunc<SpaceScalarType, 1, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(boundx);
          fVaveip1 = fip1 + boud * (gamma / _deltaTVal);
        } else {
          MMG402Grid::IMMG402GridIndexer<1>::GridIndex im1 = iWhere;
          im1(0)--;
          fVaveim1 = fim1 + pTmpData->GetGridData()->GetData(im1) * (gamma / _deltaTVal);

          MMG402Grid::IMMG402GridIndexer<1>::GridIndex ip1 = iWhere;
          ip1(0)++;
          fVaveip1 = fip1 + pTmpData->GetGridData()->GetData(ip1) * (gamma / _deltaTVal);
        }

       // fVaveim1 = fVavei;
      //  fVaveip1 = fVavei;

        DataScalarType descriminant = GUSqr((uVal / (2 * muVal))) + dVave / ( muVal );
        GUAssert(descriminant > 0);

        DataScalarType lambda1 = - uVal /( 2 * muVal) + GUSqrt(descriminant);
        DataScalarType lambda2 = - uVal /( 2 * muVal) - GUSqrt(descriminant);


        DataScalarType nu1 = lambda1 * stepPos;
        DataScalarType nu2 = lambda2 * stepPos;


        DataScalarType B0Plus = (muPlus12 / stepPos) / (1.00 - exp(nu2 - nu1));
        DataScalarType B0Minus = (muMinus12 / stepNeg) / (1.00 - exp(nu2 - nu1));


        DataScalarType Bi1 = B0Minus * (nu1 * exp(nu2 - nu1) - nu2);
        DataScalarType Bi2 = B0Plus * (nu1 - nu2 * exp(nu2 - nu1));

        DataScalarType A = 1.00 / (1.00 - exp(nu2 - nu1));


        MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> F1 = 0;
        MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> F2 = 0;

        SpaceScalarType dx = stepPos;

        SpaceScalarType xi = 1;
        SpaceScalarType xim1 = xi - dx;
        SpaceScalarType xip1 = xi + dx;

        F1 = fVavei * A * (1.00 / lambda1 - exp(-nu1) /lambda1 - exp(nu2 - nu1) / lambda2 + exp(-nu1) / lambda2);
        F2 = fVavei * A * (exp(nu2) / lambda2 - 1.00 / lambda2 - exp(nu2) / lambda1 + exp(nu2 - nu1) / lambda1);

        /*
        F1 = (Power(E,-(dx*lambda1) - lambda2*xim1)*(Power(E,dx*lambda1 + lambda2*xim1)*Power(lambda2,2)*
        (fVavei - fVaveim1 - fVavei*lambda1*xi + fVavei*lambda1*xim1 + 
          Power(E,lambda1*(-xi + xim1))*(-fVavei + fVaveim1 + fVaveim1*lambda1*xi - fVaveim1*lambda1*xim1)) + 
       Power(lambda1,2)*(Power(E,lambda2*xi)*(fVaveim1 + fVavei*(-1 + lambda2*xi - lambda2*xim1)) + 
          Power(E,lambda2*xim1)*(fVavei + fVaveim1*(-1 - lambda2*xi + lambda2*xim1)))))/(Power(lambda1,2)*Power(lambda2,2)*(-xi + xim1));

        F2 = (Power(E,lambda1*xip1)*Power(lambda1,2)*(fVavei - fVaveip1 - fVavei*lambda2*xi + fVavei*lambda2*xip1) + 
     Power(E,-(lambda2*xi) + (lambda1 + lambda2)*xip1)*Power(lambda1,2)*(-fVavei + fVaveip1 + fVaveip1*lambda2*xi - fVaveip1*lambda2*xip1) + 
     Power(E,dx*lambda2 + lambda1*xi)*Power(lambda2,2)*(fVaveip1 + fVavei*(-1 + lambda1*xi - lambda1*xip1)) + 
     Power(E,dx*lambda2 + lambda1*xip1)*Power(lambda2,2)*(fVavei + fVaveip1*(-1 - lambda1*xi + lambda1*xip1)))/
   (Power(E,lambda1*xip1)*Power(lambda1,2)*Power(lambda2,2)*(xi - xip1));

        F1 *= A;
        F2 *= A;
        */

        if(iWhere(0) == 0) {
          // pMatrixData->Value(currentRow, currentRow) = Bi2; // atention with boundary!!!
          pMatrixData->Value(currentRow, currentRow) = Bi1 + Bi2;
          pMatrixData->Value(currentRow, currentRow + 1) = - (B0Plus * (nu1 - nu2) * exp(nu2)); // A value

          // pMatrixData->Value(currentRow, n) = F2;
          MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> boundx = currX;
          boundx(0) -= stepNeg;
          MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> boud = ipFunc->GetFunc<SpaceScalarType, 1, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(boundx);
          DataScalarType v = (B0Plus * (nu1 - nu2) * exp(nu2));
          
          pMatrixData->Value(currentRow, n) = (F1 + F2) + v * boud;

        } else if(iWhere(0) == n - 1) {
          // pMatrixData->Value(currentRow, currentRow) = Bi1; // atention with boundary!!!
          pMatrixData->Value(currentRow, currentRow) = Bi1 + Bi2;
          pMatrixData->Value(currentRow, currentRow - 1) = - (B0Minus * (nu1 - nu2) * exp(-nu1)); // C value

          MMG402Domain::MMG402DomainPoint<SpaceScalarType, 1> boundx = currX;
          boundx(0) += stepPos;
          MMG402Domain::MMG402DomainPoint<DataScalarType, DataDim> boud = ipFunc->GetFunc<SpaceScalarType, 1, DataScalarType, DataDim>(MMG402Func::MMG402FuncDestination_BoundaryData)->GetValue(boundx);
          DataScalarType v = (B0Minus * (nu1 - nu2) * exp(-nu1));
          pMatrixData->Value(currentRow, n) = (F1 + F2) +  v * boud;
        } else {
          pMatrixData->Value(currentRow, currentRow - 1) = - (B0Minus * (nu1 - nu2) * exp(-nu1)); // C value
          pMatrixData->Value(currentRow, currentRow) = Bi1 + Bi2;
          pMatrixData->Value(currentRow, currentRow + 1) = - (B0Plus * (nu1 - nu2) * exp(nu2)); // A value

          pMatrixData->Value(currentRow, n) = (F1 + F2);
        }
        ioTargetData->GetGridData()->GetData(iWhere) = 0;
        return true;
      }

      virtual int GetType() const
      {
        return MMG402SolverOperatorRegistry_MMG402OperatorCDR;
      }

    protected:
      virtual void PrepareForEvaluate() const
      {
        _uComponent = 0;
        _pUFunc = _pInput->pFuncStorage->GetFunc<SpaceScalarType, 1, DataScalarType, OriginalSpaceDim>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "U"));
        // check is U reduced func ?
        IMMG402FuncSpaceDimReductorDescriptor* pReduced = dynamic_cast<IMMG402FuncSpaceDimReductorDescriptor*> (_pUFunc);
        if(pReduced != 0) {
          std::vector<bool> ind = pReduced->GetIsReductedSpaceInfo();
          for(unsigned int i = 0; i < ind.size(); i++) {
            if(!ind[i]) {
              _uComponent = i;
              break;
            }
          }
        }
        _pMuFunc = _pInput->pFuncStorage->GetFunc<SpaceScalarType, 1, DataScalarType, 1>(MMG402Func::MMG402FuncStorageKey(MMG402Func::MMG402FuncDestination_Special, "Mu"));

        const IMESymbolData* pDeltaTRaw = _pInput->symbolTable.GetData(Symbols::DeltaT);
        GUAssert(pDeltaTRaw != 0);
        GUAssert(pDeltaTRaw->GetType() == MMG402SymbolDataType_Scalar);

        const MMG402Expression::MMG402SymbolDataScalar<double>* pAsScalar = dynamic_cast<const MMG402Expression::MMG402SymbolDataScalar<double>*>(pDeltaTRaw);
        _deltaTVal = (DataScalarType) pAsScalar->Value();

        GUAssert(_pMuFunc != 0);
        GUAssert(_pUFunc != 0);
      }

      mutable unsigned int _uComponent;
      mutable IMMG402Func<SpaceScalarType, 1, DataScalarType, OriginalSpaceDim>* _pUFunc;
      mutable IMMG402Func<SpaceScalarType, 1, DataScalarType, 1>* _pMuFunc;
      mutable DataScalarType _deltaTVal;

    };


    template <typename SpaceScalarType, typename DataScalarType, int DataDim, int OriginalSpaceDim>
    class MMG402OperatorCDR : public IMMG402OperatorTemporal<SpaceScalarType, 1, DataScalarType, DataDim>
    {
    public:
      virtual IMESymbolOperatorUnary* CreateImplementation(MMG402SolverInputTemporal<SpaceScalarType, 1, DataScalarType, DataDim>* ipGeneral) const
      {
        return new MMG402OperatorCDRImplementation<SpaceScalarType, DataScalarType, DataDim, OriginalSpaceDim>(ipGeneral);
      }

      virtual MMG402Expression::MMG402OperatorSchemeType GetSchemeType() const
      {
        return MMG402Expression::MMG402OperatorSchemeType_ImplicitTemporal;
      }

      MESymbol operator () (const MESymbol& iSymb)
      {
        return PrepareSymbol(MESymbol(iSymb, MMG402SolverOperatorRegistry_MMG402OperatorCDR, "CDR" ));
      }
    protected:
    };

    class MESymbolOperatorCDRPrinter  : public IMESymbolPrinter
    {
    public:
      virtual bool Print(std::ostream& out, const MESymbol& iSymb)
      {
        switch (iSymb.GetSymbolType()) {
          case MMG402SolverOperatorRegistry_MMG402OperatorCDR :
            out << " CDR(" << iSymb.GetArguments()[0]  << "," << iSymb.GetSymbolName() << ")";
            return true;
        }
        return false;
      }
    };

    REGISTER_DERIVED_ID(MMG402Expression::IMMG402Operator, MMG402OperatorCDR, MMG402SolverOperatorRegistry_MMG402OperatorCDR);
    REGISTER_OBJECT(MExp::IMESymbolPrinter, MESymbolOperatorCDRPrinter);

  } // end namespace Temporal
}// end namespace MMG402Solver

#endif // MMG402CDROperator_H
