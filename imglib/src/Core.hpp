#pragma once

#ifdef IMGLIB_PLATFORM_WINDOWS
	#ifdef IMGLIB_BUILD_DLL
		#define IMGLIB_API __declspec(dllexport)
	#else
		#define IMGLIB_API __declspec(dllimport)
	#endif 
#else
	#error imglib supports only Windows
#endif
