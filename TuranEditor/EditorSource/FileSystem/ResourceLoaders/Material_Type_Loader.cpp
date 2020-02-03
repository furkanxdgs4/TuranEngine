#include "Material_Type_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
//To display import window
#include "TuranAPI/API_IMGUI.h"
//To show import status
#include "EditorSource/Editors/Status_Window.h"
//To compile shader program!
#include "GFXSource/GFX_Core.h"

using namespace TuranAPI::File_System;
using namespace TuranAPI::IMGUI;


const vector<string> Uniform_VarType_Names { "Unsigned Integer", "Signed Integer", "Float 4 Byte", "Vector 2D", "Vector 3D", "Vector 4D", "Matrix 4x4", "Texture 2D"};


Material_Import_Window::Material_Import_Window(TuranAPI::File_System::FileSystem* filesystem) : IMGUI_WINDOW("Material Type Import"), FILESYSTEM(filesystem) {}

TuranAPI::TuranAPI_ENUMs Find_in_Uniform_VarTypes(const int& var_type);

void Material_Import_Window::Run_Window() {
	vector<string> UNIFORM_VAR_TYPE_NAMEs = { "Unsigned Integer 32-bit", "Integer 32-bit", "Float 32-bit", "Vec2 (2 float)", "Vec3 (3 float)", "Vec4 (4 float)", "Matrix 4x4", "Texture 2D" };
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
	}

	cout << "Material Type Import screen is active!\n";
	IMGUI::Input_Text("Vertex Resource Path", &MATERIALTYPE_VERTEX_PATH);
	IMGUI::Input_Text("Fragment Resource Path", &MATERIALTYPE_FRAGMENT_PATH);
	IMGUI::Input_Text("Output Folder", &OUTPUT_FOLDER);
	IMGUI::Input_Text("Output Name", &OUTPUT_NAME);
	
	if (IMGUI::Button("Import")) {
		string status;
		string PATH = OUTPUT_FOLDER + OUTPUT_NAME;

		//Check if this resource is already loaded to Content_List!
		for (Resource_Type* RESOURCE : *FILESYSTEM->Get_Const_FileListContentVector()) {
			if (PATH == RESOURCE->PATH) {
				status = "Resource is already loaded and is in the Resource List!";
				Status_Window* error_window = new Status_Window(status);
				return;
			}
		}


		//A little check for each uniform! Only variable names are checked.
		for (Material_Uniform UNIFORM : Material_Uniforms) {
			if (UNIFORM.VARIABLE_NAME == "") {
				cout << "A uniform's name isn't set, please check all uniform names!\n";
				this_thread::sleep_for(chrono::seconds(8));
				IMGUI::End_Window();
				return;
			}
		}
		Resource_Type* imported_resource = Material_Type_Loader::Load_MaterialType(&MATERIALTYPE_VERTEX_PATH, &MATERIALTYPE_FRAGMENT_PATH, &PATH, &Material_Uniforms, &status);
		Status_Window* error_window = new Status_Window(status);
		if (imported_resource) {
			FILESYSTEM->Add_Content_toFileList(imported_resource);
		}

		Material_Uniforms.clear();
		selectlist_vector.clear();
		

		//Finish the window here! No status checking for now!
		Is_Window_Open = false;
		IMGUI::End_Window();
		return;
	}

	IMGUI::Same_Line();
	if (IMGUI::Button("Add Uniform")) {
		Material_Uniforms.push_back(Material_Uniform());
	}
	IMGUI::Same_Line();
	if (IMGUI::Button("Read Material Type")) {
		VERTEX_SOURCE = FileSystem::Read_TextFile(MATERIALTYPE_VERTEX_PATH);
		FRAGMENT_SOURCE = FileSystem::Read_TextFile(MATERIALTYPE_FRAGMENT_PATH);
		is_Reading_Shaders = true;
	}
	selectlist_vector.resize(Material_Uniforms.size());
	if (IMGUI::Begin_Tree("Uniform List")) {
		for (unsigned int i = 0; i < Material_Uniforms.size(); i++) {
			Material_Uniform* UNIFORM = &Material_Uniforms[i];

			if (IMGUI::Begin_Tree(to_string(i))) {
				IMGUI::Input_Text("Uniform Name", &UNIFORM->VARIABLE_NAME);
				if (IMGUI::SelectList_OneLine("Uniform Variable Type", &selectlist_vector[i], &Uniform_VarType_Names)) {
					UNIFORM->VARIABLE_TYPE = Find_in_Uniform_VarTypes(selectlist_vector[i]);
					if (UNIFORM->VARIABLE_TYPE == TuranAPI::TuranAPI_ENUMs::TURAN_NULL)
						Status_Window* error_window = new Status_Window("Error: Intended var_type index isn't supported for now in Find_in_Uniform_VarTypes! This means, UNIFORM_VARTYPEs string vector doesn't match with this function!");
				}
				IMGUI::End_Tree();
			}
		}
		IMGUI::End_Tree();
	}
	if (is_Reading_Shaders) {
		IMGUI::Text("Vertex Source:");
		IMGUI::Text(VERTEX_SOURCE);
		IMGUI::Text("Fragment Source:");
		IMGUI::Text(FRAGMENT_SOURCE + "\n\n");
	}


	IMGUI::End_Window();
}




