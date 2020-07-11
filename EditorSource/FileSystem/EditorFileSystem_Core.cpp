#include "EditorFileSystem_Core.h"
#include "TuranAPI/Logger/Logger_Core.h"
#include "TuranAPI/Profiler/Profiler_Core.h"

//Data Formats created by Flatbuffers!
#include <flatbuffers/flatbuffers.h>
#include "DataFormats/EditorResources_generated.h"
#include "EditorSource/FileSystem/ResourceTypes/ResourceTYPEs.h"

namespace TuranEditor {
	Editor_FileSystem* Editor_FileSystem::SELF = nullptr;
	EditorAsset::File_Type Convert_toFB_ResourceType(RESOURCETYPEs resource_type);
	RESOURCETYPEs Convert_fromFB_ResourceType(EditorAsset::File_Type resource_type);

	bool Verify_SceneResource(Resource_Identifier* IDENTIFIER);
	void* Load_SceneResource(Resource_Identifier* IDENTIFIER);
	void Save_SceneResource(Resource_Identifier* IDENTIFIER);

	bool Verify_StaticModel(Resource_Identifier* IDENTIFIER);
	void* Load_StaticModel(Resource_Identifier* IDENTIFIER);
	void Save_StaticModel(Resource_Identifier* IDENTIFIER);

	bool Verify_Texture(Resource_Identifier* IDENTIFIER);
	void* Load_Texture(Resource_Identifier* IDENTIFIER);
	void Save_Texture(Resource_Identifier* IDENTIFIER);

	bool Verify_ShaderSource(Resource_Identifier* IDENTIFIER);
	void* Load_ShaderSource(Resource_Identifier* IDENTIFIER);
	void Save_ShaderSource(Resource_Identifier* IDENTIFIER);

	bool Verify_MatInst(Resource_Identifier* IDENTIFIER);
	void* Load_MatInst(Resource_Identifier* IDENTIFIER);
	void Save_MatInst(Resource_Identifier* IDENTIFIER);

	bool Verify_MatType(Resource_Identifier* IDENTIFIER);
	void* Load_MatType(Resource_Identifier* IDENTIFIER);
	void Save_MatType(Resource_Identifier* IDENTIFIER);


	Editor_FileSystem::Editor_FileSystem() : FileList("C:/dev/TuranEngine/TuranEditor/File_List.enginecont") {
		TuranAPI::LOG_STATUS("Starting the Editor FileSystem! But this doesn't make anything!");
		SELF = this;
		Load_FileList();
	}

	void Editor_FileSystem::Add_anAsset_toFileList(Resource_Identifier* RESOURCE) {
		if (RESOURCE == nullptr) {
			TuranAPI::LOG_ERROR("Editor FileSystem couldn't add the asset to File List, because Resource is nullptr!");
			return;
		}
		
		if (!Verify_Resource(RESOURCE->ID)) {
			TuranAPI::LOG_ERROR("Resource isn't added to FileList because it's not verified!");
			return;
		}
		RESOURCE->ID = FileList.Create_Resource_ID();
		FileList.ContentListvector.push_back(RESOURCE);
		FileList.Write_ToDisk();
	}
	void Editor_FileSystem::Delete_anAsset_fromFileList(unsigned int ID) {
		for (unsigned int i = 0; i < FileList.Get_ContentListvector()->size(); i++) {
			Resource_Identifier* ASSET = (*FileList.Get_ContentListvector())[i];
			if (ID = ASSET->ID) {
				Unload_Resource(ASSET->ID);
				FileList.Delete_Resource_ID(ASSET->ID);
				FileList.Get_ContentListvector()->erase(FileList.Get_ContentListvector()->begin() + i);
				FileList.Write_ToDisk();
				return;
			}
		}
		TuranAPI::LOG_ERROR("Editor_FileSystem tried to delete an Asset, but it is not in FileList! There is a lack in somewhere!");
	}
	void Editor_FileSystem::Load_FileList() {
		FileList.Load_FromDisk(LASTUSEDALLOCATOR);
		for (unsigned int i = 0; i < FileList.ContentListvector.size(); i++) {
			Load_Resource(FileList.ContentListvector[i]->ID);
		}
	}
	const vector<Resource_Identifier*>& Editor_FileSystem::Get_AssetList() {
		return FileList.ContentListvector;
	}
	void Editor_FileSystem::Clear_AllFileList() {
		for (unsigned int i = 0; i < FileList.ContentListvector.size(); i++) {
			Unload_Resource(FileList.ContentListvector[i]->ID);
		}
		FileList.ContentListvector.clear();
		FileList.Write_ToDisk();
		TuranAPI::LOG_STATUS("FileList is cleaned successfully!");
	}

