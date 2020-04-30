#pragma once
#include "EditorSource/Editor_Includes.h"
#include "GFXSource/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"

class MaterialInstance_CreationWindow : public GFX_API::IMGUI_WINDOW {
	GFX_API::Material_Type_Resource* selected_materialtype = nullptr;
	String OUTPUT_FOLDER, OUTPUT_NAME, UNIFORM_NAME;
public:
	MaterialInstance_CreationWindow();
	virtual void Run_Window() override;
};

