#include "Texture_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
//To display import window
#include "TuranAPI/API_IMGUI.h"
//To show import status
#include "EditorSource/Editors/Status_Window.h"
#include "EditorSource/Editors/Properties_Window.h"
//To send textures to GPU memory
#include "GFXSource/GFX_Core.h"

using namespace TuranAPI::File_System;
using namespace TuranAPI::IMGUI;

//To import textures from 3rd party data formats
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


Texture_Import_Window::Texture_Import_Window(TuranAPI::File_System::FileSystem* filesystem) : IMGUI_WINDOW("Texture Import"), FILESYSTEM(filesystem){}

void Texture_Import_Window::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
	}

	cout << "Texture Import screen is active!\n";
	IMGUI::Input_Text("Texture Path", &TEXTURE_PATH);
	IMGUI::Input_Text("Output Folder", &OUTPUT_FOLDER);
	IMGUI::Input_Text("Output Name", &OUTPUT_NAME);
	if (IMGUI::Button("Import")) {
		string PATH = OUTPUT_FOLDER + OUTPUT_NAME;
		string status;

		//Check if this resource is already loaded to Content_List!
		for (Resource_Type* RESOURCE : *FILESYSTEM->Get_Const_FileListContentVector()) {
			if (TEXTURE_PATH == RESOURCE->PATH) {
				status = "Resource is already loaded and is in the Resource List!";
				Status_Window* error_window = new Status_Window(status);
				return;
			}
		}


		//If application arrives here, Resource isn't loaded to Content_List before!
		Resource_Type* imported_resource = Texture_Loader::Import_Texture(TEXTURE_PATH, &PATH, &status);
		Status_Window* error_window = new Status_Window(status);
		if (imported_resource) {
			FILESYSTEM->Add_Content_toFileList(imported_resource);
		}
	}

	IMGUI::End_Window();
}


//Output Path should be Directory + Name like "C:/dev/Content/FirstTexture". Every Texture has .texturecont extension!
TuranAPI::File_System::Resource_Type* Texture_Loader::Import_Texture(const string& PATH, const string* output_path, string* compilation_status, const bool& flip_vertically, const TuranAPI::File_System::Texture_Properties& Properties) {
	TuranAPI::TuranAPI_ENUMs CHANNEL_TYPE;
	int WIDTH, HEIGHT, CHANNELs;
	stbi_set_flip_vertically_on_load(flip_vertically);
	unsigned char* data = stbi_load(PATH.c_str(), &WIDTH, &HEIGHT, &CHANNELs, 0);
	if (data == nullptr) {
		*compilation_status = "Texture isn't loaded from source! Data is nullptr!\n";
		return nullptr;
	}
	switch (CHANNELs) {
	case 0:
		*compilation_status = "Loaded texture's channel number is 0! Nothing has happened!";
		return nullptr;
	case 3:
		CHANNEL_TYPE = TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGB;
		break;
	case 4:
		CHANNEL_TYPE = TuranAPI::TuranAPI_ENUMs::API_TEXTURE_RGBA;
		break;
	default:
		*compilation_status = "Loaded texture's channel type isn't supported for now! Channel number is: " + CHANNELs;
		return nullptr;
	}
	//If application arrives here, loading is successful!

	TuranAPI::File_System::Texture_Resource* texture_resource = new TuranAPI::File_System::Texture_Resource;
	texture_resource->DATA = data;
	texture_resource->WIDTH = WIDTH;
	texture_resource->HEIGHT = HEIGHT;
	texture_resource->Properties = Properties;
	string NAME = output_path->substr(output_path->find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	texture_resource->NAME = NAME;
	string Compiled_PATH = *output_path + ".texturecont";
	texture_resource->PATH = Compiled_PATH;
	texture_resource->DATA_SIZE = texture_resource->WIDTH * texture_resource->HEIGHT * CHANNELs;
	texture_resource->GL_ID = 0;


	TuranAPI::File_System::FileSystem::Write_a_Resource_toDisk(texture_resource);
	*compilation_status = "Texture is compiled and saved successfully!";
	return texture_resource;
}