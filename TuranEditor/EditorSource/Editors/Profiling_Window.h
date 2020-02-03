#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/Profiler/Profiler_Core.h"

class Profiling_Window : public TuranAPI::IMGUI::IMGUI_WINDOW {
	TuranAPI::Profiler::Profiling_Session* Profiled_Session = nullptr;
	string PATH;
public:
	Profiling_Window();
	virtual void Run_Window() override;
};
