#ifndef MMG402CImg_H
#define MMG402CImg_H

#include "MMG402IO\libs\CImg.h"

namespace MMG402IO
{

  namespace libs
  {

    struct MMG402RGBColor {
      unsigned char r;
      unsigned char g;
      unsigned char b;
    };

#define MMG402ABS2FF(x) ((x) > 0 ? (x) : -(x))

    inline int GetDistance(const MMG402RGBColor& iV1, const MMG402RGBColor& iV2)
    {
      return (int)MMG402ABS2FF((int)iV1.b - (int)iV2.b) + (int)MMG402ABS2FF((int)iV1.g - (int)iV2.g) + (int)MMG402ABS2FF((int)iV1.r - (int)iV2.r);
    }

    inline MMG402RGBColor Get(const cimg_library::CImg<unsigned char>& iFrom, int x, int y)
    {
      MMG402RGBColor v;
      v.r = *iFrom.data(x, y, 0, 0);
      v.g = *iFrom.data(x, y, 0, 1);
      v.b = *iFrom.data(x, y, 0, 2);
      return v;
    }

    inline void Set(cimg_library::CImg<unsigned char>& iFrom, int x, int y, const MMG402RGBColor& v)
    {
      *iFrom.data(x, y, 0, 0) = v.r;
      *iFrom.data(x, y, 0, 1) = v.g;
      *iFrom.data(x, y, 0, 2) = v.b;
    }

    template<typename DataScalarType>
    class MMG402RGBColorGradientInterpretator
    {
    public:
      MMG402RGBColorGradientInterpretator(const char* ipGradFile, DataScalarType iMinValue, DataScalarType iMaxValue)
      {
        _pImage = new cimg_library::CImg<unsigned char>(ipGradFile);
        _minV = iMinValue;
        _maxV = iMaxValue;
      }

      DataScalarType Recognize(const MMG402RGBColor& iV)
      {
        int mid = 255000;
        int idx = 0;
        for (int i = 0; i < _pImage->width(); i++) {
          int v = GetDistance(Get(*_pImage, i, 0), iV);
          if (mid > v) {
            mid = v;
            idx = i;
          }
        }
        return  ((DataScalarType) idx) / (_pImage->width()) * (_maxV - _minV) + _minV;
      }

      MMG402RGBColor ToColor(DataScalarType v)
      {
        v = (v - _minV) / (_maxV - _minV);
        if (v >= 1 || GUIsNAN(v)) {
          v = 0.9999;
        }
        if (v < 0) {
          v = 0;
        }
        v = v * _pImage->width();
        return Get(*_pImage, (int)v, 0);
      }

    private:
      cimg_library::CImg<unsigned char>* _pImage;
      DataScalarType _minV;
      DataScalarType _maxV;
    };

  }; // libs

}; // MMG402IO

#endif // MMG402CImg_H
