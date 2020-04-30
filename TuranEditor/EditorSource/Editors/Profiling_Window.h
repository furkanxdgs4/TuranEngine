#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/Profiler/Profiler_Core.h"

namespace TuranEditor {
	class Profiling_Window : public GFX_API::IMGUI_WINDOW {
		TuranAPI::Profiling_Session* Profiled_Session = nullptr;
		String PATH;
	public:
		Profiling_Window();
		virtual void Run_Window() override;
	};
}