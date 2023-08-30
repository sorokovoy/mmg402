#ifndef ObjRegistry_H
#define ObjRegistry_H

#include <map>
#include <vector>
#include <string>
#include <exception>

namespace ObjRegistry
{
  template<class OBJECT_BASE_TYPE>
  struct ObjectRegistryInfo {
    ObjectRegistryInfo(OBJECT_BASE_TYPE* ipInstance)
    {
      pInstance = ipInstance;
    }
    OBJECT_BASE_TYPE* pInstance;
  };

  template<class OBJECT_BASE_TYPE>
  struct RegistryEntryData {
    RegistryEntryData()
    {}

    std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>> objectRegisty;
    std::map<std::string, int> registeredDerivedIds;
    std::vector<std::string> objectTypes;
  };

  template<class OBJECT_BASE_TYPE>
  class RegistryEntry
  {
  public:
    static const std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>& GetRegistry()
    {
      RegistryEntry<OBJECT_BASE_TYPE>& inst = GetRegSection();
      return inst.getData().objectRegisty;
    }

    static const std::vector<std::string>& GetTypes()
    {
      RegistryEntry<OBJECT_BASE_TYPE>& inst = GetRegSection();
      return inst.getData().objectTypes;
    }

    template<typename OBJECT_DERIVED_TYPE>
    static OBJECT_DERIVED_TYPE* GetInstance()
    {
      std::string derivedName = typeid(OBJECT_DERIVED_TYPE).name();
      return (OBJECT_DERIVED_TYPE*)GetInstance(derivedName);
    }

    static OBJECT_BASE_TYPE* GetInstance(const std::string& iDerivedName)
    {
      RegistryEntry<OBJECT_BASE_TYPE>& inst = GetRegSection();
      std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>& reg = inst.getData().objectRegisty;
      std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>::iterator it = reg.find(iDerivedName);
      if (it == reg.end()) {
        return 0;
      } else {
        return (OBJECT_BASE_TYPE*)it->second.pInstance;
      }
    }

    template<typename OBJECT_DERIVED_TYPE>
    inline static RegistryEntry<OBJECT_BASE_TYPE>& InsetObject()
    {
      RegistryEntry<OBJECT_BASE_TYPE>& inst = GetRegSection();
      RegistryEntryData<OBJECT_BASE_TYPE>& data = inst.getData();
      std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>& reg = data.objectRegisty;
      std::string derivedName = typeid(OBJECT_DERIVED_TYPE).name();
      std::map<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>::iterator it = reg.find(derivedName);
      if (it != reg.end()) {
        // alredy found
        return inst;
      }
      //reg.insert(std::make_pair<std::string, ObjectRegistryInfo<OBJECT_BASE_TYPE>>(derivedName, ObjectRegistryInfo<OBJECT_BASE_TYPE>(new OBJECT_DERIVED_TYPE())));
      reg.insert(std::make_pair(derivedName, ObjectRegistryInfo<OBJECT_BASE_TYPE>(new OBJECT_DERIVED_TYPE())));
      data.objectTypes.push_back(derivedName);

      return inst;
    }

    inline static RegistryEntry<OBJECT_BASE_TYPE>& RegisterDerivedID(const std::string derivedName, int iId)
    {
      RegistryEntry<OBJECT_BASE_TYPE>& inst = GetRegSection();
      RegistryEntryData<OBJECT_BASE_TYPE>& data = inst.getData();

      std::map<std::string, int>::iterator it = data.registeredDerivedIds.find(derivedName);
      if (it != data.registeredDerivedIds.end() && it->second != iId) {
        throw new std::exception("Derived type alredy has other ID");
      }

      for (it = data.registeredDerivedIds.begin(); it != data.registeredDerivedIds.end(); it++) {
        if (it->second == iId && it->first != derivedName) {
          throw new std::exception(("This Id alredy registered for type " + it->first).c_str());
        }
      }
      data.registeredDerivedIds[derivedName] = iId;
      return inst;
    }

  private:
    static RegistryEntry<OBJECT_BASE_TYPE>& GetRegSection()
    {
      static RegistryEntry<OBJECT_BASE_TYPE> inst;
      return inst;
    }

    inline RegistryEntryData<OBJECT_BASE_TYPE>& getData()
    {
      static RegistryEntryData<OBJECT_BASE_TYPE> reg;
      return reg;
    }
  };
};

#define OBJECT_REGISTRY_METAOPERATION_BEGIN(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE, OPERATION)  \
    namespace ORegistryMakroExp {                                           \
    namespace SPACE_##OBJECT_DERIVED_TYPE {                                 \
    namespace SPACE_##OPERATION {                                           \
      namespace {                                                           \
        template<class T>                                                   \
        class ComponentRegistration;                                        \
                                                                            \
        template<>                                                          \
        class ComponentRegistration<OBJECT_BASE_TYPE>                       \
        {                                                                   \
          static const ObjRegistry::RegistryEntry<OBJECT_BASE_TYPE>& reg;   \
        };                                                                  \

#define OBJECT_REGISTRY_METAOPERATION_END(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE)    \
        };                                                                  \
      };                                                                    \
    };                                                                      \
  };                                                                        \


#define REGISTER_OBJECT(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE)                     \
        OBJECT_REGISTRY_METAOPERATION_BEGIN(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE, REGISTER_OBJECT_OPERATION) \
        const ObjRegistry::RegistryEntry<OBJECT_BASE_TYPE>& ComponentRegistration<OBJECT_BASE_TYPE>::reg = ObjRegistry::RegistryEntry<OBJECT_BASE_TYPE>::InsetObject<OBJECT_DERIVED_TYPE>(); \
        OBJECT_REGISTRY_METAOPERATION_END(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE)   \


#define REGISTER_DERIVED_ID(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE, ID)             \
        OBJECT_REGISTRY_METAOPERATION_BEGIN(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE, REGISTER_DERIVED_OPERATION) \
        const ObjRegistry::RegistryEntry<OBJECT_BASE_TYPE>& ComponentRegistration<OBJECT_BASE_TYPE>::reg = ObjRegistry::RegistryEntry<OBJECT_BASE_TYPE>::RegisterDerivedID(#OBJECT_DERIVED_TYPE, ID); \
        OBJECT_REGISTRY_METAOPERATION_END(OBJECT_BASE_TYPE, OBJECT_DERIVED_TYPE)   \

/* Usage
class A
{
public:
  virtual const char* Name() { return "A"; }
};

class B : public A
{
public:
  virtual const char* Name() { return "B"; }
};

class C : public A
{
public:
  virtual const char* Name() { return "C"; }
};

class D : public A
{
public:
  virtual const char* Name() { return "D"; }
};

REGISTER_OBJECT(A, B);
REGISTER_OBJECT(A, C);
REGISTER_OBJECT(A, D);

bool Test()
{
  A* p;
  const char* val;
  p = ObjRegistry::RegistryEntry<A>::GetInstance<B>();
  val = p->Name();
  p = ObjRegistry::RegistryEntry<A>::GetInstance<C>();
  val = p->Name();
  p = ObjRegistry::RegistryEntry<A>::GetInstance<D>();
  val = p->Name();
}
*/

#endif //ObjRegistry_H
