#include "EditorSource/Editor_Includes.h"
#include "EditorSource/FileSystem/ResourceTypes/Scene_Resource.h"
#include "EditorSource/FileSystem/ResourceTypes/Resource_Identifier.h"


#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"
#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponentTypes.h"

namespace TuranEditor {



	StaticModel_Component* Create_StaticModel_Component(const EditorAsset::GameComponent* GameComponent_Data);
	Camera_Component* Create_Camera_Component(const EditorAsset::GameComponent* GameComponent_Data);
	GameComponentType Convert_fromFB_GameCompType(const EditorAsset::GameComponent_Type& GameComp_Type);
	EditorAsset::GameComponent_Type Convert_toFB_GameCompType(const GameComponentType& GameComp_Type);
	flatbuffers::Offset<EditorAsset::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component);
	flatbuffers::Offset<EditorAsset::GameComponent> Save_Camera_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component);


	bool Verify_SceneResource(Resource_Identifier* IDENTIFIER) {
		Scene_Resource* RESOURCE = (Scene_Resource*)IDENTIFIER->DATA;
		if (!RESOURCE) {
			EDITOR_FILESYSTEM->Load_Resource(IDENTIFIER->ID);
		}
		for (unsigned int i = 0; i < RESOURCE->ADDED_COMPONENTs.size(); i++) {
			TuranAPI::GameComponent* COMPONENT = RESOURCE->ADDED_COMPONENTs[i];
			if (COMPONENT == nullptr) {
				TuranAPI::Breakpoint("Scene Resource isn't verified because a Game Component is nullptr!");
				return false;
			}
		}
		return true;
	}

	void Load_SceneResource(Resource_Identifier* IDENTIFIER) {
		unsigned int data_size = 0;
		void* file_data = TAPIFILESYSTEM::Read_BinaryFile(IDENTIFIER->PATH.c_str(), data_size, LASTUSEDALLOCATOR);
		if (!file_data) {
			TuranAPI::Breakpoint(("Error: Loading failed! File isn't found at the PATH: " + IDENTIFIER->PATH).c_str());
			return;
		}
		auto RESOURCE_typeless = EditorAsset::GetResource(file_data);
		if (RESOURCE_typeless == nullptr) {
			TuranAPI::Breakpoint("Error: Loading failed! Scene isn't a valid resource!");
			return;
		}
		std::cout << "Loading Scene ID: " << IDENTIFIER->ID << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Scene();

		if (!RESOURCE) {
			std::cout << "Error: Type isn't Scene, type definition is wrong!\n";
			TuranAPI::Breakpoint();
			return;
		}


		Scene_Resource* SCENE = new Scene_Resource;
		IDENTIFIER->DATA = SCENE;
		for (unsigned int i = 0; i < RESOURCE->COMPONENTs()->Length(); i++) {
			auto FB_GameComponent = RESOURCE->COMPONENTs()->Get(i);
			GameComponentType GameComponent_Type = Convert_fromFB_GameCompType(FB_GameComponent->COMPONENT_type());
			TuranAPI::GameComponent* Game_Component;
			switch (GameComponent_Type) {
			case GameComponentType::STATIC_MODEL_C:
				std::cout << "Loading a StaticModel Component!\n";
				Game_Component = Create_StaticModel_Component(FB_GameComponent);
				break;
			case GameComponentType::CAMERA_C:
				std::cout << "Loading a Camera Component!\n";
				Game_Component = Create_Camera_Component(FB_GameComponent);
				break;
			default:
				std::cout << "ERROR: Intended component type isn't supported for loading in Load_Scene() in TuranAPI!\n";
				TuranAPI::Breakpoint();
				continue;
			}

			SCENE->ADDED_COMPONENTs.push_back(Game_Component);
		}


	}


	void Save_SceneResource(Resource_Identifier* IDENTIFIER) {
		Scene_Resource* RESOURCE = (Scene_Resource*)IDENTIFIER->DATA;
		flatbuffers::FlatBufferBuilder builder(1024);

		std::vector<flatbuffers::Offset<EditorAsset::GameComponent>> Scene_GameComponents;
		for (unsigned int i = 0; i < RESOURCE->ADDED_COMPONENTs.size(); i++) {
			TuranAPI::GameComponent* GAMECOMPONENT = RESOURCE->ADDED_COMPONENTs[i];
			EditorAsset::GameComponent_Type ComponentType = Convert_toFB_GameCompType(GAMECOMPONENT->TYPE);


			switch (ComponentType) {
			case EditorAsset::GameComponent_Type_StaticModel_Component:
				Scene_GameComponents.push_back(Save_StaticModel_Component(builder, GAMECOMPONENT));
				break;
			case EditorAsset::GameComponent_Type_Camera_Component:
				Scene_GameComponents.push_back(Save_Camera_Component(builder, GAMECOMPONENT));
				break;
			default:
				TuranAPI::Breakpoint("Intended GameComponent Type isn't supported to save to scene in Save_a_Scene_toDisk in TuranAPI!");
				break;
			}
		}


		auto finished_SCENE = EditorAsset::CreateSCENEDirect(builder, IDENTIFIER->Get_Name().c_str(), &Scene_GameComponents);
		auto finished_RESOURCE = EditorAsset::CreateResource(builder, EditorAsset::Resource_Type_Scene, finished_SCENE.Union());
		builder.Finish(finished_RESOURCE);

		unsigned int data_size = builder.GetSize();
		std::cout << "Compiled Scene to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();
		std::cout << "Exporting resource as a .scenecont: " << IDENTIFIER->PATH << std::endl;
		TAPIFILESYSTEM::FileSystem::Overwrite_BinaryFile(IDENTIFIER->PATH.c_str(), data_ptr, data_size);
	}


	GameComponentType Convert_fromFB_GameCompType(const EditorAsset::GameComponent_Type& GameComp_Type) {
		switch (GameComp_Type) {
		case EditorAsset::GameComponent_Type_StaticModel_Component:
			return GameComponentType::STATIC_MODEL_C;
		case EditorAsset::GameComponent_Type_Camera_Component:
			return GameComponentType::CAMERA_C;
		default:
			TuranAPI::Breakpoint("ERROR: Intended GameComponent type isn't found in Convert_fromFBGameCompType_toTuranAPIGameCompType() in TuranAPI! Probably, loading this type isn't supported for now!");
		}
		//There should be an ERROR type, maybe later
		return GameComponentType::STATIC_MODEL_C;
	}
	EditorAsset::GameComponent_Type Convert_toFB_GameCompType(const GameComponentType& GameComp_Type) {
		switch (GameComp_Type) {
		case GameComponentType::STATIC_MODEL_C:
			return EditorAsset::GameComponent_Type_StaticModel_Component;
		case GameComponentType::CAMERA_C:
			return EditorAsset::GameComponent_Type_Camera_Component;
		default:
			std::cout << "ERROR: Intended GameComponent type couldn't convert to EditorAsset::GameComponentType in Load_Scene_Resource.cpp in TuranAPI!\n";
			TuranAPI::Breakpoint();
			return EditorAsset::GameComponent_Type_NONE;
		}
	}
	StaticModel_Component* Create_StaticModel_Component(const EditorAsset::GameComponent* GameComponent_Data) {
		auto FB_STATICMODELCOMP = GameComponent_Data->COMPONENT_as_StaticModel_Component();
		if (!FB_STATICMODELCOMP) {
			TuranAPI::Breakpoint("Error: Component Type isn't Static Model, type definition is wrong!");
			return nullptr;
		}
		Resource_Identifier* MODEL = EDITOR_FILESYSTEM->Find_ResourceIdentifier_byID(FB_STATICMODELCOMP->Model_ID());

		//When the scene is loaded for the first time, we don't know if this resource is available. For now, I will just leave it like that!
		if (!MODEL) {
			TuranAPI::LOG_ERROR("There is no Model Asset, so component creation failed!");
			return nullptr;
		}

		StaticModel_Component* COMPONENT = new StaticModel_Component;
		COMPONENT->NAME = FB_STATICMODELCOMP->NAME()->str().c_str();
		COMPONENT->MODEL = MODEL;


		for (unsigned int i = 0; i < FB_STATICMODELCOMP->MaterialInstances_IDs()->Length(); i++) {
			COMPONENT->MATERIALs.push_back(
				EDITOR_FILESYSTEM->Find_ResourceIdentifier_byID(FB_STATICMODELCOMP->MaterialInstances_IDs()->Get(i))
			);
		}


		const EditorAsset::Vec3* FB_LOCATION = FB_STATICMODELCOMP->LOCATION();
		const EditorAsset::Vec3* FB_ROTATION = FB_STATICMODELCOMP->ROTATION();
		const EditorAsset::Vec3* FB_SCALE = FB_STATICMODELCOMP->SCALE();
		vec3 LOCATION(FB_LOCATION->x(), FB_LOCATION->y(), FB_LOCATION->z()), ROTATION(FB_ROTATION->x(), FB_ROTATION->y(), FB_ROTATION->z()), SCALE(FB_SCALE->x(), FB_SCALE->y(), FB_SCALE->z());

		COMPONENT->Translate(LOCATION);	COMPONENT->Rotate(ROTATION); COMPONENT->Scale(SCALE);

		return COMPONENT;
	}
	flatbuffers::Offset<EditorAsset::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component) {
		std::cout << "Compiles a StaticModel Component to Flatbuffer Data!\n";
		StaticModel_Component* StaticModel_Comp = (StaticModel_Component*)Component;
		if (StaticModel_Comp->MODEL == nullptr) {
			TuranAPI::LOG_CRASHING(("Static Model Component: " + StaticModel_Comp->NAME + "doesn't have any Static_Model!").c_str());
		}

		std::vector<uint32_t> Material_IDs;
		for (unsigned int i = 0; i < StaticModel_Comp->MATERIALs.size(); i++) {
			Material_IDs.push_back(StaticModel_Comp->MATERIALs[i]->ID);
		}

		vec3 LOCATION = StaticModel_Comp->Get_Position(); vec3 ROTATION = StaticModel_Comp->Get_Rotation(); vec3 SCALE = StaticModel_Comp->Get_Scale();
		EditorAsset::Vec3 FB_LOCATION = EditorAsset::Vec3(LOCATION.x, LOCATION.y, LOCATION.z);
		EditorAsset::Vec3 FB_ROTATION = EditorAsset::Vec3(ROTATION.x, ROTATION.y, ROTATION.z);
		EditorAsset::Vec3 FB_SCALE = EditorAsset::Vec3(SCALE.x, SCALE.y, SCALE.z);

		auto FINISHED_MODELCOMP = EditorAsset::CreateStaticModel_ComponentTableDirect(builder, StaticModel_Comp->NAME.c_str(), StaticModel_Comp->MODEL->ID, &Material_IDs, &FB_LOCATION, &FB_ROTATION, &FB_SCALE);
		return EditorAsset::CreateGameComponent(builder, EditorAsset::GameComponent_Type::GameComponent_Type_StaticModel_Component, FINISHED_MODELCOMP.Union());
	}


	Camera_Component* Create_Camera_Component(const EditorAsset::GameComponent* GameComponent_Data) {
		auto FB_CAMERACOMP = GameComponent_Data->COMPONENT_as_Camera_Component();
		if (!FB_CAMERACOMP) {
			std::cout << "Error: Component Type isn't Camera, type definition is wrong!\n";
			TuranAPI::Breakpoint();
			return nullptr;
		}
		//Component is successfully loaded as CameraComp here!

		vec3 Target(FB_CAMERACOMP->Target()->x(), FB_CAMERACOMP->Target()->y(), FB_CAMERACOMP->Target()->z());
		vec3 Position(FB_CAMERACOMP->LOCATION()->x(), FB_CAMERACOMP->LOCATION()->y(), FB_CAMERACOMP->LOCATION()->z());

		Camera_Component* CAMERACOMP = new Camera_Component(Target);
		CAMERACOMP->Set_Camera_Properties(FB_CAMERACOMP->FOV_inAngle(), FB_CAMERACOMP->Aspect_WIDTH(), FB_CAMERACOMP->Aspect_HEIGHT(), FB_CAMERACOMP->Near_Plane(), FB_CAMERACOMP->Far_Plane());
		CAMERACOMP->NAME = FB_CAMERACOMP->NAME()->str().c_str();
		CAMERACOMP->Translate(Position);
		return CAMERACOMP;
	}
	flatbuffers::Offset<EditorAsset::GameComponent> Save_Camera_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component) {
		Camera_Component* COMP = (Camera_Component*)Component;

		EditorAsset::Vec3 TARGET(COMP->Get_Target().x, COMP->Get_Target().y, COMP->Get_Target().z);
		EditorAsset::Vec3 POSITION(COMP->Get_Position().x, COMP->Get_Position().y, COMP->Get_Position().z);
		auto finished_CameraCompTable = EditorAsset::CreateCamera_ComponentTableDirect(builder, COMP->NAME.c_str(), &POSITION,
			COMP->Get_FOV_inAngle(), COMP->Get_Aspect_Width_and_Height().x, COMP->Get_Aspect_Width_and_Height().y,
			COMP->Get_Near_and_FarPlane().x, COMP->Get_Near_and_FarPlane().y, &TARGET
		);

		return EditorAsset::CreateGameComponent(builder, EditorAsset::GameComponent_Type_Camera_Component, finished_CameraCompTable.Union());
	}



}