#pragma once
#include "EngineSource/TuranEngine.h"
#include "TuranAPI/API_FileSystem.h"

/*
1) This namespace is defined in Engine
2) But extended in Editor to handle Editor specific files too!
*/
namespace Editor {
	namespace File_System {
		/*
		1) This class is used to load the project's editor datas;
		2) You can specify how to create and load new data types here!
		3) If project's File_List.bin isn't found, gives error to either specify the location or create a new project!
		*/
		class Editor_FileSystem : public TuranAPI::File_System::FileSystem {
			static TuranAPI::File_System::FileList_Resource GameContentList_onDisk;
		public:
			static Editor_FileSystem* SELF;

			virtual void Add_Content_toFileList(TuranAPI::File_System::Resource_Type* Resource) override;
			virtual void Remove_Content_fromFileList(unsigned int index) override;
			virtual const vector<TuranAPI::File_System::Resource_Type*>* Get_Const_FileListContentVector() override;
			virtual unsigned int Get_LengthOf_FileListContentVector() override;


			static void Start_EditorFileSystem();
			//Read File_List.usercont to load the project!
			virtual void Load_FileListContents_fromDisk() override;
			//Clear all of the resource list! But resource files (.gamecont) aren't deleted.
			virtual void Clear_FileListContents() override;

			//GETTER-SETTERs
			static TuranAPI::File_System::Resource_Type* Get_GameContent_byName(string NAME);
			static TuranAPI::File_System::Resource_Type* Get_GameContent_byID(unsigned int ID);
		};

	}
}
#define EDITOR_FILESYSTEM Editor::File_System::Editor_FileSystem::SELF