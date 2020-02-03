#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/API_FileSystem.h"

class MaterialInstance_CreationWindow : public TuranAPI::IMGUI::IMGUI_WINDOW {
	TuranAPI::File_System::Material_Type_Resource* selected_materialtype = TuranAPI::File_System::Material_Type_Resource::ALL_MATERIAL_TYPEs[0];
	string OUTPUT_FOLDER, OUTPUT_NAME;
	TuranAPI::File_System::FileSystem* FILESYSTEM;
public:
	MaterialInstance_CreationWindow(TuranAPI::File_System::FileSystem* filesystem);
	virtual void Run_Window() override;
};

