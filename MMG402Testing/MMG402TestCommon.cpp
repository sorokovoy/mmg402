#include "MMG402TestCommon.h"

#include "MMG402\libs\ObjRegistry.h"
#include "MMG402\MMG402Matrix\MMG402MatrixSparce.h"

bool MMG402TestCommon::Run()
{
  RUN_TMMG402T(TestObjRegistryStorage());
  return true;
}

class A
{
public:
  virtual std::string GetName() const
  {
    return "class A";
  }
};
class B : public A
{
public:
  virtual std::string GetName() const
  {
    return "class B";
  }
};
class C : public A
{
public:
  virtual std::string GetName() const
  {
    return "class C";
  }
};
class D : public A
{
public:
  virtual std::string GetName() const
  {
    return "class D";
  }
};
class E : public A
{
public:
  virtual std::string GetName() const
  {
    return "class E";
  }
};
class F : public A
{
public:
  virtual std::string GetName() const
  {
    return "class F";
  }
};
class R : public A
{
public:
  virtual std::string GetName() const
  {
    return "class R";
  }
};

REGISTER_DERIVED_ID(A, B, 4);
REGISTER_DERIVED_ID(A, C, 5);
REGISTER_DERIVED_ID(A, D, 6);
//REGISTER_DERIVED_ID(A, B, 4);
//REGISTER_DERIVED_ID(A, B, 5); // cased exception

bool MMG402TestCommon::TestObjRegistryStorage()
{
  std::vector<std::string> types = ObjRegistry::RegistryEntry<A>::GetTypes();
  for (unsigned int i = 0; i < types.size(); i++) {
    ASSERT_TRUE(ObjRegistry::RegistryEntry<A>::GetInstance(types[i])->GetName() == types[i]);
  }
  return true;
}