//Use this function to import a shader stage first time! Not for reloading a shader!
//Output Path defines DIRECTORY + NAME! Like "C:/dev/Content/First_Material". Every MatType has .mattypecont extension!
TuranAPI::File_System::Resource_Type* Material_Type_Loader::Load_MaterialType(const string* vertex_path, const string* fragment_path, const string* output_path, vector<Material_Uniform>* material_inputs, string* compilation_status) {
	//Name and Save Location calculations! Name will only be used in UI for better user experience.
	string PATH = *output_path + ".mattypecont";

	//Check if this resource is already compiled and saved!
	for (Resource_Type* RESOURCE : *EDITOR_FILESYSTEM->Get_Const_FileListContentVector()) {
		if (PATH == RESOURCE->PATH) {
			*compilation_status = "Resource is already loaded before and is in the Resource List!";;
			return nullptr;
		}
	}

	//Get the source codes of the shaders!
	string vertex_source = FileSystem::Read_TextFile(*vertex_path);
	string fragment_source = FileSystem::Read_TextFile(*fragment_path);

	string gfx_compile_status = "";
	TuranAPI::LOG_NOTCODED("In Material_Type_Loader, Compile_MaterialType(&gfx_compile_status, &vertex_source, &fragment_source); should be coded in NOT_CODED's line!", true);

	if (gfx_compile_status != "") {
		*compilation_status = gfx_compile_status;
		return nullptr;
	}
	//If application arrives here, that means shader compiles perfectly!

	TuranAPI::File_System::Material_Type_Resource* MATERIAL = new TuranAPI::File_System::Material_Type_Resource();
	MATERIAL->VERTEX_SOURCE = vertex_source;
	MATERIAL->FRAGMENT_SOURCE = fragment_source;
	MATERIAL->GFX_API = TuranAPI::TuranAPI_ENUMs::OPENGL4;

	//Add Material Uniforms to compiled resource!
	for (Material_Uniform material_uniform : *material_inputs) {
		MATERIAL->UNIFORMs.push_back(material_uniform);
	}

	//Set material's other things to help saving!
	MATERIAL->PATH = PATH;
	auto NAME = PATH.substr(PATH.find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	MATERIAL->NAME = NAME;

	//For now, all of the shader's will be set as OpenGL3 shader!
	FileSystem::Write_a_Resource_toDisk(MATERIAL);
	
	*compilation_status = "Succesfully compiled and saved the resource to disk!";
	return MATERIAL;
}


TuranAPI::TuranAPI_ENUMs Find_in_Uniform_VarTypes(const int& var_type) {
	switch (var_type) {
	case 0:
		return TuranAPI::TuranAPI_ENUMs::VAR_UINT32;
	case 1:
		return TuranAPI::TuranAPI_ENUMs::VAR_INT32;
	case 2:
		return TuranAPI::TuranAPI_ENUMs::VAR_FLOAT32;
	case 3:
		return TuranAPI::TuranAPI_ENUMs::VAR_VEC2;
	case 4:
		return TuranAPI::TuranAPI_ENUMs::VAR_VEC3;
	case 5:
		return TuranAPI::TuranAPI_ENUMs::VAR_VEC4;
	case 6:
		return TuranAPI::TuranAPI_ENUMs::VAR_MAT4x4;
	case 7:
		return TuranAPI::TuranAPI_ENUMs::API_TEXTURE_2D;
	default:
		return TuranAPI::TuranAPI_ENUMs::TURAN_NULL;
	}
}