#ifndef MESymbolPrinter_H
#define MESymbolPrinter_H

#include <iostream>
#include <sstream>

#include "MMG402\libs\ObjRegistry.h"

namespace MExp
{

  inline std::ostream& operator<<(std::ostream& out, const MESymbol& iSymb);

  class IMESymbolPrinter
  {
  public:
    virtual bool Print(std::ostream& out, const MESymbol& iSymb) = 0;
  };

  inline std::ostream& operator<<(std::ostream& out, const MESymbol& iSymb)
  {
    const std::map<std::string, ObjRegistry::ObjectRegistryInfo<IMESymbolPrinter>>& inst = ObjRegistry::RegistryEntry<IMESymbolPrinter>::GetRegistry();
    for (std::map<std::string, ObjRegistry::ObjectRegistryInfo<IMESymbolPrinter>>::const_iterator it = inst.cbegin(); it != inst.cend(); it++) {
      if (it->second.pInstance->Print(out, iSymb)) return out;
    }
    return out << "ERROR TYPE " << iSymb.GetSymbolType() << ".";
  }

  inline std::string ToString(const MESymbol& iSymb)
  {
    std::stringstream res;
    res << iSymb;
    return res.str();
  }

} // end namespace MExp
#endif // MESymbolPrinter_H
