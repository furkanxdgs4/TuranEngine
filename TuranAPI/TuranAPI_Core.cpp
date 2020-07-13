#include "TuranAPI_Core.h"

namespace TuranAPI {
	TAPI_SystemsBase::TAPI_SystemsBase() : MemoryManager(1024 * 1024 * 30) {

	}
	TAPI_SystemsBase::~TAPI_SystemsBase() {
		std::cout << "Memory Manager will close soon enough!\n";
	}
	TAPI_Systems::TAPI_Systems() : Profiling_Session("C:/Users/furka/Desktop/ProfilingData.txt") {
		std::cout << "TAPI Systems' constructor has finished!\n";
	}
	TAPI_Systems::~TAPI_Systems() {
		std::cout << "TAPI_Systems destructor started!\n";
		delete Logging::Logger::SELF;
	}
	void Start_TAPI_Systems(TAPI_Systems* InitializedSystems) {
		MemoryManagement::TMemoryManager::SELF = &(InitializedSystems->MemoryManager);
		TuranAPI::Logging::Logger::SELF = &(InitializedSystems->Log_Sys);
		TuranAPI::Active_Profiling_Session::SELF = (TuranAPI::Profiling_Session*)&(InitializedSystems->Profiling_Session);
		TuranAPI::LOG_STATUS("TuranAPI DLL got the pointers!");
	}
}