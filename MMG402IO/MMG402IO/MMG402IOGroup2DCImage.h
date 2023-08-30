#ifndef MMG402IOGroup2DCImage_H
#define MMG402IOGroup2DCImage_H

#include "MMG402.h"
#include "MMG402IO\libs\MMG402CImg.h"
#include "MMG402IO\libs\MMG402Directory.h"
#include <string>

namespace MMG402IO
{
  class IMMG402IOGroup2DCImageFileNameParcer
  {
  public:
    virtual double Parce(const char* ipFileName) const = 0;
  };


  template <typename SpaceScalarType, typename DataScalarType>
  class MMG402IOGroup2DCImage
  {
  public:
    MMG402IOGroup2DCImage(const char* ipImageLegentFile, DataScalarType iLegendMinValue, DataScalarType iLegendMaxValue)
    {
      _pColorConverter = new MMG402IO::libs::MMG402RGBColorGradientInterpretator<DataScalarType>(ipImageLegentFile, iLegendMinValue, iLegendMaxValue);
    }

    ~MMG402IOGroup2DCImage()
    {
      delete _pColorConverter;
    }

    // file name should be in *.bmp extension
    virtual void Write(const MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 2, DataScalarType, 1>* ipData, std::string iDirName)
    {
      unsigned int totalTimeLayers = ipData->GetCurrentStorageDeep();
      GUAssertR(totalTimeLayers != 0);
      const MMG402Grid::IMMG402GridIndexer<2>* pIndexer = ipData->Pop(0)->pData->GetMasterGrid()->GetIndexer();
      for (unsigned int t = 0; t < totalTimeLayers; t++) {
        MMG402Grid::IMMG402GridIndexer<2>::GridIndex max = pIndexer->GetIndexMaxValue();
        MMG402NumericData::IMMG402NumData<SpaceScalarType, 2, DataScalarType, 1>* pCurrData = ipData->Pop(totalTimeLayers - 1 - t)->pData;

        cimg_library::CImg<unsigned char> res(max(0), max(1), 1, 3);
        for (unsigned int x = 0; x < max(0); x++) {
          for (unsigned int y = 0; y < max(1); y++) {
            MMG402Grid::IMMG402GridIndexer<2>::GridIndex curr;
            curr(0) = x;
            curr(1) = y;

            libs::Set(res, x, y, _pColorConverter->ToColor(pCurrData->GetData(curr)(0)));
          }
        }

        char filename[64];
        sprintf(filename, "T_%fsec.bmp", ipData->Pop(totalTimeLayers - 1 - t)->t);

        std::string filenamePath  = iDirName + "\\" + filename;
        res.save(filenamePath.c_str());
      }
    }

    // time format correspond your file name by template http://en.cppreference.com/w/cpp/io/manip/get_time
    // fo example: for parce string "2011-01-18 23:12:34" use "%Y-%m-%d %H:%M:%S" template
    virtual MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 2, DataScalarType, 1>* Read(std::string iDirName, const MMG402Grid::IMMG402Grid<SpaceScalarType, 2>* ipMasterGrid, IMMG402IOGroup2DCImageFileNameParcer* ipFileNameParcer)
    {
      std::vector<std::string> images = MMG402IO::libs::GetFilesFromFolder(iDirName.c_str());
      if (images.empty()) return 0;

      MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 2, DataScalarType, 1>* pRes = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporal<SpaceScalarType, 2, DataScalarType, 1>();
      pRes->SetStorageMaxDeep(images.size());

      MMG402Grid::IMMG402GridIndexer<2>::GridIndex max = ipMasterGrid->GetIndexer()->GetIndexMaxValue();
      for (unsigned int i = 0; i < images.size(); i++) {
        cimg_library::CImg<unsigned char> src((iDirName + "\\" + images[i]).c_str());
        GUAssert(max(0) == src.height() && max(1) == src.width());

        MMG402NumericData::IMMG402NumData<SpaceScalarType, 2, DataScalarType, 1>* pCurrData = new MMG402NumericData::MMG402NumDataDensy<SpaceScalarType, 2, DataScalarType, 1>(ipMasterGrid);
        for (unsigned int x = 0; x < max(0); x++) {
          for (unsigned int y = 0; y < max(1); y++) {
            MMG402Grid::IMMG402GridIndexer<2>::GridIndex curr;
            curr(0) = x;
            curr(1) = y;

            pCurrData->GetData(curr) = _pColorConverter->Recognize(libs::Get(src, x, y));
          }
        }

        MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, 2, DataScalarType, 1>* pItem = new MMG402Solver::Temporal::MMG402SolverDataStorageTemporalDataItem<SpaceScalarType, 2, DataScalarType, 1>();
        pItem->pData = pCurrData;

        pItem->t = ipFileNameParcer->Parce(images[i].c_str());
        pRes->Push(pItem);
      }

      return pRes;
    }

  protected:
    MMG402IO::libs::MMG402RGBColorGradientInterpretator<DataScalarType>* _pColorConverter;
  };
}; // end namespace MMG402IO

#endif // MMG402IOGroup2DCImage_H