#include "TuranAPI/FileSystem/FileSystem_Core.h"
//Data Formats created by Flatbuffers!
#include <flatbuffers/flatbuffers.h>
#include "TuranAPI/FileSystem/DataFormats/GameResource_generated.h"

using namespace TuranAPI::File_System;
Material_Type_Resource* Find_MaterialType_byID(unsigned int ID);
TuranAPI::TuranAPI_ENUMs Convert_to_TuranAPIEnums_UniformVarType(GameContent::MATERIAL_INSTANCE_UNIFORM uniform_var_type);
string Get_UniformName_fromFlatbuffer(const GameContent::MATERIAL_INSTANCE_UNIFORM_TABLE* flatbuffer_uniform, TuranAPI::TuranAPI_ENUMs uniform_var_type);



Resource_Type* FileSystem::Load_Material_Inst(void* data, unsigned int id, const string& path) {
	auto RESOURCE_typeless = GameContent::GetResource(data);
	if (RESOURCE_typeless == nullptr) {
		cout << "Error: Loading failed! Material Instance isn't a valid resource!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	cout << "Loading Material Instance (Shader Stage) ID: " << id << endl;
	auto RESOURCE = RESOURCE_typeless->TYPE_as_Material_Instance();
	if (!RESOURCE) {
		cout << "Error: Loading failed! Material Instance isn't verified as Material Instance Type!\n";
		TuranAPI::Breakpoint();
		return nullptr;
	}
	//If application arrives here, loading the file is successful!

	if (id > LAST_ID) {
		LAST_ID = id;
	}
	Material_Instance* MaterialInst = new Material_Instance;
	MaterialInst->Material_Type = Find_MaterialType_byID(RESOURCE->MATERIAL_TYPE_ID());
	MaterialInst->ID = id;
	MaterialInst->PATH = path;
	string NAME = path.substr(path.find_last_of('/') + 1);
	NAME = NAME.substr(0, NAME.find_last_of('.'));
	MaterialInst->NAME = NAME;
	for (unsigned int i = 0; i < RESOURCE->UNIFORMs()->Length(); i++) {
		Material_Uniform uniform;
		auto UNIFORM_typeless = RESOURCE->UNIFORMs()->Get(i);
		uniform.VARIABLE_TYPE = Convert_to_TuranAPIEnums_UniformVarType(UNIFORM_typeless->UNIFORM_type());
		uniform.VARIABLE_NAME = Get_UniformName_fromFlatbuffer(UNIFORM_typeless, uniform.VARIABLE_TYPE);
		//Please add here data loading!
		MaterialInst->UNIFORM_LIST.push_back(uniform);
	}

	return MaterialInst;
}

flatbuffers::Offset<void> Create_a_Uniform(flatbuffers::FlatBufferBuilder* builder, const Material_Uniform* material_uniform);
GameContent::MATERIAL_INSTANCE_UNIFORM Convert_to_Flatbufer_Uniform_Type(TuranAPI::TuranAPI_ENUMs uniform_type);

void Save_a_MaterialInst_toDisk(Resource_Type* Material_Inst_Data) {
	Material_Instance* MATERIAL_INSTANCE = (TuranAPI::File_System::Material_Instance*)Material_Inst_Data;
	flatbuffers::FlatBufferBuilder builder(1024);

	vector<flatbuffers::Offset<GameContent::MATERIAL_INSTANCE_UNIFORM_TABLE>> MATERIAL_UNIFORMs;
	//Add uniforms to Flatbuffer data!
	for (Material_Uniform material_uniform : MATERIAL_INSTANCE->UNIFORM_LIST) {
		cout << "Uniform is being proccessed: " << material_uniform.VARIABLE_NAME << endl;
		MATERIAL_UNIFORMs.push_back(
			GameContent::CreateMATERIAL_INSTANCE_UNIFORM_TABLE(builder,
				Convert_to_Flatbufer_Uniform_Type(material_uniform.VARIABLE_TYPE),
				Create_a_Uniform(&builder, &material_uniform)
			)
		);
	}
	auto finished_UNIFORMs_VECTOR = builder.CreateVector(MATERIAL_UNIFORMs);

	auto finished_MaterialInstance = GameContent::CreateMATERIAL_INSTANCE(builder, MATERIAL_INSTANCE->Material_Type->ID, finished_UNIFORMs_VECTOR);
	auto finished_RESOURCE = GameContent::CreateResource(builder, GameContent::Resource_Type_Material_Instance, finished_MaterialInstance.Union());
	builder.Finish(finished_RESOURCE);

	unsigned int data_size = builder.GetSize();

	cout << "Compiled Material Instance Resource to Flatbuffer type!\n";
	void* data_ptr = builder.GetBufferPointer();

	//Check if the data is complete!
	flatbuffers::Verifier verifier((uint8_t*)data_ptr, data_size);
	if (!GameContent::VerifyResourceBuffer(verifier)) {
		cout << "Data isn't verified for name: " << MATERIAL_INSTANCE->NAME << endl;
		assert(false && "Error while compiling data to disk!");
	}

	cout << "Exporting resource as a .matinstcont: " << MATERIAL_INSTANCE->PATH << endl;
	FileSystem::Overwrite_BinaryFile(MATERIAL_INSTANCE->PATH, data_ptr, data_size);

	cout << "Resource is successfully added to the Content_List.enginecont\n";
}

Material_Type_Resource* Find_MaterialType_byID(unsigned int ID) {
	for (unsigned int i = 0; i < Material_Type_Resource::ALL_MATERIAL_TYPEs.size(); i++) {
		Material_Type_Resource* MatInst = Material_Type_Resource::ALL_MATERIAL_TYPEs[i];
		if (ID == MatInst->ID) {
			return MatInst;
		}
	}
	cout << "Error: Material Type isn't found for Material Instance!\n";
	TuranAPI::Breakpoint();
	return nullptr;
}
