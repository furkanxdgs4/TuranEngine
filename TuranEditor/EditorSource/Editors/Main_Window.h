#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/RenderContext/Game_RenderGraph.h"

namespace TuranEditor {
	class Main_Window : public GFX_API::IMGUI_WINDOW {
		unsigned int Scene_View_Texture = 0;
		Game_RenderGraph* GameRenderGraph = nullptr;
	public:
		Main_Window(Game_RenderGraph* GameRenderGraph = nullptr);
		virtual void Run_Window();
	};

}