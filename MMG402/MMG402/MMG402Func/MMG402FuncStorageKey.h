#ifndef MMG402FuncStorageKey_H
#define MMG402FuncStorageKey_H

#include <string>

namespace MMG402Func
{

  enum MMG402FuncDestination {
    MMG402FuncDestination_Undefined = -1,
    MMG402FuncDestination_Special = 0,
    MMG402FuncDestination_BoundaryData = 1,
    MMG402FuncDestination_RightAdditionalFunction = 2,
    MMG402FuncDestination_InitialData = 3
  };

  struct MMG402FuncStorageKey {
    MMG402FuncStorageKey(MMG402FuncDestination iType, std::string iFuncName = "Any") : FuncName(iFuncName), FuncType(iType) {}
    const std::string FuncName;
    const MMG402FuncDestination FuncType;

    bool operator <  (const MMG402FuncStorageKey& iOther) const
    {
      return ((FuncName <  iOther.FuncName) || (FuncName == iOther.FuncName && FuncType < iOther.FuncType));
    }
    bool operator == (const MMG402FuncStorageKey& iOther) const
    {
      return ((FuncName == iOther.FuncName  &&  FuncType == iOther.FuncType));
    }
  };

} // end namespace MMG402Func
#endif
