#include "MaterialInstance_Import.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "TuranAPI/IMGUI/IMGUI_Core.h"

#include "EditorSource/Editors/Status_Window.h"

using namespace TuranAPI::File_System;
using namespace TuranAPI::IMGUI;

static Material_Instance* Create_Instance_ofMaterialType(Material_Type_Resource* material_type, vector<Material_Uniform>& Uniform_List, const string& output_path);

MaterialInstance_CreationWindow::MaterialInstance_CreationWindow(TuranAPI::File_System::FileSystem* filesystem) : IMGUI_WINDOW("Material Instance Import"), FILESYSTEM(filesystem){}

void MaterialInstance_CreationWindow::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
	}

	IMGUI::Input_Text("Output Folder", &OUTPUT_FOLDER);
	IMGUI::Input_Text("Output Name", &OUTPUT_NAME);
	string PATH = OUTPUT_FOLDER + OUTPUT_NAME;

	Material_Instance* created_matinst;
	vector<Material_Uniform> UNIFORM_LIST;

	if (IMGUI::Button("Create")) {
		Material_Instance* created_matinst = Create_Instance_ofMaterialType(selected_materialtype, UNIFORM_LIST, PATH);
		FILESYSTEM->Add_Content_toFileList(created_matinst);
	}


	vector<string> item_names;
	for (Resource_Type* RESOURCE : TuranAPI::File_System::Material_Type_Resource::ALL_MATERIAL_TYPEs) {
		item_names.push_back(RESOURCE->NAME);
	}
	static int selected_list_item_index = 0;

	//Show Material Type list and if one of them is selected, create uniforms for the material instance
	if (IMGUI::SelectList_OneLine("Material Type List", &selected_list_item_index, &item_names)) {
		selected_materialtype = TuranAPI::File_System::Material_Type_Resource::ALL_MATERIAL_TYPEs[selected_list_item_index];
		for (Material_Uniform UNIFORM : selected_materialtype->UNIFORMs) {
			UNIFORM_LIST.push_back(UNIFORM);
		}
	}

	if (IMGUI::Begin_Tree("Uniform List")) {
		for (unsigned int i = 0; i < UNIFORM_LIST.size(); i++) {
			Material_Uniform* UNIFORM = &UNIFORM_LIST[i];

			if (IMGUI::Begin_Tree(to_string(i))) {
				IMGUI::Input_Text("Uniform Name", &UNIFORM->VARIABLE_NAME);
				
				IMGUI::End_Tree();
			}
		}
		IMGUI::End_Tree();
	}


	IMGUI::End_Window();
}


//Output Path should be Directory + Name. Like "C:/dev/Content/First". Every Material Instance has .matinstcont extension!
TuranAPI::File_System::Material_Instance* Create_Instance_ofMaterialType(TuranAPI::File_System::Material_Type_Resource* material_type, vector<Material_Uniform>& UNIFORM_LIST, const string& output_path) {
	Material_Instance* MATERIAL_INSTANCE = new Material_Instance;
		MATERIAL_INSTANCE->ID = FileSystem::Create_Resource_ID();
	MATERIAL_INSTANCE->Material_Type = material_type;
	string NAME = output_path.substr(output_path.find_last_of('/') + 1);
	NAME = NAME.substr(0, output_path.find_last_of('.'));
	MATERIAL_INSTANCE->NAME = NAME;
	MATERIAL_INSTANCE->UNIFORM_LIST = material_type->UNIFORMs;
	string PATH = output_path + ".matinstcont";
	cout << "Path of Material Instance is: " << PATH << endl;
	MATERIAL_INSTANCE->PATH = PATH;
	return MATERIAL_INSTANCE;
}