#include "TuranAPI/FileSystem/DataFormats/GameResource_generated.h"
#include "TuranAPI/API_FileSystem.h"
using namespace TuranAPI::File_System;
using namespace TuranAPI::Game_Object;

StaticModel_Component* Create_StaticModel_Component(const GameContent::GameComponent* GameComponent_Data);
Camera_Component* Create_Camera_Component(const GameContent::GameComponent* GameComponent_Data);

TuranAPI::TuranAPI_ENUMs Convert_fromFBGameCompType_toTuranAPIGameCompType(const GameContent::GameComponent_Type& GameComp_Type);

Resource_Type* FileSystem::Load_Scene(void* data, unsigned int id, const string& path) {
	auto RESOURCE_typeless = GameContent::GetResource(data);
	if (RESOURCE_typeless == nullptr) {
		cout << "Error: Loading failed! Scene isn't a valid resource!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	cout << "Loading Scene ID: " << id << endl;
	auto RESOURCE = RESOURCE_typeless->TYPE_as_Scene();

	if (!RESOURCE) {
		cout << "Error: Type isn't Scene, type definition is wrong!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	if (id > LAST_ID) {
		LAST_ID = id;
	}
	Scene_Resource* SCENE = new Scene_Resource();

	for (unsigned int i = 0; i < RESOURCE->COMPONENTs()->Length(); i++) {
		auto FB_GameComponent = RESOURCE->COMPONENTs()->Get(i);
		TuranAPI::TuranAPI_ENUMs GameComponent_Type = Convert_fromFBGameCompType_toTuranAPIGameCompType(FB_GameComponent->COMPONENT_type());
		GameComponent* Game_Component;
		switch (GameComponent_Type) {
		case TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_COMP:
			cout << "Loading a StaticModel Component!\n";
			Game_Component = Create_StaticModel_Component(FB_GameComponent);
			break;
		case TuranAPI::TuranAPI_ENUMs::CAMERA_COMP:
			cout << "Loading a Camera Component!\n";
			Game_Component = Create_Camera_Component(FB_GameComponent);
			break;
		default:
			cout << "ERROR: Intended component type isn't supported for loading in Load_Scene() in TuranAPI!\n";
			TuranAPI::Breakpoint();
			continue;
		}

		SCENE->ADDED_COMPONENTs.push_back(Game_Component);
	}

	SCENE->ID = id;
	SCENE->PATH = path;
	SCENE->NAME = RESOURCE->NAME()->str();
	return SCENE;
}

flatbuffers::Offset<GameContent::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder* builder, GameComponent* Component);
flatbuffers::Offset<GameContent::GameComponent> Save_Camera_Component(flatbuffers::FlatBufferBuilder* builder, GameComponent* Component);




GameContent::GameComponent_Type Convert_fromTuranAPIGameCompType_toFBGameCompType(const TuranAPI::TuranAPI_ENUMs& GameComp_Type);

void Save_a_Scene_toDisk(Resource_Type* Scene_Data) {
	Scene_Resource* SCENE = (Scene_Resource*)Scene_Data;
	flatbuffers::FlatBufferBuilder builder(1024);

	vector<flatbuffers::Offset<GameContent::GameComponent>> Scene_GameComponents;
	for (GameComponent* GAMECOMPONENT : SCENE->ADDED_COMPONENTs) {
		GameContent::GameComponent_Type ComponentType = Convert_fromTuranAPIGameCompType_toFBGameCompType(GAMECOMPONENT->Get_Component_Type());
		switch (ComponentType) {
		case GameContent::GameComponent_Type_StaticModel_Component:
			Scene_GameComponents.push_back(Save_StaticModel_Component(&builder, GAMECOMPONENT));
			break;
		case GameContent::GameComponent_Type_Camera_Component:
			Scene_GameComponents.push_back(Save_Camera_Component(&builder, GAMECOMPONENT));
			break;
		default:
			TuranAPI::Breakpoint("Intended GameComponent Type isn't supported to save to scene in Save_a_Scene_toDisk in TuranAPI!");
			break;
		}
	}


	auto finished_SCENE = GameContent::CreateSCENEDirect(builder, SCENE->NAME.c_str(), &Scene_GameComponents);
	auto finished_RESOURCE = GameContent::CreateResource(builder, GameContent::Resource_Type_Scene, finished_SCENE.Union());
	builder.Finish(finished_RESOURCE);

	unsigned int data_size = builder.GetSize();
	cout << "Compiled Scene to Flatbuffer type!\n";
	void* data_ptr = builder.GetBufferPointer();
	cout << "Exporting resource as a .scenecont: " << SCENE->PATH << endl;
	FileSystem::Overwrite_BinaryFile(SCENE->PATH, data_ptr, data_size);
}


