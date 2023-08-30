#ifndef ObjExtension_H
#define ObjExtension_H

#include <map>
#include <string>

namespace ObjExtension
{
  template <typename ExtensionType>
  class ObjExtensionBase
  {
  public:
    void AddExtensionObject(ExtensionType iObj, const std::string& iId)
    {
      _extensionObjects[iId] = iObj;
    }

  protected:
    virtual ExtensionType GetExtensionObjectTemplate(const std::string& iId) const
    {
      std::map<std::string, ExtensionType>::const_iterator it = _extensionObjects.find(iId);
      if (it != _extensionObjects.end()) {
        return it->second;
      } else {
        return GetDefaultValue();
      }
    }

    std::map<std::string, ExtensionType> _extensionObjects;

    virtual ExtensionType GetDefaultValue() const = 0;
  };

  class ObjExtensionClass : protected virtual ObjExtensionBase<void*>
  {
  public:
    void* GetExtensionObject(const std::string& iId) const
    {
      return GetExtensionObjectTemplate(iId);
    }

  private:
    virtual void* GetDefaultValue() const
    {
      return 0;
    }
  };

  class ObjExtensionInfo : protected virtual ObjExtensionBase<std::string>
  {
  public:
    std::string GetExtensionString(const std::string& iId)
    {
      return GetExtensionObjectTemplate(iId);
    }
  private:
    virtual std::string GetDefaultValue() const
    {
      return "";
    }
  };
}// end namespace
#endif //ObjExtension_H
