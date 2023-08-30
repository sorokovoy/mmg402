#ifndef IMESymbolData_H
#define IMESymbolData_H

namespace MExp
{
  class IMESymbolData
  {
  public:
    virtual ~IMESymbolData() {};

    virtual int GetType() const
    {
      return -1;
    }
    virtual IMESymbolData* CreateCopy() const  = 0;
  };
} // end namespace MExp
#endif //IMESymbolData_H
