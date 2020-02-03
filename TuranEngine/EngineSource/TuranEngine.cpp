#include "TuranEngine.h"
#include "TuranAPI/API_FileSystem.h"
#include "TuranAPI/API_IMGUI.h"
#include "TuranAPI/API_Logger.h"

#include "GFXSource/GFX_Core.h"

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

bool TuranEngine::ShouldApplicationClose = false;
string TuranEngine::ApplicationClose_Reason = "";
GFX::GFX_Core* TuranEngine::GFX = nullptr;

TuranEngine::TuranEngine() {
	TuranAPI::LOG_CRASHING("Turan Engine doesn't work as object, please don't instantinate! Nothing happened!");
}

/*Progress:
1) Detect Computer Specs (only Set OpenGL3 for now!)
2) Start GFX_API to display something!
3) Start File_System to find files!
Note: Load resources later by another function!
*/
void TuranEngine::Start_Engine() {
	TuranAPI::LOG_STATUS("Engine is starting!");
	SYSTEM_INSPECTOR::Detect_Computer_Specs();
	Start_GFX();
	TuranAPI::Logging::Logger::Start_LoggingSystem();
	TuranAPI::LOG_STATUS("Started the engine!");
}

void TuranEngine::Start_GFX() {
	GFX = SYSTEM_INSPECTOR::Create_Best_GFX_API();
}


void TuranEngine::Close_Engine(string reason) {
	ApplicationClose_Reason = reason;
	ShouldApplicationClose = true;
}

//In final product, Input Handling will use OS functionality, not GLFW!
void TuranEngine::Take_Inputs() {
	GFX->Take_Inputs();
	if (GFX->Get_Window_List().size() == 0) {
		ShouldApplicationClose = true;
		ApplicationClose_Reason = "GFX_API is closed because of no window is active!\n";
	}
}

void Engine_Closing_Operations();

//Check if something called to Close_Engine() or check predefined circumstances!
bool TuranEngine::ShouldEngine_Close() {
	if (!ShouldApplicationClose) {
		//There is no crashing error so far, so check all of the engine to see if there is an close event!
		//If close event, return true to close the app!

		if (!TuranAPI::IMGUI::IMGUI::Is_IMGUI_Open) {
			ShouldApplicationClose = true;
			ApplicationClose_Reason = "IMGUI main window is closed!";
			Engine_Closing_Operations();
			return true;
		}

		//If there isn't any window, close the application!
		if (GFX->Get_Window_List().size() == 0) {
			ApplicationClose_Reason = "All windows are closed!";
			Engine_Closing_Operations();
			return true;
		}

		//If there isn't any close event, return false;
		return false;
	}
	else {
		Engine_Closing_Operations();
		return true;
	}
}

string TuranEngine::Why_Engine_Closed() {
	return ApplicationClose_Reason;
}

void Engine_Closing_Operations() {
	//IMGUI, GLFW and GLAD resources are destroyed by GFX!
	TuranEngine::GFX->Destroy_GFX_Resources();
}