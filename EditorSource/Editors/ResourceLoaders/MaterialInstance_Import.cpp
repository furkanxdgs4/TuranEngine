#include "MaterialInstance_Import.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "GFXSource/GFX_Core.h"
#include "TuranAPI/FileSystem/FileSystem_Core.h"

#include "EditorSource/Editors/Status_Window.h"
#include <string>

using namespace TuranAPI;
using namespace TuranEditor;

static GFX_API::Material_Instance* Create_Instance_ofMaterialType(GFX_API::Material_Type* material_type, vector<GFX_API::Material_Uniform>& Uniform_List, const char* output_path);

MaterialInstance_CreationWindow::MaterialInstance_CreationWindow() : IMGUI_WINDOW("Material Instance Import") {
	IMGUI_REGISTERWINDOW(this);
}

void MaterialInstance_CreationWindow::Run_Window() {
	if (!Is_Window_Open) {
		IMGUI_DELETEWINDOW(this);
		return;
	}
	if (!IMGUI->Create_Window(Window_Name.c_str(), Is_Window_Open, false)) {
		IMGUI->End_Window();
	}

	IMGUI->Input_Text("Output Folder", &OUTPUT_FOLDER);
	IMGUI->Input_Text("Output Name", &OUTPUT_NAME);
	string PATH = OUTPUT_FOLDER;
	PATH.append(OUTPUT_NAME);

	GFX_API::Material_Instance* created_matinst;
	vector<GFX_API::Material_Uniform> UNIFORM_LIST;

	if (IMGUI->Button("Create")) {
		GFX_API::Material_Instance* created_matinst = Create_Instance_ofMaterialType(selected_materialtype, UNIFORM_LIST, PATH.c_str());
		created_matinst->Write_ToDisk(true);
		EDITOR_FILESYSTEM->Add_anAsset_toFileList(created_matinst);
	}


	vector<const char*> item_names;
	for (unsigned int i = 0; i < TuranEditor::CONTENTMANAGER->Read_ALLMaterialTypes()->size(); i++) {
		Resource_Type* RESOURCE = (*TuranEditor::CONTENTMANAGER->Read_ALLMaterialTypes())[i];
		item_names.push_back(RESOURCE->NAME.c_str());
	}
	static int selected_list_item_index = 0;
	
	const char* nameof_selectlistline = "Material Type List";
	unsigned int selected_listitemindex = selected_list_item_index;

	//Show Material Type list and if one of them is selected, create uniforms for the material instance
	GFX_API::Material_Uniform* UNIFORM = nullptr;
	if (IMGUI->SelectList_OneLine(nameof_selectlistline, &selected_listitemindex, &item_names)) {
		selected_materialtype = (*TuranEditor::CONTENTMANAGER->Read_ALLMaterialTypes())[selected_list_item_index];
		for (unsigned int i = 0; i < selected_materialtype->UNIFORMs.size(); i++) {
			UNIFORM = &selected_materialtype->UNIFORMs[i];
			UNIFORM_LIST.push_back(*UNIFORM);
		}
	}

	if (IMGUI->Begin_Tree("Uniform List")) {
		for (unsigned int i = 0; i < UNIFORM_LIST.size(); i++) {
			UNIFORM = &UNIFORM_LIST[i];

			if (IMGUI->Begin_Tree(std::to_string(i).c_str())) {
				IMGUI->Input_Text("Uniform Name", &UNIFORM_NAME);
				
				IMGUI->End_Tree();
			}
		}
		IMGUI->End_Tree();
	}


	IMGUI->End_Window();
}


//Output Path should be Directory + Name. Like "C:/dev/Content/First". Every Material Instance has .matinstcont extension!
GFX_API::Material_Instance* Create_Instance_ofMaterialType(GFX_API::Material_Type* material_type, vector<GFX_API::Material_Uniform>& UNIFORM_LIST, const char* output_path) {
	GFX_API::Material_Instance* MATERIAL_INSTANCE = new GFX_API::Material_Instance;
	MATERIAL_INSTANCE->Material_Type = material_type;
	std::string NAME = output_path;
	NAME = NAME.substr(NAME.find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	MATERIAL_INSTANCE->NAME = NAME.c_str();
	MATERIAL_INSTANCE->UNIFORM_LIST = material_type->UNIFORMs;
	MATERIAL_INSTANCE->PATH = output_path;
	MATERIAL_INSTANCE->PATH.append(".matinstcont");
	std::cout << "Path of Material Instance is: " << MATERIAL_INSTANCE->PATH << std::endl;
	return MATERIAL_INSTANCE;
}