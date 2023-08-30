// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Implement auto deleted ptr
//
//===================================================================
// DEC 2012 Creation:                           Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUPointerStorage_H
#define GUPointerStorage_H

#include <exception>

// ! ONLY private interherit
class GUPointerStorage
{
private:
  int* _pReference; // Reference count

public:
  GUPointerStorage() : _pReference(new int(1))
  {
    _isOnDerivedDestructorCallCalled = false;
  }

  GUPointerStorage(const GUPointerStorage& sp) : _pReference(sp._pReference)
  {
    // Copy constructor
    // Copy the data and _pReference pointer
    // and increment the _pReference count
    (*_pReference)++;
    _isOnDerivedDestructorCallCalled = false;
  }

  virtual ~GUPointerStorage()
  {
    if (!_isOnDerivedDestructorCallCalled) {
      throw new std::exception("Invalid usage of GUPointerStorage");
      // for using GUPointerStorage you shoul
      // 1) !ONLY private interherit
      // 2) implement virtual void DeleteData() = 0; in derived
      // 3) call OnDerivedDestructorCall in derived destructor
    }
  }

  GUPointerStorage& operator = (const GUPointerStorage& sp)
  {
    // Assignment operator
    if (this != &sp) { // Avoid self assignment
      // Decrement the old _pReference count
      (*_pReference)--;
      // if _pReference become zero delete the data
      if (*_pReference == 0) {
        DeleteData();
        delete _pReference;
      }

      // Copy the data and _pReference pointer
      // and increment the _pReference count
      _pReference = sp._pReference;
      (*_pReference)++;
    }
    return *this;
  }

protected:
  // require call in derived destructor
  void OnDerivedDestructorCall()
  {
    // Destructor
    // Decrement the _pReference count
    (*_pReference)--;

    // if _pReference become zero delete the data
    if (*_pReference == 0) {
      DeleteData();
      delete _pReference;
    }
    _isOnDerivedDestructorCallCalled = true;
  }
  virtual void DeleteData() = 0;

private:
  bool _isOnDerivedDestructorCallCalled;
};

#endif // GUAutoPtr_H
