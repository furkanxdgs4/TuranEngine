#pragma once
#include "ENGINE_ENUMs.h"
#include "TuranAPI/API_includes.h"
#include "TuranAPI/API_Logger.h"
#include "TuranAPI/API_Profiler.h"


	//COMPILING PROCESS
#ifdef WINDOWS10_FORENGINE
	#ifdef ENGINE_BUILD
		#define ENGINE __declspec(dllexport) 
	#else
		#define ENGINE __declspec(dllimport)
	#endif
#else
	#error "TuranEngine only supports Windows 10 for now!"
#endif // _WINDOWS_

