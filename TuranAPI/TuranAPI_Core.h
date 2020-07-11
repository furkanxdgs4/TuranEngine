#pragma once
#include "API_includes.h"
#include "MemoryManager/MemoryManager_Core.h"
#include "TSTL/Bitset.h"
#include "Logger/Logger_Core.h"
#include "Profiler/Profiler_Core.h"
#include "FileSystem/FileSystem_Core.h"



namespace TuranAPI {
	class TURANAPI TAPI_SystemsBase {
	public:
		TMemoryManager MemoryManager;
		TAPI_SystemsBase();
		~TAPI_SystemsBase();
	};
	class TURANAPI TAPI_Systems : public TAPI_SystemsBase {
	public:
		Logging::Logger Log_Sys;
		Active_Profiling_Session Profiling_Session;
		TAPI_Systems();
		~TAPI_Systems();
	};
	void TURANAPI Start_TAPI_Systems(TAPI_Systems* InitializedSystems);
}