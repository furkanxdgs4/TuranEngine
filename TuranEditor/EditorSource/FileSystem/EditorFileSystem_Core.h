#pragma once
#include "EngineSource/TuranEngine.h"
#include "ResourceTypes/FileList_Resource.h"
#include "ResourceTypes/Model_Resource.h"

/*
1) This namespace is defined in Engine
2) But extended in Editor to handle Editor specific files too!
*/
namespace TuranEditor {

	//There is no checks for now but this system is to check for every asset's dependencies.
	//For example; while deleting a Texture, this class checks if any Material Instance uses it.
	//If it is, then it's gonna give user a Window like this (Which texture do you want to replace this texture?)
	//Also this mechanism should return SUCCESS or FAIL (Maybe it's not useful for now, because everything is thightly coupled in this engine; maybe for future projects)
	class ContentManager {
		friend class Editor_FileSystem;
		Vector<GFX_API::Material_Instance*> ALL_MaterialInstances;
		Vector<GFX_API::Material_Type_Resource*> ALL_MaterialTypes;
		Vector<Static_Model_Data*> ALL_StaticModels;
		Vector<GFX_API::Texture_Resource*> ALL_Textures;
		void Solve_ReferencestoAsset(unsigned int ID);

		void Add_aContent(TuranAPI::Resource_Type* RESOURCE);
		void Delete_aContent(TuranAPI::Resource_Type* RESOURCE);
		ContentManager();
	public:
		static ContentManager* SELF;
		const Vector<GFX_API::Material_Instance*>* Read_ALLMaterialInstances() const;
		const Vector<GFX_API::Material_Type_Resource*>* Read_ALLMaterialTypes() const;
		const Vector<Static_Model_Data*>* Read_ALLStaticModelAssets() const;
		const Vector<GFX_API::Texture_Resource*>* Read_ALLTextureAssets() const;

		Static_Model_Data* Find_StaticModel_byID(unsigned int ID) const ;
		GFX_API::Texture_Resource* Find_Texture_byID(unsigned int ID) const ;
		GFX_API::Material_Type_Resource* Find_MaterialType_byID(unsigned int ID) const ;
		GFX_API::Material_Instance* Find_MaterialInstance_byID(unsigned int ID) const ;
	};
	/*
	1) This class is used to load the project's editor datas;
	2) You can specify how to create and load new data types here!
	3) If project's File_List.bin isn't found, gives error to either specify the location or create a new project!
	*/
	class Editor_FileSystem : public TAPIFILESYSTEM {
		FileList_Resource FileList;
		ContentManager AssetManager;
	public:
		Editor_FileSystem();
		static Editor_FileSystem* SELF;

		//GETTER-SETTERs
		TuranAPI::Resource_Type* Get_GameContent_byName(const char* NAME);
		TuranAPI::Resource_Type* Get_GameContent_byID(unsigned int ID);

		void Add_anAsset_toFileList(TuranAPI::Resource_Type* RESOURCE);
		void Delete_anAsset_fromFileList(TuranAPI::Resource_Type* RESOURCE);
		void Load_Assets_fromDisk();
		void Clear_AllFileList();
		const Vector<TuranAPI::Resource_Type*>& Get_AssetList();
	};
#define EDITOR_FILESYSTEM Editor_FileSystem::SELF
#define CONTENTMANAGER ContentManager::SELF
}