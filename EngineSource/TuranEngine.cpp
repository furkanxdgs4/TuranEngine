#include "TuranEngine.h"
#include <windows.h>

#include "GFXSource/GFX_Core.h"
#include "OpenGL4Source/OGL4_Core.h"

namespace Engine {

	/*
			Turan Engine Running Process:
		1) Load Model_Files when Model_Loader_OBJ is created!
		2) For now: Create a Static_Model_Instance from first loaded Model!
		3) For now: Create a Scene and Add every Static_Model_Instance to Scene, no Camera!
		4) Create OGL3_SYS to initialize GLFW and OpenGL and create an offscreen and an onscreen window!
		5) For now: Call GFX->Creation() -> Create a Main_RenderGraph and give its pointer to Scene!
		6) For now: Create resources of Main_RenderGraph of the Scene! This means: Send Models to GPU, Create Draw Passes (Framebuffers etc.) -Only G-Buffer for now!-
		7)
	*/
	TuranEngine_Core* TuranEngine_Core::SELF = nullptr;

	TuranEngine_Core::TuranEngine_Core(TuranAPI::TAPI_Systems* TAPISystems) : ApplicationClose_Reason("Nothing!\n") {
		TuranAPI::LOG_STATUS("TuranEngine's constructor has started!");
		TuranEngine_Core::SELF = this;
		Start_GFX(TAPISystems);
		TuranAPI::LOG_STATUS("TuranEngine's constructor has finished!");
	}
	TuranEngine_Core::~TuranEngine_Core() {
		std::cout << "TuranEngine's destructor is called!\n";
		OpenGL4::Close_OGL4DLL();
		std::cout << "TuranEngine's destructor has finished!\n";
	}

	void TuranEngine_Core::Start_GFX(TuranAPI::TAPI_Systems* TAPI_Systems) {
		GFX = OpenGL4::Start_OGL4Systems(TAPI_Systems);
	}

	GFX_API::GFX_Core* TuranEngine_Core::Get_GFXSystems() {
		return GFX;
	}

	void TuranEngine_Core::Close_Engine(const char* reason) {
		ApplicationClose_Reason = reason;
		ShouldApplicationClose = true;
		std::cout << ApplicationClose_Reason << std::endl;
	}

	//In final product, Input Handling will use OS functionality, not GLFW!
	void TuranEngine_Core::Take_Inputs() {
		GFX->Take_Inputs();
		if (GFX->Main_Window == nullptr) {
			ShouldApplicationClose = true;
			ApplicationClose_Reason = "GFX_API is closed because of no window is active!\n";
			std::cout << ApplicationClose_Reason << std::endl;
		}
	}


	//Check if something called to Close_Engine() or check predefined circumstances!
	bool TuranEngine_Core::ShouldEngine_Close() {
		if (!ShouldApplicationClose) {
			if (!IMGUI->Is_IMGUI_Open) {
				SELF->ShouldApplicationClose = true;
				SELF->ApplicationClose_Reason = "IMGUI main window is closed!";
				std::cout << SELF->ApplicationClose_Reason << std::endl;
				return true;
			}

			//If there isn't any window, close the application!
			if (GFX->Main_Window == nullptr) {
				SELF->ApplicationClose_Reason = "All windows are closed!";
				std::cout << SELF->ApplicationClose_Reason << std::endl;
				return true;
			}
			std::cout << "Engine should run this frame!\n";
			//If there isn't any close event, return false;
			return false;
		}
		else {
			if (SELF->ApplicationClose_Reason.length() == 0) {
				std::cout << "Application Close Reason isn't set!\n";
			}
			else {
				std::cout << SELF->ApplicationClose_Reason << std::endl;
			}
			return true;
		}
	}

	const char* TuranEngine_Core::Why_Engine_Closed() {
		std::cout << "Application Closing Reason: " << ApplicationClose_Reason << std::endl;
		return ApplicationClose_Reason.c_str();
	}

	void TuranEngine_Core::SetEngineClosing_Callback(ShouldEngineWait_Callback Callback_Function) {

	}

	ShouldEngineWait_Callback TuranEngine_Core::GetEngineClosing_Callback() {
		return Waiting_inClose_Callback;
	}

	void Engine_Closing_Operations() {
		TuranAPI::WRITE_LOGs_toFILEs();
	}


	TuranEngine_Core* Start_EngineDLL(TuranAPI::TAPI_Systems* TAPISystems) {
		std::cout << "Start_EngineDLL is called!\n";
		TMemoryManager::SELF = &TAPISystems->MemoryManager;
		TuranAPI::Logging::Logger::SELF = &TAPISystems->Log_Sys;
		TuranAPI::Active_Profiling_Session::SELF = (TuranAPI::Profiling_Session*)&(TAPISystems->Profiling_Session);
		std::cout << "Start_EngineDLL has finished, calling new TuranEngine_Core!\n";
		return new TuranEngine_Core(TAPISystems);
	}

	void Close_EngineDLL() {
		std::cout << "Closing TuranEngine DLL!\n";
		delete TuranEngine;
	}
}