#pragma once

#define ENABLE_PROFILING 1

#if ENABLE_PROFILING 
#include "../Vendor/Optick/src/optick.h"
#endif

#if ENABLE_PROFILING
#define PROFILE_FRAME(...)           OPTICK_FRAME(__VA_ARGS__)
#define PROFILE_FUNC(...)            OPTICK_EVENT(__VA_ARGS__)
#define PROFILE_TAG(NAME, ...)       OPTICK_TAG(NAME, __VA_ARGS__)
#define PROFILE_SCOPE_DYNAMIC(NAME)  OPTICK_EVENT_DYNAMIC(NAME)
#define PROFILE_THREAD(...)          OPTICK_THREAD(__VA_ARGS__)
#else
#define PROFILE_FRAME(...)
#define PROFILE_FUNC(...)
#define PROFILE_TAG(NAME, ...) 
#define PROFILE_SCOPE_DYNAMIC(NAME)
#define PROFILE_THREAD(...)
#endif
