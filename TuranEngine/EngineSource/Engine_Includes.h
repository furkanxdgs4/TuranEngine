#pragma once
#include "ENGINE_ENUMs.h"
#include "TuranAPI/TuranAPI_Core.h"


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

