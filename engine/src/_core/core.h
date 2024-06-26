#include <cstdlib> // Add the missing include directive for the <cstdlib> header file

 #ifdef _WIN32
 #ifdef NYL_BUILD_DLL
 #define NASSERT(x) \
     { \
         if((x) == 0) {__debugbreak();abort();}}   // break into the debugger and use the abort function from the <stdlib.h> header to terminate the program if the assertion fail
 #define NYL_API __declspec(dllexport)             // define NYL_API as __declspec(dllexport) when building a DLL to export functions
 #else
 #define NYL_API __declspec(dllimport)             // define NYL_API as __declspec(dllimport) when using the DLL to import functions
 #endif
 //#else
 //#error nyl only works for windows!  // Display an error if nyl is used on a platform other than Windows
 #else
#define NYL_API
#endif
