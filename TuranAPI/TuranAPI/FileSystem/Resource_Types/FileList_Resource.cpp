#include "FileList_Resource.h"

using namespace TuranAPI::File_System;

FileList_Resource::FileList_Resource(const string& path) {
	PATH = path;
	NAME = path.substr(path.find_last_of('/') + 1, path.size());
	NAME = path.substr(0, path.find_last_of('.'));
}

TuranAPI::TuranAPI_ENUMs FileList_Resource::Get_Resource_Type() {
	return TuranAPI::TuranAPI_ENUMs::FILE_LIST_RESOURCE;
}

vector<Resource_Type*>* FileList_Resource::Get_ContentListVector() {
	return &All_Resources;
}

bool FileList_Resource::Verify_Resource_Data() {
	if (NAME != "" && PATH != "") {
		for (Resource_Type* Resource : All_Resources) {
			if (!Resource->Verify_Resource_Data()) {
				TuranAPI::Breakpoint("File List Resource isn't verified because a Resource isn't verified!\n");
				return false;
			}
		}
	}
	else {
		TuranAPI::Breakpoint("File List Resource isn't verified because it has invalid Name or Path!\n" +
			string("Name: ") + NAME + string("\nPath: ") + PATH + string("\n"));
		return false;
	}
	return true;
}