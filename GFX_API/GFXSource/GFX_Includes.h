#pragma once
#include "TuranAPI/API_includes.h"
#include "GFX_ENUMs.h"
#include "TuranAPI/API_Logger.h"
#include "TuranAPI/API_Profiler.h"

#ifdef WINDOWS10_FORENGINE
	#ifdef GFX_BUILD
		#define GFXAPI __declspec(dllexport)
	#else
		#define GFXAPI __declspec(dllimport)
	#endif // GFX_API
#else
	#error GFX_API only support Windows 10 for now!
#endif