// COPYRIGHT: Open source by Academic Free License ("AFL") v. 3.0
//===================================================================
//
// Functions and macros for safe and powerful checks
//
//===================================================================
//  OCT 2012  Creation:                       Sorokovoy Alexey, NSU
//===================================================================

#ifndef GUAssert_H
#define GUAssert_H

#define GU_ASSERTS

#ifdef GU_ASSERTS

#include <sstream>
#include <stdio.h>

#if defined(_MSC_VER) && _MSC_VER >= 1400
  #include <intrin.h>
#endif

enum GUAssertMode {
  amNone,
  amWriteToConsole,
  amBreak,
};

#define CURRENT_ASSERT_SETTINGS amWriteToConsole

#define GUAssertError(iExp, ipMGUsage) \
{                                      \
  if(CURRENT_ASSERT_SETTINGS == amBreak) { \
    __debugbreak();                    \
  }                                    \
                                       \
  if(CURRENT_ASSERT_SETTINGS == amWriteToConsole) { \
    fprintf(stdout, "Assertion failed %s at %s:%s in %s m=%s", iExp, __FILE__, __LINE__, __FUNCTION__, ipMGUsage);  \
  }                                    \
                                       \
}                                      \

#define GUAssert(exp) \
  if (!(exp)) { \
    GUAssertError(#exp, ""); \
  }

#define GUAssertT(exp, mGUsage) \
  if (!(exp)) { \
    std::ostringstream msg; \
    msg << mGUsage; \
    GUAssertError(#exp, msg.str().c_str()); \
  }

#define GUAssertR(exp) \
  if (!(exp)) { \
    GUAssertError(#exp, ""); \
    return; \
  }

#define GUAssertV(exp, ret) \
  if (!(exp)) { \
    GUAssertError(#exp, ""); \
    return ret; \
  }

#define GUAssertB(exp) \
  if (!(exp)) { \
    GUAssertError(#exp, ""); \
    break; \
  }

#define GUAssertC(exp) \
  if (!(exp)) { \
    GUAssertError(#exp, ""); \
    continue; \
  }

#else

#define GUAssert(exp)

#define GUAssertT(exp, mGUsage)

#define GUAssertR(exp) \
  if (!(exp)) { \
    return; \
  }

#define GUAssertV(exp, ret) \
  if (!(exp)) { \
    return ret; \
  }

#define GUAssertB(exp) \
  if (!(exp)) { \
    break; \
  }

#define GUAssertC(exp) \
  if (!(exp)) { \
    continue; \
  }

#endif // if not defined AI_ASSERTS

#endif // GUAssert_h
