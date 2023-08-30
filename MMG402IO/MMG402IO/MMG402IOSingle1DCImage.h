#ifndef MMG402IOSingle1DCImage_H
#define MMG402IOSingle1DCImage_H

#include "MMG402.h"

#include "MMG402IO\libs\MMG402CImg.h"

#include <string>

namespace MMG402IO
{
  template <typename SpaceScalarType, typename DataScalarType>
  class MMG402IOSingle1DCImage
  {
  public:
    MMG402IOSingle1DCImage(const char* ipImageLegentFile, DataScalarType iLegendMinValue, DataScalarType iLegendMaxValue)
    {
      _pColorConverter = new MMG402IO::libs::MMG402RGBColorGradientInterpretator<DataScalarType>(ipImageLegentFile, iLegendMinValue, iLegendMaxValue);
    }

    ~MMG402IOSingle1DCImage()
    {
      delete _pColorConverter;
    }

    // file name should be in *.bmp extension
    virtual void Write(const MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, 1>* ipData, std::string iFileName)
    {
      unsigned int totalTimeLayers = ipData->GetCurrentStorageDeep();
      unsigned int valuesPerLayer  = ipData->Pop(0)->pData->GetMasterGrid()->GetIndexer()->GetIteratorMaxValue();

      cimg_library::CImg<unsigned char> res(totalTimeLayers, valuesPerLayer, 1, 3);
      for (unsigned int t = 0; t < totalTimeLayers; t++) {
        MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>* pCurrData = ipData->Pop(totalTimeLayers - 1 - t)->pData;
        for (unsigned int r = 0; r < valuesPerLayer; r++) {
          libs::Set(res, t, r, _pColorConverter->ToColor(pCurrData->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(valuesPerLayer - r - 1))(0)));
        }
      }
      res.save(iFileName.c_str());
    }

    virtual MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, 1>* Read(std::string iFileName, const MMG402Grid::IMMG402Grid<SpaceScalarType, 1>* ipMasterGrid, double iTimeStep)
    {
      cimg_library::CImg<unsigned char> src(iFileName.c_str());
      unsigned int valuesPerLayer = src.height();
      unsigned int totalTimeLayers = src.width();

      MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, 1>* pRes = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 1, DataScalarType, 1>();
      pRes->SetStorageMaxDeep(totalTimeLayers + 1);

      double currT = 0;

      GUAssert(ipMasterGrid->GetIndexer()->GetIteratorMaxValue() == valuesPerLayer);
      for (unsigned int t = 0; t < totalTimeLayers; t++) {
        MMG402NumericData::IMMG402NumData<SpaceScalarType, 1, DataScalarType, 1>* pCurrData = new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, 1, DataScalarType, 1>(ipMasterGrid);
        for (unsigned int r = 0; r < valuesPerLayer; r++) {
          pCurrData->GetData(MMG402Grid::IMMG402GridIndexer<1>::GridIndex(r)) = _pColorConverter->Recognize(libs::Get(src, t, r));
        }

        MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, 1, DataScalarType, 1>* pItem = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, 1, DataScalarType, 1>();
        pItem->pData = pCurrData;
        pItem->t = currT;
        currT += iTimeStep;
        pRes->Push(pItem);
      }
      return pRes;
    }

  protected:
    MMG402IO::libs::MMG402RGBColorGradientInterpretator<DataScalarType>* _pColorConverter;
  };
};

#endif // MMG402IOSingle1DCImage_H
