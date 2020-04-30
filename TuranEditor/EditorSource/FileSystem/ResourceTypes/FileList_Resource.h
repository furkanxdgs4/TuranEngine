#pragma once
#include "EditorSource/Editor_Includes.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"

namespace TuranEditor {
	class FileList_Resource {
		friend class Editor_FileSystem;
		Vector<TuranAPI::Resource_Type*> ContentListVector;
		String PATH;
		//This isn't stored in Disk! We determine the bits at start
		Bitset ID_BITSET;

		//When we load assets at start, we should read each asset and save it to ID_BITSET because ID_BITSET isn't stored!
		void Read_ID(unsigned int ID);

		//DON'T CALL THESE FUNCTIONS OUTSIDE OF EDITORFILESYSTEM

		bool Verify_Resource_Data();
		void Write_ToDisk(bool Verify_Data);
		//Loads an asset! If you want to write a override function, first you have to be sure if flatbuffer and Resource_Type is related
		//Giving an unrelated flatbuffer's data doesn't do anyting. 
		void Load_FromDisk(IAllocator* Allocator);
	public:
		FileList_Resource(const char* path);
		Vector<TuranAPI::Resource_Type*>* Get_ContentListVector();


		size_t Create_Resource_ID();
		void Delete_Resource_ID(unsigned int ID);
	};

}