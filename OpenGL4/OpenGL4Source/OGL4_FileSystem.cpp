#include "OGL4_FileSystem.h"

namespace OpenGL4 {
	OGL4_FileSystem* OGL4_FileSystem::SELF = nullptr;
	TuranAPI::File_System::FileList_Resource OGL4_FileSystem::OpenGLContentList_onDisk("C:/dev/TuranEngine/OpenGL4/GFX_ContentsList.gfxcont");

	void OGL4_FileSystem::Load_FileListContents_fromDisk() {
		cout << "OpenGL3 Contents are being loaded now!\n";

		TuranAPI::File_System::FileSystem::Load_Resources_fromFileList(&OpenGLContentList_onDisk);
	}

	void OGL4_FileSystem::Clear_FileListContents() {
		OpenGLContentList_onDisk.Get_ContentListVector()->clear();
		TuranAPI::File_System::FileSystem::Write_a_Resource_toDisk(&OpenGLContentList_onDisk);
	}

	void OGL4_FileSystem::Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) {
		TuranAPI::Breakpoint("Add Content to FileList in OGL3 doesn't coded!");
	}
	void OGL4_FileSystem::Remove_Content_fromFileList(unsigned int index) {
		TuranAPI::Breakpoint("Remove Content from FileList in OGL3 doesn't coded!");
	}
	const vector<TuranAPI::File_System::Resource_Type*>* OGL4_FileSystem::Get_Const_FileListContentVector() {
		return OpenGLContentList_onDisk.Get_ContentListVector();
	}
	unsigned int OGL4_FileSystem::Get_LengthOf_FileListContentVector() {
		return OpenGLContentList_onDisk.Get_ContentListVector()->size();
	}
}