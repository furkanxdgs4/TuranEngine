#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

namespace TuranEditor {

	class IMGUI_DemoWindow : public GFX_API::IMGUI_WINDOW {
	public:
		IMGUI_DemoWindow();
		virtual void Run_Window() override;
	};

	class IMGUI_MetricsWindow : public GFX_API::IMGUI_WINDOW {
	public:
		IMGUI_MetricsWindow();
		virtual void Run_Window() override;
	};


}