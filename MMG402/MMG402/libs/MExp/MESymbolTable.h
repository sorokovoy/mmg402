#ifndef IMESymbolTable_H
#define IMESymbolTable_H

#include "MMG402\libs\GUtils\GUPointerStorage.h"
#include "MMG402\libs\GUtils\GUAssert.h"
#include <map>

#include "MMG402\libs\MExp\IMESymbolOperators.h"

namespace MExp
{

  class MESymbolTable : private GUPointerStorage
  {
  public:
    MESymbolTable()
    {
      _pConstantParser = 0;
    }

    virtual ~MESymbolTable()
    {
      OnDerivedDestructorCall();
    };

    virtual IMESymbolConstantParser* GetContantParser() const
    {
      GUAssert(_pConstantParser != 0);
      return _pConstantParser;
    }

    virtual bool SetContantParser(IMESymbolConstantParser* ipParser)
    {
      //GUAssertV(_pConstantParser == 0, false);
      _pConstantParser = ipParser;
      return true;
    }

    const IMESymbolData* GetData(const std::string& iSymbolName) const
    {
      std::map<std::string, const IMESymbolData**>::const_iterator it = _data.find(iSymbolName);
      GUAssertV(it != _data.end(), 0);
      return *it->second;
    }

    virtual bool SetData(const std::string& iSymbolName, IMESymbolData* ipData)
    {
      IMESymbolData** pCurrentReference = new IMESymbolData*(ipData);
      _deletePtrs.push_back(pCurrentReference);
      return SetData(iSymbolName, pCurrentReference);
    }

    virtual bool SetData(const std::string& iSymbolName, IMESymbolData** ipData)
    {
      if (_data.find(iSymbolName) == _data.end()) {
        _data.insert(std::make_pair<std::string, const IMESymbolData**>(std::string(iSymbolName), (const IMESymbolData**)(ipData)));
      } else {
        _data[iSymbolName] = (const IMESymbolData**)(ipData);
      }
      return true;
    }

    const IMESymbolOperatorUnary* GetUnaryOperator(int id) const
    {
      std::map<int, IMESymbolOperatorUnary*>::const_iterator it = _unaryOp.find(id);
      return it != _unaryOp.end() ? it->second : 0;
    }

    virtual bool AddUnaryOperator(IMESymbolOperatorUnary* ipData)
    {
      //GUAssertV(_unaryOp.find(ipData->GetType()) == _unaryOp.end(), false);
      //_unaryOp.insert(std::make_pair<int, IMESymbolOperatorUnary*>(ipData->GetType(), ipData));
      _unaryOp.insert(std::make_pair(ipData->GetType(), ipData));
      return true;
    }

    const IMESymbolOperatorBinary* GetBinaryOperator(int id) const
    {
      std::map<int, IMESymbolOperatorBinary*>::const_iterator it = _binaryOp.find(id);
      return it != _binaryOp.end() ? it->second : 0;
    }

    virtual bool AddBinaryOperator(IMESymbolOperatorBinary* ipData)
    {
      //GUAssertV(_binaryOp.find(ipData->GetType()) == _binaryOp.end(), false);
      //_binaryOp.insert(std::make_pair<int, IMESymbolOperatorBinary*>(ipData->GetType(), ipData));
      _binaryOp.insert(std::make_pair(ipData->GetType(), ipData));
      return true;
    }

  protected:

    IMESymbolConstantParser* _pConstantParser;
    std::vector<void*> _deletePtrs;
    std::map<std::string, const IMESymbolData**> _data;
    std::map<int, IMESymbolOperatorUnary*>   _unaryOp;
    std::map<int, IMESymbolOperatorBinary*> _binaryOp;


    // from GUPointerStorage
    virtual void DeleteData()
    {
      for (unsigned int i = 0; i < _deletePtrs.size(); i++) {
        delete _deletePtrs[i];
      }

      for (std::map<int, IMESymbolOperatorUnary*>::iterator it = _unaryOp.begin(); it != _unaryOp.end(); it++) {
        delete it->second;
      }
      for (std::map<int, IMESymbolOperatorBinary*>::iterator it = _binaryOp.begin(); it != _binaryOp.end(); it++) {
        delete it->second;
      }
      delete _pConstantParser;
    }
  };

} // end MExp namespace

#endif // IMESymbolTable_H


