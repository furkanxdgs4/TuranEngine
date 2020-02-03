#include "Content_Browser.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "TuranAPI/API_IMGUI.h"
#include "GFXSource/GFX_FileSystem.h"

#include "Status_Window.h"
#include "Properties_Window.h"

#include "EditorSource/FileSystem/ResourceLoaders/Model_Loader.h"
#include "EditorSource/FileSystem/ResourceLoaders/Material_Type_Loader.h"
#include "EditorSource/FileSystem/ResourceLoaders/MaterialInstance_Import.h"
#include "EditorSource/FileSystem/ResourceLoaders/Texture_Loader.h"
#include "EditorSource/Editors/Scene_Editor.h"


using namespace TuranAPI::IMGUI;
using namespace TuranAPI::File_System;
using namespace Editor::File_System;

GameContent_Browser::GameContent_Browser() : IMGUI_WINDOW("GameContent Browser") {

}

void GameContent_Browser::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
		return;
	}
	if (GameContent_EditMode) {
		vector<string> item_names;
		for (Resource_Type* RESOURCE : *EDITOR_FILESYSTEM->Get_Const_FileListContentVector()) {
			item_names.push_back(RESOURCE->NAME);
		}
		GameContentList_CheckList.resize(item_names.size(), false);

		if (IMGUI::Button("Delete All")) {
			EDITOR_FILESYSTEM->Clear_FileListContents();
			GameContentList_CheckList.clear();
			GameContent_EditMode = false;
		}
		IMGUI::Same_Line();
		if (IMGUI::Button("Delete")) {
			if (EDITOR_FILESYSTEM->Get_LengthOf_FileListContentVector() != GameContentList_CheckList.size()) {
				cout << "Error: in Delete Items from Vector(), 2 vectors should have same size!\n";
				TuranAPI::Breakpoint();
			}
			for (unsigned int i = 0; i < EDITOR_FILESYSTEM->Get_LengthOf_FileListContentVector(); i++) {
				bool should_erase = GameContentList_CheckList[i];
				if (should_erase) {
					Resource_Type* resource_to_delete = (*EDITOR_FILESYSTEM->Get_Const_FileListContentVector())[i];
					EDITOR_FILESYSTEM->Remove_Content_fromFileList(i);
					GameContentList_CheckList.erase(GameContentList_CheckList.begin() + i);
					i -= 1;
					cout << "Deleted an item!\n";
				}
			}
			GameContentList_CheckList.clear();
			GameContent_EditMode = false;
		}
		IMGUI::Same_Line();
		if (IMGUI::Button("Cancel")) {
			GameContentList_CheckList.clear();
			GameContent_EditMode = false;
		}
		else {
			IMGUI::CheckListBox("Game Content List", &GameContentList_CheckList, &item_names);
		}

		IMGUI::End_Window();
		return;
	}
	else {
		if (IMGUI::Button("Edit List")) {
			GameContent_EditMode = true;
		}
		IMGUI::Same_Line();
		//Import Buttons
		{
			if (IMGUI::Button("Import Model")) {new Model_Import_Window((FileSystem*)&EDITOR_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Import Material Type")) {new Material_Import_Window((FileSystem*)&EDITOR_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Import Texture")) {new Texture_Import_Window((FileSystem*)&EDITOR_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Create Material Instance")) {new MaterialInstance_CreationWindow((FileSystem*)&EDITOR_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Create Scene")) { new Scene_Create_Window((FileSystem*)&EDITOR_FILESYSTEM); }
		}

		//Even if there is a importing proccess, show the contents!
		int selected_list_item_index = -1;
		vector<string> item_names;
		for (Resource_Type* RESOURCE : *EDITOR_FILESYSTEM->Get_Const_FileListContentVector()) {
			item_names.push_back(RESOURCE->NAME);
		}
		//Show selected content's properties!
		if (IMGUI::Selectable_ListBox("Game Content List", &selected_list_item_index, &item_names)) {
			ResourceProperties_Window* properties = new ResourceProperties_Window(
				(*EDITOR_FILESYSTEM->Get_Const_FileListContentVector())[selected_list_item_index]
			);
		}

		IMGUI::End_Window();
		return;
	}
}


