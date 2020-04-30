#pragma once
#include "API_includes.h"
#include "MemoryManager/MemoryManager_Core.h"
#include "TSTL/TSTL_Core.h"
#include "TSTL/Bitset.h"
#include "Logger/Logger_Core.h"
#include "Profiler/Profiler_Core.h"
#include "FileSystem/FileSystem_Core.h"

#include "FileSystem/Resource_Types/Resource_Type.h"


namespace TuranAPI {
	class TURANAPI TAPI_Systems {
	public:
		TMemoryManager MemoryManager;
		Logging::Logger Log_Sys;
		Active_Profiling_Session Profiling_Session;
		TAPI_Systems();
		~TAPI_Systems();
	};
	void TURANAPI Start_TAPI_Systems(TAPI_Systems* InitializedSystems);
}