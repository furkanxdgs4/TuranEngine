#pragma once
#include "EditorSource/Editor_Includes.h"
#include "Resource_Identifier.h"

namespace TuranEditor {
	class FileList_Resource {
		friend class Editor_FileSystem;
		vector<Resource_Identifier*> ContentListvector;
		string PATH;
		//This isn't stored in Disk! We determine the bits at start
		Bitset ID_BITSET;
	public:
		FileList_Resource(const char* path);
		vector<Resource_Identifier*>* Get_ContentListvector();

		//Meaningless but I'll leave this for now. If I can't find a proper verifying process, I'll delete.
		bool Verify_Resource_Data();
		//This function only loads Resource_Identifiers, not the resources!
		//That means if you want to load a resource into memory, you need to call Load_Resource in Editor_FileSystem
		void Load_FromDisk(IAllocator* Allocator);
		//Save Resource_Identifiers to disk!
		void Write_ToDisk();

		size_t Create_Resource_ID();
		void Delete_Resource_ID(unsigned int ID);
		void Read_ID(unsigned int Asset_ID);
	};

}