	void Editor_FileSystem::Load_Resource(unsigned int ID) {
		Resource_Identifier* RESOURCE = Find_ResourceIdentifier_byID(ID);
		if (!RESOURCE) {
			TuranAPI::LOG_ERROR("Loading the resource has failed because Identifier isn't found!");
			return;
		}
		switch (RESOURCE->TYPE) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			RESOURCE->DATA = Load_StaticModel(RESOURCE);
			return ;
		case RESOURCETYPEs::EDITOR_SCENE:
			RESOURCE->DATA = Load_SceneResource(RESOURCE);
			return; 
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			RESOURCE->DATA = Load_Texture(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_SHADERSOURCE:
			RESOURCE->DATA = Load_ShaderSource(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATINST:
			RESOURCE->DATA = Load_MatInst(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			RESOURCE->DATA = Load_MatType(RESOURCE);
			return;
		default:
			TuranAPI::LOG_NOTCODED("Editor_FileSystem::Load_Resource() doesn't support to load this type of resource!", true);
			return;
		}
	}

	void Editor_FileSystem::Unload_Resource(unsigned int ID) {
		Resource_Identifier* RESOURCE = Find_ResourceIdentifier_byID(ID);
		switch (RESOURCE->TYPE) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			delete (Static_Model*)RESOURCE->DATA;
			break;
		case RESOURCETYPEs::EDITOR_SCENE:
			delete (Scene_Resource*)RESOURCE->DATA;
			break;
		case RESOURCETYPEs::GFXAPI_MATINST:
			delete (GFX_API::Material_Instance*)RESOURCE->DATA;
			break;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			delete (GFX_API::Material_Type*)RESOURCE->DATA;
			break;
		case RESOURCETYPEs::GFXAPI_SHADERSOURCE:
			delete (GFX_API::ShaderSource_Resource*)RESOURCE->DATA;
			break;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			delete (GFX_API::Texture_Resource*)RESOURCE->DATA;
			break;
		default:
			TuranAPI::LOG_NOTCODED("Unload_Resource() doesn't support this type of resource!", true);
			break;
		}
		RESOURCE->DATA = nullptr;
	}

	void Editor_FileSystem::Save_Resource(unsigned int ID) {
		Resource_Identifier* RESOURCE = Find_ResourceIdentifier_byID(ID);
		switch (RESOURCE->TYPE) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			Save_StaticModel(RESOURCE);
			return;
		case RESOURCETYPEs::EDITOR_SCENE:
			Save_SceneResource(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			Save_Texture(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATINST:
			Save_MatInst(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			Save_MatType(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_SHADERSOURCE:
			Save_ShaderSource(RESOURCE);
			return;
		default:
			TuranAPI::LOG_NOTCODED("Verify_Resource() doesn't support this type of resource!", true);
			return;
		}
	}

	bool Editor_FileSystem::Verify_Resource(unsigned int ID) {
		Resource_Identifier* RESOURCE = Find_ResourceIdentifier_byID(ID);
		switch (RESOURCE->TYPE) {
		case RESOURCETYPEs::EDITOR_STATICMODEL:
			Verify_StaticModel(RESOURCE);
			return;
		case RESOURCETYPEs::EDITOR_SCENE:
			Verify_SceneResource(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_TEXTURE:
			Verify_Texture(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATINST:
			Verify_MatInst(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_MATTYPE:
			Verify_MatType(RESOURCE);
			return;
		case RESOURCETYPEs::GFXAPI_SHADERSOURCE:
			Save_ShaderSource(RESOURCE);
			return;
		default:
			TuranAPI::LOG_NOTCODED("Verify_Resource() doesn't support this type of resource!", true);
			return false;
		}
	}


	bool Editor_FileSystem::Does_ResourceExist(unsigned int Resource_ID, RESOURCETYPEs TYPE) const {
		for (Resource_Identifier* resource : FileList.ContentListvector) {
			if (Resource_ID == resource->ID && TYPE == resource->TYPE) {
				return true;
			}
		}
		TuranAPI::LOG_WARNING("Does_ResourceExist() failed to find a resource!\n");
		return false;
	}


	Resource_Identifier* Editor_FileSystem::Find_ResourceIdentifier_byID(unsigned int ID) {
		for (Resource_Identifier* RESOURCE : FileList.ContentListvector) {
			if (RESOURCE->ID == ID) {
				return RESOURCE;
			}
		}
		TuranAPI::LOG_ERROR("Find_ResourceIdentifier_byID has failed to find the resource!");
		return nullptr;
	}




	EditorAsset::File_Type Convert_toFB_ResourceType(RESOURCETYPEs resource_type) {
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
	RESOURCETYPEs Convert_fromFB_ResourceType(EditorAsset::File_Type resource_type) {
		switch (resource_type) {
		case EditorAsset::File_Type::File_Type_Static_Model:
			return RESOURCETYPEs::EDITOR_STATICMODEL;
		case EditorAsset::File_Type::File_Type_Scene:
			return RESOURCETYPEs::EDITOR_SCENE;
		case EditorAsset::File_Type::File_Type_Texture:
			return RESOURCETYPEs::GFXAPI_TEXTURE;
		case EditorAsset::File_Type::File_Type_Material_Instance:
			return RESOURCETYPEs::GFXAPI_MATINST;
		case EditorAsset::File_Type::File_Type_Material_Type:
			return RESOURCETYPEs::GFXAPI_MATTYPE;
		default:
			TuranAPI::LOG_CRASHING("Convert_fromFB_ResourceType doesn't support this EditorAsset::File_Type!\n");
		}
	}

}