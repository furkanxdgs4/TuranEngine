#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"
#include "TuranAPI/FileSystem/Resource_Types/FileList_Resource.h"
#include "TuranAPI/FileSystem/Resource_Types/Scene_Resource.h"
#include "GFXSource/Renderer/GFX_RenderGraph.h"

class Scene_Editor : public TuranAPI::IMGUI::IMGUI_WINDOW {
	TuranAPI::File_System::Scene_Resource* SCENE_to_EDIT;
	GFX::RenderGraph* RenderGraph_forScene;
public:
	Scene_Editor();
	virtual void Run_Window() override;
};

class Scene_Create_Window : public TuranAPI::IMGUI::IMGUI_WINDOW {
	string Scene_NAME, Scene_Folder;
	TuranAPI::File_System::FileSystem* FILESYSTEM;


public:
	Scene_Create_Window(TuranAPI::File_System::FileSystem* filesystem);
	virtual void Run_Window() override;
};