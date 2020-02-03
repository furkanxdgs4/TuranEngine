#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/API_IMGUI.h"
#include "TuranAPI/API_FileSystem.h"

class Material_Import_Window : public TuranAPI::IMGUI::IMGUI_WINDOW {
	string MATERIALTYPE_VERTEX_PATH = "", MATERIALTYPE_FRAGMENT_PATH = "", OUTPUT_FOLDER = "", OUTPUT_NAME = "", VERTEX_SOURCE = "", FRAGMENT_SOURCE = "";
	bool is_Reading_Shaders = false;
	vector<TuranAPI::File_System::Material_Uniform> Material_Uniforms;
	vector<int> selectlist_vector;

	//This is used to find which Content List we are importing the texture to! For example GFX_Contents, Engine_Contents, GameContents... 
	TuranAPI::File_System::FileSystem* FILESYSTEM;
public:
	Material_Import_Window(TuranAPI::File_System::FileSystem* filesystem);
	virtual void Run_Window();
};

class Material_Type_Loader {
public:
	static TuranAPI::File_System::Resource_Type* Load_MaterialType(const string* vertex_path, const string* fragment_path, const string* output_path, vector<TuranAPI::File_System::Material_Uniform>* material_inputs, string* compilation_status);
};

