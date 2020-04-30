#include "EditorFileSystem_Core.h"
#include "TuranAPI/Logger/Logger_Core.h"
#include "TuranAPI/Profiler/Profiler_Core.h"

//Data Formats created by Flatbuffers!
#include <flatbuffers/flatbuffers.h>
#include "DataFormats/EditorResources_generated.h"
#include "TuranAPI/FileSystem/Resource_Types/ResourceTYPEs.h"

namespace TuranEditor {
	ContentManager* ContentManager::SELF = nullptr;
	ContentManager::ContentManager() : ALL_MaterialInstances(LASTUSEDALLOCATOR, 10, 10), ALL_MaterialTypes(LASTUSEDALLOCATOR, 10, 10), ALL_StaticModels(LASTUSEDALLOCATOR, 10, 10),
		ALL_Textures(LASTUSEDALLOCATOR, 10, 10) {}
	void ContentManager::Add_aContent(TuranAPI::Resource_Type* RESOURCE) {

	}
	void ContentManager::Delete_aContent(TuranAPI::Resource_Type* RESOURCE) {
		switch (RESOURCE->Get_Type()) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			for (unsigned int i = 0; i < ALL_StaticModels.size(); i++) {
				Static_Model_Data* model = ALL_StaticModels[i];
				if (RESOURCE == model) {
					ALL_StaticModels.erase(i);
					TuranAPI::LOG_STATUS("ContentManager deleted a Static Model!");
				}
			}
			TuranAPI::LOG_ERROR("ContentManager tried to delete a Static Model, but it's not found! There is a lack in somewhere!");
			break;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			for (unsigned int i = 0; i < ALL_Textures.size(); i++) {
				GFX_API::Texture_Resource* texture = ALL_Textures[i];
				if (RESOURCE == texture) {
					ALL_Textures.erase(i);
					TuranAPI::LOG_STATUS("ContentManager deleted a Texture!");
				}
			}
			TuranAPI::LOG_ERROR("ContentManager tried to delete a Texture, but it's not found! There is a lack in somewhere!");
			break;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			for (unsigned int i = 0; i < ALL_MaterialTypes.size(); i++) {
				GFX_API::Material_Type_Resource* mattype = ALL_MaterialTypes[i];
				if (RESOURCE == mattype) {
					ALL_MaterialTypes.erase(i);
					TuranAPI::LOG_STATUS("ContentManager deleted a Material Type!");
				}
			}
			TuranAPI::LOG_ERROR("ContentManager tried to delete a Material Type, but it's not found! There is a lack in somewhere!");
			break;
		case RESOURCETYPEs::GFXAPI_MATINST:
			for (unsigned int i = 0; i < ALL_StaticModels.size(); i++) {
				GFX_API::Material_Instance* matinst = ALL_MaterialInstances[i];
				if (RESOURCE == matinst) {
					ALL_MaterialInstances.erase(i);
					TuranAPI::LOG_STATUS("ContentManager deleted a Material Instance!");
				}
			}
			TuranAPI::LOG_ERROR("ContentManager tried to delete a Material Instance, but it's not found! There is a lack in somewhere!");
			break;
		default:
			TuranAPI::LOG_CRASHING("This assset type isn't supported by Content Manager!");
			return;
		}

