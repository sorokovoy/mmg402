// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// implement nullable type
//
//===================================================================
//  AUG 2013  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUNullableType_H
#define GUNullableType_H

template <class T>
class GUNullableType
{
public:
  GUNullableType(const T& iValue)
  {
    _storedValue = iValue;
    _hasValue = true;
  }

  GUNullableType()
  {
    _storedValue = 0;
    _hasValue = false;
  }

  GUNullableType& operator = (const GUNullableType& iValue)
  {
    _hasValue = iValue._hasValue;
    _storedValue = iValue._storedValue;
    return *this;
  }

  bool HasValue() const
  {
    return _hasValue;
  }

  T& GetValue()
  {
    return _storedValue;
  }

  const T& GetValue() const
  {
    return _storedValue;
  }

private:
  bool _hasValue;
  T _storedValue;
};

#endif // AIFixedSizeBuffer_H
