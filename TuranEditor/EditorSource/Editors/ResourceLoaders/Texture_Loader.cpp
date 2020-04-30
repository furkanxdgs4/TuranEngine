#include "Texture_Loader.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
//To show import status
#include "EditorSource/Editors/Status_Window.h"
#include "EditorSource/Editors/Properties_Window.h"
//To send textures to GPU memory
#include "GFXSource/GFX_Core.h"

using namespace TuranAPI;

//To import textures from 3rd party data formats
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>

namespace TuranEditor {

	Texture_Import_Window::Texture_Import_Window() : IMGUI_WINDOW("Texture Import"){
		IMGUI_REGISTERWINDOW(this);
	}

	void Texture_Import_Window::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}

		std::cout << "Texture Import screen is active!\n";
		IMGUI->Input_Text("Texture Path", &TEXTURE_PATH);
		IMGUI->Input_Text("Output Folder", &OUTPUT_FOLDER);
		IMGUI->Input_Text("Output Name", &OUTPUT_NAME);
		if (IMGUI->Button("Import")) {
			String PATH;
			PATH = OUTPUT_FOLDER;
			PATH.append(OUTPUT_NAME);

			//Check if this resource is already loaded to Content_List!
			for (unsigned int i = 0; i < EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
				Resource_Type* RESOURCE = EDITOR_FILESYSTEM->Get_AssetList().Get(i);
				if (TEXTURE_PATH == RESOURCE->PATH) {
					TuranAPI::LOG_ERROR("Resource is already loaded and is in the Resource List!");
					return;
				}
			}


			//If application arrives here, Resource isn't loaded to Content_List before!
			Resource_Type* imported_resource = Texture_Loader::Import_Texture(TEXTURE_PATH, PATH);
			if (imported_resource) {
				imported_resource->Write_ToDisk(true);
				EDITOR_FILESYSTEM->Add_anAsset_toFileList(imported_resource);
				TuranAPI::LOG_STATUS("Texture is loaded successfully!");
			}
			else {
				TuranAPI::LOG_STATUS("Texture isn't loaded!");
			}
		}

		IMGUI->End_Window();
	}


	//Output Path should be Directory + Name like "C:/dev/Content/FirstTexture". Every Texture has .texturecont extension!
	TuranAPI::Resource_Type* Texture_Loader::Import_Texture(const char* PATH, const char* output_path, const bool& flip_vertically, const GFX_API::Texture_Properties& Properties) {
		GFX_API::TEXTURE_CHANNELs CHANNEL_TYPE;
		int WIDTH, HEIGHT, CHANNELs;
		stbi_set_flip_vertically_on_load(flip_vertically);
		unsigned char* data = stbi_load(PATH, &WIDTH, &HEIGHT, &CHANNELs, 0);
		if (data == nullptr) {
			new TuranEditor::Status_Window("Texture isn't loaded from source! Data is nullptr!");
			return nullptr;
		}
		switch (CHANNELs) {
		case 0:
			new TuranEditor::Status_Window("Texture isn't loaded from source! Data is nullptr!");
			return nullptr;
		case 3:
			CHANNEL_TYPE = GFX_API::TEXTURE_CHANNELs::API_TEXTURE_RGB;
			break;
		case 4:
			CHANNEL_TYPE = GFX_API::TEXTURE_CHANNELs::API_TEXTURE_RGBA;
			break;
		default:
			new TuranEditor::Status_Window("Texture isn't loaded from source! Data is nullptr!");
			return nullptr;
		}
		//If application arrives here, loading is successful!

		GFX_API::Texture_Resource* texture_resource = new GFX_API::Texture_Resource;
		texture_resource->DATA = data;
		texture_resource->WIDTH = WIDTH;
		texture_resource->HEIGHT = HEIGHT;
		texture_resource->Properties = Properties;
		std::string NAME = output_path;
		NAME = NAME.substr(NAME.find_last_of('/') + 1);
		NAME = NAME.substr(0, NAME.find_last_of('.'));
		texture_resource->NAME = NAME.c_str();
		texture_resource->PATH = output_path;
		texture_resource->PATH.append(".texturecont");
		texture_resource->DATA_SIZE = texture_resource->WIDTH * texture_resource->HEIGHT * CHANNELs;


		texture_resource->Write_ToDisk(true);
		new TuranEditor::Status_Window("Texture isn't loaded from source! Data is nullptr!");
		return texture_resource;
	}
}