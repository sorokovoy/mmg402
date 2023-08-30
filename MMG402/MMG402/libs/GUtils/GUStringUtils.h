#ifndef GUStringUtils_H
#define GUStringUtils_H

#include <string>
#include <sstream>
#include <stdarg.h>

template<typename T>
static std::string GUToString(const T& v)
{
  std::ostringstream ss;
  ss.precision(12);
  ss << v;
  return ss.str();
}

inline static std::string GUToString(const char* ipFormat, ... )
{
  char dest[1024];
  va_list argptr;
  va_start(argptr, ipFormat);
  vsprintf_s(dest, 1024, ipFormat, argptr);
  va_end(argptr);
  return std::string(dest);
}

template<>
static std::string GUToString<std::string>(const std::string& v)
{
  return v;
}

template<typename T>
static T GUParse(const std::string& str)
{
  T oValue;
  std::istringstream ss(str);
  ss >> oValue;
  return oValue;
}

template<typename T>
static bool GUTryParse(const std::string& str, T& oValue)
{
  std::istringstream ss(str);
  ss >> oValue;
  return GUToString(oValue).find(str.substr(0, str.length() - 1)) == 0; // printed value start with source string
}

// specialized template impleGUntation for string
template<>
static bool GUTryParse<std::string>(const std::string& str, std::string& oValue)
{
  oValue = str;
  return true;
}

#endif
