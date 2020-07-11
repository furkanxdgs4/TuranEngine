#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "EditorSource/FileSystem/ResourceTypes/Scene_Resource.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"

namespace TuranEditor {

	class Scene_Editor : public GFX_API::IMGUI_WINDOW {
		Scene_Resource* SCENE_to_EDIT;
		GFX_API::RenderGraph* RenderGraph_forScene;
		vector<string> item_names, component_names;
	public:
		Scene_Editor(Scene_Resource* SCENE);
		virtual void Run_Window() override;
	};

	class Scene_Create_Window : public GFX_API::IMGUI_WINDOW {
		string Scene_NAME, Scene_Folder;

	public:
		Scene_Create_Window();
		virtual void Run_Window() override;
	};
}