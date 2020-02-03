#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/IMGUI/IMGUI_WINDOW.h"
#include "TuranAPI/API_FileSystem.h"

class Model_Import_Window : public TuranAPI::IMGUI::IMGUI_WINDOW {
	string MODEL_IMPORT_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "";
	//This is used to find which Content List we are importing the model to! For example GFX_Contents, Engine_Contents, GameContents... 
	TuranAPI::File_System::FileSystem* FILESYSTEM;
public:
	Model_Import_Window(TuranAPI::File_System::FileSystem* filesystem);
	virtual void Run_Window() override;
};

class Model_Loader {
public:
	static TuranAPI::File_System::Resource_Type* Import_Model(const string& PATH, const string* output_path, string* compilation_status);
};