		Solve_ReferencestoAsset(RESOURCE->ID);
	}

	void ContentManager::Solve_ReferencestoAsset(unsigned int ID) {
		TuranAPI::LOG_NOTCODED("Solving references isn't supported for now!\n", false);
	}
	const Vector<GFX_API::Material_Instance*>* ContentManager::Read_ALLMaterialInstances() const { return &(SELF)->ALL_MaterialInstances; }
	const Vector<GFX_API::Material_Type_Resource*>* ContentManager::Read_ALLMaterialTypes() const { return &(SELF)->ALL_MaterialTypes; }
	const Vector<Static_Model_Data*>* ContentManager::Read_ALLStaticModelAssets() const { return &(SELF)->ALL_StaticModels; }
	const Vector<GFX_API::Texture_Resource*>* ContentManager::Read_ALLTextureAssets() const { return &(SELF)->ALL_Textures; }
	Static_Model_Data* ContentManager::Find_StaticModel_byID(unsigned int ID) const {
		for (unsigned int i = 0; i < ALL_StaticModels.size(); i++) {
			Static_Model_Data* model = ALL_StaticModels[i];
			if (model->ID == ID) {
				return model;
			}
		}
		TuranAPI::LOG_CRASHING("There is no Static Model with ID: number->string isn't supported yet!");
		return nullptr;
	}
	GFX_API::Texture_Resource* ContentManager::Find_Texture_byID(unsigned int ID) const {
		for (unsigned int i = 0; i < ALL_Textures.size(); i++) {
			GFX_API::Texture_Resource* texture = ALL_Textures[i];
			if (texture->ID == ID) {
				return texture;
			}
		}
		TuranAPI::LOG_CRASHING("There is no Texture with ID: number->string isn't supported yet!");
		return nullptr;
	}
	GFX_API::Material_Type_Resource* ContentManager::Find_MaterialType_byID(unsigned int ID) const {
		for (unsigned int i = 0; i < ALL_MaterialTypes.size(); i++) {
			GFX_API::Material_Type_Resource* mattype = ALL_MaterialTypes[i];
			if (mattype->ID == ID) {
				return mattype;
			}
		}
		TuranAPI::LOG_CRASHING("There is no Material Type with ID: number->string isn't supported yet!");
		return nullptr;
	}
	GFX_API::Material_Instance* ContentManager::Find_MaterialInstance_byID(unsigned int ID) const {
		for (unsigned int i = 0; i < ALL_MaterialInstances.size(); i++) {
			GFX_API::Material_Instance* matinst = ALL_MaterialInstances[i];
			if (matinst->ID == ID) {
				return matinst;
			}
		}
		TuranAPI::LOG_CRASHING("There is no Material Instance with ID: number->string isn't supported yet!");
		return nullptr;
	}




	Editor_FileSystem* Editor_FileSystem::SELF = nullptr;
	EditorAsset::File_Type Convert_to_GameCont_Type(RESOURCETYPEs resource_type);


	Editor_FileSystem::Editor_FileSystem() : FileList("C:/dev/TuranEngine/TuranEditor/File_List.enginecont") {
		TuranAPI::LOG_STATUS("Starting the Editor FileSystem! But this doesn't make anything!");
		FileList.Load_FromDisk(LASTUSEDALLOCATOR);
		ContentManager::SELF = &AssetManager;
		SELF = this;
	}

	void Editor_FileSystem::Add_anAsset_toFileList(TuranAPI::Resource_Type* RESOURCE) {
		if (RESOURCE == nullptr) {
			TuranAPI::LOG_ERROR("Editor FileSystem couldn't add the asset to File List, because Resource is nullptr!");
			return;
		}
		if (!RESOURCE->Verify_Resource_Data()) {
			TuranAPI::LOG_ERROR("Resource isn't added to FileList because it's not verified!");
			return;
		}
		RESOURCE->ID = FileList.Create_Resource_ID();
		FileList.ContentListVector.push_back(RESOURCE);
		FileList.Write_ToDisk(true);
	}
	void Editor_FileSystem::Delete_anAsset_fromFileList(TuranAPI::Resource_Type* RESOURCE) {
		for (unsigned int i = 0; i < FileList.Get_ContentListVector()->size(); i++) {
			TuranAPI::Resource_Type* ASSET = FileList.Get_ContentListVector()->Get(i);
			if (RESOURCE = ASSET) {
				FileList.Delete_Resource_ID(RESOURCE->ID);
				CONTENTMANAGER->Delete_aContent(ASSET);
				FileList.Get_ContentListVector()->erase(i);
				FileList.Write_ToDisk(true);
				return;
			}
		}
		TuranAPI::LOG_ERROR("Editor_FileSystem tried to delete an Asset, but it is not in FileList! There is a lack in somewhere!");
	}
	void Editor_FileSystem::Load_Assets_fromDisk() {
		FileList.Load_FromDisk(LASTUSEDALLOCATOR);
	}
	const Vector<TuranAPI::Resource_Type*>& Editor_FileSystem::Get_AssetList() {
		return FileList.ContentListVector;
	}
	void Editor_FileSystem::Clear_AllFileList() {
		FileList.ContentListVector.clear();
		FileList.Write_ToDisk(true);
		TuranAPI::LOG_STATUS("Editor Asset Clearing is successful!");
	}



	EditorAsset::File_Type Convert_to_GameCont_Type(RESOURCETYPEs resource_type) {
		switch (resource_type) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			return EditorAsset::File_Type_Static_Model;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			return EditorAsset::File_Type_Texture;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			return EditorAsset::File_Type_Material_Type;
		case RESOURCETYPEs::GFXAPI_MATINST:
			return EditorAsset::File_Type_Material_Instance;
		case RESOURCETYPEs::EDITOR_SCENE:
			return EditorAsset::File_Type_Scene;
		default:
			TuranAPI::LOG_CRASHING("Intended resource type isn't found in FileList!");
			return EditorAsset::File_Type_ERROR;
		}
	}

	TuranAPI::Resource_Type* Editor_FileSystem::Get_GameContent_byName(const char* NAME) {
		for (unsigned int i = 0; i < FileList.Get_ContentListVector()->size(); i++) {
			TuranAPI::Resource_Type* RESOURCE = (*FileList.Get_ContentListVector())[i];
			if (RESOURCE->NAME == NAME) {
				return RESOURCE;
			}
		}
	}

	TuranAPI::Resource_Type* Editor_FileSystem::Get_GameContent_byID(unsigned int ID) {
		for (unsigned int i = 0; i < FileList.Get_ContentListVector()->size(); i++) {
			TuranAPI::Resource_Type* RESOURCE = (*FileList.Get_ContentListVector())[i];
			if (RESOURCE->ID == ID) {
				return RESOURCE;
			}
		}
	}
}