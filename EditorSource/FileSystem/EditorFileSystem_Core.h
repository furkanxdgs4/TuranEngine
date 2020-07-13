#pragma once
#include "EngineSource/TuranEngine.h"
#include "ResourceTypes/Resource_Identifier.h"
#include "ResourceTypes/FileList_Resource.h"
#include "ResourceTypes/Model_Resource.h"
#include "ResourceTypes/Scene_Resource.h"

/*
1) This namespace is defined in Engine
2) But extended in Editor to handle Editor specific files too!
*/
namespace TuranEditor {
	/*
	1) This class is used to load the project's editor datas;
	2) You can specify how to create and load new data types here!
	3) If project's File_List.bin isn't found, gives error to either specify the location or create a new project!
	*/
	class Editor_FileSystem : public TAPIFILESYSTEM {
		FileList_Resource FileList;

	public:
		Editor_FileSystem();
		static Editor_FileSystem* SELF;

		//FILE LIST OPERATIONs

		void Add_anAsset_toFileList(Resource_Identifier* RESOURCE);
		void Delete_anAsset_fromFileList(unsigned int ID);
		void Load_FileList();
		void Clear_AllFileList();
		bool Does_ResourceExist(unsigned int Resource_ID, RESOURCETYPEs TYPE) const;
		const vector<Resource_Identifier*>& Get_AssetList() const;
		vector<Resource_Identifier*> Get_SpecificAssetType(RESOURCETYPEs TYPE);


		//RESOURCE LOADING AND VERIFYING OPERATIONs
		
		void Load_Resource(unsigned int ID);
		void Unload_Resource(unsigned int ID);
		void Save_Resource(unsigned int ID);
		bool Verify_Resource(unsigned int ID);

		Resource_Identifier* Find_ResourceIdentifier_byID(unsigned int ID);
	};
#define EDITOR_FILESYSTEM Editor_FileSystem::SELF
}