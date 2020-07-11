#include "Scene_Resource.h"

#include "EditorSource/FileSystem/DataFormats/EditorResources_generated.h"
#include "ResourceTYPEs.h"
#include "EditorSource/FileSystem/EditorFileSystem_Core.h"

#include "EditorSource/FileSystem/GameObjects/Camera_Component.h"
#include "EditorSource/FileSystem/GameObjects/StaticModel_Component.h"
#include "TuranAPI/FileSystem/GameObjects/GameComponents/GameComponentTypes.h"

namespace TuranEditor {


	Scene_Resource::Scene_Resource() {}


	StaticModel_Component* Create_StaticModel_Component(const EditorAsset::GameComponent* GameComponent_Data);
	Camera_Component* Create_Camera_Component(const EditorAsset::GameComponent* GameComponent_Data);
	GameComponentType Convert_fromFBGameCompType_toTuranAPIGameCompType(const EditorAsset::GameComponent_Type& GameComp_Type);


	void Scene_Resource::Load_FromDisk(void* data, unsigned int id, const char* path, IAllocator* Allocator) {
		auto RESOURCE_typeless = EditorAsset::GetResource(data);
		if (RESOURCE_typeless == nullptr) {
			std::cout << "Error: Loading failed! Scene isn't a valid resource!\n";
			TuranAPI::Breakpoint();
			return;
		}
		std::cout << "Loading Scene ID: " << id << std::endl;
		auto RESOURCE = RESOURCE_typeless->TYPE_as_Scene();

		if (!RESOURCE) {
			std::cout << "Error: Type isn't Scene, type definition is wrong!\n";
			TuranAPI::Breakpoint();
			return;
		}
		for (unsigned int i = 0; i < RESOURCE->COMPONENTs()->Length(); i++) {
			auto FB_GameComponent = RESOURCE->COMPONENTs()->Get(i);
			GameComponentType GameComponent_Type = Convert_fromFBGameCompType_toTuranAPIGameCompType(FB_GameComponent->COMPONENT_type());
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

			ADDED_COMPONENTs.push_back(Game_Component);
		}

		ID = id;
		PATH = path;
		NAME = RESOURCE->NAME()->str().c_str();
	}

