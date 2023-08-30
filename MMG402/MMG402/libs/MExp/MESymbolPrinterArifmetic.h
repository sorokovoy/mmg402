#ifndef MESymbolPrinterArifmetic_H
#define MESymbolPrinterArifmetic_H

#include "MMG402\libs\ObjRegistry.h"

#include "MMG402\libs\MExp\IMESymbolPrinter.h"

namespace MExp
{

  class MESymbolArifmeticPrinter : public IMESymbolPrinter
  {
  public:
    virtual bool Print(std::ostream& out, const MESymbol& iSymb)
    {
      switch (iSymb.GetSymbolType()) {
        case MESymbolType_Zero         :
          return true;
        case MESymbolType_Symbol       :
          out << iSymb.GetSymbolName();
          return true;
        case MESymbolType_Constant     :
          out << iSymb.GetSymbolName();
          return true;
        case MESymbolType_Sum          : {
          out << "(";
          for (unsigned int i = 0; i < iSymb.GetArguments().size() - 1; i++) {
            out << iSymb.GetArguments()[i]  << "+";
          }
          out << iSymb.GetArguments()[iSymb.GetArguments().size() - 1]  << ")";
          return true;
        }

        case MESymbolType_Prod: {
          out << " ";
          for (unsigned int i = 0; i < iSymb.GetArguments().size() - 1; i++) {
            out << iSymb.GetArguments()[i]  << "*";
          }
          out << iSymb.GetArguments()[iSymb.GetArguments().size() - 1]  << " ";
          return true;
        }

        case MESymbolType_Devide       :
          out << " (" << iSymb.GetArguments()[0]  << ")/(" << iSymb.GetArguments()[1]  << ") ";
          return true;
        case MESymbolType_UnaryInverce :
          out << " -" << iSymb.GetArguments()[0];
          return true;
      }
      return false;
    }
  };

  REGISTER_OBJECT(IMESymbolPrinter, MESymbolArifmeticPrinter);

} // end MExp namespace
#endif // MESymbolPrinterArifmetic_H
