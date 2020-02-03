#include "EditorFileSystem_Core.h"
#include "TuranAPI/Profiler/Profiler_Core.h"
//Data Formats created by Flatbuffers!
#include <flatbuffers/flatbuffers.h>
#include "TuranAPI/FileSystem/DataFormats/GameResource_generated.h"

using namespace Editor::File_System;
using namespace TuranAPI::File_System;
string GameContentList_PATH = "C:/dev/TuranEngine/TuranEditor/File_List.enginecont";

TuranAPI::File_System::FileList_Resource Editor_FileSystem::GameContentList_onDisk(GameContentList_PATH);

Editor_FileSystem* Editor_FileSystem::SELF = new Editor_FileSystem;
GameContent::File_Type Convert_to_GameCont_Type(TuranAPI::TuranAPI_ENUMs resource_type);



void Editor_FileSystem::Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) {
	TuranAPI::LOG_STATUS("Adding a Game Content to List! Name: " + Resource->NAME + " ID: " + to_string(Resource->ID) + " PATH: " + Resource->PATH);
	GameContentList_onDisk.Get_ContentListVector()->push_back(Resource);
	FileSystem::Write_a_Resource_toDisk(&GameContentList_onDisk);
	FileSystem::Write_a_Resource_toDisk(Resource);
	TuranAPI::LOG_STATUS("Added the Game Content to List successfully!");
}

void Editor_FileSystem::Remove_Content_fromFileList(unsigned int index) {
	TuranAPI::LOG_STATUS("Removing a Game Content from the List! List Index: " + to_string(index));

	Resource_Type* Resource = ((*GameContentList_onDisk.Get_ContentListVector())[index]);

	FileSystem::Delete_File(Resource->PATH);
	//Clear all of the loaded resources from the memory
	delete Resource;
	Resource = nullptr;
	GameContentList_onDisk.Get_ContentListVector()->erase(GameContentList_onDisk.Get_ContentListVector()->begin() + index);
	Write_a_Resource_toDisk(&GameContentList_onDisk, true);

	TuranAPI::LOG_STATUS("Removed the Game Content from the List successfully!");
}

const vector<TuranAPI::File_System::Resource_Type*>* Editor_FileSystem::Get_Const_FileListContentVector() {
	return GameContentList_onDisk.Get_ContentListVector();
}

unsigned int Editor_FileSystem::Get_LengthOf_FileListContentVector() {
	return GameContentList_onDisk.Get_ContentListVector()->size();
}




void Editor_FileSystem::Start_EditorFileSystem() {
	TuranAPI::LOG_STATUS("Starting the Editor FileSystem!");
	SELF = new Editor_FileSystem;
}

void Editor_FileSystem::Load_FileListContents_fromDisk() {
	TuranAPI::LOG_STATUS("Started to load Game Contents!");
	TuranAPI::File_System::FileSystem::Load_Resources_fromFileList(&GameContentList_onDisk);
	TuranAPI::LOG_STATUS("Finished loading Game Contents!");
}


void Editor_FileSystem::Clear_FileListContents() {
	TuranAPI::LOG_STATUS("Clearing the Game Contents!");

	for (Resource_Type* RESOURCE : *GameContentList_onDisk.Get_ContentListVector()) {
		string PATH = RESOURCE->PATH;

		FileSystem::Delete_File(PATH);
		//Clear all of the loaded resources from the memory
		delete RESOURCE;
		RESOURCE = nullptr;
	}
	//Clear all of the loaded resource references!
	GameContentList_onDisk.Get_ContentListVector()->clear();

	FileSystem::Write_a_Resource_toDisk(&GameContentList_onDisk);
	TuranAPI::LOG_STATUS("Cleared the Game Contents!");
}

GameContent::File_Type Convert_to_GameCont_Type(TuranAPI::TuranAPI_ENUMs resource_type) {
	switch (resource_type) {
	case TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_RESOURCE:
		return GameContent::File_Type_Static_Model;
	case TuranAPI::TuranAPI_ENUMs::MATERIAL_TYPE_RESOURCE:
		return GameContent::File_Type_Material_Type;
	case TuranAPI::TuranAPI_ENUMs::TEXTURE_RESOURCE:
		return GameContent::File_Type_Texture;
	default:
		TuranAPI::LOG_CRASHING("Intended resource type isn't found in FileList!");
		return GameContent::File_Type_ERROR;
	}
}

TuranAPI::File_System::Resource_Type* Editor_FileSystem::Get_GameContent_byName(string NAME) {
	for (Resource_Type* RESOURCE : *GameContentList_onDisk.Get_ContentListVector()) {
		if (RESOURCE->NAME == NAME) {
			return RESOURCE;
		}
	}
}

TuranAPI::File_System::Resource_Type* Editor_FileSystem::Get_GameContent_byID(unsigned int ID) {
	for (Resource_Type* RESOURCE : *GameContentList_onDisk.Get_ContentListVector()) {
		if (RESOURCE->ID == ID) {
			return RESOURCE;
		}
	}
}