#pragma once
#include "OpenGL4_Includes.h"
#pragma once
#include "TuranAPI/FileSystem/FileSystem_Core.h"

namespace OpenGL4 {
	class OGL4_API OGL4_FileSystem : public TuranAPI::File_System::FileSystem {
		friend class OpenGL4_Core;
		static TuranAPI::File_System::FileList_Resource OpenGLContentList_onDisk;
	public:
		static OGL4_FileSystem* SELF;

		virtual void Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) override;
		virtual void Remove_Content_fromFileList(unsigned int index) override;
		virtual const vector<TuranAPI::File_System::Resource_Type*>* Get_Const_FileListContentVector() override;
		virtual unsigned int Get_LengthOf_FileListContentVector() override;


		//Read File_List.usercont to load the project!
		virtual void Load_FileListContents_fromDisk() override;
		//Clear all of the resource list! But resource files (.gamecont) aren't deleted.
		virtual void Clear_FileListContents() override;
	};

}
#define OpenGL4_FILESYSTEM OpenGL4::OGL4_FileSystem::SELF