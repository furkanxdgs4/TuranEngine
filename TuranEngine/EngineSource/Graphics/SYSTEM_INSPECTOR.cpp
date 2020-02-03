#include "SYSTEM_INSPECTOR.h"
#include "OpenGL4Source/OGL4_Core.h"


TuranAPI::TuranAPI_ENUMs SYSTEM_INSPECTOR::BEST_GFX_API;

void SYSTEM_INSPECTOR::Detect_Computer_Specs() {
	BEST_GFX_API = TuranAPI::TuranAPI_ENUMs::OPENGL4;
}

GFX::GFX_Core* SYSTEM_INSPECTOR::Create_Best_GFX_API() {
	return new OpenGL4::OpenGL4_Core;
}