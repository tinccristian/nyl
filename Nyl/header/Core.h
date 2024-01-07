#pragma once

#ifdef NYL_PLATFORM_WINDOWS
	#ifdef NYL_BUILD_DLL
		#define NYL_API __declspec(dllexport)
	#else
		#define NYL_API __declspec(dllimport)
	#endif
#else
	#error Nyl only works for windows!
#endif