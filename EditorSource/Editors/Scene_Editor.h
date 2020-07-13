#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"

namespace TuranEditor {

	class Scene_Editor : public GFX_API::IMGUI_WINDOW {
		Resource_Identifier* SCENE_to_EDIT;
		GFX_API::RenderGraph* RenderGraph_forScene;
		vector<string> item_names, component_names;
	public:
		Scene_Editor(Resource_Identifier* SCENE);
		virtual void Run_Window() override;
	};

	class Scene_Create_Window : public GFX_API::IMGUI_WINDOW {
		string Scene_NAME, Scene_Folder;

	public:
		Scene_Create_Window();
		virtual void Run_Window() override;
	};
}