#include "FileSystem_Core.h"
//Data Formats created by Flatbuffers!
#include <flatbuffers/flatbuffers.h>
#include "DataFormats/GameResource_generated.h"

#include "TuranAPI/Profiler/Profiler_Core.h"


using namespace TuranAPI::File_System;
unsigned int FileSystem::LAST_ID;


void Save_a_MaterialType_toDisk(Resource_Type* Material_Type_Data);
void Save_a_MaterialInst_toDisk(Resource_Type* Material_Inst_Data);
void Save_a_StaticModel_toDisk(Resource_Type* Model_Data);
void Save_a_Texture_toDisk(Resource_Type* Texture_Data);
void Save_a_Scene_toDisk(Resource_Type* Scene_Data);
void Save_a_FileList_toDisk(Resource_Type* FileList_Data);


string FileSystem::Read_TextFile(string path) {
	const char* path_c_str = path.c_str();
	ifstream textfile;
	textfile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		textfile.open(path_c_str);
		stringstream string_data;
		string_data << textfile.rdbuf();
		textfile.close();
		return string_data.str();
	}
	catch (ifstream::failure error)
	{
		cout << "Error: Text file couldn't read: " << path << endl;
		return "";
	}
}

void FileSystem::Write_TextFile(string text, string path, bool write_to_end) {
	if (write_to_end) {
		std::ofstream Output_File;
		Output_File.open(path, ios::out | ios::app);
		Output_File << text << endl;
		Output_File.close();
	}
	else {
		std::ofstream Output_File;
		Output_File.open(path, ios::out | ios::trunc);
		Output_File << text << endl;
		Output_File.close();
	}
}

//If read fails, data_ptr is set to nullptr!
void* FileSystem::Read_BinaryFile(string path, unsigned int& size) {
	std::ifstream Binary_File;
	Binary_File.open(path, ios::binary | ios::in | ios::ate);
	if (!(Binary_File.is_open())) {
		cout << "There is no such file: " << path << endl;
		return nullptr;
	}

	/*
	int begin = Binary_File.tellg();
	Binary_File.seekg(0, ios::end);
	int end = Binary_File.tellg();
	size = end - begin;
	cout << "Size in Reading: " << size << endl;*/

	Binary_File.seekg(0, ios::end);
	int length = Binary_File.tellg();
	Binary_File.seekg(0, ios::beg);
	char* read_data = new char[length];
	Binary_File.read(read_data, length);
	Binary_File.close();
	size = length;
	return read_data;
}

//Use only for creating new binary files! 
//If you want to overwrite to a file, Overwrite_BinaryFile is available!
void FileSystem::Write_BinaryFile(string path, void* data, unsigned int size) {
	//Try to create to a file (Check if the operation fails)
	std::ofstream Output_File(path, ios::binary | ios::out);
	if (!Output_File.is_open()) {
		TuranAPI::Breakpoint("Error: " + path + " couldn't be outputted!\n");
		return;
	}

	//Write to a file and finish all of the operation!
	Output_File.write((const char*)data, size);
	Output_File.close();
	cout << path << " is outputted successfully!\n";
}

//This function will delete the previous content of the file and fill with new data!
void FileSystem::Overwrite_BinaryFile(string path, void* data, unsigned int size) {
	//ios::trunc is used to clear the file before outputting the data!
	std::ofstream Output_File(path, ios::binary | ios::out | ios::trunc);
	if (!Output_File.is_open()) {
		cout << "Error: " << path << " couldn't be outputted!\n";
		return;
	}

	if (data == nullptr) {
		cout << "data is nullptr!\n";
	}
	if (size == 0) {
		cout << "data size is 0!\n";
	}
	//Write to a file and finish all of the operation!
	Output_File.write((const char*)data, size);
	Output_File.close();
	cout << path << " is outputted successfully!\n";
}

void FileSystem::Delete_File(string path) {
	std::remove(path.c_str());
}




//There should be smart things, but I don't have time for that!
//Every imported resource will have ID that isn't used before!
unsigned int FileSystem::Create_Resource_ID() {
	LAST_ID += 1;
	return LAST_ID;
}


void FileSystem::Write_a_Resource_toDisk(Resource_Type* resource_data, bool Verify_ResourceData) {
	resource_data->ID = Create_Resource_ID();
	if (Verify_ResourceData) {
		if (!resource_data->Verify_Resource_Data()) {
			//There should be log!
			TuranAPI::Breakpoint("While writing a resource to disk, the resource data isn't verified!\n");
			return;
		}
	}

	//Model Proccess
	switch (resource_data->Get_Resource_Type()) {
	case TuranAPI_ENUMs::STATIC_MODEL_RESOURCE:
		cout << "Compiles a Static_Model Flatbuffer data!\n";
		Save_a_StaticModel_toDisk(resource_data);
		break;
	case TuranAPI_ENUMs::MATERIAL_TYPE_RESOURCE:
		cout << "Compiles a Material_Type Flatbuffer data!\n";
		Save_a_MaterialType_toDisk(resource_data);
		break;
	case TuranAPI_ENUMs::MATERIAL_INSTANCE_RESOURCE:
		cout << "Compiles a Material_Instance Flatbuffer data!\n";
		Save_a_MaterialInst_toDisk(resource_data);
		break;
	case TuranAPI_ENUMs::TEXTURE_RESOURCE:
		cout << "Compiles a Texture Flatbuffer data!\n";
		Save_a_Texture_toDisk(resource_data);
		break;
	case TuranAPI_ENUMs::FILE_LIST_RESOURCE:
		cout << "Compiles a FileList Flatbuffer data!\n";
		Save_a_FileList_toDisk(resource_data);
		break;
	case TuranAPI_ENUMs::SCENE_RESOURCE:
		cout << "Compiles a Scene Flatbuffer data!\n";
		Save_a_Scene_toDisk(resource_data);
		break;
	default:
		cout << "Error: This data type can't be saved to disk!\n";
		TuranAPI::Breakpoint();
	}
}


unsigned int FileSystem::Get_LAST_ID() {
	return LAST_ID;
}


