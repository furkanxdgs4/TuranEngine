#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"
#include "EditorSource/FileSystem/ResourceTypes/Scene_Resource.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"

namespace TuranEditor {

	class Scene_Editor : public GFX_API::IMGUI_WINDOW {
		Scene_Resource* SCENE_to_EDIT;
		GFX_API::RenderGraph* RenderGraph_forScene;
		Vector<String> item_names, component_names;
	public:
		Scene_Editor();
		virtual void Run_Window() override;
	};

	class Scene_Create_Window : public GFX_API::IMGUI_WINDOW {
		String Scene_NAME, Scene_Folder;

	public:
		Scene_Create_Window();
		virtual void Run_Window() override;
	};
}