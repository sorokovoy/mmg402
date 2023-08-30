#ifndef MMG402TestingRegistryFucntion_H
#define MMG402TestingRegistryFucntion_H

#include <vector>
#include "MMG402\libs\MExp\MESymbol.h"
#include "MMG402\libs\MExp\MESymbolFunc.h"

#define DEFINE_1D_VAR \
  std::vector<MExp::MESymbol> vars;    \
  MExp::MESymbol t("t");            \
  MExp::MESymbol x("x");            \
  vars.push_back(t);          \
  vars.push_back(x);          \

#define DEFINE_2D_VAR     \
  DEFINE_1D_VAR;          \
  MExp::MESymbol y("y");        \
  vars.push_back(y);      \

#define DEFINE_3D_VAR     \
  DEFINE_2D_VAR;          \
  MExp::MESymbol z("z");        \
  vars.push_back(z);      \

#define DEFINE_4D_VAR     \
  DEFINE_3D_VAR;          \
  MExp::MESymbol v("v");        \
  vars.push_back(v);      \


class MMG402TestingRegistryFucntion
{
public:
  static const MMG402TestingRegistryFucntion& GetInstance()
  {
    static MMG402TestingRegistryFucntion instance;
    return instance;
  }

  unsigned int MMG402TestingRegistryFucntion::GetTFucntionCount(unsigned int iDim) const
  {
    if (iDim >= _tfunctions.size()) {
      return 0;
    }
    return _tfunctions[iDim].size();
  }

  const MExp::MESymbol& MMG402TestingRegistryFucntion::GetTFunction(unsigned int iDim, unsigned int iFuncID) const
  {
    if (iDim >= _tfunctions.size() || iFuncID >= _tfunctions[iDim].size()) {
      throw new std::invalid_argument("Out of range");
    }
    return _tfunctions[iDim][iFuncID];
  }


  const std::vector<MExp::MESymbol>& GetVars() const
  {
    return _vars;
  }

private:
  MMG402TestingRegistryFucntion()
  {
    DEFINE_4D_VAR;

    // create 0D Functions
    std::vector<MExp::MESymbol> func0D;
    func0D.push_back(0);
    func0D.push_back(t + 5);

    // create 1D Functions
    std::vector<MExp::MESymbol> func1D;
    func1D.push_back(x + t);
    func1D.push_back(1);
    func1D.push_back(x * x + x + 5);

    // create 2D Functions
    std::vector<MExp::MESymbol> func2D;
    func2D.push_back(x + y + t);
    func2D.push_back(1);
    func2D.push_back(x * x + y + MExp::MEExp(y) - 5 + t * t);

    // create 3D Functions
    std::vector<MExp::MESymbol> func3D;
    func3D.push_back(x + y + z + t);
    func3D.push_back(1);
    func3D.push_back(3 + x * x + y + MExp::MEExp(y) + z * z - 5 + t * t);

    // create 4D Functions
    std::vector<MExp::MESymbol> func4D;
    func4D.push_back(x + y + z + v);
    func4D.push_back(1);
    func4D.push_back(x * x + y + MExp::MEExp(y) + z * z + v * v - 5 + t * t);

    _tfunctions.push_back(func0D);
    _tfunctions.push_back(func1D);
    _tfunctions.push_back(func2D);
    _tfunctions.push_back(func3D);
    _tfunctions.push_back(func4D);

    _vars = vars;
  }

  MMG402TestingRegistryFucntion(const MMG402TestingRegistryFucntion&) {};

  std::vector<std::vector<MExp::MESymbol>> _tfunctions;
  std::vector<MExp::MESymbol> _vars;
};

#endif