GFX_Content_Browser::GFX_Content_Browser() : IMGUI_WINDOW("GFX Content Browser"){}

void GFX_Content_Browser::Run_Window() {
	if (!Is_Window_Open) {
		delete this;
		return;
	}
	if (!IMGUI::Create_Window(Window_Name, Is_Window_Open, false)) {
		IMGUI::End_Window();
		return;
	}
	if (GFXContent_EditMode) {
		vector<string> item_names;
		for (Resource_Type* RESOURCE : *GFX::GFX_FILESYSTEM->Get_GFXContentList()->Get_ContentListVector()) {
			item_names.push_back(RESOURCE->NAME);
		}
		GFXContentList_CheckList.resize(item_names.size(), false);

		if (IMGUI::Button("Delete All")) {
			GFX::GFX_FILESYSTEM->Clear_FileListContents();
			GFXContentList_CheckList.clear();
			GFXContent_EditMode = false;
		}
		IMGUI::Same_Line();
		if (IMGUI::Button("Delete")) {
			vector<Resource_Type*>* Vector_to_Erase = GFX::GFX_FILESYSTEM->Get_GFXContentList()->Get_ContentListVector();
			if (Vector_to_Erase->size() != GFXContentList_CheckList.size()) {
				cout << "Error: Vector to Erase and Erase list doesn't match in Content Browser.cpp!\n";
				TuranAPI::Breakpoint();
			}
			for (unsigned int i = 0; i < Vector_to_Erase->size(); i++) {
				bool should_erase = GFXContentList_CheckList[i];
				if (should_erase) {
					Vector_to_Erase->erase(Vector_to_Erase->begin() + i);
					GFXContentList_CheckList.erase(GFXContentList_CheckList.begin() + i);
					i -= 1;
					cout << "Deleted an item!\n";
				}
			}
			GFXContentList_CheckList.clear();
			GFXContent_EditMode = false;
			TuranAPI::File_System::FileSystem::Write_a_Resource_toDisk(GFX::GFX_FILESYSTEM->Get_GFXContentList());
		}
		IMGUI::Same_Line();
		if (IMGUI::Button("Cancel")) {
			GFXContentList_CheckList.clear();
			GFXContent_EditMode = false;
		}
		else {
			IMGUI::CheckListBox("GFX Content List", &GFXContentList_CheckList, &item_names);
		}

		IMGUI::End_Window();
		return;
	}
	else {
		if (IMGUI::Button("Edit List")) {
			GFXContent_EditMode = true;
		}
		IMGUI::Same_Line();
		//Import Buttons
		{
			if (IMGUI::Button("Import Model")) { Model_Import_Window* import_window = new Model_Import_Window((FileSystem*)GFX::GFX_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Import Material Type")) { Material_Import_Window* import_window = new Material_Import_Window((FileSystem*)GFX::GFX_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Import Texture")) { Texture_Import_Window* import_window = new Texture_Import_Window((FileSystem*)GFX::GFX_FILESYSTEM); }
			IMGUI::Same_Line();
			if (IMGUI::Button("Create Material Instance")) { MaterialInstance_CreationWindow* import_window = new MaterialInstance_CreationWindow((FileSystem*)GFX::GFX_FILESYSTEM); }
		}

		//Even if there is a importing proccess, show the contents!
		int selected_list_item_index = -1;
		vector<string> item_names;
		for (Resource_Type* RESOURCE : *GFX::GFX_FILESYSTEM->Get_GFXContentList()->Get_ContentListVector()) {
			item_names.push_back(RESOURCE->NAME);
		}
		//Show selected content's properties!
		if (IMGUI::Selectable_ListBox("GFX Content List", &selected_list_item_index, &item_names)) {
			ResourceProperties_Window* properties = new ResourceProperties_Window(
				(*GFX::GFX_FILESYSTEM->Get_Const_FileListContentVector())[selected_list_item_index]
			);
		}

		IMGUI::End_Window();
		return;
	}
}