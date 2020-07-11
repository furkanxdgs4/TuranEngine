#include "Content_Browser.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

#include "Status_Window.h"
#include "Properties_Window.h"

#include "ResourceLoaders/Material_Type_Loader.h"
#include "ResourceLoaders/MaterialInstance_Import.h"
#include "ResourceLoaders/Model_Loader.h"
#include "ResourceLoaders/Texture_Loader.h"
#include "EditorSource/Editors/Scene_Editor.h"

namespace TuranEditor {
	using namespace GFX_API;
	GameContent_Browser::GameContent_Browser() : IMGUI_WINDOW("GameContent Browser"), Asset_CheckList(100, LASTUSEDALLOCATOR), item_names(LASTUSEDALLOCATOR, 10, 100) {
		IMGUI_REGISTERWINDOW(this);
	}
	GameContent_Browser::~GameContent_Browser() {
		std::cout << "GameContent Browser destructor is called!\n";
	}

	void GameContent_Browser::Run_Window() {
		if (!Is_Window_Open) {
			IMGUI_DELETEWINDOW(this);
			return;
		}
		if (!IMGUI->Create_Window(Window_Name, Is_Window_Open, false)) {
			IMGUI->End_Window();
			return;
		}
		if (GameContent_EditMode) {
			if (item_names.size() != EDITOR_FILESYSTEM->Get_AssetList().size()) {
				item_names.clear();
				for (unsigned int i = 0; i < EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
					TuranAPI::Resource_Type* RESOURCE = EDITOR_FILESYSTEM->Get_AssetList().Get(i);
					item_names.push_back(RESOURCE->NAME);
				}
			}

			if (IMGUI->Button("Delete All")) {
				EDITOR_FILESYSTEM->Clear_AllFileList();
				Asset_CheckList.Clear(false);
				GameContent_EditMode = false;
			}
			IMGUI->Same_Line();
			if (IMGUI->Button("Delete")) {
				if (EDITOR_FILESYSTEM->Get_AssetList().size() > Asset_CheckList.GetByte_Length() * 8) {
					TuranAPI::LOG_STATUS("Bitset's size isn't enough to create a Checklist for all Assets! Expanding its size!");
					unsigned int expand_size = (EDITOR_FILESYSTEM->Get_AssetList().size() - (Asset_CheckList.GetByte_Length() * 8)) / 8;
					Asset_CheckList.Expand(expand_size);
				}
				for (unsigned int i = 0; i < EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
					bool should_erase = Asset_CheckList.GetBit_Value(i);
					if (should_erase) {
						TuranAPI::Resource_Type* resource_to_delete = EDITOR_FILESYSTEM->Get_AssetList()[i];
						EDITOR_FILESYSTEM->Delete_anAsset_fromFileList(resource_to_delete);
						std::cout << "Deleted an item!\n";
					}
				}
				Asset_CheckList.Clear(false);
				GameContent_EditMode = false;
			}
			IMGUI->Same_Line();
			if (IMGUI->Button("Cancel")) {
				Asset_CheckList.Clear(false); 
				GameContent_EditMode = false;
			}
			else {
				IMGUI->CheckListBox("Game Content List", &Asset_CheckList, &item_names);
			}

			IMGUI->End_Window();
			return;
		}
		else {
			if (IMGUI->Button("Edit List")) {
				GameContent_EditMode = true;
			}
			
			IMGUI->Same_Line();
			//Import Buttons
			{
				if (IMGUI->Button("Import Model")) { new Model_Import_Window; }
				IMGUI->Same_Line();
				if (IMGUI->Button("Import Material Type")) { new Material_Import_Window; }
				IMGUI->Same_Line();
				if (IMGUI->Button("Import Texture")) { new Texture_Import_Window; }
				IMGUI->Same_Line();
				if (IMGUI->Button("Create Material Instance")) { new MaterialInstance_CreationWindow; }
				IMGUI->Same_Line();
				if (IMGUI->Button("Create Scene")) { new Scene_Create_Window; }
			}
			
			//Even if there is a importing proccess, show the contents!
			int selected_list_item_index = -1;
			if (item_names.size() != EDITOR_FILESYSTEM->Get_AssetList().size() || item_names[0].length() == 0) {
				item_names.clear();
				for (unsigned int i = 0; i < EDITOR_FILESYSTEM->Get_AssetList().size(); i++) {
					TuranAPI::Resource_Type* RESOURCE = EDITOR_FILESYSTEM->Get_AssetList().Get(i);
					item_names.push_back(RESOURCE->NAME);
				}
			}
			//Show selected content's properties!
			if (IMGUI->Selectable_ListBox("Game Content List", &selected_list_item_index, &item_names)) {
				ResourceProperties_Window* properties = new ResourceProperties_Window(
					EDITOR_FILESYSTEM->Get_AssetList()[selected_list_item_index]
				);
			}

			IMGUI->End_Window();
			return;
		}
	}


}