// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Object for
//
//===================================================================
// MAY 2014 Creation:                           Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUWeakPtr_H
#define GUWeakPtr_H

#include "GUAutoPtr.h"

class GUWeakReferableObject
{
  template<typename T> friend class GUWeakPtr;
public:
  GUWeakReferableObject() : _isDeleted(new bool(false)) { }
  GUWeakReferableObject(const GUWeakReferableObject& iOther) : _isDeleted(new bool(false)) { }

  virtual ~GUWeakReferableObject()
  {
    *_isDeleted = true;
  }

private:
  GUAutoPtr<bool> _isDeleted;
};

template<typename T>
class GUWeakPtr
{
private:
  T* _pData; // data pointer
  GUAutoPtr<bool> _isDeleted;

public:
  GUWeakPtr() : _pData(0), _isDeleted(new bool(true))
  {}

  GUWeakPtr(T* pValue)
  {
    _pData = pValue;
    GUWeakReferableObject* pValueAsDeletableObject = pValue;
    _isDeleted = pValueAsDeletableObject->_isDeleted;
  }

  GUWeakPtr(const GUWeakPtr<T>& sp) : _pData(sp._pData)
  {
    // Copy constructor
    _isDeleted = sp._isDeleted;
  }

  bool IsDeleted() const
  {
    return *_isDeleted;
  }

  T& operator* ()
  {
    return *_pData;
  }

  T* operator-> ()
  {
    return _pData;
  }

  T* GetPointer()
  {
    return _pData;
  }

  // explicit conversation cast
  operator T* ()
  {
    return _pData;
  }

  const T& operator* () const
  {
    return *_pData;
  }

  const T* operator-> () const
  {
    return _pData;
  }

  const T* GetPointer() const
  {
    return _pData;
  }

  // explicit conversation cast
  operator const T* () const
  {
    return _pData;
  }

  GUWeakPtr<const T> GetConstInstance()
  {
    return GUWeakPtr<const T>(_pData, _pReference);
  }
};

#endif // GUWeakPtr_H
