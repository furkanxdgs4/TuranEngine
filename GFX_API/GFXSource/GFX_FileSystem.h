#pragma once
#include "GFX_Includes.h"
#include "TuranAPI/FileSystem/Resource_Types/Resource_Type.h"
#include "TuranAPI/FileSystem/Resource_Types/FileList_Resource.h"

namespace GFX {
	//This class is for storing GFX related resources (But not for a specific GFX_API such as Vulkan, OpenGL etc)
	class GFXAPI GFX_FileSystem : public TuranAPI::File_System::FileSystem {
	protected:
		static TuranAPI::File_System::FileList_Resource GFXContentList_onDisk;
	public:
		static GFX_FileSystem* SELF;

		virtual void Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) override;
		virtual void Remove_Content_fromFileList(unsigned int index) override;
		virtual const vector<TuranAPI::File_System::Resource_Type*>* Get_Const_FileListContentVector() override;
		virtual unsigned int Get_LengthOf_FileListContentVector() override;

		//Read File_List.usercont to load the project!
		virtual void Load_FileListContents_fromDisk() override;
		//Clear all of the resource list! But resource files (.gfxcont) aren't deleted.
		virtual void Clear_FileListContents() override;

		//GETTER-SETTERs
		static TuranAPI::File_System::Resource_Type* Get_GFXContent_byName(string NAME);
		static TuranAPI::File_System::Resource_Type* Get_GFXContent_byID(unsigned int ID);
		static TuranAPI::File_System::FileList_Resource* Get_GFXContentList();
	};
#define GFX_FILESYSTEM GFX_FileSystem::SELF
}