	flatbuffers::Offset<EditorAsset::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component);
	flatbuffers::Offset<EditorAsset::GameComponent> Save_Camera_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component);




	EditorAsset::GameComponent_Type Convert_fromTuranAPIGameCompType_toFBGameCompType(const GameComponentType& GameComp_Type);

	void Scene_Resource::Write_ToDisk(bool Verify_Data) {
		if (Verify_Data) {
			Verify_Resource_Data();
		}
		flatbuffers::FlatBufferBuilder builder(1024);

		std::vector<flatbuffers::Offset<EditorAsset::GameComponent>> Scene_GameComponents;
		for (unsigned int i = 0; i < ADDED_COMPONENTs.size(); i++) {
			TuranAPI::GameComponent* GAMECOMPONENT = ADDED_COMPONENTs[i];
			EditorAsset::GameComponent_Type ComponentType = Convert_fromTuranAPIGameCompType_toFBGameCompType(GAMECOMPONENT->TYPE);


			std::vector<uint32_t> Material_IDs;
			StaticModel_Component* StaticModel_Comp = nullptr;
			vec3 LOCATION, ROTATION, SCALE;
			EditorAsset::Vec3 FB_LOCATION;
			EditorAsset::Vec3 FB_ROTATION;
			EditorAsset::Vec3 FB_SCALE;
			switch (ComponentType) {
			case EditorAsset::GameComponent_Type_StaticModel_Component:
				std::cout << "Compiles a StaticModel Component to Flatbuffer Data!\n";
				StaticModel_Comp = (StaticModel_Component*)GAMECOMPONENT;
				if (StaticModel_Comp->MODEL == nullptr) {
					TuranAPI::LOG_CRASHING("There is no Model in Static Model Component!\n");
				}
				for (unsigned int i = 0; i < StaticModel_Comp->MATERIALs.size(); i++) {
					Material_IDs.push_back(StaticModel_Comp->MATERIALs[i]->ID);
				}

				LOCATION = StaticModel_Comp->Get_Position(); ROTATION = StaticModel_Comp->Get_Rotation(); SCALE = StaticModel_Comp->Get_Scale();
				FB_LOCATION = EditorAsset::Vec3(LOCATION.x, LOCATION.y, LOCATION.z);
				FB_ROTATION = EditorAsset::Vec3(ROTATION.x, ROTATION.y, ROTATION.z);
				FB_SCALE = EditorAsset::Vec3(SCALE.x, SCALE.y, SCALE.z);

				Scene_GameComponents.push_back(
					EditorAsset::CreateGameComponent(builder, EditorAsset::GameComponent_Type_StaticModel_Component,
						EditorAsset::CreateStaticModel_ComponentTable(builder, builder.CreateString(StaticModel_Comp->NAME), StaticModel_Comp->MODEL->ID,
						builder.CreateVector(Material_IDs), &FB_LOCATION, &FB_ROTATION, &FB_SCALE).Union()
					)
				);
				break;
			case EditorAsset::GameComponent_Type_Camera_Component:
				Scene_GameComponents.push_back(Save_Camera_Component(builder, GAMECOMPONENT));
				break;
			default:
				TuranAPI::Breakpoint("Intended GameComponent Type isn't supported to save to scene in Save_a_Scene_toDisk in TuranAPI!");
				break;
			}
		}


		auto finished_SCENE = EditorAsset::CreateSCENEDirect(builder, NAME.c_str(), &Scene_GameComponents);
		auto finished_RESOURCE = EditorAsset::CreateResource(builder, EditorAsset::Resource_Type_Scene, finished_SCENE.Union());
		builder.Finish(finished_RESOURCE);

		unsigned int data_size = builder.GetSize();
		std::cout << "Compiled Scene to Flatbuffer type!\n";
		void* data_ptr = builder.GetBufferPointer();
		std::cout << "Exporting resource as a .scenecont: " << PATH << std::endl;
		TAPIFILESYSTEM::FileSystem::Overwrite_BinaryFile(PATH.c_str(), data_ptr, data_size);
	}


	StaticModel_Component* Create_StaticModel_Component(const EditorAsset::GameComponent* GameComponent_Data) {
		auto FB_STATICMODELCOMP = GameComponent_Data->COMPONENT_as_StaticModel_Component();
		if (!FB_STATICMODELCOMP) {
			std::cout << "Error: Component Type isn't Static Model, type definition is wrong!\n";
			TuranAPI::Breakpoint();
			return nullptr;
		}
		Static_Model* MODEL = CONTENTMANAGER->Find_StaticModel_byID(FB_STATICMODELCOMP->Model_ID());

		//When the scene is loaded for the first time, we don't know if this resource is available. For now, I will just leave it like that!
		if (!MODEL) {
			StaticModel_Component* COMPONENT = new StaticModel_Component(MODEL);
			COMPONENT->NAME = FB_STATICMODELCOMP->NAME()->str().c_str();
			return COMPONENT;
		}

		//If resource isn't in memory, we just set components name. Whenever the engine needs model's datas, it will load them according to the file that resource's PATH has address to.
		if (!MODEL->Is_ResourceLoaded) {
			StaticModel_Component* COMPONENT = new StaticModel_Component(MODEL);
			COMPONENT->NAME = FB_STATICMODELCOMP->NAME()->str().c_str();
			return COMPONENT;
		}



		vector<GFX_API::Material_Instance*> Material_Instances;
		for (unsigned int i = 0; i < FB_STATICMODELCOMP->MaterialInstances_IDs()->Length(); i++) {
			Material_Instances.push_back(
				CONTENTMANAGER->Find_MaterialInstance_byID(FB_STATICMODELCOMP->MaterialInstances_IDs()->Get(i))
			);
		}
		const EditorAsset::Vec3* FB_LOCATION = FB_STATICMODELCOMP->LOCATION();
		const EditorAsset::Vec3* FB_ROTATION = FB_STATICMODELCOMP->ROTATION();
		const EditorAsset::Vec3* FB_SCALE = FB_STATICMODELCOMP->SCALE();
		vec3 LOCATION(FB_LOCATION->x(), FB_LOCATION->y(), FB_LOCATION->z()), ROTATION(FB_ROTATION->x(), FB_ROTATION->y(), FB_ROTATION->z()), SCALE(FB_SCALE->x(), FB_SCALE->y(), FB_SCALE->z());

		StaticModel_Component* COMPONENT = new StaticModel_Component(MODEL);
		COMPONENT->Translate(LOCATION);	COMPONENT->Rotate(ROTATION); COMPONENT->Scale(SCALE);
		COMPONENT->MATERIALs = Material_Instances;
		COMPONENT->NAME = FB_STATICMODELCOMP->NAME()->str().c_str();

		return COMPONENT;
	}
	flatbuffers::Offset<EditorAsset::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder& builder, TuranAPI::GameComponent* Component) {
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