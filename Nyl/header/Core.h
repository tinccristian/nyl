#pragma once
#include <stdlib.h>

#ifdef NYL_PLATFORM_WINDOWS
#ifdef NYL_BUILD_DLL
#define NASSERT(x) \
    { \
        if((x) == 0) {__debugbreak();abort();}}   // break into the debugger and use the abort function from the <stdlib.h> header to terminate the program if the assertion fails

#define NYL_API __declspec(dllexport)             // define NYL_API as __declspec(dllexport) when building a DLL to export functions
#else
#define NYL_API __declspec(dllimport)             // define NYL_API as __declspec(dllimport) when using the DLL to import functions
#endif
#else
#error Nyl only works for windows!  // Display an error if Nyl is used on a platform other than Windows
#endif
