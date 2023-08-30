// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Implement auto deleted ptr
//
//===================================================================
// DEC 2012 Creation:                           Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUAutoPtr_H
#define GUAutoPtr_H

template < typename T >
class GUAutoPtr
{
private:
  T*   _pData; // data pointer
  int* _pReference; // Reference count

public:
  GUAutoPtr() : _pData(0), _pReference(new int(1))
  {}

  GUAutoPtr(T* pValue) : _pData(pValue), _pReference(new int(1))
  {}

  GUAutoPtr(T* pValue, int* pRefCounter) : _pData(pValue), _pReference(pRefCounter)
  {
    (*_pReference)++;
  }

  GUAutoPtr(const GUAutoPtr<T>& sp) : _pData(sp._pData), _pReference(sp._pReference)
  {
    // Copy constructor
    // Copy the data and _pReference pointer
    // and increment the _pReference count
    (*_pReference)++;
  }


  ~GUAutoPtr()
  {
    // Destructor
    // Decrement the _pReference count
    (*_pReference)--;

    // if _pReference become zero delete the data
    if (*_pReference == 0) {
      delete _pData;
      delete _pReference;
    }
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

  GUAutoPtr<T>& operator = (const GUAutoPtr<T>& sp)
  {
    // Assignment operator
    if (this != &sp) { // Avoid self assignment
      // Decrement the old _pReference count
      (*_pReference)--;
      // if _pReference become zero delete the data
      if (*_pReference == 0) {
        delete _pData;
        delete _pReference;
      }

      // Copy the data and _pReference pointer
      // and increment the _pReference count
      _pData = sp._pData;
      _pReference = sp._pReference;
      (*_pReference)++;
    }
    return *this;
  }

  GUAutoPtr<const T> GetConstInstance() const
  {
    return GUAutoPtr<const T>(_pData, _pReference);
  }

  T* ExtractPtr()
  {
    if (*_pReference != 1) {
      throw new std::exception("Only one reference allowed for ptr extraction");
    }
    T* val = _pData;
    _pData = 0;
    return val;
  }
};

#endif // GUAutoPtr_H
