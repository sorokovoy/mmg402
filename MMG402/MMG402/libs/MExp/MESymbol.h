
#ifndef MESymbol_H
#define MESymbol_H

#include <vector>
#include "MMG402\libs\GUtils\GUStringUtils.h"
#include "MMG402\libs\ObjExtension.h"

namespace MExp
{

#define MESymbolType_Zero        -1
#define MESymbolType_Symbol       0
#define MESymbolType_Constant     1
#define MESymbolType_Sum          2
#define MESymbolType_Prod         3
#define MESymbolType_Devide       4
#define MESymbolType_UnaryInverce 5

  class MESymbol : public ObjExtension::ObjExtensionClass
  {
    friend class MESymbolDerivator;
  public:
    MESymbol() : _symbolType(MESymbolType_Zero) { };

    MESymbol(int iV)          : _symbolType(MESymbolType_Constant)
    {
      _symbolName = GUToString(iV);
    }
    MESymbol(double iV)       : _symbolType(MESymbolType_Constant)
    {
      _symbolName = GUToString(iV);
    }
    MESymbol(float iV)        : _symbolType(MESymbolType_Constant)
    {
      _symbolName = GUToString(iV);
    }
    MESymbol(unsigned int iV) : _symbolType(MESymbolType_Constant)
    {
      _symbolName = GUToString(iV);
    }

    MESymbol(std::string iName, bool iIsConstant = false)
      : _symbolType(iIsConstant ? MESymbolType_Constant : MESymbolType_Symbol)
    {
      _symbolName = iName;
    };

    MESymbol(const MESymbol& iSymb, int iOpType, std::string iName = "op")
    {
      _symbolName = iName;
      _symbolType = iOpType;
      _arguments.push_back(iSymb);
    }

    MESymbol(const MESymbol& iSymb)
    {
      CopyFrom(iSymb);
    }

    virtual ~MESymbol() {};

    const std::string& GetSymbolName() const
    {
      return _symbolName;
    }
    int GetSymbolType() const
    {
      return _symbolType;
    }
    const std::vector<MESymbol>& GetArguments() const
    {
      return _arguments;
    }

    MESymbol Replace(const MESymbol& iSource, const MESymbol& iTarget) const
    {
      MESymbol result(*this);
      for (unsigned int i = 0; i < _arguments.size(); i++) {
        if (result._arguments[i] == iSource) {
          result._arguments[i] = iTarget;
        } else {
          result._arguments[i] = result._arguments[i].Replace(iSource, iTarget);
        }
      }
      return result;
    }

    virtual MESymbol operator + (const MESymbol& iRight) const
    {
      MESymbol res(0, MESymbolType_Sum);
      if (this->_symbolType == MESymbolType_Sum) {
        res._arguments = this->_arguments;
      } else {
        if (this->GetSymbolType() != MESymbolType_Zero) {
          res._arguments.push_back(*this);
        }
      }

      if (iRight._symbolType == MESymbolType_Sum) {
        res._arguments.insert(res._arguments.end(), iRight._arguments.begin(), iRight._arguments.end());
      } else {
        if (iRight.GetSymbolType() != MESymbolType_Zero) {
          res._arguments.push_back(iRight);
        }
      }
      return res;
    }

    virtual MESymbol operator += (const MESymbol& iRight)
    {
      if (iRight.GetSymbolType() == MESymbolType_Zero) {
        return *this;
      }

      if (this->GetSymbolType() != MESymbolType_Zero) {
        CopyFrom(*this + iRight);
      } else {
        CopyFrom(iRight);
      }
      return *this;
    }

    virtual MESymbol operator - (const MESymbol& iRight) const
    {
      return *this + (-iRight);
    }

    virtual MESymbol operator -= (const MESymbol& iRight)
    {
      CopyFrom(*this - iRight);
      return *this;
    }

    virtual MESymbol operator * (const MESymbol& iRight) const
    {
      MESymbol res(0, MESymbolType_Prod);
      if (this->_symbolType == MESymbolType_Prod) {
        res._arguments = this->_arguments;
      } else {
        res._arguments.push_back(*this);
      }

      if (iRight._symbolType == MESymbolType_Prod) {
        res._arguments.insert(res._arguments.end(), iRight._arguments.begin(), iRight._arguments.end());
      } else {
        res._arguments.push_back(iRight);
      }
      return res;
    }

    virtual MESymbol operator *= (const MESymbol& iRight)
    {
      CopyFrom(*this * iRight);
      return *this;
    }

    virtual MESymbol operator / (const MESymbol& iRight) const
    {
      MESymbol res(0, MESymbolType_Devide);
      res._arguments.push_back(*this);
      res._arguments.push_back(iRight);
      return res;
    }

    virtual MESymbol operator -() const
    {
      MESymbol res(0, MESymbolType_UnaryInverce);
      res._arguments.push_back(*this);
      return res;
    }

    virtual void operator = (const MESymbol& iRight)
    {
      CopyFrom(iRight);
    }

    virtual bool operator == (const MESymbol& iRight) const
    {
      if (_symbolType != iRight._symbolType) return false;
      if (_symbolName != iRight._symbolName) return false;
      if (_arguments != iRight._arguments) return false;
      return true;
    }

    bool operator < (const MESymbol& iRight) const
    {
      if (_symbolType <  iRight._symbolType) return true;
      if (_symbolType == iRight._symbolType && _symbolName <  iRight._symbolName) return true;
      if (_symbolType == iRight._symbolType && _symbolName == iRight._symbolName && _arguments < iRight._arguments) return true;
      return false;
    }

  protected:
    MESymbol(int, int iOperatorType)
      : _symbolType(iOperatorType)
    {
      _symbolName = "op";
    }

    void CopyFrom(const MESymbol& iSymb)
    {
      _symbolType = iSymb._symbolType;
      _symbolName = iSymb._symbolName;
      _arguments = iSymb._arguments;
      _extensionObjects = iSymb._extensionObjects;
    }

    std::string _symbolName;
    int         _symbolType;
    std::vector<MESymbol> _arguments;
  };


  inline MESymbol operator + (const int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) + iV2;
  }
  inline MESymbol operator - (const int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) - iV2;
  }
  inline MESymbol operator * (const int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) * iV2;
  }
  inline MESymbol operator / (const int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) / iV2;
  }

  inline MESymbol operator + (const double& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) + iV2;
  }
  inline MESymbol operator - (const double& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) - iV2;
  }
  inline MESymbol operator * (const double& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) * iV2;
  }
  inline MESymbol operator / (const double& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) / iV2;
  }

  inline MESymbol operator + (const float& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) + iV2;
  }
  inline MESymbol operator - (const float& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) - iV2;
  }
  inline MESymbol operator * (const float& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) * iV2;
  }
  inline MESymbol operator / (const float& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) / iV2;
  }

  inline MESymbol operator + (const unsigned int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) + iV2;
  }
  inline MESymbol operator - (const unsigned int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) - iV2;
  }
  inline MESymbol operator * (const unsigned int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) * iV2;
  }
  inline MESymbol operator / (const unsigned int& iV1, const MESymbol& iV2)
  {
    return MESymbol(iV1) / iV2;
  }
} // end MExp namespace

#endif // end MESymbol_H