StaticModel_Component* Create_StaticModel_Component(const GameContent::GameComponent* GameComponent_Data) {
	auto FB_STATICMODELCOMP = GameComponent_Data->COMPONENT_as_StaticModel_Component();
	if (!FB_STATICMODELCOMP) {
		cout << "Error: Component Type isn't Static Model, type definition is wrong!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	Static_Model_Data* MODEL = Static_Model_Data::Find_Model_byID(FB_STATICMODELCOMP->Model_ID());
	vector<Material_Instance*> Material_Instances;
	for (unsigned int i = 0; i < FB_STATICMODELCOMP->MaterialInstances_IDs()->Length(); i++) {
		Material_Instances.push_back(
			Material_Instance::Find_MaterialInstance_byID(FB_STATICMODELCOMP->MaterialInstances_IDs()->Get(i))
		);
	}
	const GameContent::Vec3* FB_LOCATION = FB_STATICMODELCOMP->LOCATION();
	const GameContent::Vec3* FB_ROTATION = FB_STATICMODELCOMP->ROTATION();
	const GameContent::Vec3* FB_SCALE = FB_STATICMODELCOMP->SCALE();
	vec3 LOCATION(FB_LOCATION->x(), FB_LOCATION->y(), FB_LOCATION->z()), ROTATION(FB_ROTATION->x(), FB_ROTATION->y(), FB_ROTATION->z()), SCALE(FB_SCALE->x(), FB_SCALE->y(), FB_SCALE->z());

	StaticModel_Component* COMPONENT = new StaticModel_Component(MODEL);
	COMPONENT->Translate(LOCATION);	COMPONENT->Rotate(ROTATION); COMPONENT->Scale(SCALE);
	COMPONENT->MATERIALs = Material_Instances;
	COMPONENT->NAME = FB_STATICMODELCOMP->NAME()->str();

	return COMPONENT;
}
flatbuffers::Offset<GameContent::GameComponent> Save_StaticModel_Component(flatbuffers::FlatBufferBuilder* builder, GameComponent* Component) {
	cout << "Compiles a StaticModel Component to Flatbuffer Data!\n";
	StaticModel_Component* StaticModel_Comp = (StaticModel_Component*)Component;
	vector<uint32_t> Material_IDs;
	for (Material_Instance* Material_Inst : StaticModel_Comp->MATERIALs) {
		Material_IDs.push_back(Material_Inst->Get_ID());
	}
	vec3 LOCATION(StaticModel_Comp->Get_Position()), ROTATION(StaticModel_Comp->Get_Rotation()), SCALE(StaticModel_Comp->Get_Scale());
	GameContent::Vec3 FB_LOCATION(LOCATION.x, LOCATION.y, LOCATION.z);
	GameContent::Vec3 FB_ROTATION(ROTATION.x, ROTATION.y, ROTATION.z);
	GameContent::Vec3 FB_SCALE(SCALE.x, SCALE.y, SCALE.z);

	auto finished_StaticModelCompTable = GameContent::CreateStaticModel_ComponentTableDirect(*builder,
		StaticModel_Comp->NAME.c_str(),
		StaticModel_Comp->MODEL->Get_ID(),
		&Material_IDs,
		&FB_LOCATION, &FB_ROTATION, &FB_SCALE
	);

	return GameContent::CreateGameComponent(*builder, GameContent::GameComponent_Type_StaticModel_Component, finished_StaticModelCompTable.Union());
}


Camera_Component* Create_Camera_Component(const GameContent::GameComponent* GameComponent_Data) {
	auto FB_CAMERACOMP = GameComponent_Data->COMPONENT_as_Camera_Component();
	if (!FB_CAMERACOMP) {
		cout << "Error: Component Type isn't Camera, type definition is wrong!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	//Component is successfully loaded as CameraComp here!

	vec3 Target(FB_CAMERACOMP->Target()->x(), FB_CAMERACOMP->Target()->y(), FB_CAMERACOMP->Target()->z());
	vec3 Position(FB_CAMERACOMP->LOCATION()->x(), FB_CAMERACOMP->LOCATION()->y(), FB_CAMERACOMP->LOCATION()->z());

	Camera_Component* CAMERACOMP = new TuranAPI::Game_Object::Camera_Component(Target);
	CAMERACOMP->Set_Camera_Properties(FB_CAMERACOMP->FOV_inAngle(), FB_CAMERACOMP->Aspect_WIDTH(), FB_CAMERACOMP->Aspect_HEIGHT(), FB_CAMERACOMP->Near_Plane(), FB_CAMERACOMP->Far_Plane());
	CAMERACOMP->NAME = FB_CAMERACOMP->NAME()->str();
	CAMERACOMP->Translate(Position);
	return CAMERACOMP;
}
flatbuffers::Offset<GameContent::GameComponent> Save_Camera_Component(flatbuffers::FlatBufferBuilder* builder, GameComponent* Component) {
	Camera_Component* COMP = (Camera_Component*)Component;

	GameContent::Vec3 TARGET(COMP->Get_Target().x, COMP->Get_Target().y, COMP->Get_Target().z);
	GameContent::Vec3 POSITION(COMP->Get_Position().x, COMP->Get_Position().y, COMP->Get_Position().z);
	auto finished_CameraCompTable = GameContent::CreateCamera_ComponentTableDirect(*builder, COMP->NAME.c_str(), &POSITION, 
		COMP->Get_FOV_inAngle(), COMP->Get_Aspect_Width_and_Height().x, COMP->Get_Aspect_Width_and_Height().y, 
		COMP->Get_Near_and_FarPlane().x, COMP->Get_Near_and_FarPlane().y, &TARGET
	);

	return GameContent::CreateGameComponent(*builder, GameContent::GameComponent_Type_Camera_Component, finished_CameraCompTable.Union());
}





TuranAPI::TuranAPI_ENUMs Convert_fromFBGameCompType_toTuranAPIGameCompType(const GameContent::GameComponent_Type& GameComp_Type) {
	switch (GameComp_Type) {
	case GameContent::GameComponent_Type_StaticModel_Component:
		return TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_COMP;
	case GameContent::GameComponent_Type_Camera_Component:
		return TuranAPI::TuranAPI_ENUMs::CAMERA_COMP;
	default:
		cout << "ERROR: Intended GameComponent type isn't found in Convert_fromFBGameCompType_toTuranAPIGameCompType() in TuranAPI! Probably, loading this type isn't supported for now!\n";
		TuranAPI::Breakpoint();
		return TuranAPI::TuranAPI_ENUMs::TURAN_NULL;
	}
}
GameContent::GameComponent_Type Convert_fromTuranAPIGameCompType_toFBGameCompType(const TuranAPI::TuranAPI_ENUMs& GameComp_Type) {
	switch (GameComp_Type) {
	case TuranAPI::TuranAPI_ENUMs::STATIC_MODEL_COMP:
		return GameContent::GameComponent_Type_StaticModel_Component;
	case TuranAPI::TuranAPI_ENUMs::CAMERA_COMP:
		return GameContent::GameComponent_Type_Camera_Component;
	default:
		cout << "ERROR: Intended GameComponent type couldn't convert to GameContent::GameComponentType in Load_Scene_Resource.cpp in TuranAPI!\n";
		TuranAPI::Breakpoint();
		return GameContent::GameComponent_Type_NONE;
	}
}