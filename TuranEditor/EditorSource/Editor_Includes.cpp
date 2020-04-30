#include "Editor_Includes.h"

using namespace TuranEditor;

TE_DependentSystems::TE_DependentSystems() {
	TuranAPI::Start_TAPI_Systems(&TAPISystems);
	TMemoryManager::SELF = &TAPISystems.MemoryManager;
	TuranAPI::Logging::Logger::SELF = &TAPISystems.Log_Sys;
	TuranAPI::Active_Profiling_Session::SELF = (TuranAPI::Profiling_Session*)&(TAPISystems.Profiling_Session);

	TuranEngine = Engine::Start_EngineDLL(&TAPISystems);
	GFX = TuranEngine->Get_GFXSystems();
}
TE_DependentSystems::~TE_DependentSystems() {
	GFX = nullptr;
	TuranAPI::Logging::Logger::SELF = nullptr;
	TMemoryManager::SELF = nullptr;

	TuranEngine = nullptr;
	Engine::Close_EngineDLL();
	std::cout << "All systems should be shut down!\n";
}